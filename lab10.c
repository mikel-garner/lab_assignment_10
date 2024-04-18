//Author: Mikel Garner

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie structure
struct Trie
{
    struct Trie *children[ALPHABET_SIZE];
    int count; // Number of occurrences of the word ending at this node
};

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *trie = malloc(sizeof(struct Trie));
    if (trie) {
        trie->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            trie->children[i] = NULL;
        }
    }
    return trie;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie *current = pTrie;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (current->children[index] == NULL) {
            current->children[index] = createTrie();
        }
        current = current->children[index];
    }
    current->count++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct Trie *current = pTrie;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (current->children[index] == NULL) {
            return 0; // Word not found
        }
        current = current->children[index];
    }
    return current->count;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
   if (pTrie) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            deallocateTrie(pTrie->children[i]);
        }
        free(pTrie);
    }
    return NULL;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    char word[100]; // Assuming max word length is 100 characters
    while (fscanf(file, "%s", word) != EOF) {
        pInWords[count] = strdup(word); // Store a copy of the word
        count++;
    }

    fclose(file);
    return count;
}

int main(void)
{
	char *inWords[256];

	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}

	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse line by line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}
