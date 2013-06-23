#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void pretty_print( const char* cale, const char* afis);

int
main( int argc, char* argv[]){
    char* director;
    struct stat atribute;

    if (argc > 2){
	fprintf( stderr, "Numar incorect de argumente!\n");
	exit( EXIT_FAILURE);
    }

    director = ( argc == 1 ? "." : argv[1]);
    if (lstat( director, &atribute) != 0){
	perror(director);
	exit( EXIT_FAILURE);
    }

    if ( S_ISDIR( atribute.st_mode)){
	DIR* d;
	struct dirent* id;

	d = opendir(director);

	if ( d == NULL){
	    perror(director);
	    exit( EXIT_FAILURE);
	}

	while ( ( id = readdir(d)) != NULL)
	    if (id->d_name[0] != '.'){
	    char* aux;
	    aux = malloc ( strlen(director) + strlen(id->d_name) + 2 );
	    if ( aux == NULL)
		exit( EXIT_FAILURE);

	    strcpy( aux, director);
	    strcat( aux, "/");
	    strcat( aux, id->d_name);

	    pretty_print( aux, id->d_name);
	    free(aux);
	}

	closedir(d);

    }
    else
	pretty_print(director, director);

    return EXIT_SUCCESS;
}

void
pretty_print( const char* cale, const char* afis){
    char* drepturi;
    int i;
    struct stat atribute;
    struct passwd* ppasswd;
    struct group* pgrp;

    drepturi = malloc( 11);
    if ( drepturi == NULL)
	exit( EXIT_FAILURE);
    for( i=0; i<10; ++i)
	drepturi[i] = '-';
    drepturi[10] = '\0';

    if ( 0 != lstat( cale, &atribute)){
	perror( cale);
	free( drepturi);
	exit( EXIT_FAILURE);
    }

    if (S_ISDIR( atribute.st_mode))
	drepturi[0] = 'd';
    if (S_ISLNK( atribute.st_mode))
	drepturi[0] = 'l';
    if (S_ISBLK( atribute.st_mode))
	drepturi[0] = 'b';
    if (S_ISCHR( atribute.st_mode))
	drepturi[0] = 'c';

    if ( S_IRUSR  & atribute.st_mode)
	drepturi[1] = 'r';
    if ( S_IWUSR  & atribute.st_mode)
	drepturi[2] = 'w';
    if ( S_IXUSR  & atribute.st_mode)
	drepturi[3] = 'x';

    if ( S_IRGRP & atribute.st_mode)
	drepturi[4] = 'r';
    if ( S_IWGRP & atribute.st_mode)
	drepturi[5] = 'w';
    if ( S_IXGRP & atribute.st_mode)
	drepturi[6] = 'x';

    if ( S_IROTH & atribute.st_mode)
	drepturi[7] = 'r';
    if ( S_IWOTH & atribute.st_mode)
	drepturi[8] = 'w';
    if ( S_IXOTH & atribute.st_mode)
	drepturi[9] = 'x';

    ppasswd = getpwuid( atribute.st_uid);
    pgrp = getgrgid( atribute.st_gid);

    printf("%s %2d %s %s %8d %s\n",
	    drepturi, atribute.st_nlink,
	    ppasswd->pw_name, pgrp->gr_name,
	    (int)atribute.st_size, afis);

    free( drepturi);
}

