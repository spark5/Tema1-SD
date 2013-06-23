#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void myremove(const char*);

int
main( int argc, char* argv[]){
    if (argc != 2){
	printf("Numar insuficient de argumente!\n");
	exit(EXIT_FAILURE);
    }

    myremove(argv[1]);

    return EXIT_SUCCESS;
}

void
myremove( const char* cale){
    DIR* d;

    errno = 0;
    d = opendir(cale);
    if ( errno == ENOTDIR){
	/* fisier normal*/
	remove(cale);
    }
    else{
	if ( d == NULL){
	    perror(cale);
	    exit( EXIT_FAILURE);
	}
	else{
	    char* noua_cale = NULL;
	    char* aux = NULL;
	    struct dirent* id;

	    while (( id = readdir(d)) != NULL){
		if ((strcmp(id->d_name, ".") == 0) || ( strcmp( id->d_name, "..") == 0))
		    ;
		else{
		    aux = realloc ( noua_cale, strlen(cale) + strlen(id->d_name) + 2 );
		    if (aux == NULL)
			exit(EXIT_FAILURE);
		    noua_cale = aux;
		    strcpy( noua_cale, cale);
		    strcat( noua_cale, "/");
		    strcat( noua_cale, id->d_name);
		    myremove(noua_cale);
		    free(noua_cale);
		    noua_cale = NULL;
		}
	    }
	    if (noua_cale != NULL)
		free( noua_cale);

	    closedir(d);
	    remove(cale);
	}
    }
}

