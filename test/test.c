#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Global variables
int sentence_counter = 0;
FILE *result;


// Creating a array of sentence structure.
struct sentence {
    int sentence_number;
    char real_sentence[1024];
} sentence_array[1000+1];

struct Sentence_with_Word {
    int sentence_number;
    int words_in_sentence;
    char words[1024][100];
} sentence_with_word_array[1001];

// Prototyping Functions
// Helper functions
int is_both_equal_word(char a[], char b[]);
void sentence_output(FILE *input, FILE *output);
void sentence_print(char *line, FILE *output);
void create_array_of_sentence_with_word();


int main() {
    // Defining Files
    FILE *input, *sentences;
    input = fopen("simple_input.txt", "r");
    sentences = fopen("sentences.txt", "w");
    result = fopen("output.txt", "w");

    // Creating a text file where every line has only one sentence. So that we can work easily with sentences
    sentence_output(input, sentences);
    // Closing sentences file because it has to open again in reading mode
    fclose(sentences);

    // Taking input from the sentence file and creating an array of sentence structure
    FILE *ss;
    ss = fopen("sentences.txt", "r");

    char temp[1024];
    int cnt = 0;
    while(fgets(temp, sizeof(temp), ss) != NULL) {
        sentence_array[cnt].sentence_number = cnt + 1;
        strcpy(sentence_array[cnt].real_sentence, temp);
        cnt++;
    }

    create_array_of_sentence_with_word();

    // for(int i=0; i<sentence_counter; i++) {
    //     for(int j=0; j<sentence_with_word_array[i].words_in_sentence; j++) {
    //         printf("%s\t", sentence_with_word_array[i].words[j]);
    //     }
    //     printf("______________________\n");
    // }
    

    // Closing files
    fclose(input);
    fclose(sentences);
    fclose(ss);
    fclose(result);

    return 0;
}

int is_both_equal_word(char a[], char b[]) {
    if(strlen(a) != strlen(b)) return 0;
    for(int i=0; i<strlen(a); i++) {
        if(tolower(a[i]) != tolower(b[i])) return 0;
    }
    return 1;
}

void sentence_print(char *line, FILE *output) {
    int start = 0;
    int len = strlen(line);

    for(int i=0; i<len; i++) {
        // checking for direct sentence
        // to handle -> "I was bad.", he said.
        if((line[i] == '.' || line[i] == '?' || line[i] == '!') && line[i+1] == '"') continue;
        if(line[i] == ',' && line[i+1] == ' ' && line[i+2] == '"') {    // to handle -> He said, "I was bad."
            for(int j=i; j<len; j++) {
                if((line[j] == '.' || line[j] == '?' || line[j] == '!') && line[j+1] == '"') {
                    // Add punctuation at this index
                    int end = j+1;
                    char sentence[2048];
                    // Length of sentence
                    int sen_len = end - start + 1;
                    // strncpy() assigns a sentence of sen_len length to sentence[1024] array
                    strncpy(sentence, line + start, sen_len);
                    // at last index of the sentence a null character is added so that end of sentence can be indentified
                    sentence[sen_len] = '\0';
                    // counting sentence
                    sentence_counter++;
                    // writing sentence in desired file
                    fprintf(output, "%s\n", sentence);

                    // as we done with a sentence. now start of a new sentence will begin from the next character. using loop we are omitting spaces, new lines and tabs
                    start = end + 1;
                    while(start < len && (line[start] == ' ' || line[start] == '\n' || line[start] == '\t')) {
                        start++;
                    }
                    // new value of i will be start of new sentence
                    i = start - 1;
                }
            }
        }
        // Checking end of line
        if(line[i] == '.' || line[i] == '?' || line[i] == '!') {
            // Add punctuation at this index
            int end = i;
            char sentence[2048];
            // Length of sentence
            int sen_len = end - start + 1;
            // strncpy() assigns a sentence of sen_len length to sentence[1024] array
            strncpy(sentence, line + start, sen_len);
            // at last index of the sentence a null character is added so that end of sentence can be indentified
            sentence[sen_len] = '\0';
            // counting sentence
            sentence_counter++;
            // writing sentence in desired file
            fprintf(output, "%s\n", sentence);

            // as we done with a sentence. now start of a new sentence will begin from the next character. using loop we are omitting spaces, new lines and tabs
            start = end + 1;
            while(start < len && (line[start] == ' ' || line[start] == '\n' || line[start] == '\t')) {
                start++;
            }
            // new value of i will be start of new sentence
            i = start - 1;
        }
    }
    // if any character is left the we have to add them into new sentenece. so we are checking if any character/word is left. then we assign it to new sentence[1024] variable and write it to ouput text file
    if(start < len) {
        char sentence[2048];
        strncpy(sentence, line + start, len - start);
        sentence[len - start] = '\0';
        if(strlen(sentence) > 0 && sentence[0] != '\n') {
            fprintf(output, "%s\n", sentence);
        }
    }
}


void sentence_output(FILE *input, FILE *output) {
    // Checking files
    if(input == NULL || output == NULL) {
        printf("Error opening file\n");
        return;
    }
    printf("Creating a file where a line has only one sentence......\n");
    char line[1024];
    // taking input a line at a time from input file. using sentence_print() function to print them a sentence in a line
    while(fgets(line, sizeof(line), input) != NULL) {
        sentence_print(line, output);
    }
}

