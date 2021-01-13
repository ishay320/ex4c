#include <stdio.h>
#include <stdlib.h>
#define NUM_LETTERS 26

typedef enum
{
    FALSE = 0,
    TRUE = 1
} boolean;
typedef struct node
{
    char letter;
    long unsigned int count;
    boolean is_word;
    struct node *children[NUM_LETTERS];
} node;

node *newNode();
boolean addWord(node *root, char *word);
char *fixWords(char *word);
int getNextWord(char *txt, char **str, int wordNumber);
boolean addString(node *root, char *txt);

int main(int argc, char const *argv[])
{
    node *root;
    root = newNode();
    addString(root, "ab23 24 khglk 323 2324 242497");
    if (root->children[1] == NULL)
    {
        printf("works");
    }
    free(root);
    return 0;
}

/**
 * @brief makes an empty new node
 * 
 * @return node* 
 */
node *newNode()
{
    node *a = (node *)malloc(sizeof(node));
    a->is_word = FALSE;
    a->count = 0;
    a->letter = '\0';
    for (int i = 0; i < NUM_LETTERS; i++)
    {
        a->children[i] = NULL;
    }
    return a;
}

boolean addString(node *root, char *txt)
{
    int more_words = 1;
    int i = 0;
    char *str = NULL;
    while (more_words)
    {
        i++;
        more_words = getNextWord(txt, &str, i);
        if (more_words)
        {
            addWord(root, str);
        }
        free(str);
    }

    return TRUE;
}

/**
 * @brief Get the Next Word numbered "wordNumber"
 *        starts from 1
 * @param txt the txt to look in
 * @param str the returned word
 * @param wordNumber what number of word to look
 * @return int return 1 if succeeded 0 if no word found
 */
int getNextWord(char *txt, char **str, int wordNumber)
{
    int strSIZE = 10;
    *str = (char *)malloc(sizeof(char) * strSIZE);
    int i = 0;
    int posInWord = 0;
    int passedWord = 0;
    while (*(txt + i) != '\n' && *(txt + i) != '\0' && passedWord != wordNumber)
    {
        if (*(txt + i) != '\t' && *(txt + i) != ' ' && *(txt + i) != '\r')
        {
            if (posInWord > strSIZE - 2)
            {
                strSIZE *= 2;
                *str = (char *)realloc(*str, sizeof(char) * strSIZE);
            }
            *(*str + posInWord++) = *(txt + i++);
            *(*str + posInWord) = '\0';
        }
        else
        {
            if (posInWord)
            {
                passedWord++;
            }
            posInWord = 0;
            i++;
        }
    }
    if (passedWord == wordNumber && wordNumber > 0)
    {
        return 1;
    }
    if (passedWord + 1 == wordNumber && posInWord != 0)
    {
        return 1;
    }
    *(*str) = '\0';
    return 0;
}

/**
 * @brief fill the trie with the word given
 *  
 * @param root the root of the tree
 * @param word the word
 * @return boolean true if successful false if failed
 */
boolean addWord(node *root, char *word)
{
    char *fixedWord = fixWords(word);
    char *f = fixedWord;
    while (*fixedWord != '\0')
    {
        if (root->letter == '\0')
        {
            root->letter = *(fixedWord++);
        }
        else if (root->children[*fixedWord - 'a'] != NULL)
        {
            root = root->children[*fixedWord - 'a'];
        }
        else if (root->children[*fixedWord - 'a'] == NULL)
        {
            root->children[*fixedWord - 'a'] = newNode();
        }
        else
        {
            free(f);
            return FALSE;
        }
    }
    root->is_word = TRUE;
    free(f);
    return TRUE;
}

/**
 * @brief this fuction return the word in lowercase and jump on non letters
 * 
 * @param word the word
 * @return char* of the new word
 */
char *fixWords(char *word)
{
    char *a = word;
    int size = 1;
    while (*a++ != '\0')
    {
        size++;
    }
    char *newWord = (char *)malloc(sizeof(char) * size);
    int i = 0;
    while (*word != '\0')
    {
        if (*word > 'z')
            word++;
        else if (*word < 'a')
        {
            newWord[i] = *word++ + 'a' - 'A';
            if (newWord[i] >= 'a')
            {
                i++;
            }
        }
        else
        {
            newWord[i++] = *word++;
        }
    }
    newWord[i] = '\0';
    return newWord;
}