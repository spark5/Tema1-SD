#include <unistd.h>
#include <dirent.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int
main(int argc, char* argv[]){
    char* s=NULL;
    if (argc != 2){
	fprintf(stderr, "Numar insuficient de argumente!\n");;
	exit( EXIT_FAILURE);
    }

    s = getcwd( s, 0);
    if ( s == NULL){
	perror(NULL);
	exit( EXIT_FAILURE);
    }
    printf( "Directorul inainte de schimbare: %s\n", s);
    free( s);
    s = NULL;

    if ( 0 != chdir(argv[1])){
	perror(argv[1]);
	exit( EXIT_FAILURE);
    }

    s = getcwd( s, 0);
    if ( s == NULL){
	perror(NULL);
	exit( EXIT_FAILURE);
    }
    printf( "Directorul dupa schimbare: %s\n", s);
    free( s);
    return EXIT_SUCCESS;
}

