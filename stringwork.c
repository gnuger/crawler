#include <string.h>
#include <malloc.h>


extern char *DOMAIN;

char *adddots (char *dirname)
{
    char *dotedname = malloc ((strlen (dirname) + 3 + 1) * sizeof (char));
    int i = 0;
    *(dotedname + i++) = '.';
    *(dotedname + i++) = '.';
    *(dotedname + i++) = '/';
    while (*dirname != '\0')
    {
	*(dotedname + i++) = *dirname;
	dirname++;
    }
    *(dotedname + i) = '\0';
    return dotedname;
}

char *appendpath (char *rootdir, char *childdir)
{
    char *destdir = malloc ((strlen (rootdir) + strlen (childdir) + 1 + 1) * sizeof (char));
    int i = 0, flag;
    while (*(rootdir + i) != '\0')
    {
	if (*(rootdir + i) == '/' && *(rootdir + i + 1) == '\0')
	    break;
	*(destdir + i) = *(rootdir + i);
	i++;
    }
    if (i == 1 && *destdir == '.')
	i = 0;
    if (i || *childdir == '/')
	*(destdir + i++) = '/';
    flag = 1;
    while (*childdir != '\0')
    {
	if (flag == 1)
	{
	    if (*childdir == '/')
	    {
		childdir++;
		continue;
	    }
	    else if (*childdir == '.')
		flag = 2;
	}
	else if (flag == 2)
	{
	    if (*childdir == '.')
		flag = 3;
	    else if (*childdir == '/')
	    {
		while (*(destdir + --i) != '/' && i != -1);
		i++;
		childdir++;
		flag = 1;
		continue;
	    }
	}
	else if (flag == 3 && *childdir == '/')
	{
	    if (i == 2)
		return "\0";
	    while (*(destdir + --i) != '/');
	    while (*(destdir + --i) != '/' && i != -1);
	    i++;
	    childdir++;
	    flag = 1;
	    continue;
	}
	else
	{
	    flag = 0;
	}
	*(destdir + i++) = *childdir;
	if (*childdir == '/')
	    flag = 1;
	else if (*childdir != '.')
	    flag = 0;
	childdir++;
    }
    if (flag != 0 && i != 1)
	while (i && *(destdir + --i) != '/');
    if (flag == 3)
	while (i && *(destdir + --i) != '/');
    *(destdir + i) = '\0';
    return destdir;
}

int checkhtml (char *filename)
{
    int i = strlen (filename) - 1, j;
    char _html[] = "lmth.";
    j = 0;
    if (*(filename + i) == 'm')
	j = 1;
    while (*(filename + i) == _html[j] && _html[j] != '\0' && i)
    {
	i--;
	j++;
    }
    if (j == 5)
	return 1;
    return 0;
}

char *abslink (char *soulink, char *reflink)
{
    int i = 0;
    char _http[] = "http://";
    char *curdir;
    while (_http[i] != '\0' && _http[i] == *(reflink + i))
	i++;
    if (i == 7)
    {
	reflink += i;
	i = 0;
	while (DOMAIN[i] != '\0' && *(reflink + i) != '/' && *(reflink + i) != '\0')
	{
	    if (DOMAIN[i] != *(reflink + i))
		return NULL;
	    i++;
	}
	if (DOMAIN[i] != '\0' || (*(reflink + i) != '/' && *(reflink + i) != '\0'))
	    return NULL;
	reflink = reflink + i + 1;
	if (*reflink == '\0' || *(reflink + strlen (reflink) - 1) == '/')
	{
	    return appendpath (reflink, "index.html");
	}
	return reflink;
    }
    else
    {
	while (*(reflink + i) != ':' && *(reflink + i) != '\0' && *(reflink + i) != '/')
	    i++;
	if (*(reflink + i) == ':' && *(reflink + i + 1) == '/' && *(reflink + i + 2) == '/')
	    return NULL;
    }
    curdir = appendpath (soulink, "..");
    return appendpath (curdir, reflink);
}
