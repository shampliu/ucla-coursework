#include <stdio.h>
#include <stdlib.h>

int rot13cmp (const char *a, const char *b) {

 while (*a == *b && *a != '\n') {
  a++;
  b++;
 }
 
 if (*a == *b) { return 0; } 
 else if (*a == '\n') { return -1; } 
 else if (*b == '\n') { return 1; } 
 else {
  int diff = *a - *b;
  if ('A' <= *a && *a  <= 'M' && 'N' <= *b && *b <= 'Z' ||
      'N' <= *a && *b <= 'Z' && 'A' <= *b && *b <= 'M' || 
      'a' <= *a && *a <= 'm' && 'n' <= *b && *b <= 'z' ||
      'n' <= *a && *a <= 'z' && 'a' <= *b && *b <= 'm') {
   return -diff; 
  }  
  return diff;
 }
}

/* function to call in qsort */ 
int cmp (const void *a, const void *b) {
 return rot13cmp( *(const char**)a, *(const char**)b ); 
}

int main() {
 
 char *buffer = (char *) malloc(sizeof(char) * 512);
 if (buffer == NULL) {
  /* output error */
  fprintf(stderr, "Did not allocate memory correctly!"); 
  exit(1); 
 }
 int maxSize = 512; 
 int count = 0; 

 int isEmpty = 1; // check for empty file
 char c;

 /* infinite loop until end of file */
 while (1) { 
  c =  getchar();
  if (c == EOF) { break; }
  isEmpty = 0; 
  buffer[count++] = (char) c;
  if (count == maxSize) {
   buffer = (char*) realloc(buffer, maxSize * 2);
   if (buffer == NULL) { 
    fprintf(stderr, "Did not allocate memory correctly!"); 
    exit(1);
   } 
   maxSize *= 2;
  } 
 }

 /* check for empty file */
 if (isEmpty == 1) { return 0; } 
 
 /* append newline if there isn't one */
 if (buffer[count-1] != '\n') {
  buffer[count++] = '\n'; 
  if (count == maxSize) { 
   buffer = (char*) realloc(buffer, maxSize + 32);
   if (buffer == NULL) {   
    fprintf(stderr, "Did not allocate memory correctly!"); 
    exit(1);
   }
   maxSize += 32;  
  }
 }
 
 char **words = NULL; 
 int word_count = 0; 

 /* take pointer to beginning of words from buffer */ 
 for (int i = 0; i < count; i++) {
  if (i == 0 || buffer[i-1] == '\n') {
   words = realloc(words, sizeof(char *) * (word_count + 1));
   if (words == NULL) {
    /* return error */
    fprintf(stderr, "Did not allocate memory correctly!"); 
    exit(1);
   }
   words[word_count++] = &buffer[i];
  }
 }
 // fprintf(stdout, "%d words", word_count); 
 qsort(words, word_count, sizeof(char*), cmp); 
 
 for (int i = 0; i < word_count; i++) {
  char *ptr = words[i];
  while (*ptr != '\n') {
   fprintf(stdout, "%c", *ptr);
   // char c = rot13(*ptr); 
   // fprintf(stdout, "%c", c);
   ptr++;
  }
  fprintf(stdout, "%c", *ptr);
 }

 free(words);
 free(buffer);
 return 0; 
}
