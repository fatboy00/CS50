/**
 * dictionary.c
 *
 * Implements a dictionary's functionality using Hash table
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Size of Hash Table
#define HASHTABLESIZE 10000


#include "dictionary.h"


/**
 * create the linked list structure for words in the dictionary
 * 
 */


typedef struct NodeTrie
{
    bool is_word;
    struct NodeTrie* child[27];
}
NodeTrie;

typedef struct NodeHash
{
    char word[LENGTH + 1];
    struct NodeHash* next;
}
NodeHash;





// Prototype
NodeHash* CreateNewNodeHash();
unsigned long HashFunction();
bool ClearHashTable();
bool ClearPtrHash();

// some variables
NodeTrie* TrieRoot;
NodeTrie* Trie;
int words;
NodeHash* HashTable[HASHTABLESIZE];



/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* passedword)
{
    // TODO
    // define variables
    int hash;
    NodeHash* TempPtr;
    bool FoundWord = false;
    char word[LENGTH + 1];
    int l = strlen(passedword);

    for (int i = 0; i< l + 1; i++)
    {
        word[i] = tolower(passedword[i]); // convert the word to lower case.
    }
    
    hash = HashFunction(word, HASHTABLESIZE);
    TempPtr = HashTable[hash];

    
    while (TempPtr != NULL && FoundWord == false)
    {
        if (strcmp(TempPtr -> word, word) == 0)
        {
            FoundWord = true;
        } 
        else
        {
            TempPtr = TempPtr -> next;
        }
    }
    return FoundWord;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // TODO
    // Open the dictionary
    FILE* fp = fopen(dictionary, "r");

    //  check the dictionary exists...
    if (fp == NULL)
    {
        return false;
    }
    //  Initialise the Hash table
    for ( int i = 0; i < HASHTABLESIZE; i++)
    {
        HashTable[i] = NULL;
    }

    // Load the dictionary into a Hash Table Structure
    words = 0;
    int hash;
    NodeHash* TempPtr;
    TempPtr = CreateNewNodeHash();
    
    while (fscanf(fp,"%s", TempPtr -> word) != EOF && TempPtr != NULL)
    {
        words++;
        hash = HashFunction(TempPtr -> word, HASHTABLESIZE);

        if (HashTable[hash] == NULL) // create a node that attaches directly to the hash table
        {
            HashTable[hash] = TempPtr;
        } else // create a new node attached to the previous hash table node
        {
            TempPtr -> next = HashTable[hash];
            HashTable[hash] = TempPtr;
        }
        TempPtr = CreateNewNodeHash();
        
    }

    // close the dictionary once loaded
    fclose(fp);
    if (TempPtr == NULL) 
    {
        free(TempPtr);
        return false;
    }
    else
    {
        free(TempPtr);
        return true;
    }
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    // words was counted in dictioanry load
    return words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    return ClearHashTable();
}

/**
 * Create a new node for use with a HASH table
 * 
 */
 NodeHash* CreateNewNodeHash()
 {
    NodeHash* temp;
    temp = (NodeHash*) malloc(sizeof(NodeHash)); //malloc/Calloc?
    if (temp == NULL) 
    {
        return NULL;
    }
    temp -> next = NULL;
    return temp;
 }
/**
 * Create a clear Hashtable Function
 * 
 */
bool ClearHashTable()
{
        int i = 0;
        while (i < HASHTABLESIZE)
        {
            if (HashTable[i] != NULL)
            {
                ClearPtrHash(HashTable[i]);
            }
            i++;
        }

    return true;
}
/**
 * Create a clear pointr function for hash
 * 
 */
bool ClearPtrHash(NodeHash* Ptr)
{
        if (Ptr -> next != NULL)
        {
            ClearPtrHash(Ptr -> next);
        } 
    free(Ptr);
    return true;
}

/*
 * Create HASH Function
 * dbj2 Hash function
 * http://www.cs.dartmouth.edu/~campbell/cs50/hash.c
 * http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned long HashFunction(char* key, unsigned long size)
{
    unsigned long hash = 5381;
    int i = 0;
    
    while (( i = *key++) !=0)
    {
        hash = ((hash << 5) + hash) + i; /* hash * 33 + c */
    } 
    hash %= size;
    return hash;
}
