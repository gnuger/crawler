#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

char *DOMAIN;

findlinks (char *soulink)
{
    FILE *soufile = fopen (soulink, "r");
    char c, _link[1024], *reflink;
    int i;
    while (findpattern (soufile, "<a "))
    {
	i = 0;
	if (findpattern (soufile, "href"))
	{
	    while ((c = fgetc (soufile)) != '\"' && c != EOF);
	    if (c == EOF)
		break;
	    while ((c = fgetc (soufile)) != '\"' && c != EOF)
	    {
		_link[i++] = c;
	    }
	    if (c == EOF)
		break;
	    _link[i] = '\0';
	    reflink = abslink (soulink, _link);
	    if (reflink)
	    {
		if (checkhtml (reflink) && file_exists (reflink))
		    insertref (soulink, reflink);
	    }
	}
    }
    fclose (soufile);
}

parsedir (char *basedir)
{
    DIR *dir;
    struct dirent *entry;
    char *curdir = calloc (1, sizeof (char)), *htmlfile;
    //struct stack *s_list = malloc (sizeof (struct stack));
    //s_list->start = NULL;
    stack *s_list = newstack();
    int flag = 0;
    
    initdb ();
    push (s_list, basedir);
    while (s_list->start)
    {
	curdir = appendpath (curdir, pop (s_list));
	dir = opendir (curdir);
	while (entry = readdir (dir))
	{
	    if (entry->d_type == DT_DIR && strcmp (entry->d_name, ".") != 0 && strcmp (entry->d_name, "..") != 0)
	    {
		if (flag)
		{
		    if (s_list->start)
			push (s_list, adddots (pop (s_list)));
		}
		else
		{
		    flag = 1;
		}
		push (s_list, entry->d_name);
	    }
	    else if (entry->d_type == DT_REG)
	    {
		if (checkhtml (entry->d_name))
		{
		    htmlfile = appendpath (curdir, entry->d_name);
		    runquery (buildq ("INSERT INTO pages (link) VALUES ('%s')", htmlfile));
		    words (htmlfile);
		    findlinks (htmlfile);
		}
	    }
	}
	closedir (dir);
    }
    closedb ();
}

int main (int argc, char *argv[])
{
    char *PATH = ".";
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "p:d:")) != -1)
	switch (c)
	{
	case 'p':
	    PATH = optarg;
	    break;
	case 'd':
	    DOMAIN = optarg;
	    break;
	case '?':
	    if (optopt == 'p')
		fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	    else if (optopt == 'd')
		fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	    else if (isprint (optopt))
		fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	    else
		fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
	    return 1;
	default:
	    return 1;
	}

    if (!DOMAIN)
    {
	fprintf (stderr, "Usage: crawler [-p ROOT_PATH] -d DOMAIN\n");
	return 1;
    }
    
    loaddict ();
    
    parsedir (PATH);
    return 0;
}
