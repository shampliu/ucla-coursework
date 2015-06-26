#include <unistd.h>
#include <stdlib.h>
/* fstat function */
#include <sys/types.h>
#include <sys/stat.h> 
#include <stdio.h>

/* b[0] = *ptr
 * write(1, 1, 1);
 */

int cmpcount = 0; 

int rot13cmp (const char *a, const char *b) {
 cmpcount++; 

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
 
 int bufferSize = 0;
 int fileSize = 0;
 int count = 0; 
 char *buffer;  
 char b[1];  

 // if regular file 
 struct stat st;
 if (0 == fstat(0, &st) && S_ISREG(st.st_mode)) {
  if (read(0, &b, 1) == 0) {  // empty
   fprintf(stderr, "Number of comparisons: %d\n", cmpcount);
   return 0;
  }
  
  fileSize = st.st_size;
  buffer = (char *) malloc(sizeof(char) * fileSize); 
  bufferSize = fileSize;   

  if (buffer == NULL) {
   fprintf(stderr, "Did not allocate memory correctly!");
   exit(1);
  }

  int j = 0;
  while (j < fileSize) {
   read(0, &b, 1);
   buffer[count++] = b[0];
   if (count == bufferSize) {
    buffer = (char *) realloc (buffer, bufferSize + 512);
    if (buffer == NULL) {
     fprintf(stderr, "Did not reallocate memory correctly!");
     exit(1);
    }
    bufferSize += 512; 
   }
   // changing file size
   fstat(0, &st);
   fileSize = st.st_size;
   j++;
  }
 }

 else {
  bufferSize = 512; 
  buffer = (char *) malloc(sizeof(char) * bufferSize); 
 }
 if (buffer == NULL) {
  /* output error */
  fprintf(stderr, "Did not allocate memory correctly!"); 
  exit(1); 
 }

 int isEmpty = 1; // check for empty file 

 while (0 != read(0, b, 1)) { 
  isEmpty = 0; 
  buffer[count++] = b[0];
  if (count == bufferSize) {
   buffer = (char*) realloc(buffer, bufferSize * 2);
   if (buffer == NULL) { 
    fprintf(stderr, "Did not allocate memory correctly!"); 
    exit(1);
   } 
   bufferSize *= 2;
  } 
 }

 /* check for empty file */
 if (isEmpty == 1) { 
  fprintf(stderr, "Number of comparisons: %d\n", cmpcount); 
  return 0; 
 } 
 
 /* append newline if there isn't one */
 if (buffer[count-1] != '\n') {
  buffer[count++] = '\n'; 
  if (count == bufferSize) { 
   buffer = (char*) realloc(buffer, bufferSize + 32);
   if (buffer == NULL) {   
    fprintf(stderr, "Did not allocate memory correctly!"); 
    exit(1);
   }
   bufferSize += 32;  
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
  char b[1];
  while (*ptr != '\n') {
   //fprintf(stdout, "%c", *ptr);
   b[0] = *ptr;
   write(1, b, 1);
   ptr++;
  }
  b[0] = *ptr;
  write(1, b, 1);
 }

 free(words);
 free(buffer);

 fprintf(stderr, "Number of comparison: %d \n", cmpcount);
 return 0; 
}
