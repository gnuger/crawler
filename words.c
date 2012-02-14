#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include "stack.h"

stack *dict[4];

loaddict ()
{
    FILE *dicfile;
    char ch, word[30], filename[] = "dictx", *pword;
    int i = 0, n = 0;

    while (n < 4)
    {
	filename[4] = 48 + n;
	dicfile = fopen (filename, "r");
	
	//dict[n] = malloc (sizeof (struct stack));
	//dict[n]->start = NULL;
	
	dict[n] = newstack();
	
	do
	{
	    ch = getc (dicfile);

	    if (ch == '\n')
	    {
		word[i] = '\0';
		i++;
		pword = malloc (i * sizeof (char));
		strcpy (pword, word);
		push (dict[n], pword);
		i = 0;
	    }
	    else
	    {
		word[i] = ch;
		i++;
	    }
	}
	while (ch != EOF);
	fclose (dicfile);
	n++;
    }
}

int checkword (char *word)
{
    struct node *t_node;
    int n = 0;
    while (n < 4)
    {
	t_node = dict[n]->start;

	while (t_node)
	{
	    if (strcasecmp (word, t_node->value) == 0)
	    {
		return n;
	    }
	    t_node = t_node->next;
	}
	n++;
    }
    return -1;
}

words (char *filename)
{
    FILE *file = fopen (filename, "r");
    char ch, word[30];
    int i = 0, isWord = 0, isAlpha, wordscore[4] = { 0, 0, 0, 0 };
    do
    {
	ch = getc (file);

	while (ch == '<')
	{
	    while (ch != '>')
		ch = getc (file);
	    ch = getc (file);
	}

	isAlpha = (ch > 64 && ch < 91) || (ch > 96 && ch < 123);
	if (!isWord)
	{
	    if (isAlpha)
	    {
		isWord = 1;
		word[i] = ch;
		i++;
	    }
	}
	else
	{
	    if (isAlpha)
	    {
		word[i] = ch;
		i++;
	    }
	    else
	    {
		word[i] = '\0';
		i = 0;
		wordscore[checkword (word)]++;
		isWord = 0;
	    }
	}
    }
    while (ch != EOF);
    fclose (file);

    runquery (buildq ("INSERT INTO wordc VALUES ('%s', %d, %d, %d, %d)", filename, wordscore[0], wordscore[1], wordscore[2], wordscore[3]));
}
