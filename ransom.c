#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readline();
char **split_string(char*);
void checkMagazine(int, char**, int, char**);
struct map *create_map(int, char**);
void free_map(struct map*);

struct map_node {
    char *word;
    int reps;
    struct map_node *next;
};

struct map {
    struct map_node *head;
    struct map_node *tail;
};

int word_exists(char*, struct map_node*);

// Complete the checkMagazine function below.
/*
* This function gets a number of words and a pointer to the beginning of a note and a magazine, and checks if we can build the note with only words from the magazine.
* the magazine_count and note_count parameters define the number of words in the note and the magazine.
* the magazine and note pointers, points to the beggining of the strings array.
* We will print "Yes" if the magazine contains all the words we need, and we will print "NO" else.
*/
void checkMagazine(int magazine_count, 
                   char **magazine,
                   int note_count, 
                   char **note) {

    struct map *bagM = create_map(magazine_count, magazine);
    struct map *bagN = create_map(note_count, note);
    
    struct map_node *temp_N = bagN->head;
    struct map_node *temp_M = bagM->head;
    int word_on_magazine = 0;
    while(temp_N) {
        word_on_magazine = 0;
        temp_M = bagM->head;
        while(temp_M) {
            if(!strcmp(temp_N->word,temp_M->word)) {
                word_on_magazine = 1;
                if(temp_N->reps > temp_M->reps) {
                    free_map(bagN);
                    free_map(bagM);
                    printf("No");
                    return;
                }
            }
            temp_M = temp_M->next;
        }
        if(word_on_magazine == 0) {
            free_map(bagN);
            free_map(bagM);
            printf("No");
            return;
        }
        temp_N = temp_N->next;
    }
    free_map(bagN);
    free_map(bagM);
    printf("Yes");
    return;
}

/*
* This function using free to free the allocated memory of the maps we built.
* The parameter it gets is the pointer to the map we want to free.
*/
void free_map(struct map *map_to_free) {
    struct map_node *node = map_to_free->head;
    while(node) {
        struct map_node *temp = node;
        node = node->next;
        free(temp);
    }
    free(map_to_free);
    return;
}

/*
* This function creates linked list in which each node hold a word that appears in the text, and the number of the appearences.
* the parameter words_count is the number of words in the text given. 
* the parameter text, is a pointer to the beggining of the array of strings.
* the function returns a pointer to struct (the map) we created, in which we have a pointer to the "head" node and to the "tail" node.
*/
struct map *create_map(int words_count, 
                       char **text) {
    struct map *map_ret = malloc(sizeof(struct map));
    if(!map_ret) {
        exit(1);
    }
    struct map_node *cur_node = malloc(sizeof(struct map_node));
    if(!(cur_node)) {
        free(map_ret);
        exit(1);
    }
    map_ret->head = cur_node;
    map_ret->tail = cur_node;
    cur_node->word = text[0];
    cur_node->next = NULL;
    cur_node->reps = 1;

    struct map_node *new_node;

    for(int i=1;i<words_count;i++) {
        if(!(word_exists(text[i],map_ret->head))) continue;
        else {
            new_node = malloc(sizeof(struct map_node));
            if(!(new_node)) {
                free_map(map_ret);
                exit(1);
            }
            map_ret->tail->next = new_node;
            map_ret->tail = new_node;
            new_node->word = text[i];
            new_node->reps = 1;
        }
    }
    return map_ret;
}

/* Function comment:
* This function get a word from the text, and checks if there is a key in the map which refers this word. If the word already exists on the map we build, we will add 1 to its node's repetitions field. 
* new_word is a parameter which points to the string we want to check.
* head is the pointer to the first node of the map we build.
* The function returns 0 if we found that the word already exists in our map, and 1 if it does not exist.
*/
int word_exists(char *new_word, 
                struct map_node *head) {
    struct map_node *temp = head;
    while (temp) {
        if(!strcmp(new_word,temp->word)) {
            temp->reps++;
            return 0;  /* word exists */
        }
        temp = temp->next;
    }
    return 1; /* word does not exists */
}


int main()
{
    char** mn = split_string(readline());

    char* m_endptr;
    char* m_str = mn[0];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;
    char* n_str = mn[1];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline());

    char** magazine = malloc(m * sizeof(char*));

    for (int i = 0; i < m; i++) {
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline());

    char** note = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    checkMagazine(magazine_count, magazine, note_count, note);
    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}



char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}
