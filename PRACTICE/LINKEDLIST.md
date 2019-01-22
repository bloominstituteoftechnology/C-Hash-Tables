
 how It Works
You’ve added a member to the Horse structure with the name next that is a pointer to a Horse structure. This will be used to link all the horses you have, where each Horse structure will have a pointer containing the address of the next. The last Horse structure will be an exception, of course. Because there is no subsequent Horse structure, its next member will be NULL. 

Note that with the typedef for Horse, you can use just Horse to specify the type of the next member of the Horse structure. You must always use the struct keyword when you define a structure though.
This time, not only do you have no space for structures allocated, but you also have only three pointers defined initially. These pointers are declared and initialized in these statements:
 
Horse *first = NULL;                 // Pointer to first horse Horse *current = NULL;               // Pointer to current horse Horse *previous = NULL;              // Pointer to previous horse
 
Each pointer is defined as a pointer to a Horse structure. The pointer first is used solely to store the address of the first structure. The second and third pointers are working storage: current holds the address of the current Horse structure you’re dealing with, and previous keeps track of the address of the previous structure that was processed.
The input loop is the following:
 
for( ; ; ) {  ... }
 
The input loop is now an indefinite loop because you don’t have an array to worry about. You don’t need to mess around with indexes. It’s also unnecessary to keep count of how many sets of data are read in, so you don’t need the variable hcount or the loop variable i. Because you allocate memory for each horse, you can just take them as they come.

The initial statements in the loop are the following:
 
printf_s("Do you want to enter details of a%s horse (Y or N)? ",
first != NULL?"nother" : "" ); 
scanf_s(" %c", &test, sizeof(test)); if(tolower(test) == 'n')  
break;
After the prompt, you exit from the loop if the response 'N' or 'n' is detected. Otherwise, you expect another set of structure members to be entered. You use the pointer first to get a slightly different prompt on the second and subsequent iterations, because the only time it will be NULL is on the first loop iteration.
Assuming you get past the initial question in the loop, you execute these statements:
 
current = (Horse*) malloc(sizeof(Horse)); if(first == NULL)                  // If there's no 1st Horse...  first = current;                 // ...set this as 1st Horse
 
if(previous != NULL)               // If there was a previous...  previous->next = current;        // ...set its next to this one
 
On each iteration, you allocate the memory necessary for the new Horse structure. To keep things short, there’s no check for a NULL return from malloc(), although you should do this in practice.
If first is NULL, you must be on the first loop iteration, and this must be the first structure about to be entered. Consequently, you set first to the address that you’ve just obtained from malloc() and stored in current. The address in first is the key to accessing the first horse in the chain. You can get to any of the others by starting with the address in first and then use its next member pointer to access the address of the next structure. You can step from there to the next structure and so on to any horse in the sequence.
The next pointer for each Horse object always needs to point to the next Horse structure in the chain if there is one, but the address of the next structure can be determined only once you have the next structure. Therefore, on the second and subsequent iterations, you store the address of the current structure in the next member of the previous structure, whose address you’ll have saved in previous. On the first iteration the pointer previous will be NULL, so of course you do nothing.
At the end of the loop, following all the input statements, you have these statements:
 
current->next = NULL;              // In case it's the last... previous = current;                // ...save its address
 
The next pointer in the structure pointed to by current, which you’re presently working with, is set to NULL in case this is the last structure, and there’s no next structure. If there is a next structure, this next pointer will be filled in on the next iteration. The previous pointer is set to current and is ready for the next iteration, when the current structure will indeed be the previous structure. Have you got that?
The strategy of the program for storing information on an unlimited number of horses is to generate a daisy chain of Horse structures in which the next member of each structure points to the next structure in the chain. The last is an exception because there’s no next Horse, so its next pointer contains NULL. This arrangement is called a linked list.
Once you have the horse data in a linked list, you process it by starting with the first structure and then getting the next structure through the pointer member next. When the pointer next is NULL, you know that you’ve reached the end of the list. This is how you generate the output list of all the input.
Linked lists are invaluable in applications in which you need to process an unknown number of structures, such as you have here. The main advantages of a linked list relate to memory usage and ease of handling. You occupy only the minimum memory necessary to store and process the list. Although the memory used may be fragmented, you have no problem progressing from one structure to the next. As a consequence, in a practical situation in which you may need to deal with several different types of objects simultaneously, each can be handled using its own linked list, with the result that memory use is optimized. There is one small cloud 
associated with this—as there is with any silver lining—and it’s that you pay a penalty in slower access to the data, particularly if you want to access it randomly.
The output process shows how you access a linked list because it steps through the linked list of Horse objects you have created with these statements:
 
current = first;                    // Start at the beginning
 while (current != NULL)             // As long as we have a valid pointer 
 { // Output the data  printf_s("%s is %d years old, %d hands high,",                     
 current->name, current->age, current->height);  
 printf_s(" and has %s and %s as parents.\n", current->father,                                             
 current->mother); 
 previous = current;    // Save the pointer so we can free memory  
 current = current->next;          // Get the pointer to the next 
 free(previous);                   // Free memory for the old one  
 previous = NULL; 
 }
 
The current pointer controls the output loop, and it’s set to first at the outset. Remember that the first pointer contains the address of the first structure in the list. The loop steps through the list, and as the members of each structure are displayed, the address stored in the member next, which points to the next structure, is assigned to current.
The memory for the structure displayed is then freed. It’s obviously fairly essential that you only free the memory for a structure once you have no further need to reference it. It’s easy to fall into the trap of putting the call of the function free() immediately after you’ve output all of the member values for the current structure. This would create some problems, because then you couldn’t legally reference the current structure’s next member to get the pointer to the next Horse structure. For the last structure in the linked list, the next pointer will contain NULL and the loop will terminate. It is not strictly necessary to set the pointers to NULL because the program ends, but  it’s a good policy to always set a pointer to NULL when the address it contains is no longer valid.
