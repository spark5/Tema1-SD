#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <string.h>


#define MAX 256
#define FILE_MAX 65535


/*functie pentru sortare lexicografica a fisierelor, in caz de egalitate dupa un criteriu*/
int sort_lex(void *a, void *b)
{
	struct dirent *x, *y;

	x = (struct dirent *)a;
	y = (struct dirent *)b;

	if(strcmp(x->d_name, y->d_name) < 0)
		return 1;

	return 0;
}


/*functie pentru sortare lexicografica descrescatoare a fisierelor*/
int r_sort_lex(void *a, void *b)
{
	struct dirent *x, *y;

	x = (struct dirent *)a;
	y = (struct dirent *)b;

	if(strcmp(x->d_name, y->d_name) > 0)
		return 1;

	return 0;
}


/*functie comparator pentru sortarea crescatoare dupa data unui fisier*/
int sort_data(void *a, void *b)
{
	struct stat info1, info2;
	struct dirent *x, *y;

	x = (struct dirent *)a;
	y = (struct dirent *)b;

	stat(x->d_name, &info1);

	stat(y->d_name, &info2);

	if (info1.st_mtime - info2.st_mtime < 0)
		return 1;

	if (info1.st_mtime - info2.st_mtime == 0)
		return sort_lex(a, b);

	return 0;
}


/*functie comparator pentru sortarea descrescatoare dupa data unui fisier*/
int r_sort_data(void *a, void *b)
{
	struct stat info1, info2;
	struct dirent *x, *y;

	x = (struct dirent *)a;
	y = (struct dirent *)b;

	stat(x->d_name, &info1);

	stat(y->d_name, &info2);

	if (info1.st_mtime - info2.st_mtime > 0)
		return 1;

	if (info1.st_mtime - info2.st_mtime == 0)
		return r_sort_lex(a, b);

	return 0;
}


/*functie comparator pentru sortarea crescatoare dupa size-ul unui fisier*/
int sort_size(void *a, void *b)
{
	struct stat info1, info2;
	struct dirent *x, *y;

	x = (struct dirent *)a;
	y = (struct dirent *)b;

	stat(x->d_name, &info1);
	stat(y->d_name, &info2);

	if(info1.st_size < info2.st_size)
		return 1;

	if(info1.st_size == info2.st_size)
		return sort_lex(a, b);

	return 0;
}


/*functie comparator pentru sortarea descrescatoare dupa size-ul unui fisier*/
int r_sort_size(void *a, void *b)
{
	struct stat info1, info2;
	struct dirent *x, *y;

	x = (struct dirent *)a;
	y = (struct dirent *)b;

	stat(x->d_name, &info1);
	stat(y->d_name, &info2);

	if(info1.st_size > info2.st_size)
		return 1;

	if(info1.st_size == info2.st_size)
		return r_sort_lex(a, b);

	return 0;
}


/*functia de interclasare pentru mergesort cu comparator*/
void merge(int p, int q, int r, struct dirent **v, int (*compare)(void *, void *))
{
	struct dirent **temp = (struct dirent **)malloc((r-p)*sizeof(struct dirent *));
	int i, j, k;

	k = 0;
	i = p;
	j = q;

	while(i < q && j < r)
		if(compare(v[i], v[j]))
			temp[k++] = v[i++];
		else
			temp[k++] = v[j++];

	while(i < q)
		temp[k++] = v[i++];
	while(j < r)
		temp[k++] = v[j++];

	for(i = 0; i < k; i++)
		v[i + p] = temp[i];

	free(temp);
}


void msort(int i, int j, struct dirent **v, int (*compare)(void *, void *))
{
	if (i < j-1)
	{
	   int k = (i + j)/2;
       	   msort(i, k, v, compare);
  	   msort(k, j, v, compare);
  	   merge(i, k, j, v, compare);
  	}
}


void mergesort(int n, struct dirent **v, int (*compare)(void *, void *))
{
	msort(0, n, v, compare);
}


/*functie care afiseaza informatiile despre un fisier*/
void print_fis(struct dirent *id, int max)
{
	struct tm *time;
	struct stat info;
	char date[MAX];

	stat(id->d_name, &info);				//se iau informatiile despre fisier
	time = localtime(&info.st_mtime);
	strftime(date, sizeof(date), "%T, %d %B %Y", time); 	//se formateaza data si timpul

	printf("%*s\t%d:%d\t%lu\t%s\n", max, id->d_name, info.st_uid, info.st_gid, info.st_size, date);
}


/* functie care parcurge un director si sorteaza sau nu, fisierele, in functie de parametrii dati*/
struct dirent ** walk_dir(char *dir, int *p, int *n)
{
	DIR *d;
	struct dirent *id, **fid = NULL;

	/*se deschide directorul si se initializeaza cu 0 numarul de fisiere citite*/
	d = opendir(dir);
	*n = 0;

	/*se citesc fisierele si se aloca un vector de pointeri la structuri dirent cu fisierele citite*/
	while ((id = readdir(d)) != NULL)
		if(strcmp(id->d_name, "lf") != 0 && strcmp(id->d_name, ".") != 0 && strcmp(id->d_name, "..") != 0)
		{
			fid = (struct dirent **)realloc(fid, ++(*n) * (sizeof(struct dirent *)));
			fid[*n-1] = id;
		}

	if(p[3] == 1 && p[4] == 0)			//daca se cere sortare dupa size
	{
		if(p[2] == 0)				//nu s-a specificat sortare descrescatoare
			mergesort(*n, fid, sort_size);
		else
			mergesort(*n, fid, r_sort_size);
	}

	if(p[4] == 1 && p[3] == 0)			//daca se cere sortare dupa time
	{
		if(p[2] == 0)				//nu s-a specificat sortare descrescatoare
			mergesort(*n, fid, sort_data);
		else
			mergesort(*n, fid, r_sort_data);
	}

	if(p[3] == 0 && p[4] == 0 && p[2] == 1)		//daca s-a cerut doar sortare descrescatoare
			mergesort(*n, fid, r_sort_lex);

	if(p[3] == 0 && p[4] == 0 && p[2] == 0)
			mergesort(*n, fid, sort_lex); 	//daca nu s-a specificat niciun parametru de sortare

	return fid;
}


/* functie recursiva care afiseaza si continutul subdirectoarelor */
void print_rec(int max, int depth, int *p)
{
	char *dir = NULL;
	struct dirent **fid;
	struct stat f;
	int i, j, n;
	unsigned long path_size;

	path_size = pathconf(".", _PC_PATH_MAX);
	dir = getcwd(dir,(size_t)path_size);
	
	//se parcurge directorul curent si se obtin inclusiv fisierele ascunse, sortate sau nu, in functie de parametrii dati
	fid = walk_dir(dir, p, &n);
	
	for(i = 0; i < n; i++)
		if(strlen(fid[i]->d_name) > max)
			max = strlen(fid[i]->d_name);	
	
	for (i = 0; i < n; i++)
	{
		stat(fid[i]->d_name, &f);			//se iau informatiile despre un fisier

		for(j = 0; j < depth; j++)
			printf("\t");				//se aliniaza fisierul, in functie de adancimea subdirectorului

		if(p[0] == 1)					//daca se cer si fisierele ascunse
			print_fis(fid[i], max);
		else
			if(fid[i]->d_name[0] != '.')
				print_fis(fid[i], max);		//se omit fisierele ascunse
				
		if(S_ISDIR(f.st_mode))				//daca fisierul este un director
		{	
			if(p[0] == 1 && (fid[i]->d_name[0] == '.' || fid[i]->d_name[0] != '.')) //daca se cer si directoare ascunse
			{
				path_size = pathconf(".", _PC_PATH_MAX);
				dir = getcwd(dir, path_size);		//se face path-ul pentru a intra in director
				strcat(dir, "/");			
				strcat(dir, fid[i]->d_name);
				
				chdir(dir);				//se schimba directorul si se creste adancimea
				depth++;
			
				print_rec(max, depth, p);		//se reapeleaza functia, pentru fisiere director
				chdir("..");				//se urca in structura de directoare
				depth--;				//se scade adancimea
				
			}
			
			if(p[0] == 0 && fid[i]->d_name[0] != '.')	//daca nu se cer directoare ascunse
			{
				path_size = pathconf(".", _PC_PATH_MAX);
				dir = getcwd(dir, path_size);		//se face path-ul pentru a intra in director
				strcat(dir, "/");			
				strcat(dir, fid[i]->d_name);
				
				chdir(dir);				//se schimba directorul si se creste adancimea
				depth++;
						
				print_rec(max, depth, p);		//se reapeleaza functia, pentru fisiere director
				
				chdir("..");				//se urca in structura de directoare
				depth--;				//se scade adancimea
			}
		}		
	}
	
	free(dir);
	free(fid);			//se elibereaza memoria ocupata
}


int main(int argc, char *argv[])
{
	DIR *d;
	struct dirent *id, **fid = NULL;
	int i, n = 0, p[5], max = 0;
	char *dir = NULL;

	for(i = 0; i < 5; i++)				//vector pentru a tine evidenta parametrilor dati
		p[i] = 0;
	
	if(argc == 1)
	{
		dir = getcwd(dir, 0);	//se obtine directorul curent si se deschide
		d = opendir(dir);

		/*se citesc toate fisierele din director, adica inclusiv cele ascunse;
		 *se aloca dinamic un vector de pointeri la structuri dirent cu fisierele citite
		 */
		while ((id = readdir(d)) != NULL)
			if(id->d_name[0] != '.' && strcmp(id->d_name, ".") != 0 && strcmp(id->d_name, "..") != 0
				&& strcmp(id->d_name, "lf") != 0)
			{
				fid = (struct dirent **)realloc(fid, ++n * (sizeof(struct dirent *)));
				fid[n-1] = id;

				if(strlen(id->d_name) > max)
					max = strlen(id->d_name);
			}

		mergesort(n, fid, sort_lex);		//se sorteaza lexicografic

		for(i = 0; i < n; i++)			//se afiseaza fisierele
			print_fis(fid[i], max);

		closedir(d);
		
		free(fid);	
		free(dir);
		
		exit(1);
	}
	else
		for(i = 1; i < argc; i++)
		{
			//se verifica daca parametrii dati sunt corecti
			if(strcmp(argv[i], "-a") != 0 && strcmp(argv[i], "-R") != 0 && strcmp(argv[i], "-r") != 0
				&& strcmp(argv[i], "-s") != 0 && strcmp(argv[i], "-t") != 0)
			{
				fprintf(stderr, "Usage: ./lf [-a|-R|-r] <-s|-t>\n");
				exit(1);
			}

			//in vectorul parametrilor, se tine cont de cate ori a fost citit un parametru
			if(strcmp(argv[i], "-a") == 0)
				p[0]++;

			if(strcmp(argv[i], "-R") == 0)
				p[1]++;

			if(strcmp(argv[i], "-r") == 0)
				p[2]++;

			if(strcmp(argv[i], "-s") == 0)
				p[3]++;

			if(strcmp(argv[i], "-t") == 0)
				p[4]++;
		}

	//daca macar un parametru a fost citit mai mult de o data sau daca parametrii -s, -t au fost dati simultan
	for(i = 0; i < 5; i++)
		if(p[i] > 1 || (p[4] == 1 && p[3] == 1))
		{
			fprintf(stderr, "Usage: ./lf [-a|-R|-r] <-s|-t>\n");
			exit(1);
		}

	//daca s-a cerut si listare recursiva
	if(p[1] == 1)
		print_rec(0, 0, p);
	else
		{	//se trateaza separat combinatiile de parametri

			//se obtine directorul curent si se deschide
			dir = getcwd(dir, 0);
			d = opendir(dir);

			//se parcurge directorul curent si se obtin inclusiv fisierele ascunse, sortate sau nu, in functie de parametrii dati
			fid = walk_dir(dir, p, &n);
			
			if(p[0] == 1)					//daca se cer inclusiv fisierele ascunse
				for(i = 0; i < n; i++)
					print_fis(fid[i], max);
			else
				for(i = 0; i < n; i++)
					if(fid[i]->d_name[0] != '.')	//nu se afiseaza fisierele ascunse
						print_fis(fid[i], max);

			closedir(d);
			
			free(fid);
			free(dir);
		}
		
	return 0;
}
