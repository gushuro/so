#include "mt.h"

int main(int argc, char* argv[]) {
    /* TODO: Implementar */

    int                 sock;
    struct sockaddr_in  name;
    char                buf[MAX_MSG_LENGTH];

    /* Crear socket sobre el que se escribe: dominio INET, protocolo UDP (DGRAM). */
    if (argc <2) {
        perror("poneme una ip migo\n");
        exit(1);
    }
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("abriendo socket");
        exit(1);
    }
    name.sin_family = AF_INET;
   	if(!inet_aton(argv[1], &name.sin_addr)){
   		perror("error ip");
   		exit(1);
   	}
    name.sin_port = htons(PORT);

    // strcpy(name.sun_path, SOCK_PATH);
    // len = strlen(remote.sun_path) + sizeof(remote.sun_family);

    /* Conectarse. */
    if (connect(sock, (const struct sockaddr *)&name, sizeof(name)) == -1) {
        perror("conectandose");
        exit(1);
    }
    puts("soy el client");

    
   /* if (sendto(sock, &buf, strlen(buf), 0,  (const struct sockaddr *)&name, sizeof(name)) < 0 ){
    	perror("errxr mandando sendto migo");
    	exit(1);
    }*/
    	int n;
    while(printf("> "), fgets(buf, MAX_MSG_LENGTH, stdin), !feof(stdin)) {
        if (send(sock, &buf, strlen(buf), 0) == -1) {
            perror("enviando");
            exit(1);
        }
        memset(buf,'\0',MAX_MSG_LENGTH);
        n = recv(sock, buf, MAX_MSG_LENGTH, 0);
        printf("%s\n", buf);
        if (n == 0) 
            break;
        if (n < 0) { 
            perror("recibiendo");
            close(sock);
            exit(1);
        }
        memset(buf,'\0',MAX_MSG_LENGTH);
    }

    close(sock);

    return 0;
}
