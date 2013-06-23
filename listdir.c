#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#include <stdlib.h>
#include <stdio.h>

int
main( int argc, char* argv[]){
    
    DIR* d;
    struct dirent* id; /* intrare director */

    if (argc != 2){
	printf("Numar insuficient de argumente!\n");
	exit(EXIT_FAILURE);
    }

    d = opendir(argv[1]);
    if ( d == NULL){
	perror(argv[1]);
	exit(EXIT_FAILURE);
    }

    while ((id = readdir(d)) != NULL)
	printf("%s\n", id->d_name);

    closedir(d);

    return EXIT_SUCCESS;
}

