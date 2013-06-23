#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>

#define BUFF_SIZE 100

int
main( int argc, char* argv[]){
    int fd; /* file descriptor */
    int citit;
    char buff[BUFF_SIZE];

    if ( argc != 2){
	printf("Numar incorect de argumente!\n");
	exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1){
	perror(argv[1]);
	exit(EXIT_FAILURE);
    }

    /* atat cat citirea reuseste afisez pe iesirea standard */
    while ( ( citit = read(fd, buff, BUFF_SIZE)) > 0)
	write( 1, buff, citit);

    /* nu testez codul de retur pentru close(); daca deschideam
     * pentru scriere, trebuia sa testam, pentru a ne asigura ca
     * nu avem eroare de golire a bufferelor */
    close(fd);

    return EXIT_SUCCESS;
}

