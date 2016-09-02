#include "mt.h"

int main(int argc, char* argv[]) {
    int                 sock,n;
    int                 s1;
    int                 t;
    struct sockaddr_in  name;
    char                buf[MAX_MSG_LENGTH+1];
    //char                result[MAX_MSG_LENGTH+1];
    /* Crear socket sobre el que se lee: dominio INET, protocolo UDP (DGRAM). */
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("abriendo socket");
        exit(1);
    }

    /* Crear nombre, usamos INADDR_ANY para indicar que cualquiera puede enviar aquí. */
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_port = htons(PORT);

    if (bind(sock, (void*) &name, sizeof(name))) {
        perror("binding datagram socket");
            close(sock);
        exit(1);
    }
    if (listen(sock, 5) == -1) {
        perror("escuchando");
            close(sock);
        exit(1);
    }
    t = sizeof(name);
    if ((s1 = accept(sock, (struct sockaddr*) &name, (socklen_t*) &t)) == -1) {
        perror("aceptando la conexión entrante");
            close(sock);
        exit(1);
    }

    dup2(s1,1);

    /* Recibimos mensajes hasta que alguno sea el que marca el final. */
    for(;;) {
        n = recv(s1, buf, MAX_MSG_LENGTH, 0);
        if (n == 0) 
            break;
        if (n < 0) { 
            perror("recibiendo");
            close(sock);
            exit(1);
        }
        if (strncmp(buf, END_STRING, MAX_MSG_LENGTH) == 0)
            break;
        printf("Comando: %s", buf);
        
        system(buf);
        buf[n] = '\0'; /* Agregar caracter de fin de cadena a lo recibido. */
        memset(buf,'\0',MAX_MSG_LENGTH);
        char buf2[MAX_MSG_LENGTH];
        fgets(buf2, MAX_MSG_LENGTH, stdout);    //"prueba\0";
        if (send(s1 , &buf2, strlen(buf2), 0) == -1) {
            close(sock);
            perror("enviando");
            exit(1);
        }

    }

    // for (;;) {
    //     memset(buf, 0, MAX_MSG_LENGTH);
    //     read(sock, buf, MAX_MSG_LENGTH);
    //     if (strncmp(buf, END_STRING, MAX_MSG_LENGTH) == 0)
    //         break;
    //     printf("Comando: %s", buf);
    //     system(buf);
    // }

    /* Cerrar socket de recepción. */
    close(sock);

    return 0;
}
