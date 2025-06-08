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

// Prototyping Functions
// Helper functions
void sentence_output(FILE *input, FILE *output);
void sentence_print(char *line, FILE *output);
int is_noun(char str[]);
int is_adjective(char str[]);
int is_adverb(char str[]);

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
    result = fopen("output.txt", "w");

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
    repeated_word_check();
    missing_articles();
    ending_punctuation();

    

    // Closing files
    fclose(input);
    fclose(sentences);
    fclose(ss);
    fclose(result);


    return 0;
}

void sentence_print(char *line, FILE *output) {
    int start = 0;
    int len = strlen(line);

    for(int i=0; i<len; i++) {
        // checking for direct sentence
        if((line[i] == '.' || line[i] == '?' || line[i] == '!') && line[i+1] == '"') continue;
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

int is_noun(char str[]) {
    FILE* nouns;
    nouns = fopen("noun_2.txt", "r");
    if(nouns == NULL) {
        printf("Error opening file\n");
        return 0;
    }
    char temp[1024];
    while(fscanf(nouns, "%s", temp) == 1) {
        // comparing two words using strcmp() function. if strcmp() returns 0 then both words are same.
        if(strcmp(temp, str) == 0) { 
            return 1;
        }
    }
    return 0;
}

int is_adjective(char str[]) {
    FILE* adjective;
    adjective = fopen("adjective.txt", "r");
    if(adjective == NULL) {
        printf("Error opening file\n");
        return 0;
    }
    char temp[1024];
    while(fscanf(adjective, "%s", temp) == 1) {
        // comparing two words using strcmp() function. if strcmp() returns 0 then both words are same.
        if(strcmp(temp, str) == 0) { 
            return 1;
        }
    }
    return 0;
}

int is_adverb(char str[]) {
    FILE* adverb;
    adverb = fopen("adverb.txt", "r");
    if(adverb == NULL) {
        printf("Error opening file\n");
        return 0;
    }
    char temp[1024];
    while(fscanf(adverb, "%s", temp) == 1) {
        // comparing two words using strcmp() function. if strcmp() returns 0 then both words are same.
        if(strcmp(temp, str) == 0) { 
            return 1;
        }
    }
    return 0;
}



int is_article(char str[]) {
    if(strlen(str) < 1 || strlen(str) > 3) return 0;
    if(str[0] == 'a' && str[1] == 'n') return 1;
    else if(str[0] == 't' && str[1] == 'h' && str[2] == 'e') return 1;
    else if(str[0] == 'a') return 1;
    else return 0;
}

int is_direct_sentence(int sentence_number) {
    int flag = 0;
    int quotation_counter = 0;
    for(int i=0; i<strlen(sentence_array[sentence_number].real_sentence); i++) {
        if(sentence_array[sentence_number].real_sentence[i] == '"') {
            quotation_counter++;
        }
    }
    if(quotation_counter != 0) {
        return 1;
    }
    return 0;
}

bool is_both_equal_word(char a[], char b[]) {
    if(strlen(a) != strlen(b)) return 0;
    for(int i=0; i<strlen(a); i++) {
        if(tolower(a[i]) != tolower(b[i])) return 0;
    }
    return 1;
}

// Writing Functions
void sentence_capitalization() {
    fprintf(result, "This is capitalization error..\n\n");
    for(int i=0; i<sentence_counter; i++) {
        char temp = sentence_array[i].real_sentence[0];
        if(temp >= 97 && temp <= 122) {
            // Error
            sentence_array[i].real_sentence[0] = toupper(sentence_array[i].real_sentence[0]);
            fprintf(result, "'%c' should be capital letter at sentence no. %d.  %s\n", temp, sentence_array[i].sentence_number, sentence_array[i].real_sentence);
        }
        sentence_array[i].real_sentence[0] = toupper(sentence_array[i].real_sentence[0]);
        printf("%d -> %s\n", sentence_array[i].sentence_number, sentence_array[i].real_sentence);

        // for direct sentence
        int flag = 0;
        char curr_sen[1024];
        strcpy(curr_sen, sentence_array[i].real_sentence);
        for(int j=0; j<strlen(curr_sen)-1; j++) {
            if(flag == 0 && curr_sen[j] == '"') {
                if(curr_sen[j+1] >= 97 && curr_sen[j+1] <= 122) {
                    sentence_array[i].real_sentence[j+1] = toupper(sentence_array[i].real_sentence[j+1]);
                    fprintf(result, "'%c' should be capital letter at sentence no. %d.  %s\n", curr_sen[j+1], sentence_array[i].sentence_number, sentence_array[i].real_sentence);
                }
            }
        }
    }
}

void ending_punctuation() {
    fprintf(result, "This is Punctuation error..\n\n");

    char *wh_words[] = {"what", "when", "where", "who", "whom", "which", "whose", "why", "how"};
    char *auxiliary_verbs[] = {"am", "is", "are", "was", "were", "being", "been", "have", "has", "had", "having", "do", "does", "did", "will", "shall", "can", "could", "may", "might", "must", "would", "should", "ought to", "need", "dare"};

    for(int i=0; i<sentence_counter; i++) {
        int sentence_lenght = strlen(sentence_array[i].real_sentence);
        char current_sentence[1024];
        for(int j=0; j<1024; j++) {
            current_sentence[j] = '\0';
        }
        strcpy(current_sentence, sentence_array[i].real_sentence);
        // interogative sentence
        // for indirect sentence
        if(!is_direct_sentence(i)) {
            // wh-word + auxiliary verb
            // checking if 1st word is wh-word
            char first_word[1024], second_word[1024];
            char last_char = current_sentence[sentence_lenght-2];
            for(int j=0; j<1024; j++) {
                first_word[j] = '\0';
                second_word[j] = '\0';
            }
            int f_i = 0, s_i = 0, blank_cnt = 0;
            for(int j=0; j<sentence_lenght; j++) {
                char current_character = sentence_array[i].real_sentence[j];
                if(blank_cnt == 2) break;
                else if(blank_cnt == 0) {
                    if(current_character != ' ') {
                        first_word[f_i] = current_character;
                        f_i++;
                    }
                    else {
                        blank_cnt++;
                    }
                }
                else {
                    if(current_character != ' ') {
                        second_word[s_i] = current_character;
                        s_i++;
                    }
                    else {
                        blank_cnt++;
                    }
                }
            }
            int is_first_word_wh = 0, is_second_word_aux_v = 0;
            for(int j=0; j<9; j++) {
                if(is_both_equal_word(wh_words[j], first_word)) {
                    is_first_word_wh = 1;
                }
            }
            for(int j=0; j<26; j++) {
                if(is_both_equal_word(auxiliary_verbs[j], second_word)) {
                    is_second_word_aux_v = 1;
                }
            }
            // checking if interrogative sentence
            if(is_first_word_wh && is_second_word_aux_v) {
                // printf("%s\t%s\t%c\n", first_word, second_word, last_char);
                if(last_char != '?') {
                    fprintf(result, "Punctualtion error at sentence no. '%d'. Correct puntuation will be '?'.\n", sentence_array[i].sentence_number);
                }
            }
            // printf("%s\t%d\t%s\t%d %c\n", first_word, strlen(first_word), second_word, strlen(second_word), last_char);



            // for exclamatory sentence
            // wh-word + "a"/"an"
            else if(is_first_word_wh && (strcmp(second_word, "a") == 0 || strcmp(second_word, "an") == 0)) {
                if(last_char != '!') {
                    fprintf(result, "Punctualtion error at sentence no. '%d'. Correct puntuation will be '!'.\n", sentence_array[i].sentence_number);
                }
            }
            // wh-word + adjective/adverb
            else if(is_first_word_wh && (is_adjective(second_word) || is_adverb(second_word))) {
                if(last_char != '!') {
                    fprintf(result, "Punctualtion error at sentence no. '%d'. Correct puntuation will be '!'.\n", sentence_array[i].sentence_number);
                }
            }
            else {
                if(last_char != '.') {
                    fprintf(result, "Punctualtion error at sentence no. '%d'. Correct puntuation will be '.'.\n", sentence_array[i].sentence_number);
                }
            }

        }
        else {
            // for direct sentence. 
            char first_word[1024], second_word[1024];
            char sentence_inside_quotation[1024];
            for(int j=0; j<1024; j++) {
                first_word[j] = '\0';
                second_word[j] = '\0';
                sentence_inside_quotation[j] = '\0';
            }
            int is_opening_quotation_encountered = 0, c_i = 0;
            for(int j=0; j<sentence_lenght; j++) {
                if(is_opening_quotation_encountered && current_sentence[j] == '"') break;
                if(is_opening_quotation_encountered && current_sentence[j] != '"') sentence_inside_quotation[c_i++] = current_sentence[j];
                if(!is_opening_quotation_encountered && current_sentence[j] == '"') is_opening_quotation_encountered = 1;
            }
            // printf("%s\n", sentence_inside_quotation);
            int f_i = 0, s_i = 0, blank_cnt = 0;
            int inside_quote_sentence_lenght = strlen(sentence_inside_quotation);
            char last_char = sentence_inside_quotation[inside_quote_sentence_lenght - 1];
            for(int j=0; j<inside_quote_sentence_lenght; j++) {
                char current_character = sentence_inside_quotation[j];
                if(blank_cnt == 2) break;
                else if(blank_cnt == 0) {
                    if(current_character != ' ') {
                        first_word[f_i] = current_character;
                        f_i++;
                    }
                    else {
                        blank_cnt++;
                    }
                }
                else {
                    if(current_character != ' ') {
                        second_word[s_i] = current_character;
                        s_i++;
                    }
                    else {
                        blank_cnt++;
                    }
                }
            }
            int is_first_word_wh = 0, is_second_word_aux_v = 0;
            for(int j=0; j<9; j++) {
                if(is_both_equal_word(wh_words[j], first_word)) {
                    is_first_word_wh = 1;
                }
            }
            for(int j=0; j<26; j++) {
                if(is_both_equal_word(auxiliary_verbs[j], second_word)) {
                    is_second_word_aux_v = 1;
                }
            }
            // checking if interrogative sentence
            if(is_first_word_wh && is_second_word_aux_v) {
                // printf("%s\t%s\t%c\n", first_word, second_word, last_char);
                if(last_char != '?') {
                    fprintf(result, "Punctualtion error at sentence no. '%d'. Correct puntuation will be '?'.\n", sentence_array[i].sentence_number);
                }
            }
            // printf("%s\t%d\t%s\t%d %c\n", first_word, strlen(first_word), second_word, strlen(second_word), last_char);



            // for exclamatory sentence
            // wh-word + "a"/"an"
            else if(is_first_word_wh && (strcmp(second_word, "a") == 0 || strcmp(second_word, "an") == 0)) {
                if(last_char != '!') {
                    fprintf(result, "Punctualtion error at sentence no. '%d'. Correct puntuation will be '!'.\n", sentence_array[i].sentence_number);
                }
            }
            // wh-word + adjective/adverb
            else if(is_first_word_wh && (is_adjective(second_word) || is_adverb(second_word))) {
                if(last_char != '!') {
                    fprintf(result, "Punctualtion error at sentence no. '%d'. Correct puntuation will be '!'.\n", sentence_array[i].sentence_number);
                }
            }
            else {
                if(last_char != '.') {
                    fprintf(result, "Punctualtion error at sentence no. '%d'. Correct puntuation will be '.'.\n", sentence_array[i].sentence_number);
                }
            }
        }
    }
}

void repeated_word_check(){
    fprintf(result, "This is Repeated Word error..\n\n");
    for(int i=0; i<sentence_counter; i++) {
        char curr_sentence[1024];
        strcpy(curr_sentence, sentence_array[i].real_sentence);
        int sen_length = strlen(curr_sentence);
        char curr_str[1024], prev_str[1024];
        int k=0;
        for(int j=0; j<sen_length; j++) {
            if(curr_sentence[j] == ' ' || curr_sentence[j] == ',' || curr_sentence[j] == '"' || curr_sentence[j] == '.' || curr_sentence[j] == '?' || curr_sentence[j] == '!' || curr_sentence[j] == '\n') {
                // end of a word
                if(strlen(curr_str) >= 1 && strcmp(curr_str, prev_str) == 0) {
                    // printf("%s\t%s\n", prev_str, curr_str);
                    fprintf(result, "Repeated words at sentence no. %d and it is '%s'.\n", sentence_array[i].sentence_number, curr_str);
                }
                if(curr_str != '\0') {
                    strcpy(prev_str, curr_str);
                }
                // erasing the current string
                for(int p=0; p<=1023; p++) {
                    curr_str[p] = '\0';
                }
                k=0;
            }
            else {
                curr_str[k++] = curr_sentence[j];
            }
        }
    }
    fprintf(result, "\n\n");
}

void missing_articles(){
    fprintf(result, "This is Artice error..\n\n");
    for(int i=0; i<sentence_counter; i++) {
        char curr_sentence[1024];
        strcpy(curr_sentence, sentence_array[i].real_sentence);
        int sen_length = strlen(curr_sentence);
        char curr_str[1024], prev_str[1024];
        int k=0;
        for(int j=0; j<sen_length; j++) {
            if(curr_sentence[j] == ' ' || curr_sentence[j] == ',' || curr_sentence[j] == '"' || curr_sentence[j] == '.' || curr_sentence[j] == '?' || curr_sentence[j] == '!' || curr_sentence[j] == '\n') {
                // end of a word
                // printf("%s\n", curr_str);
                if(is_noun(curr_str)) {
                    // printf("%s\t%s\t", prev_str, curr_str);
                    if(!is_article(prev_str)) {
                        //printf("The article error is in the sentence no. %d\n", sentence_array[i].sentence_number);
                        printf("%d %s\n",strlen(prev_str), prev_str);
                        fprintf(result, "The artice is missing in sentence no. %d..\n", sentence_array[i].sentence_number);
                    }
                }
                if(curr_str != '\0') {
                    strcpy(prev_str, curr_str);
                }
                // erasing the current string
                for(int p=0; p<=1023; p++) {
                    curr_str[p] = '\0';
                }
                k=0;
            }
            else {
                curr_str[k++] = curr_sentence[j];
            }
        }
    }
    fprintf(result, "\n\n");
}