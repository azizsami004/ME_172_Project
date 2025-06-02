#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Global variables
int sentence_counter = 0;

// Creating a array of sentence structure.
struct sentence {
    int sentence_number;
    char real_sentence[1024];
} sentence_array[1000+1];

// Prototyping Functions
// Helper functions
void sentence_output(FILE *input, FILE *output);
void sentence_print(char *line, FILE *output);

// Features Functions
void sentence_capitalization();
void ending_punctuation();
void repeated_word_check();
void missing_articles();


int main() {
    // Defining Files
    FILE *input, *sentences;
    input = fopen("simple_input.txt", "r");
    sentences = fopen("simple_output.txt", "w");

    // Creating a text file where every line has only one sentence. So that we can work easily with sentences
    sentence_output(input, sentences);
    // Closing sentences file because it has to open again in reading mode
    fclose(sentences);

    // Taking input from the sentence file and creating an array of sentence structure
    FILE *ss;
    ss = fopen("simple_output.txt", "r");

    char temp[1024];
    int cnt = 0;
    while(fgets(temp, sizeof(temp), ss) != NULL) {
        sentence_array[cnt].sentence_number = cnt + 1;
        strcpy(sentence_array[cnt].real_sentence, temp);
        cnt++;
    }

    // for(int i=0; i<sentence_counter; i++) {
    //     printf("%d -> %s\n", sentence_array[i].sentence_number, sentence_array[i].real_sentence);
    // }

    sentence_capitalization();


    
    // Closing files
    fclose(input);
    fclose(sentences);
    fclose(ss);


    return 0;
}

void sentence_print(char *line, FILE *output) {
    int start = 0;
    int len = strlen(line);

    for(int i=0; i<len; i++) {
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

// Writing Functions
void sentence_capitalization() {
    FILE *result;
    result = fopen("output.txt", "w");
    fprintf(result, "This is capitalization error..\n");
    for(int i=0; i<sentence_counter; i++) {
        char temp = sentence_array[i].real_sentence[0];
        if(temp >= 97 && temp <= 122) {
            // Error
            sentence_array[i].real_sentence[0] = toupper(sentence_array[i].real_sentence[0]);
            fprintf(result, "%c should be capital letter at sentence no. %d.  %s\n", temp, sentence_array[i].sentence_number, sentence_array[i].real_sentence);
        }
        sentence_array[i].real_sentence[0] = toupper(sentence_array[i].real_sentence[0]);
        printf("%d -> %s\n", sentence_array[i].sentence_number, sentence_array[i].real_sentence);

    }
}

void ending_punctuation() {

}

void repeated_word_check(){

}

void missing_articles(){

}