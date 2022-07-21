/* COP 3502C Programming Assignment 5
This program is written by: Judah Libera */
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXLEN 1000001

typedef struct trie
{
	int frequancy; // how many of this specific string there are
	int sum; // frequancy including the frequancy of all other strings using this node
	int maxfreq; // the largest of added sums
	struct trie* next[26];
}trie;

void insert(trie* t, char word[], int count, int k);
char* query(trie* t, char word[], int k);
trie* init();
void freemem(trie* t);

void insert(trie* t, char word[], int count, int k)
{
	t->sum += count; //adds up sum as it loops through to proper node

	if (k == strlen(word)) //at proper node
	{
		t->frequancy += count;
		return;
	}

	int index = word[k] - 'a'; //which node to go to
	if (t->next[index] == NULL)
		t->next[index] = init(); //initliaze the node if it hasn't been already

	int a = t->next[index]->sum + count; // gets max frequancy value
	if (a > t->maxfreq) // if thats larger then update
		t->maxfreq = a;

	insert(t->next[index], word, count, k + 1); // reccursive call incrementing position
}

char* query(trie* t, char word[], int k) // returns a string of most likely next characters
{
	if (k == strlen(word)) // once proper node is found
	{
		if (t->maxfreq == 0)
			return NULL;
		int nodes = 0, letind = 0;
		for (int i = 0; i < 26; i++) //finds how many characters the ending string will have
		{
			if (t->next[i] == NULL)
				continue;
			if (t->next[i]->sum == t->maxfreq)
				nodes++;
		}
		char* newword = (char*)malloc(sizeof(char) * (nodes + 1)); //makes space for the string

		for (int i = 0; i < 26; i++) //same call as above but instead ads the character to the newly made string
		{
			if (t->next[i] == NULL)
				continue;
			if (t->next[i]->sum == t->maxfreq)
				newword[letind++] = (char)(i + 'a');
		}
		newword[nodes] = '\0'; //ending null char
		return newword;
	}

	int index = word[k] - 'a'; //gets next node for next letter in the string
	if (t->next[index] == NULL)
		return NULL;

	return query(t->next[index], word, k + 1);
}

trie* init()
{
	trie* temp = (trie*)malloc(sizeof(trie)); //room for node and then make everything 0/null
	temp->frequancy = 0;
	temp->maxfreq = 0;
	temp->sum = 0;
	for (int i = 0; i < 26; i++)
		temp->next[i] = NULL;
	return temp;
}

void freemem(trie* t) // recursive call to go through entire trie then freeing on its way back through the returns
{
	if (t == NULL)
		return;
	for (int i = 26; i < 0; i++)
	{
		if (t->next[i] != NULL)
			freemem(t->next[i]);
	}
	free(t);
	return;
}

int main()
{
	FILE* ifp = fopen("in.txt", "r");
	FILE* ofp = fopen("out.txt", "w");
	int command, count, mainloop;
	int hf = 0, index = 0;
	char word[MAXLEN];
	trie* maintrie = init();

	fscanf(ifp, "%d", &mainloop);
	for (int k = 0; k < mainloop; k++) // runs for each file line
	{
		fscanf(ifp, "%d ", &command); //insert or query
		if (command == 1)
		{
			fscanf(ifp, "%s %d", word, &count);
			insert(maintrie, word, count, 0);
		}
		else
		{
			fscanf(ifp, "%s", word);
			char* freethis = query(maintrie, word, 0); //temp variable to hold the string, is freed immediatly after use
			if (freethis == NULL)
			{
				printf("unknown word\n");
				fprintf(ofp, "unknown word\n");
			}
			else
			{
				printf("%s\n", freethis);
				fprintf(ofp, "%s\n", freethis);
			}
			free(freethis);
		}
	}

	freemem(maintrie); //frees
	fclose(ifp);
	fclose(ofp);
	return 0;
}