#include <stdio.h>
#include <stdlib.h>
#define NUM_LETTERS 26
#define BIGEST_WORD 256
#define LINE_SIZE 1024
#define NUM_OF_LINES 256

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
boolean addWord(node *root, char *word, int wordNumber);
char *fixWords(char *word);
int getNextWord(char *txt, char **str, int wordNumber);
int addString(node *root, char *txt, int offset);
boolean removeTrie(node *root);
void printTrie(node *root, char f);
void printRec(node *root, char *n, char f, int posInN);

int main(int argc, char const *argv[])
{
    char f = 'q';
    if (argc > 1)
    {
        f = *argv[1];
    }
    node *root;
    root = newNode();

    char line[LINE_SIZE] = {0};
    int lineCount = 1;
    int offset = 0;

    while (lineCount++ != NUM_OF_LINES)
    {
        if (fgets(line, LINE_SIZE, stdin) == 0) //gets the line
        {
            break;
        }
        int wordRead = addString(root, line, offset); //add the line to the trie
        if (wordRead == -1)                           //check for errors
        {
            printf("error");
            if (!removeTrie(root))
            {
                printf("failed to free the trie");
                return -1;
            }
            return -1;
        }
        offset += wordRead;
    }
    printTrie(root, f);
    if (!removeTrie(root))
    {
        printf("failed to free the trie");
        return -1;
    }
    return 0;
}

/**
 * @brief print the trie by command
 * 
 * @param root the root of the trie
 * @param f the command
 */
void printTrie(node *root, char f)
{
    char c[BIGEST_WORD] = {0};
    printRec(root, c, f, 0);
}

/**
 * @brief its an recursive function that maneged by printTrie
 */
void printRec(node *root, char *n, char f, int posInN)
{
    if (root->is_word)
    {
        printf("%s %ld\n", n, root->count);
    }
    if (f == 'r')
    {
        for (int i = NUM_LETTERS - 1; i >= 0; i--)
        {
            if (root->children[i] != NULL)
            {
                n[posInN] = root->children[i]->letter;
                n[posInN + 1] = '\0';
                printRec(root->children[i], n, f, posInN + 1);
            }
        }
    }
    else
    {
        for (int i = 0; i < NUM_LETTERS; i++)
        {
            if (root->children[i] != NULL)
            {
                n[posInN] = root->children[i]->letter;
                n[posInN + 1] = '\0';
                printRec(root->children[i], n, f, posInN + 1);
            }
        }
    }
}

/**
 * @brief makes an empty new node
 * 
 * @return node* and null if failed 
 */
node *newNode()
{
    node *a = (node *)malloc(sizeof(node));
    if (a == NULL)
    {
        return NULL;
    }

    a->is_word = FALSE;
    a->count = 0;
    a->letter = '\0';
    for (int i = 0; i < NUM_LETTERS; i++)
    {
        a->children[i] = NULL;
    }
    return a;
}

/**
 * @brief recursive function that deconstruct the trie from the pos given + the pos given
 *  in O(n)
 * 
 * @param root the pos in the trie to deconstruct
 * @return boolean if succeeded
 */
boolean removeTrie(node *root)
{
    if (root == NULL)
    {
        return TRUE;
    }
    for (int i = 0; i < NUM_LETTERS; i++)
    {
        boolean c = removeTrie(root->children[i]);
        if (!c)
        {
            free(root);
            return FALSE;
        }
    }
    free(root);
    return TRUE;
}

/**
 * @brief add a string of words to the trie
 * 
 * @param root the base of the trie
 * @param txt the txt to put in
 * @param offset the offset in the word numbering
 * @return return the num of word read -1 if failed
 */
int addString(node *root, char *txt, int offset)
{
    int more_words = 1;
    int i = 0;
    char *str = NULL;
    int word_number_for_return = 0;
    while (more_words)
    {
        i++;
        more_words = getNextWord(txt, &str, i);
        if (more_words)
        {
            if (addWord(root, str, i + offset))
            {
                word_number_for_return += 1;
            }
            else
            {
                offset--;
            }
        }
        free(str);
        if (more_words == -1)
        {
            return -1;
        }
    }

    return word_number_for_return;
}

/**
 * @brief Get the Next Word numbered "wordNumber"
 *        starts from 1
 * @param txt the txt to look in
 * @param str the returned word
 * @param wordNumber what number of word to look
 * @return int return 1 if succeeded 0 if no word found -1 if failed
 */
int getNextWord(char *txt, char **str, int wordNumber)
{
    int strSIZE = 10;
    *str = (char *)malloc(sizeof(char) * strSIZE);
    if (*str == NULL)
    {
        return -1;
    }

    int i = 0;
    int posInWord = 0;
    int passedWord = 0;
    char *tmp; //for checking realloc
    while (*(txt + i) != '\n' && *(txt + i) != '\0' && passedWord != wordNumber)
    {
        if (*(txt + i) != '\t' && *(txt + i) != ' ' && *(txt + i) != '\r')
        {
            if (posInWord > strSIZE - 2) //if the word is too big
            {
                strSIZE *= 2;
                tmp = (char *)realloc(*str, sizeof(char) * strSIZE);
                if (tmp == NULL)
                    return -1;
                else
                    *str = tmp;
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
 * @param wordNumber the pos of the word in the txt
 * @param root the root of the tree
 * @param word the word
 * @return boolean true if successful false if failed
 */
boolean addWord(node *root, char *word, int wordNumber)
{
    char *fixedWord = fixWords(word);
    if (*fixedWord == '\0')
    {
        free(fixedWord);
        return FALSE;
    }

    // first we step over the first true root of the trie
    if (*fixedWord != '\0' && root->children[*fixedWord - 'a'] != NULL)
    {
        root = root->children[*fixedWord - 'a'];
    }
    else if (*fixedWord != '\0' && root->children[*fixedWord - 'a'] == NULL)
    {
        root->children[*fixedWord - 'a'] = newNode();
        if (root->children[*fixedWord - 'a'] == NULL)
        {
            free(fixedWord);
            return FALSE;
        }
        root = root->children[*fixedWord - 'a'];
    }

    if (fixedWord == NULL) //check for failed malloc
    {
        free(fixedWord);
        return FALSE;
    }
    int i = 0;
    char *f = fixedWord;
    while (*fixedWord != '\0')
    {
        if (root->letter == *fixedWord)
        {
            fixedWord++;
            i++;
        }
        if (root->letter == '\0')
        {
            root->letter = *(fixedWord++);
            //root->count = i++;
        }
        if (*fixedWord != '\0' && root->children[*fixedWord - 'a'] != NULL)
        {
            root = root->children[*fixedWord - 'a'];
        }
        else if (*fixedWord != '\0' && root->children[*fixedWord - 'a'] == NULL)
        {
            root->children[*fixedWord - 'a'] = newNode();
            if (root->children[*fixedWord - 'a'] == NULL)
            {
                free(f);
                return FALSE;
            }
            root = root->children[*fixedWord - 'a'];
        }
    }
    root->is_word = TRUE;
    root->count = wordNumber;
    free(f);
    return TRUE;
}

/**
 * @brief this fuction return the word in lowercase and jump on non letters
 * 
 * @param word the word
 * @return char* of the new word, null if fail
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
    if (newWord == NULL)
    {
        return NULL;
    }
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