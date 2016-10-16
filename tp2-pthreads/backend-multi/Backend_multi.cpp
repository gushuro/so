#include "Backend_multi.h"


using namespace std;

// variables globales de la conexión
int socket_servidor = -1;
RWLock jugadoresLock = RWLock();
vector<jugador> jugadores;

// variables globales del juego
RWLock tableroLock = RWLock();
vector<vector<char> > tablero_confirmado; // solamente tiene las cartas confirmadas
unsigned int ancho = -1;
unsigned int alto = -1;


bool cargar_int(const char* numero, unsigned int& n) {
    char *eptr;
    n = static_cast<unsigned int>(strtol(numero, &eptr, 10));
    if(*eptr != '\0') {
        cerr << "error: " << numero << " no es un número: " << endl;
        return false;
    }
    return true;
}

int main(int argc, const char* argv[]) {
    // manejo la señal SIGINT para poder cerrar el socket cuando cierra el programa
    signal(SIGINT, cerrar_servidor);

    // parsear argumentos
    if (argc < 3) {
        cerr << "Faltan argumentos, la forma de uso es:" << endl <<
        argv[0] << " N M" << endl << "N = ancho del tablero , M = alto del tablero" << endl;
        return 3;
    }
    else {
        if (!cargar_int(argv[1], ancho)) {
            cerr << argv[1] << " debe ser un número" << endl;
            return 5;
        }
        if (!cargar_int(argv[2], alto)) {
            cerr << argv[2] << " debe ser un número" << endl;
            return 5;
        }
    }

    // inicializar tablero, se accede como tablero[fila][columna]

    tablero_confirmado = vector<vector<char> >(alto, vector<char>(ancho, VACIO));

    int socketfd_cliente, socket_size;
    struct sockaddr_in local, remoto;

    // crear un socket de tipo INET con TCP (SOCK_STREAM)
    if ((socket_servidor = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        cerr << "Error creando socket" << endl;
    }
    // permito reusar el socket para que no tire el error "Address Already in Use"
    int flag = 1;
    setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    // crear nombre, usamos INADDR_ANY para indicar que cualquiera puede conectarse aquí
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(PORT);
    if (bind(socket_servidor, (struct sockaddr *)&local, sizeof(local)) == -1) {
        cerr << "Error haciendo bind!" << endl;
        return 1;
    }

    // escuchar en el socket
    if (listen(socket_servidor, 1) == -1) {
        cerr << "Error escuchando socket!" << endl;
        return 1;
    }

    // aceptar conexiones entrantes.
    socket_size = sizeof(remoto);
    while (true) {
        if ((socketfd_cliente = accept(socket_servidor, (struct sockaddr*) &remoto, (socklen_t*) &socket_size)) == -1)
            cerr << "Error al aceptar conexion" << endl;
        else {
            atendedor_de_jugador(socketfd_cliente);
        }
    }

    close(socket_servidor);
    return 0;
}


void atendedor_de_jugador(int socket_fd) {
    // variables locales del jugador
    jugador jugadorNuevo;
    jugadorNuevo.socket = socket_fd;

    pthread_create(&(jugadorNuevo.thread), NULL, threadJugador, (void*)&jugadorNuevo);
}


void* threadJugador(void* args){
    jugador* jugadorNuevo = (jugador*) args;

    jugadorNuevo->tablero_temporal = vector<vector<char>>(alto,vector<char>(ancho));
    for (unsigned int i = 0; i < alto; ++i) {
        for (unsigned int j = 0; j < ancho; ++j) {
            jugadorNuevo->tablero_temporal[i][j] = VACIO;
        }
    }
    /*jugadoresLock.wlock();
        jugadores.push_back(jugadorNuevo);
    jugadoresLock.wunlock();*/

    cout << "tamano tablero " << (jugadorNuevo->tablero_temporal.size()) << jugadorNuevo->tablero_temporal[0].size() << endl;

    if (recibir_nombre(jugadorNuevo->socket, jugadorNuevo->nombre) != 0) {
        // el cliente cortó la comunicación, o hubo un error. Cerramos todo.
        terminar_servidor_de_jugador(jugadorNuevo);
    }

    if (enviar_dimensiones(jugadorNuevo->socket) != 0) {
        // se produjo un error al enviar. Cerramos todo.
        terminar_servidor_de_jugador(jugadorNuevo);
    }

    cout << "Esperando que juegue " << jugadorNuevo->nombre << endl;

    while (true) {

        // espera una carta o la confirmación de la jugada
        char mensaje[MENSAJE_MAXIMO+1];
        int comando = recibir_comando(jugadorNuevo->socket, mensaje);
        cout << "recibi comando" << endl;

        if (comando == MSG_CARTA) {
        cout << "es carta" << endl;

            Casillero ficha;
            if (parsear_casillero(mensaje, ficha) != 0) {
                // no es un mensaje carta bien formado, hacer de cuenta que nunca llegó
                continue;
            }

            // ficha contiene la nueva carta a colocar
            // verificar si es una posición válida del tablero
            if (es_ficha_valida_en_jugada(ficha, jugadorNuevo->jugada_actual, jugadorNuevo->tablero_temporal)) {
                jugadorNuevo->jugada_actual.push_back(ficha);
                jugadorNuevo->tablero_temporal[ficha.fila][ficha.columna] = ficha.contenido;
                // OK
        cout << "ficha valida todo bien" << endl;

                if (enviar_ok(jugadorNuevo->socket) != 0) {
                    // se produjo un error al enviar. Cerramos todo.
                    terminar_servidor_de_jugador(jugadorNuevo);
                }
            }
            else {
                quitar_cartas(jugadorNuevo->jugada_actual, jugadorNuevo->tablero_temporal);
                // ERROR
                if (enviar_error(jugadorNuevo->socket) != 0) {
                    // se produjo un error al enviar. Cerramos todo.
                    terminar_servidor_de_jugador(jugadorNuevo);
                }
            }
        }
        else if (comando == MSG_CONFIRMO) {
            // las cartas acumuladas conforman una jugada completa, escribirlas en el tablero y borrar las cartas temporales
        cout << "es confirmo" << endl;

            tableroLock.wlock();
                for (list<Casillero>::const_iterator casillero = jugadorNuevo->jugada_actual.begin(); casillero != jugadorNuevo->jugada_actual.end(); casillero++) {
                        tablero_confirmado[casillero->fila][casillero->columna] = casillero->contenido;
                }
            tableroLock.wunlock();
            jugadorNuevo->jugada_actual.clear();
            for (unsigned int i = 0; i < alto; ++i) {
                for (unsigned int j = 0; j < ancho; ++j) {
                    jugadorNuevo->tablero_temporal[i][j] = VACIO;
                }
            }

            if (enviar_ok(jugadorNuevo->socket) != 0) {
                // se produjo un error al enviar. Cerramos todo.
                terminar_servidor_de_jugador(jugadorNuevo);
            }
        }
        else if (comando == MSG_UPDATE) {
        cout << "es update" << endl;

            if (enviar_tablero(jugadorNuevo->socket) != 0) {
                // se produjo un error al enviar. Cerramos todo.
                terminar_servidor_de_jugador(jugadorNuevo);
            }
        }
        else if (comando == MSG_INVALID) {
            // no es un mensaje válido, hacer de cuenta que nunca llegó
            cout << "continuo" << endl;

            continue;
        }
        else {
            // se produjo un error al recibir. Cerramos todo.
            terminar_servidor_de_jugador(jugadorNuevo);
        }
    }
    return args;
}


// mensajes recibidos por el server

int recibir_nombre(int socket_fd, char* nombre) {
    char buf[MENSAJE_MAXIMO+1];

    if (recibir(socket_fd, buf) != 0) {
        return -1;
    }

    int res = sscanf(buf, "SOY %20s", nombre);

    if (res == EOF || res != 1) {
        cerr << "ERROR: no se pudo leer el nombre del cliente" << endl;
        return -1;
    }

    return 0;
}

// informa el tipo de comando recibido (CARTA, CONFIRMO, UPDATE o si es inválido) y deja el mensaje en mensaje por si necesita seguir parseando
int recibir_comando(int socket_fd, char* mensaje) {
    if (recibir(socket_fd, mensaje) != 0) {
        return -1;
    }

    char comando[MENSAJE_MAXIMO];
    sscanf(mensaje, "%s", comando);

    if (strcmp(comando, "CARTA") == 0) {
        // el mensaje es CARTA
        return MSG_CARTA;
    }
    else if (strcmp(comando, "CONFIRMO") == 0) {
        // el mensaje es CONFIRMO
        return MSG_CONFIRMO;
    }
    else if (strcmp(comando, "UPDATE") == 0) {
        // el mensaje es UPDATE
        return MSG_UPDATE;
    }
    else {
        cerr << "ERROR: mensaje no válido" << endl;
        return MSG_INVALID;
    }
}

int parsear_casillero(char* mensaje, Casillero& ficha) {
    int cant = sscanf(mensaje, "CARTA %d %d %c", &ficha.fila, &ficha.columna, &ficha.contenido);
    cout << "parsear recibe vacio: " << (ficha.contenido == VACIO) << endl;

    if (cant == 3) {
        // el mensaje es CARTA y ficha contiene la carta que desea colocar
        return 0;
    }
    else {
        cerr << "ERROR: " << mensaje << " no está bien formado. Debe ser CARTA <fila> <columna> <tipo>" << endl;
        return -1;
    }
}

// mensajes enviados por el server

int enviar_dimensiones(int socket_fd) {
    char buf[MENSAJE_MAXIMO+1];
    sprintf(buf, "TABLERO %d %d", ancho, alto);
    return enviar(socket_fd, buf);
}

int enviar_tablero(int socket_fd) {
    char buf[MENSAJE_MAXIMO+1];
    sprintf(buf, "STATUS ");
    int pos = 7;
    tableroLock.rlock();
        for (unsigned int fila = 0; fila < alto; ++fila) {
            for (unsigned int col = 0; col < ancho; ++col) {
                char contenido = tablero_confirmado[fila][col];
                buf[pos] = (contenido == VACIO)? '-' : contenido;
                cout << buf[pos];
                pos++;
            }
            cout << endl;
        }
    tableroLock.runlock();
    buf[pos] = 0; //end of buffer

    return enviar(socket_fd, buf);
}

int enviar_ok(int socket_fd) {
    char buf[MENSAJE_MAXIMO+1];
    sprintf(buf, "OK");
    return enviar(socket_fd, buf);
}

int enviar_error(int socket_fd) {
    char buf[MENSAJE_MAXIMO+1];
    sprintf(buf, "ERROR");
    return enviar(socket_fd, buf);
}


// otras funciones

void cerrar_servidor(int signal) {
    cout << "¡Adiós mundo cruel!" << endl;
    if (socket_servidor != -1)
        close(socket_servidor);
    exit(EXIT_SUCCESS);
}

void terminar_servidor_de_jugador(jugador* j) {
    cout << "Se interrumpió la comunicación con un cliente" << endl;

    jugadoresLock.wlock();
        for (vector<jugador>::iterator it = jugadores.begin(); it != jugadores.end(); ++it)
        {
            if (j->socket == it->socket) {
                // todo borrar el jugador
                jugadores.erase(it);
                break;
            }
        }
    jugadoresLock.wunlock();


    close(j->socket);

    quitar_cartas(j->jugada_actual, j->tablero_temporal);

    pthread_exit(NULL);
}


void quitar_cartas(list<Casillero>& jugada_actual, vector<vector<char>> &tablero_temporal) {
    for (list<Casillero>::const_iterator casillero = jugada_actual.begin(); casillero != jugada_actual.end(); casillero++) {
        tablero_temporal[casillero->fila][casillero->columna] = VACIO;
    }
    jugada_actual.clear();
}


bool es_ficha_valida_en_jugada(const Casillero& ficha, const list<Casillero>& jugada_actual, vector<vector<char>> &tablero_temporal) {
    // si está fuera del tablero, no es válida
    if (ficha.fila < 0 || ficha.fila > alto - 1 || ficha.columna < 0 || ficha.columna > ancho - 1) {
        cout << "ficha valida afuera tablero" << endl;
        return false;
    }

    // si el casillero está ocupado, tampoco es válida
    if (tablero_temporal[ficha.fila][ficha.columna] != VACIO) {
        cout << "ficha valida casilla ocupada" << endl;
        return false;
    }

    if (jugada_actual.size() > 0) {
        cout << "ficha validase fija en las otras   "<< jugada_actual.size() << endl;

        // no es la primera carta de lajugada, ya hay fichas colocadas en esta jugada
        Casillero mas_distante = casillero_mas_distante_de(ficha, jugada_actual);
        int distancia_vertical = ficha.fila - mas_distante.fila;
        int distancia_horizontal = ficha.columna - mas_distante.columna;

        if (distancia_vertical == 0) {
            // la jugada es horizontal
            for (list<Casillero>::const_iterator casillero = jugada_actual.begin(); casillero != jugada_actual.end(); casillero++) {
                if (ficha.fila - casillero->fila != 0) {
                    // no están alineadas horizontalmente
                    return false;
                }
            }

            int paso = distancia_horizontal / abs(distancia_horizontal);
            for (unsigned int columna = mas_distante.columna; columna != ficha.columna; columna += paso) {
                // el casillero DEBE estar ocupado en el tablero de jugadas confirmadas
                tableroLock.rlock();
                    char casilla = tablero_confirmado[ficha.fila][columna];
                tableroLock.runlock();
                if (!(puso_carta_en(ficha.fila, columna, jugada_actual)) && casilla == VACIO) {
                    return false;
                }
            }

        } else if (distancia_horizontal == 0) {
            // la jugada es vertical
            for (list<Casillero>::const_iterator casillero = jugada_actual.begin(); casillero != jugada_actual.end(); casillero++) {
                if (ficha.columna - casillero->columna != 0) {
                    // no están alineadas verticalmente
                    return false;
                }
            }

            int paso = distancia_vertical / abs(distancia_vertical);
            for (unsigned int fila = mas_distante.fila; fila != ficha.fila; fila += paso) {
                // el casillero DEBE estar ocupado en el tablero de jugadas confirmadas
                tableroLock.rlock();
                    char casilla = tablero_confirmado[fila][ficha.columna];
                tableroLock.runlock();
                if (!(puso_carta_en(fila, ficha.columna, jugada_actual)) && casilla == VACIO) {
                    return false;
                }
            }
        }
        else {
            // no están alineadas ni horizontal ni verticalmente
            return false;
        }
    }

    return true;
}


Casillero casillero_mas_distante_de(const Casillero& ficha, const list<Casillero>& jugada_actual) {
    const Casillero* mas_distante;
    int max_distancia = -1;
    for (list<Casillero>::const_iterator casillero = jugada_actual.begin(); casillero != jugada_actual.end(); casillero++) {
        int distancia = max<unsigned int>(abs((int)(casillero->fila - ficha.fila)), abs((int)(casillero->columna - ficha.columna)));
        if (distancia > max_distancia) {
            max_distancia = distancia;
            mas_distante = &*casillero;
        }
    }

    return *mas_distante;
}


bool puso_carta_en(unsigned int fila, unsigned int columna, const list<Casillero>& jugada_actual) {
    for (list<Casillero>::const_iterator casillero = jugada_actual.begin(); casillero != jugada_actual.end(); casillero++) {
        if (casillero->fila == fila && casillero->columna == columna)
            return true;
    }
    // si no encontró
    return false;
}

