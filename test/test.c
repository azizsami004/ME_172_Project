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
int is_direct_sentence(int sentence_number);

int is_auxiliary_verb(char str[]);
int is_base_form_of_verb(char str[]);
int is_past_form_of_verb(char str[]);
int is_verb_ing(char str[]);
int is_past_participle(char str[]);

int is_present_indifinite(int index);
int is_present_continuous(int index);
int is_present_perfect(int index);
int is_present_perfect_continuous(int index);
int is_past_indifinite(int index);
int is_past_continuous(int index);
int is_past_perfect(int index);
int is_past_perfect_continuous(int index);
int is_future_indifinite(int index);
int is_future_continuous(int index);
int is_future_perfect(int index);
int is_future_perfect_continuous(int index);




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
    //     printf("\n______________________\n");
    // }

    // for(int i=0; i<sentence_counter; i++) {
    //     int total_words = sentence_with_word_array[i].words_in_sentence;
    //     for(int j=0; j<total_words; j++) {
    //         printf("%s\n", sentence_with_word_array[i].words[j]);
    //     }
    // }

    for(int i=0; i<sentence_counter; i++) {
        
        if(is_present_continuous(i)) {
            printf("Sentence no : %d is present continuous tense\n", i + 1);
        }
        else if(is_present_perfect_continuous(i)) {
            printf("Sentence no : %d is present perfect coontinuous tense\n", i + 1);
        }
        else if(is_present_perfect(i)) {
            printf("Sentence no : %d is present perfect tense\n", i + 1);
        }
        else if(is_present_indifinite(i)) {
            printf("Sentence no : %d is present indifinite tense\n", i + 1);
        }
        else if(is_past_continuous(i)) {
            printf("Sentence no : %d is past continuous tense\n", i + 1);
        }
        else if(is_past_perfect_continuous(i)) {
            printf("Sentence no : %d is past perfect coontinuous tense\n", i + 1);
        }
        else if(is_past_perfect(i)) {
            printf("Sentence no : %d is past perfect tense\n", i + 1);
        }
        else if(is_past_indifinite(i)) {
            printf("Sentence no : %d is past indifinite tense\n", i + 1);
        }
        else if(is_future_continuous(i)) {
            printf("Sentence no : %d is future continuous tense\n", i + 1);
        }
        else if(is_future_perfect_continuous(i)) {
            printf("Sentence no : %d is future perfect coontinuous tense\n", i + 1);
        }
        else if(is_future_perfect(i)) {
            printf("Sentence no : %d is future perfect tense\n", i + 1);
        }
        else if(is_future_indifinite(i)) {
            printf("Sentence no : %d is future indifinite tense\n", i + 1);
        }
        else {
            printf("Sorry unable to detect the tense of sentence no: %d.\n", i + 1);
        }
        
    }
    

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
                    //printf("%s\n", current_word);       // considering end of word
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
        // comparing two words
        if(is_both_equal_word(temp, str)) { 
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
        // comparing two words using
        if(is_both_equal_word(temp, str)) { 
            return 1;
        }
    }
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
            return 1;
        }
    }
    fclose(f);
    return 0;
}


int is_present_indifinite(int index) {
    int verb_index = 1;

    int quotation_index;
    if(is_direct_sentence(index)) {
        
        for(int i=0; i<sentence_with_word_array[index].words_in_sentence; i++) {
            char current_word[1023];
            strcpy(current_word, sentence_with_word_array[index].words[i]);
            if(is_both_equal_word(current_word, "\"")) {
                quotation_index = i;
                break;
            }
        }
        verb_index = quotation_index + 2;
    }

    char verb[1023];
    strcpy(verb, sentence_with_word_array[index].words[verb_index]);

    if(is_base_form_of_verb(verb)) {
        return 1;
    }

    return 0;
}

int is_present_continuous(int index) {
    int subject_index = 0, aux_verb_index = 1, v_ing_index = 2;

    int quotation_index;
    if(is_direct_sentence(index)) {
        
        for(int i=0; i<sentence_with_word_array[index].words_in_sentence; i++) {
            char current_word[1023];
            strcpy(current_word, sentence_with_word_array[index].words[i]);
            if(is_both_equal_word(current_word, "\"")) {
                quotation_index = i;
                break;
            }
        }
        subject_index = quotation_index + 1, aux_verb_index = quotation_index + 2, v_ing_index = quotation_index + 3;
    }
    

    char aux_verb[1023], v_ing[1023];
    strcpy(aux_verb, sentence_with_word_array[index].words[aux_verb_index]);
    strcpy(v_ing, sentence_with_word_array[index].words[v_ing_index]);

    if((is_both_equal_word(aux_verb, "am") || is_both_equal_word(aux_verb, "is") || is_both_equal_word(aux_verb, "are"))&& is_verb_ing(v_ing)) {
        return 1;
    }

    return 0;
}

int is_present_perfect(int index) {
    int subject_index = 0, aux_verb_index = 1, verb_index = 2;

    int quotation_index;
    if(is_direct_sentence(index)) {
        
        for(int i=0; i<sentence_with_word_array[index].words_in_sentence; i++) {
            char current_word[1023];
            strcpy(current_word, sentence_with_word_array[index].words[i]);
            if(is_both_equal_word(current_word, "\"")) {
                quotation_index = i;
                break;
            }
        }
        subject_index = quotation_index + 1, aux_verb_index = quotation_index + 2, verb_index = quotation_index + 3;
    }
    

    char aux_verb[1023], past_participle_verb[1023];
    strcpy(aux_verb, sentence_with_word_array[index].words[aux_verb_index]);
    strcpy(past_participle_verb, sentence_with_word_array[index].words[verb_index]);

    if((is_both_equal_word(aux_verb, "has") || is_both_equal_word(aux_verb, "have")) && is_past_participle(past_participle_verb)) {
        return 1;
    }

    return 0;
}

int is_present_perfect_continuous(int index) {
    int subject_index = 0, aux_verb_index = 1, been_index = 2, v_ing_index = 3;

    int quotation_index;
    if(is_direct_sentence(index)) {
        
        for(int i=0; i<sentence_with_word_array[index].words_in_sentence; i++) {
            char current_word[1023];
            strcpy(current_word, sentence_with_word_array[index].words[i]);
            if(is_both_equal_word(current_word, "\"")) {
                quotation_index = i;
                break;
            }
        }
        subject_index = quotation_index + 1, aux_verb_index = quotation_index + 2, been_index = quotation_index + 3, v_ing_index = quotation_index + 4;
    }
    

    char aux_verb[1023], v_ing[1023], been[1023];
    strcpy(aux_verb, sentence_with_word_array[index].words[aux_verb_index]);
    strcpy(been, sentence_with_word_array[index].words[been_index]);
    strcpy(v_ing, sentence_with_word_array[index].words[v_ing_index]);

    if(((is_both_equal_word(aux_verb, "has") || is_both_equal_word(aux_verb, "have")) && is_both_equal_word(been, "been") )&& is_verb_ing(v_ing)) {
        return 1;
    }

    return 0;
}


int is_past_indifinite(int index) {
    int verb_index = 1;

    int quotation_index;
    if(is_direct_sentence(index)) {
        
        for(int i=0; i<sentence_with_word_array[index].words_in_sentence; i++) {
            char current_word[1023];
            strcpy(current_word, sentence_with_word_array[index].words[i]);
            if(is_both_equal_word(current_word, "\"")) {
                quotation_index = i;
                break;
            }
        }
        verb_index = quotation_index + 2;
    }

    char verb[1023];
    strcpy(verb, sentence_with_word_array[index].words[verb_index]);

    if(is_past_form_of_verb(verb)) {
        return 1;
    }

    return 0;
}

int is_past_continuous(int index) {
    int subject_index = 0, aux_verb_index = 1, v_ing_index = 2;

    int quotation_index;
    if(is_direct_sentence(index)) {
        
        for(int i=0; i<sentence_with_word_array[index].words_in_sentence; i++) {
            char current_word[1023];
            strcpy(current_word, sentence_with_word_array[index].words[i]);
            if(is_both_equal_word(current_word, "\"")) {
                quotation_index = i;
                break;
            }
        }
        aux_verb_index = quotation_index + 2, v_ing_index = quotation_index + 3;
    }
    

    char aux_verb[1023], v_ing[1023];
    strcpy(aux_verb, sentence_with_word_array[index].words[aux_verb_index]);
    strcpy(v_ing, sentence_with_word_array[index].words[v_ing_index]);

    if((is_both_equal_word(aux_verb, "was") || is_both_equal_word(aux_verb, "were"))&& is_verb_ing(v_ing)) {
        return 1;
    }

    return 0;
}

int is_past_perfect(int index) {
    int subject_index = 0, aux_verb_index = 1, v_ing_index = 2;

    int quotation_index;
    if(is_direct_sentence(index)) {
        
        for(int i=0; i<sentence_with_word_array[index].words_in_sentence; i++) {
            char current_word[1023];
            strcpy(current_word, sentence_with_word_array[index].words[i]);
            if(is_both_equal_word(current_word, "\"")) {
                quotation_index = i;
                break;
            }
        }
        aux_verb_index = quotation_index + 2, v_ing_index = quotation_index + 3;
    }
    

    char aux_verb[1023], past_participle_verb[1023];
    strcpy(aux_verb, sentence_with_word_array[index].words[aux_verb_index]);
    strcpy(past_participle_verb, sentence_with_word_array[index].words[v_ing_index]);

    if((is_both_equal_word(aux_verb, "had")) && is_past_participle(past_participle_verb)) {
        return 1;
    }

    return 0;
}

int is_past_perfect_continuous(int index) {
    int subject_index = 0, aux_verb_index = 1, been_index = 2, v_ing_index = 3;

    int quotation_index;
    if(is_direct_sentence(index)) {
        
        for(int i=0; i<sentence_with_word_array[index].words_in_sentence; i++) {
            char current_word[1023];
            strcpy(current_word, sentence_with_word_array[index].words[i]);
            if(is_both_equal_word(current_word, "\"")) {
                quotation_index = i;
                break;
            }
        }
        aux_verb_index = quotation_index + 2, been_index = quotation_index + 3, v_ing_index = quotation_index + 4;
    }
    

    char aux_verb[1023], v_ing[1023], been[1023];
    strcpy(aux_verb, sentence_with_word_array[index].words[aux_verb_index]);
    strcpy(been, sentence_with_word_array[index].words[been_index]);
    strcpy(v_ing, sentence_with_word_array[index].words[v_ing_index]);

    if(((is_both_equal_word(aux_verb, "had")) && is_both_equal_word(been, "been") )&& is_verb_ing(v_ing)) {
        return 1;
    }

    return 0;
}


int is_future_indifinite(int index) {
    int will_index = 1, verb_index = 2;

    int quotation_index;
    if(is_direct_sentence(index)) {
        
        for(int i=0; i<sentence_with_word_array[index].words_in_sentence; i++) {
            char current_word[1023];
            strcpy(current_word, sentence_with_word_array[index].words[i]);
            if(is_both_equal_word(current_word, "\"")) {
                quotation_index = i;
                break;
            }
        }
        will_index = quotation_index + 2, verb_index = quotation_index + 3;
    }

    char verb[1023], will[1023];
    strcpy(verb, sentence_with_word_array[index].words[verb_index]);
    strcpy(will, sentence_with_word_array[index].words[will_index]);

    if(is_both_equal_word("will", will) && is_base_form_of_verb(verb)) {
        return 1;
    }

    return 0;
}

int is_future_continuous(int index) {
    int subject_index = 0, will_index = 1, be_index = 2, v_ing_index = 3;

    int quotation_index;
    if(is_direct_sentence(index)) {
        
        for(int i=0; i<sentence_with_word_array[index].words_in_sentence; i++) {
            char current_word[1023];
            strcpy(current_word, sentence_with_word_array[index].words[i]);
            if(is_both_equal_word(current_word, "\"")) {
                quotation_index = i;
                break;
            }
        }
        will_index = quotation_index + 2, be_index = quotation_index + 3, v_ing_index = quotation_index + 4;
    }
    

    char will[1023], v_ing[1023], be[1023];
    strcpy(will, sentence_with_word_array[index].words[will_index]);
    strcpy(be, sentence_with_word_array[index].words[be_index]);
    strcpy(v_ing, sentence_with_word_array[index].words[v_ing_index]);

    if(((is_both_equal_word(will, "will")) && is_both_equal_word(be, "be") )&& is_verb_ing(v_ing)) {
        return 1;
    }

    return 0;
}

int is_future_perfect(int index) {
    int subject_index = 0, will_index = 1, have_index = 2, verb_index = 3;

    int quotation_index;
    if(is_direct_sentence(index)) {
        
        for(int i=0; i<sentence_with_word_array[index].words_in_sentence; i++) {
            char current_word[1023];
            strcpy(current_word, sentence_with_word_array[index].words[i]);
            if(is_both_equal_word(current_word, "\"")) {
                quotation_index = i;
                break;
            }
        }
        will_index = quotation_index + 2, have_index = quotation_index + 3, verb_index = quotation_index + 4;
    }
    

    char will[1023], verb[1023], have[1023];
    strcpy(will, sentence_with_word_array[index].words[will_index]);
    strcpy(have, sentence_with_word_array[index].words[have_index]);
    strcpy(verb, sentence_with_word_array[index].words[verb_index]);

    if(((is_both_equal_word(will, "will")) && is_both_equal_word(have, "have") )&& is_past_participle(verb)) {
        return 1;
    }

    return 0;
}

int is_future_perfect_continuous(int index) {
    int subject_index = 0, will_index = 1, have_index = 2, been_index = 3, v_ing_index = 4;

    int quotation_index;
    if(is_direct_sentence(index)) {
        
        for(int i=0; i<sentence_with_word_array[index].words_in_sentence; i++) {
            char current_word[1023];
            strcpy(current_word, sentence_with_word_array[index].words[i]);
            if(is_both_equal_word(current_word, "\"")) {
                quotation_index = i;
                break;
            }
        }
        will_index = quotation_index + 2, have_index = quotation_index + 3, been_index = 4, v_ing_index = quotation_index + 5;
    }
    

    char will[1023], v_ing[1023], have[1023], been[1023];
    strcpy(will, sentence_with_word_array[index].words[will_index]);
    strcpy(have, sentence_with_word_array[index].words[have_index]);
    strcpy(v_ing, sentence_with_word_array[index].words[v_ing_index]);
    strcpy(been, sentence_with_word_array[index].words[been_index]);

    if(((is_both_equal_word(will, "will")) && is_both_equal_word(have, "have") && is_both_equal_word("been", been) )&& is_verb_ing(v_ing)) {
        return 1;
    }

    return 0;
}
