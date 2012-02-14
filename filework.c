#include <stdio.h>

int file_exists (const char *filename)
{
    FILE *fp;
    if (fp = fopen (filename, "r"))
    {
	fclose (fp);
	return 1;
    }
    return 0;
}

int findpattern (FILE * file, char *pattern)
{
    int i;
    char c;
    do
    {
	i = 0;
	c = fgetc (file);
	if (c == *(pattern + i))
	{
	    while (*(pattern + ++i) != '\0' && (c = fgetc (file)) != EOF && *(pattern + i) == c);
	    if (*(pattern + i) == '\0' && c != EOF)
		return 1;
	}
    }
    while (c != EOF);
    return 0;
}
