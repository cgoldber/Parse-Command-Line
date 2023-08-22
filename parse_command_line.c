/**
 * Uses pointers to parse a command line input
 *
 * @author Casey Goldberg
 */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "command.h"

/**
 * check_ampersand:
 *   Parses through command and checks for an ampersand. If there is
 *   an ampersand, checks to see if it is validly formatted.
 * Parameters:
 *   - first_char : a pointer to the first character in a string.
 * Return:
 *   - int of 0 if the command contains a well-formatted ampersand, 
 *     int of 3 if the command contains a badly-formatted ampersand,
 *     and int of 1 if the command does not contain an ampersand.
 **/ 
int check_ampersand(char* first_char);

/**
 * str_count:
 *   Count the number of strings in a command.
 * Parameters:
 *   - first_char : a pointer to the first character in the command.
 * Return:
 *   - the number of intended strings in a command.
 **/ 
int str_count(char* first_char);

/** 
 * char_count:
 *   Counts the number of characters in a string.
 * Parameters:
 *   - first_char : pointer to the first character in a string.
 * Return:
 *   - the number of characters in a string from the command.
**/
int char_count(char* first_char);

/**
 * make_arry_of_chars:
 *   Allocates an array of characters to represent a string. Initially,
 *   populates array with null characers. Then, populates array with 
 *   characters corresponding to a string denotated by a given pointer.
 * Paramters:
 *   - first_char : pointer to the first character in a string.
 *   - num_chars : the number of characters in a string from the command
 *     (not including the null character to be added)
 * Return:
 *   - A pointer to an array of characters representing a string from
 *     the command line.
 **/ 
char* make_arry_of_chars(char* first_char, int num_chars);

/**
 * make_arry_of_strs:
 *   Allocates an array of pointers to characters that all represents an
 *   array of strings (array of array of characters). Intially, populates
 *   array with NULLs. Then, populates array with representations
 *   of the intended command strings. 
 * Parameters:
 *   - first_str : pointer to the first intended string in the command
 *   - num_words : the number of intended strings in the command
 * Return:
 *   - A pointer to an array of pointers to characters, where the 
 *     initial pointers point to different strings in the command.
 **/
char** make_arry_of_strs(char* first_str, int num_words);


/**
 * command_parse:
 *
 * Parse a command-line string, report foreground/background status,
 * and return a command array.  Each element in the command array
 * corresponds to one word in the command string, in order.
 *
 * A command with '&' after the last word is a background command.
 * The '&' is not part of the resulting command array.  Repetitions or
 * other placements of '&' constitute invalid command lines.
 *
 * Parameters:
 *
 * - line: non-NULL pointer to a command-line string.
 *
 * - foreground: non-NULL pointer to an int where foreground (1)/
 *   background (0) status of the command line should be stored.
 * 
 * Return:
 *
 * - a pointer to a NULL-terminated array of strings (char**) if given
 *   a valid command-line string.
 *
 * - NULL if given a command-line string with invalid placement of '&'
 */
char** command_parse(char* line, int* foreground) {
  // Check arguments: must be non-NULL pointers.
  assert(line);
  assert(foreground);

  char* cmd_finger = line;
  while(*cmd_finger == ' '){
    cmd_finger++;
  }

  //check for ampersand and formatting of ampersand
  int ampersand = check_ampersand(cmd_finger);
  if(ampersand == 3){
    return NULL;
  }
  *foreground = ampersand;

  //counts num of words and returns NULL if no strings in command
  int num_strs = str_count(cmd_finger);
  if(!num_strs){
    return NULL;
  }

  char** cmd_arry = make_arry_of_strs(cmd_finger, num_strs);
  return cmd_arry;
}

//checks if ampersand exists / is validly formatted
int check_ampersand(char* first_char){
  if(*first_char == '&'){
    return 3;
  }
  char* finger = first_char;
  while(*finger){
    if(*finger == '&'){
      char* amp_checker = finger;
      amp_checker++;
      while(*amp_checker == ' '){
        amp_checker++;
      }
      if(*amp_checker){ 
        return 3;
      }
      return 0;
    } 
    finger++;
  }
  return 1;
}

//counts number of strings in command
int str_count(char* first_char){
  char* count_fing = first_char;
  int num_words = 0;
  while(*count_fing){
    if((*count_fing != ' ') && *count_fing){
      if((*(count_fing + 1) == ' ') || !(*(count_fing + 1))){
        num_words++;
      }
    }
    count_fing++;
  }
  return num_words;
}

//counts number of characters in a string
int char_count(char* first_char){
  char* count_finger = first_char;
  int num_chars = 0;
  while(*count_finger != ' ' && *count_finger){
    count_finger++;
    num_chars++;
  }
  return num_chars;
}

//makes array of characters
char* make_arry_of_chars(char* first_char, int num_chars){
  //allocates memory and populats w null characters
  char* chars_arry = malloc((num_chars + 1) * sizeof(char)); 
  for(int j = 0; j < (num_chars + 1); j++){
    *chars_arry = '\0';
    chars_arry++;
  }
  chars_arry = chars_arry - num_chars - 1;
  
  //copies characters from the command
  char* char_fing = first_char;
  for(int i = 0; i < num_chars; i++){
    *chars_arry = *char_fing;
    char_fing++;
    chars_arry++;
  } 
  return (chars_arry - num_chars);
}

//makes array of strings
char** make_arry_of_strs(char* first_str, int num_words){
  //allocates memory and populates with NULL
  char** strs_arry = malloc((num_words + 1) * sizeof(char*));
  for(int j = 0; j < (num_words + 1); j++){
    *strs_arry = NULL;
    strs_arry ++;
  }
  strs_arry = strs_arry - num_words - 1;
 
  //copies pointer information for each string
  char* str_finger = first_str;
  for(int i = 0; i < num_words; i++){
    int num_chars = char_count(str_finger); 
    *strs_arry = make_arry_of_chars(str_finger, num_chars);
    while(*str_finger != ' '){
      str_finger++;
    }
    while(*str_finger == ' '){
      str_finger++;
    }
    strs_arry++;
  }
  return (strs_arry - num_words);
}


/**
 * command_show:
 *
 * Print the structure of a command array.
 *
 * Parameters:
 *
 * - command: gdba non-NULL pointer to the command array to print.
 */
void command_show(char** command) {
  // Check argument: must be non-NULL pointer.
  assert(command);

  char** string_pointer = command;
  int str_count = 1;
  while(*string_pointer != NULL){
    printf("#%d : ", str_count);
    char* char_pointer = *string_pointer;
    while(*char_pointer){
      if(*char_pointer != ' '){
        printf("%c", *char_pointer);
      } else {
        printf("_");
      }
      char_pointer++;
    }
    printf("\n");
    str_count++;
    string_pointer++;
  }
}


/**
 * command_print:
 *
 * Print a command array in the form of a command-line string.
 * You should *not* include a newline at the end.
 *
 * Parameters:
 *
 * - command: non-NULL pointer to the command array to print.
 */
void command_print(char** command) {
  // Check argument: must be non-NULL pointer.
  assert(command);

  char** str_ptr = command;
  while(*str_ptr != NULL){
    char* char_ptr = *str_ptr;
    while(*char_ptr && (*char_ptr != '&')){
	    printf("%c", *char_ptr);
      char_ptr++;
    }
    printf(" ");
    str_ptr++;
  }
}


/**
 * command_free:
 *
 * Free all parts of a command array created by command_parse.
 *
 * Parameters:
 *
 * - command: non-NULL pointer to the command array to free.
 */
void command_free(char** command) {
  // Check argument: must be non-NULL pointer.
  assert(command);
  
  char** cmd_ptr = command;
  while(*cmd_ptr != NULL){
    free(*cmd_ptr);
    cmd_ptr++;
  }
  free(command);
}