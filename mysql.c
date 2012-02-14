#include <mysql.h>
#include <stdarg.h>
#include <stdio.h>

char *itoa (unsigned int num)
{
    static char buffer[10];
    char *na;
    if (num == 0)
	return "0\0";

    na = &buffer[sizeof (buffer) - 1];
    *na = '\0';
    while (num != 0)
    {
	*--na = num % 10 + 48;
	num /= 10;
    }
    return na;
}

char *buildq (char *qfmt, ...)
{
    static char buffer[1024];
    char *query = buffer, *ts;
    int ti, i = 0;
    va_list argp;
    va_start (argp, qfmt);

    while (*qfmt != '\0')
    {
	if (*qfmt != '%')
	{
	    *(query + i++) = *qfmt++;
	    continue;
	}
	qfmt++;
	switch (*qfmt)
	{
	case 'd':
	    ti = va_arg (argp, int);
	    if (ti < 0)
	    {
		ti = -ti;
		*(query + i++) = '-';
	    }
	    ts = itoa (ti);
	    while (*ts != '\0')
		*(query + i++) = *ts++;
	    break;
	case 's':
	    ts = va_arg (argp, char *);
	    while (*ts != '\0')
		*(query + i++) = *ts++;
	    break;
	}
	qfmt++;
    }
    *(query + i) = '\0';
    return query;
}

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

initdb ()
{
    char *server = "localhost";
    char *user = "root";
    char *password = "voldemort";
    char *database = "test";
    conn = mysql_init (NULL);
    if (!mysql_real_connect (conn, server, user, password, database, 0, NULL, 0))
    {
	fprintf (stderr, "%s\n", mysql_error (conn));
    }
}

runquery (char *query)
{
    if (mysql_query (conn, query))
    {
	fprintf (stderr, "%s\n", mysql_error (conn));
    }
}

insertref (char *soulink, char *reflink)
{
    char *uquery;
    if (mysql_query (conn, buildq ("SELECT id FROM refernce WHERE soulink = '%s' AND reflink = '%s'", soulink, reflink)))
    {
	fprintf (stderr, "%s\n", mysql_error (conn));
	return;
    }
    res = mysql_use_result (conn);
    if ((row = mysql_fetch_row (res)) != NULL)
    {
	uquery = buildq ("UPDATE refernce SET count = count + 1 WHERE id = %s", row[0]);
	mysql_free_result (res);
	runquery (uquery);
    }
    else
	runquery (buildq ("INSERT INTO refernce (soulink, reflink) VALUES ('%s', '%s')", soulink, reflink));
}

closedb ()
{
    mysql_free_result (res);
    mysql_close (conn);
}
