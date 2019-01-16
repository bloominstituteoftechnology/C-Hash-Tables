
 // Program 11.4   Daisy chaining the horses #define __STDC_WANT_LIB_EXT1__ 1 #include <stdio.h> #include <ctype.h> #include <stdlib.h>
 
typedef struct Horse Horse;            // Define Horse as a type name
 
struct Horse                           // Structure type definition {  int age;  int height;  char name[20];  char father[20];  char mother[20];  Horse *next;                         // Pointer to next Horse structure };
 
int main(void) {  Horse *first = NULL;                 // Pointer to first horse  Horse *current = NULL;               // Pointer to current horse  Horse *previous = NULL;              // Pointer to previous horse
 
  char test = '\0';                    // Test value for ending input
 
  for( ; ; )  {    printf_s("Do you want to enter details of a%s horse (Y or N)? ",                                       first != NULL?"nother" : "" );    scanf_s(" %c", &test, sizeof(test));    if(tolower(test) == 'n')      break;
 
    // Allocate memory for a Horse structure    current = (Horse*) malloc(sizeof(Horse));    if(first == NULL)                  // If there's no 1st Horse...      first = current;                 // ...set this as 1st Horse
 
    if(previous != NULL)               // If there was a previous...      previous->next = current;        // ...set its next to this one
 
    printf_s("Enter the name of the horse: ");    scanf_s("%s", current->name, sizeof(current->name));
 