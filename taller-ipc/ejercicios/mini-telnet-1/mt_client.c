#include "mt.h"

int main(int argc, char* argv[]) {
    /* TODO: Implementar */

    int                 sock;
    struct sockaddr_in  name;
    char                buf[MAX_MSG_LENGTH];

    /* Crear socket sobre el que se escribe: dominio INET, protocolo UDP (DGRAM). */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
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

    
   /* if (sendto(sock, &buf, strlen(buf), 0,  (const struct sockaddr *)&name, sizeof(name)) < 0 ){
    	perror("errxr mandando sendto migo");
    	exit(1);
    }*/

    while(printf("> "), fgets(buf, MAX_MSG_LENGTH, stdin), !feof(stdin)) {
        if (sendto(sock, &buf, strlen(buf), 0, (const struct sockaddr *)&name, sizeof(name) ) == -1) {
            perror("enviando");
            exit(1);
        }
    }




    return 0;
}
