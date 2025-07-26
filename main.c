#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

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
int is_noun(char str[]);
int is_adjective(char str[]);
int is_adverb(char str[]);


int is_auxiliary_verb(char str[]);
int is_base_form_of_verb(char str[]);
int is_past_form_of_verb(char str[]);
int is_verb_ing(char str[]);
int is_past_participle(char str[]);

// Features Functions
void sentence_capitalization();
void ending_punctuation();
void repeated_word_check();
void missing_articles();

int is_present_indefinite(int index);
int is_present_continuous(int index);
int is_present_perfect(int index);
int is_present_perfect_continuous(int index);
int is_past_indefinite(int index);
int is_past_continuous(int index);
int is_past_perfect(int index);
int is_past_perfect_continuous(int index);
int is_future_indefinite(int index);
int is_future_continuous(int index);
int is_future_perfect(int index);
int is_future_perfect_continuous(int index);
void detect_tense();
int is_passive_voice(int index);
void detect_voice();


int main() {
    // Defining Files
    FILE *input, *sentences;
    input = fopen("input.txt", "r");
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
    sentence_capitalization();
    repeated_word_check();
    missing_articles();
    ending_punctuation();

    detect_tense();
    detect_voice();

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


void create_array_of_sentence_with_word() {
    for(int i=0; i<sentence_counter; i++) {
        char current_sentence[1024];
        strcpy(current_sentence, sentence_array[i].real_sentence);
        int current_sentence_length = strlen(sentence_array[i].real_sentence);
        char current_word[200];
        int k = 0, word_count = 0;
        for(int j=0; j<current_sentence_length; j++) {
            if(current_sentence[j] == ' ') {
                if(strlen(current_word) == 0) {
                    continue;
                }
                else {
                    //printf("%s\n", current_word);
                    strcpy(sentence_with_word_array[i].words[word_count++], current_word);
                    memset(current_word, 0, sizeof(current_word));
                    k = 0;
                }
            }
            else {
                if(current_sentence[j] == '"' || current_sentence[j] == '\'' || current_sentence[j] == ',' || current_sentence[j] == '.' || current_sentence[j] == '?' || current_sentence[j] == '!') {
                    strcpy(sentence_with_word_array[i].words[word_count++], current_word);
                    memset(current_word, 0, sizeof(current_word));
                    k = 0;
                    current_word[k++] = current_sentence[j];       // treating start of new word
                    // printf("%s\n", current_word);
                    strcpy(sentence_with_word_array[i].words[word_count++], current_word);
                    memset(current_word, 0, sizeof(current_word));
                    k = 0;
                    j++;
                }
                current_word[k++] = current_sentence[j];
            }
        }
        sentence_with_word_array[i].sentence_number = i;
        sentence_with_word_array[i].words_in_sentence = word_count;
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
        // comparing two words
        if(is_both_equal_word(temp, str)) { 
            fclose(nouns);
            return 1;
        }
    }
    fclose(nouns);
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
        // comparing two words
        if(is_both_equal_word(temp, str)) { 
            fclose(adjective);
            return 1;
        }
    }
    fclose(adjective);
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
        // comparing two words using
        if(is_both_equal_word(temp, str)) { 
            fclose(adverb);
            return 1;
        }
    }
    fclose(adverb);
    return 0;
}


int is_article(char str[]) {
    if(strlen(str) < 1 || strlen(str) > 3) return 0;
    if(strlen(str) == 2 && tolower(str[0]) == 'a' && tolower(str[1]) == 'n') return 1;
    else if(strlen(str) == 3 && tolower(str[0]) == 't' && tolower(str[1]) == 'h' && tolower(str[2]) == 'e') return 1;
    else if(strlen(str) == 1 && tolower(str[0]) == 'a') return 1;
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


// Writing Functions
void sentence_capitalization() {
    fprintf(result, "This is capitalization error..\n\n");
    for(int i=0; i<sentence_counter; i++) {
        char temp = sentence_array[i].real_sentence[0];
        if(temp >= 97 && temp <= 122) {
            // Error
            sentence_array[i].real_sentence[0] = toupper(sentence_array[i].real_sentence[0]);
            fprintf(result, "'%c' should be capital letter at sentence no. %d.\t->\t%s", temp, sentence_array[i].sentence_number, sentence_array[i].real_sentence);
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
                    fprintf(result, "'%c' should be capital letter at sentence no. %d.\t->\t%s", curr_sen[j+1], sentence_array[i].sentence_number, sentence_array[i].real_sentence);
                }
            }
        }
    }
    fprintf(result, "\n\n");
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
            char last_char = current_sentence[sentence_lenght-2];  // /0
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
                if(is_both_equal_word(wh_words[j], first_word) == 1) {
                    is_first_word_wh = 1;
                }
            }
            for(int j=0; j<26; j++) {
                if(is_both_equal_word(auxiliary_verbs[j], second_word) == 1) {
                    is_second_word_aux_v = 1;
                }
            }
            // checking if interrogative sentence
            if(is_first_word_wh && is_second_word_aux_v) {
                if(last_char != '?') {
                    fprintf(result, "Punctuation error at sentence no. '%d'. Correct puntuation will be '?'.\t->\t%s", sentence_array[i].sentence_number, sentence_array[i].real_sentence);
                }
            }
            else if(is_auxiliary_verb(first_word)) {
                if(last_char != '?') {
                    fprintf(result, "Punctuation error at sentence no. '%d'. Correct puntuation will be '?'.\t->\t%s", sentence_array[i].sentence_number, sentence_array[i].real_sentence);
                }
            }
            // for exclamatory sentence
            // wh-word + "a"/"an"
            else if(is_first_word_wh && (is_both_equal_word(second_word, "a") || is_both_equal_word(second_word, "an"))) {
                if(last_char != '!') {
                    fprintf(result, "Punctuation error at sentence no. '%d'. Correct puntuation will be '!'.\t->\t%s", sentence_array[i].sentence_number, sentence_array[i].real_sentence);
                }
            }
            // wh-word + adjective/adverb
            else if(is_first_word_wh && (is_adjective(second_word) || is_adverb(second_word))) {
                if(last_char != '!') {
                    fprintf(result, "Punctuation error at sentence no. '%d'. Correct puntuation will be '!'.\t->\t%s", sentence_array[i].sentence_number, sentence_array[i].real_sentence);
                }
            }
            else {
                if(last_char != '.') {
                    fprintf(result, "Punctuation error at sentence no. '%d'. Correct puntuation will be '.'.\t->\t%s", sentence_array[i].sentence_number, sentence_array[i].real_sentence);
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
                if(last_char != '?') {
                    fprintf(result, "Punctuation error at sentence no. '%d'. Correct puntuation will be '?'.\t->\t%s", sentence_array[i].sentence_number, sentence_array[i].real_sentence);
                }
            }
            else if(is_auxiliary_verb(first_word)) {
                if(last_char != '?') {
                    fprintf(result, "Punctuation error at sentence no. '%d'. Correct puntuation will be '?'.\t->\t%s", sentence_array[i].sentence_number, sentence_array[i].real_sentence);
                }
            }

            // for exclamatory sentence
            // wh-word + "a"/"an"
            else if(is_first_word_wh && (is_both_equal_word(second_word, "a") || is_both_equal_word(second_word, "an"))) {
                if(last_char != '!') {
                    fprintf(result, "Punctuation error at sentence no. '%d'. Correct puntuation will be '!'.\t->\t%s", sentence_array[i].sentence_number, sentence_array[i].real_sentence);
                }
            }
            // wh-word + adjective/adverb
            else if(is_first_word_wh && (is_adjective(second_word) || is_adverb(second_word))) {
                if(last_char != '!') {
                    fprintf(result, "Punctuation error at sentence no. '%d'. Correct puntuation will be '!'.\t->\t%s", sentence_array[i].sentence_number, sentence_array[i].real_sentence);
                }
            }
            else {
                if(last_char != '.') {
                    fprintf(result, "Punctuation error at sentence no. '%d'. Correct puntuation will be '.'.\t->\t%s", sentence_array[i].sentence_number, sentence_array[i].real_sentence);
                }
            }
        }
    }
    fprintf(result, "\n\n");
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
                if(strlen(curr_str) >= 1 && is_both_equal_word(curr_str, prev_str)) {
                    // printf("%s\t%s\n", prev_str, curr_str);
                    fprintf(result, "Repeated words at sentence no. %d and it is '%s'.\t->\t%s", sentence_array[i].sentence_number, curr_str, sentence_array[i].real_sentence);
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

// noun + article
void missing_articles(){
    fprintf(result, "This is Article error..\n\n");
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
                        //printf("%d %s\n",strlen(prev_str), prev_str);
                        fprintf(result, "The article is missing in sentence no. %d.\t->\t%s", sentence_array[i].sentence_number, sentence_array[i].real_sentence);
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


// from hereee

int is_auxiliary_verb(char str[]) {
    char *auxiliary_verbs[] = {"am", "is", "are", "was", "were", "being", "been", "have", "has", "had", "having", "do", "does", "did", "will", "shall", "can", "could", "may", "might", "must", "would", "should", "ought to", "need", "dare"};
    for(int i=0; i<26; i++) {
        if(is_both_equal_word(auxiliary_verbs[i], str)) {
            return 1;
        }
    }
    return 0;
}

int is_base_form_of_verb(char str[]) {
    FILE *f;
    f = fopen("verb_base_form.txt", "r");
    if(f == NULL) {
        printf("Could not open verb_base_form.txt file\n");
        return 0;
    }
    char temp[1023];
    while (fscanf(f, "%s", temp) == 1)
    {
        if(is_both_equal_word(str, temp)) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int is_past_form_of_verb(char str[]) {
    FILE *f;
    f = fopen("verb_past_form.txt", "r");
    if(f == NULL) {
        printf("Could not open verb_past_form.txt file\n");
        return 0;
    }
    char temp[1023];
    while (fscanf(f, "%s", temp) == 1)
    {
        if(is_both_equal_word(str, temp)) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int is_verb_ing(char str[]) {
    FILE *f;
    f = fopen("verb_ing.txt", "r");
    if(f == NULL) {
        printf("Could not open verb_ing.txt file\n");
        return 0;
    }
    char temp[1023];
    while (fscanf(f, "%s", temp) == 1)
    {
        if(is_both_equal_word(str, temp)) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int is_past_participle(char str[]) {
    FILE *f;
    f = fopen("past_participle_form.txt", "r");
    if(f == NULL) {
        printf("Could not open past_participle_form.txt file\n");
        return 0;
    }
    char temp[1023];
    while (fscanf(f, "%s", temp) == 1)
    {
        if(is_both_equal_word(str, temp)) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

// ------------------------ Detecting Tense ---------------

int is_present_indefinite(int index) {
    int verb_index = 1;

    for(int i=0; i<sentence_with_word_array[index].words_in_sentence; i++) {
        verb_index = i;
        char verb[1023];
        strcpy(verb, sentence_with_word_array[index].words[verb_index]); 
        if(is_base_form_of_verb(verb)) {
            return 1;
        }
    }

    return 0;
}

int is_present_continuous(int index) {
    int subject_index = 0, aux_verb_index = 1, v_ing_index = 2;

    for(int i=0; i<sentence_with_word_array[index].words_in_sentence - 1; i++) {
        aux_verb_index = i;
        v_ing_index = i + 1;
        char aux_verb[1023], v_ing[1023];
        strcpy(aux_verb, sentence_with_word_array[index].words[aux_verb_index]);
        strcpy(v_ing, sentence_with_word_array[index].words[v_ing_index]);

        if((is_both_equal_word(aux_verb, "am") || is_both_equal_word(aux_verb, "is") || is_both_equal_word(aux_verb, "are"))&& is_verb_ing(v_ing)) {
            return 1;
        }
    }

    return 0;
}

int is_present_perfect(int index) {
    int subject_index = 0, aux_verb_index = 1, verb_index = 2;

    for(int i=0; i<sentence_with_word_array[index].words_in_sentence - 1; i++) {
        aux_verb_index = i, verb_index = i + 1;
        char aux_verb[1023], past_participle_verb[1023];
        strcpy(aux_verb, sentence_with_word_array[index].words[aux_verb_index]);
        strcpy(past_participle_verb, sentence_with_word_array[index].words[verb_index]);

        if((is_both_equal_word(aux_verb, "has") || is_both_equal_word(aux_verb, "have")) && is_past_participle(past_participle_verb)) {
            return 1;
        }
    }
    
    return 0;
}

int is_present_perfect_continuous(int index) {
    int subject_index = 0, aux_verb_index = 1, been_index = 2, v_ing_index = 3;

    for(int i=0; i<sentence_with_word_array[index].words_in_sentence - 2; i++) {
        aux_verb_index = i, been_index = i + 1, v_ing_index = i + 2;
        char aux_verb[1023], v_ing[1023], been[1023];

        strcpy(aux_verb, sentence_with_word_array[index].words[aux_verb_index]);
        strcpy(been, sentence_with_word_array[index].words[been_index]);
        strcpy(v_ing, sentence_with_word_array[index].words[v_ing_index]);

        if(((is_both_equal_word(aux_verb, "has") || is_both_equal_word(aux_verb, "have")) && is_both_equal_word(been, "been") )&& is_verb_ing(v_ing)) {
            return 1;
        }
    }
    
    return 0;
}


int is_past_indefinite(int index) {
    int verb_index = 1;

    for(int i=0; i<sentence_with_word_array[index].words_in_sentence; i++) {
        char verb[1023];
        strcpy(verb, sentence_with_word_array[index].words[verb_index]);

        if(is_past_form_of_verb(verb)) {
            return 1;
        }
    }

    return 0;
}

int is_past_continuous(int index) {
    int subject_index = 0, aux_verb_index = 1, v_ing_index = 2;

    for(int i=0; i<sentence_with_word_array[index].words_in_sentence - 1; i++) {
        aux_verb_index = i, v_ing_index = i + 1;
        char aux_verb[1023], v_ing[1023];
        strcpy(aux_verb, sentence_with_word_array[index].words[aux_verb_index]);
        strcpy(v_ing, sentence_with_word_array[index].words[v_ing_index]);

        if((is_both_equal_word(aux_verb, "was") || is_both_equal_word(aux_verb, "were"))&& is_verb_ing(v_ing)) {
            return 1;
        }

    }
    
    return 0;
}

int is_past_perfect(int index) {
    int subject_index = 0, aux_verb_index = 1, v_ing_index = 2;

    for(int i=0; i<sentence_with_word_array[index].words_in_sentence - 1; i++) {
        aux_verb_index = i, v_ing_index = i + 1;
        char aux_verb[1023], past_participle_verb[1023];
        strcpy(aux_verb, sentence_with_word_array[index].words[aux_verb_index]);
        strcpy(past_participle_verb, sentence_with_word_array[index].words[v_ing_index]);

        if((is_both_equal_word(aux_verb, "had")) && is_past_participle(past_participle_verb)) {
            return 1;
        }
    }

    return 0;
}

int is_past_perfect_continuous(int index) {
    int subject_index = 0, aux_verb_index = 1, been_index = 2, v_ing_index = 3;

    for(int i=0; i<sentence_with_word_array[index].words_in_sentence - 2; i++) {
        aux_verb_index = i, been_index = i + 1, v_ing_index = i + 2;
        char aux_verb[1023], v_ing[1023], been[1023];
        strcpy(aux_verb, sentence_with_word_array[index].words[aux_verb_index]);
        strcpy(been, sentence_with_word_array[index].words[been_index]);
        strcpy(v_ing, sentence_with_word_array[index].words[v_ing_index]);

        if(((is_both_equal_word(aux_verb, "had")) && is_both_equal_word(been, "been") )&& is_verb_ing(v_ing)) {
            return 1;
        }
    }

    return 0;
}


int is_future_indefinite(int index) {
    int will_index = 1, verb_index = 2;

    for(int i=0; i<sentence_with_word_array[index].words_in_sentence - 1; i++) {
        will_index = i, verb_index = i + 1;
        char verb[1023], will[1023];
        strcpy(verb, sentence_with_word_array[index].words[verb_index]);
        strcpy(will, sentence_with_word_array[index].words[will_index]);

        if(is_both_equal_word("will", will) && is_base_form_of_verb(verb)) {
            return 1;
        }
    }

    return 0;
}

int is_future_continuous(int index) {
    int subject_index = 0, will_index = 1, be_index = 2, v_ing_index = 3;

    for(int i=0; i<sentence_with_word_array[index].words_in_sentence - 2; i++) {
        will_index = i, be_index = i + 1, v_ing_index = i + 2;
        char will[1023], v_ing[1023], be[1023];
        strcpy(will, sentence_with_word_array[index].words[will_index]);
        strcpy(be, sentence_with_word_array[index].words[be_index]);
        strcpy(v_ing, sentence_with_word_array[index].words[v_ing_index]);

        if(((is_both_equal_word(will, "will")) && is_both_equal_word(be, "be") )&& is_verb_ing(v_ing)) {
            return 1;
        }
    }

    return 0;
}

int is_future_perfect(int index) {
    int subject_index = 0, will_index = 1, have_index = 2, verb_index = 3;

    for(int i=0; i<sentence_with_word_array[index].words_in_sentence - 2; i++) {
        will_index = i, have_index = i + 1, verb_index = i + 2;
        char will[1023], verb[1023], have[1023];

        strcpy(will, sentence_with_word_array[index].words[will_index]);
        strcpy(have, sentence_with_word_array[index].words[have_index]);
        strcpy(verb, sentence_with_word_array[index].words[verb_index]);

        if(((is_both_equal_word(will, "will")) && is_both_equal_word(have, "have") )&& is_past_participle(verb)) {
            return 1;
        }
    }

    return 0;
}

int is_future_perfect_continuous(int index) {
    int subject_index = 0, will_index = 1, have_index = 2, been_index = 3, v_ing_index = 4;

    for(int i=0; i<sentence_with_word_array[index].words_in_sentence - 3; i++) {
        will_index = i, have_index = i + 1, been_index = i + 2, v_ing_index = i + 3;
        char will[1023], v_ing[1023], have[1023], been[1023];

        strcpy(will, sentence_with_word_array[index].words[will_index]);
        strcpy(have, sentence_with_word_array[index].words[have_index]);
        strcpy(v_ing, sentence_with_word_array[index].words[v_ing_index]);
        strcpy(been, sentence_with_word_array[index].words[been_index]);

        if(((is_both_equal_word(will, "will")) && is_both_equal_word(have, "have") && is_both_equal_word("been", been) )&& is_verb_ing(v_ing)) {
            return 1;
        }
    }

    return 0;
}


void detect_tense() {
    fprintf(result, "Detecting Tense..\n\n");
    for(int i=0; i<sentence_counter; i++) {
        // checking present tense
        if(is_future_continuous(i)) {
            fprintf(result, "Sentence no : %d is future continuous tense.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
        else if(is_future_perfect_continuous(i)) {
            fprintf(result, "Sentence no : %d is future perfect coontinuous tense.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
        else if(is_future_perfect(i)) {
            fprintf(result, "Sentence no : %d is future perfect tense.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
        else if(is_future_indefinite(i)) {
            fprintf(result, "Sentence no : %d is future indefinite tense.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
        // checking past tense
        else if(is_past_continuous(i)) {
            fprintf(result, "Sentence no : %d is past continuous tense.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
        else if(is_past_perfect_continuous(i)) {
            fprintf(result, "Sentence no : %d is past perfect coontinuous tense.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
        else if(is_past_perfect(i)) {
            fprintf(result, "Sentence no : %d is past perfect tense.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
        else if(is_past_indefinite(i)) {
            fprintf(result, "Sentence no : %d is past indefinite tense.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
        // checking present tense
        else if(is_present_continuous(i)) {
            fprintf(result, "Sentence no : %d is present continuous tense.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
        else if(is_present_perfect_continuous(i)) {
            fprintf(result, "Sentence no : %d is present perfect coontinuous tense.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
        else if(is_present_perfect(i)) {
            fprintf(result, "Sentence no : %d is present perfect tense.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
        else if(is_present_indefinite(i)) {
            fprintf(result, "Sentence no : %d is present indefinite tense.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
        else {
            fprintf(result, "Sorry unable to detect the tense of sentence no: %d.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
    }
    fprintf(result, "\n\n");
}

// ------------------------------Detecting Voice-----------------------------

int is_passive_voice(int index) {
    int aux_verb_index = 0, be_index = 0, been_index = 0, verb_index = 0, by_index = 0;

    int words_counter = sentence_with_word_array[index].words_in_sentence;

    char current_words[1024][100];
    for (int i = 0; i < 1024; i++) {
        strcpy(current_words[i], sentence_with_word_array[index].words[i]);
    }


    // for simple present and past tense
    for(int i=0; i<words_counter - 2; i++) {
        aux_verb_index = i, verb_index = i + 1, by_index = i + 2;
        if(is_auxiliary_verb(current_words[aux_verb_index]) && is_past_participle(current_words[verb_index]) && is_both_equal_word(current_words[by_index], "by")) {
            return 1;
        }
    }
    // simple future tense
    for(int i=0; i<words_counter - 3; i++) {
        aux_verb_index = i, be_index = i + 1, verb_index = i + 2, by_index = i + 3;
        if(is_auxiliary_verb(current_words[aux_verb_index]) && is_both_equal_word(current_words[be_index], "be") && is_past_participle(current_words[verb_index]) && is_both_equal_word(current_words[by_index], "by")) {
            return 1;
        }
    }

    // continuous tenses
    for(int i=0; i<words_counter - 3; i++) {
        aux_verb_index = i, be_index = i + 1, verb_index = i + 2, by_index = i + 3;
        if(is_auxiliary_verb(current_words[aux_verb_index]) && is_both_equal_word(current_words[be_index], "being") && is_past_participle(current_words[verb_index]) && is_both_equal_word(current_words[by_index], "by")) {
            return 1;
        }
    }

    // present and past perfect tense
    for(int i=0; i<words_counter - 3; i++) {
        aux_verb_index = i, be_index = i + 1, verb_index = i + 2, by_index = i + 3;
        if(is_auxiliary_verb(current_words[aux_verb_index]) && is_both_equal_word(current_words[be_index], "been") && is_past_participle(current_words[verb_index]) && is_both_equal_word(current_words[by_index], "by")) {
            return 1;
        }
    }
    //future perfect tense
    int have_index = 0;
    for(int i=0; i<words_counter; i++) {
        aux_verb_index = i, have_index = i + 1, be_index = i + 2, verb_index = i + 3, by_index = i + 4;
        if(is_auxiliary_verb(current_words[aux_verb_index]) && is_both_equal_word(current_words[have_index], "have") && is_both_equal_word(current_words[be_index], "been") && is_past_participle(current_words[verb_index]) && is_both_equal_word(current_words[by_index], "by")) {
            return 1;
        }
    }

    return 0;
}

void detect_voice() {
    fprintf(result, "Detecting Voice..\n\n");
    for(int i=0; i<sentence_counter; i++) {
        if(is_passive_voice(i)) {
            fprintf(result, "Sentence no: %d is a passive voice.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
        else {
            fprintf(result, "Sentence no: %d is an active voice.\t->\t%s", i + 1, sentence_array[i].real_sentence);
        }
    }
    fprintf(result, "\n\n");
}