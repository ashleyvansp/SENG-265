/* 	
	Name: Ashley Van Spankeren
	Date: January 30, 2018
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#define LINE_LENGTH 110
#define FILE_LENGTH 150
#define KEYS_LENGTH 25
#define PHRASE_LENGTH 110


/*  
	Takes a string as input
	capitalizes every letter in the string
*/
void capitalize(char *token){
	char caps[LINE_LENGTH];
	memset(caps, '\0', LINE_LENGTH);
	strncpy(caps, token, strlen(token));
	for (int i = 0; i < strlen(token); i++){
		caps[i] = toupper(caps[i]);
	}
	strncpy(token, caps, strlen(token));
}

void print_token(char *token){
	if(token[strlen(token)-1] == '\n'){
		token[strlen(token)-1] = '\0';
		printf("%s\n",token);
	}
	else
		printf("%s ", token);
}
/* 
	Indexes the phrases
*/
void index(char (*phrases)[LINE_LENGTH], char (*words)[LINE_LENGTH], int phrase_len, int words_len){
	char temp[LINE_LENGTH];
	char *token;
	// for word in non-key array
	for (int word_idx = 0; word_idx < words_len; word_idx++){
		// for phrase in phrases
		for (int phrase_idx = 0; phrase_idx < phrase_len; phrase_idx++){
			// for word in phrase (tokenize)
			memset(temp, '\0', LINE_LENGTH);
			strncpy(temp, phrases[phrase_idx], strlen(phrases[phrase_idx]));
			token = strtok(temp, " ");
			while (token != NULL){
				// If word is in phrase
				if (strcmp(token, words[word_idx])== 0){
					// For token in phrase
					memset(temp, '\0', LINE_LENGTH);
					strncpy(temp, phrases[phrase_idx], strlen(phrases[phrase_idx]));
					token = strtok(temp, " ");
					while (token!= NULL){ // While you're not at the end of the phrase
						// If current word to index is not equal to token, print it
						if(strcmp(token, words[word_idx]) != 0){
							print_token(token);
						}else{ // Otherwise, capitalize the current word then print it
							capitalize(token);
							print_token(token);							
						}
						token = strtok(NULL, " ");
					}		
				}
				token = strtok(NULL, " ");
			}
		}
	}
}



/* 
	Takes an array of strings as input
	sorts the array alphabetically
	then removes duplicates
*/
void sort(char (*array)[LINE_LENGTH], int array_len){
	char temp[LINE_LENGTH];
	int min_idx;
	for(int outer = 0; outer < array_len-1; outer++){
		min_idx = outer;
		for (int inner = outer+1; inner < array_len; inner++){
			if (strcmp(array[min_idx], array[inner]) > 0){
				min_idx = inner;
			}
		}
		memset(temp, '\0', LINE_LENGTH);
		strncpy(temp, array[outer], strlen(array[outer]));
		memset(array[outer], '\0', LINE_LENGTH);
		strncpy(array[outer], array[min_idx], strlen(array[min_idx]));
		memset(array[min_idx], '\0', LINE_LENGTH);
		strncpy(array[min_idx], temp, strlen(temp));
	}
	
	// Remove duplicates
	char newarray[LINE_LENGTH][PHRASE_LENGTH];
	int newidx = 0;

	// Create new array without duplicates
	for (int i = 0; i < array_len-1; i++){
		if (strcmp(array[i], array[i+1]) != 0){
			strncpy(newarray[newidx], array[i], strlen(array[i]));
			newidx++;
		}
	}
	strncpy(newarray[newidx], array[array_len-1], strlen(array[array_len -1]));
	newidx++;
	// Transfer values back into old array
	for (int i = 0; i < newidx; i++){
		memset(array[i], '\0', LINE_LENGTH);
		strncpy(array[i], newarray[i], strlen(newarray[i]));
	}
	for (; newidx <  array_len; newidx++)
		memset(array[newidx], '\0', LINE_LENGTH);
	return;
}	

/*
	Compares the current token to every exclusion word
	Returns 1 if the token is an exclusion word
	Returns 0 otherwise
*/
int isKey (int key_len, char *token, char keys[KEYS_LENGTH][KEYS_LENGTH]){
	char newkey[KEYS_LENGTH];
	char with_newline[KEYS_LENGTH];
	for (int i = 0; i < key_len; i++){
		memset(with_newline, '\0', KEYS_LENGTH);
		strncpy(with_newline, keys[i], strlen(keys[i])-1);
		strcat(with_newline, "\n");
		memset(newkey, '\0', KEYS_LENGTH);
		strncpy(newkey, keys[i], strlen(keys[i])-1);
		if (strncmp(token, newkey, LINE_LENGTH) == 0 || strncmp(token, with_newline, LINE_LENGTH) == 0)
			return 1;
	}
	return 0;
}

/*
	Initializes four arrays:
	1) main_storage: contains every line of the input file
	2) keys: stores the exclusion words
	3) phrases: stores the lines-for-indexing
	4) words: contains the indexing words (words that aren't keys)
*/
void init_arrays(){
	
	// Read entire file, store each line in a new element of main_storage
	char main_storage[FILE_LENGTH][LINE_LENGTH];
	char buffer[LINE_LENGTH];
	int ms_count = 0; // To count number of lines in file
	while (fgets(buffer, LINE_LENGTH, stdin) != NULL){
		strncpy(main_storage[ms_count], buffer, LINE_LENGTH);
		ms_count++;	
	}
	
	// Put exclusion words in an array
	char keys[KEYS_LENGTH][KEYS_LENGTH];
	int main_idx = 2; 
	while (strncmp(main_storage[main_idx], "::\n", KEYS_LENGTH) != 0){
		strncpy(keys[main_idx - 2], main_storage[main_idx], strlen(main_storage[main_idx]));
		main_idx++;
	}
	int key_len = main_idx - 2;
	
	// Put lines-for-indexing into an array
	main_idx++;
	char phrases[PHRASE_LENGTH][LINE_LENGTH];
	int phrase_idx = 0;
	while(main_idx < ms_count){
		strncpy(phrases[phrase_idx], main_storage[main_idx], strlen(main_storage[main_idx]));
		main_idx++;
		phrase_idx++;
	}
	
	// Create array of words to be indexed
	// Tokenize the phrases array, storing non-exclusion words
	int words_idx = 0;
	int phrase_len = phrase_idx;
	char temp[LINE_LENGTH];
	char *token;
	char words[PHRASE_LENGTH][LINE_LENGTH];
	for (phrase_idx = 0; phrase_idx < phrase_len; phrase_idx++){ // while you haven't tokenized every phrase...
		memset(temp, '\0', LINE_LENGTH);
		strncpy(temp, phrases[phrase_idx], strlen(phrases[phrase_idx]));
		token = strtok(temp, " ");
		while (token != NULL){
			// is token a key?
			if (isKey(key_len, token, keys) == 0){
				memset(words[words_idx], '\0', LINE_LENGTH);
				strncpy(words[words_idx], token, strlen(token));
				words_idx++;
			}
			token = strtok(NULL, " ");
		}
	}
	
	//Sort words array
	sort(words, words_idx);
	//Print indexed phrases
	index(phrases, words, phrase_len, words_idx);
	
}
		
int main(void){

	init_arrays();
	return 0;
}
