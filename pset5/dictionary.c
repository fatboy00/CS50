/**
 * dictionary.c
 *
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


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


// Prototype
NodeTrie* CreateNewNodeTrie();
bool ClearPtrTrie();

// some variables
NodeTrie* TrieRoot;
NodeTrie* Trie;
int words;



/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // TODO
    // define variables
    int l = strlen(word);
    int counter = 0;
    Trie = TrieRoot; //  Initialise at the top of the branch
    for (int i = 0; i < l; i++)
    {
        counter = word[i] == '\''? 26 : (int) tolower(word[i]) - 'a'; // check for apostrophe
        
        // is this character inititialized?
        Trie = Trie -> child[counter];
        if (Trie == NULL)
        {
            return false;
        }
    }
    if (!(Trie -> is_word) )
    {
        return false;
    }
    return true;
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


    
    // Load the dictionary into a the Trie Structure
    int index = 0, counter = -1;  // initialize various counters
    TrieRoot = CreateNewNodeTrie(); //  create the root node
    Trie = TrieRoot;
    words = 0;
    
    
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
    // allow only alphabetical characters and apostrophes
        if ( c != '\n')
        {

            counter = c == '\''? 26 : (int) tolower(c) - 'a'; // check for apostrophe
            // is this character already inititialized?
            if (Trie -> child[counter] == NULL) 
            {
                Trie -> child[counter] = CreateNewNodeTrie();
            }
                
            Trie = Trie -> child[counter];

            index++;
        }

        // we must have found a whole word
        else
        {
            // prepare for next word
            index = 0;
            words++;
            Trie -> is_word = true;
            Trie = TrieRoot;
        }
    }

    // close the dictionary once loaded
    fclose(fp);
    
    return true;
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
    return ClearPtrTrie(TrieRoot);
}
/**
 * Create a new node for use with a TRIE table
 * 
 */
NodeTrie* CreateNewNodeTrie()
 {
    NodeTrie* Ptr;
    Ptr = (NodeTrie*) malloc(sizeof(NodeTrie)); //malloc/Calloc?
    if (Ptr == NULL) 
    {
        return NULL;
    }
    Ptr -> is_word = false;
    memset(Ptr -> child, 0, sizeof(Ptr -> child));

    return Ptr;
 }
/**
 * Create a clear pointr function for trie
 * 
 */
bool ClearPtrTrie(NodeTrie* Ptr)
{
    for (int i = 0; i < 27; i++)
    {
        if (Ptr -> child[i] != NULL)
        {
            ClearPtrTrie(Ptr -> child[i]);
        } 
    }
    free(Ptr);
    return true;
}
