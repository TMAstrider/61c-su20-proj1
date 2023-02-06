/*
 * Include the provided hash table library.
 */
#include "hashtable.h"

/*
 * Include the header file.
 */
#include "philspel.h"

/*
 * Standard IO and file routines.
 */
#include <stdio.h>

/*
 * General utility routines (including malloc()).
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines.
 */
#include <string.h>

/*
 * This hash table stores the dictionary.
 */
HashTable *dictionary;

/*
 * The MAIN routine.  You can safely print debugging information
 * to standard error (stderr) as shown and it will be ignored in 
 * the grading process.
 */
int main(int argc, char **argv) {
  fprintf(stderr, "%d", argc);
  if (argc != 2) {
    fprintf(stderr, "Specify a dictionary\n");
    return 0;
  }
  /*
   * Allocate a hash table to store the dictionary.
   */
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(2255, &stringHash, &stringEquals);

  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  fprintf(stderr, "Dictionary loaded\n");

  fprintf(stderr, "Processing stdin\n");
  processInput();

  /*
   * The MAIN function in C should always return 0 as a way of telling
   * whatever program invoked this that everything went OK.
   */
  return 0;
}

/*
 * This should hash a string to a bucket index.  Void *s can be safely cast
 * to a char * (null terminated string) and is already done for you here 
 * for convenience.
 */
unsigned int stringHash(void *s) {
  char *string = (char *)s;
  // -- TODO --
  int t;
  unsigned int result = 0;
  while((t = *(string++))) {
    result += result * 31 + t;
  }
  return result;
}

/*
 * This should return a nonzero value if the two strings are identical 
 * (case sensitive comparison) and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
  char *string1 = (char *)s1;
  char *string2 = (char *)s2;
  // -- TODO --
  if(!strcmp(string1, string2)){
    return 1;
  } else {
    return 0;
  }
}

/*
 * This function should read in every word from the dictionary and
 * store it in the hash table.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, return.  You will need to allocate
 * (using malloc()) space for each word.  As described in the spec, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length.  You CANNOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(1)
 * to cleanly exit the program.
 *
 * Since the format is one word at a time, with new lines in between,
 * you can safely use fscanf() to read in the strings until you want to handle
 * arbitrarily long dictionary chacaters.
 */
void readDictionary(char *dictName) {
  // -- TODO --
  FILE *fptr;
  fptr = fopen(dictName, "r");
  if(fptr == NULL) {
    fprintf(stderr, "File does not exist!");
    exit(1);
  }
  int i = 0;
  int maxLenth = 65;
  char *word = (char *) malloc(sizeof(char) * maxLenth);
  char ch;
  while(ch = fgetc(fptr)) {
    if(ch == EOF) {
      break;
    }
    if(ch != '\n') {
      if(i == maxLenth) {
        maxLenth *= 2;
        word = realloc(word, sizeof(char) * maxLenth);
      }
      word[i] = ch;
      i++;
    } else {
      char *key = (char *) malloc(sizeof(char) * (i + 1));
      memcpy(key, word, i);
      key[i] = '\0';
      if(findData(dictionary, key) == NULL) {
        insertData(dictionary, key, key);
      }
      i = 0;
      continue;
    }
  }
  free(word);
  fclose(fptr);
}

/*
 * This should process standard input (stdin) and copy it to standard
 * output (stdout) as specified in the spec (e.g., if a standard 
 * dictionary was used and the string "this is a taest of  this-proGram" 
 * was given to stdin, the output to stdout should be 
 * "this is a teast [sic] of  this-proGram").  All words should be checked
 * against the dictionary as they are input, then with all but the first
 * letter converted to lowercase, and finally with all letters converted
 * to lowercase.  Only if all 3 cases are not in the dictionary should it
 * be reported as not found by appending " [sic]" after the error.
 *
 * Since we care about preserving whitespace and pass through all non alphabet
 * characters untouched, scanf() is probably insufficent (since it only considers
 * whitespace as breaking strings), meaning you will probably have
 * to get characters from stdin one at a time.
 *
 * Do note that even under the initial assumption that no word is longer than 60
 * characters, you may still encounter strings of non-alphabetic characters (e.g.,
 * numbers and punctuation) which are longer than 60 characters. Again, for the 
 * final 20% of your grade, you cannot assume words have a bounded length.
 */
void processInput() {
  // -- TODO --
  int maxLenth = 65;
  
  char *s1 = (char *) malloc(sizeof(char) * maxLenth);
  
  char *s2 = (char *) malloc(sizeof(char) * maxLenth);

  char *s3 = (char *) malloc(sizeof(char) * maxLenth);
  int c;
  int i = 0;
  while(c = getchar()) {
    if(c == EOF) {
      if(!(s1[0] == '\0')) {
        s1[i] = '\0';
        s2[i] = '\0';
        s3[i] = '\0';
        if(findData(dictionary, s1) == NULL && findData(dictionary, s2) ==NULL && findData(dictionary, s3) == NULL) {
          fprintf(stdout, "%s [sic]", s1);
        } else {
          fprintf(stdout, "%s", s1);
        }
      }
      break;
    }
    // fprintf(stderr, "Hello!");
    if(isalpha(c)) {
      // original string
      s1[i] = (char)c;
      // string with all lowercase
      s2[i] = (char)tolower(c);
      // string with all but first letter converted to lowercase
      if(i == 0) {
        s3[i] = (char)c;
      } else {
        s3[i] = (char)tolower(c);
      }
      i++;
    } else {
      if(!(s1[0] == '\0')) {
        s1[i] = '\0';
        s2[i] = '\0';
        s3[i] = '\0';
        if(findData(dictionary, s1) == NULL && findData(dictionary, s2) ==NULL && findData(dictionary, s3) == NULL) {
          fprintf(stdout, "%s [sic]%c", s1, c);
        } else {
          fprintf(stdout, "%s%c", s1, c);
        }
      } else {
        fprintf(stdout, "%c", c);
      }
      i = 0;
      s1[i] = '\0';
      s2[i] = '\0';
      s3[i] = '\0';
      continue;
    }
  }
  // I do love debug with CGDB, tho lol
  free(s1);
  free(s2);
  free(s3);
}
