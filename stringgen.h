/*
 * Basic string generation for brute-force attacks
 * Copyright (C) 2011 Radek Pazdera
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* I chose to use an one way linked list data structure
 * to avoid restrictions on the generated string length.
 * The thing is, the list must be converted to string so
 * it could be used. This conversion have to happen in
 * each cycle and causes unnecessary slowdown.
 *
 * Faster solution would be to implement the generation
 * directly on some staticaly allocated string with fixed
 * size (20 characters are more than enough).
 */
typedef struct charlist charlist_t;
struct charlist;

/* Return new initialized charlist_t element.
 *
 * Elements are initialized
 * @return charlist_t
 */
charlist_t* new_charlist_element();

/* Free memory allocated by charlist.
 *
 * @param list Pointer at the first element.
 * @return void
 */
void free_charlist(charlist_t* list);

/* Print the charlist_t data structure.
 *
 * Iterates through the whole list and prints all characters
 * in the list including any '\0'.
 *
 * @param list Input list of characters.
 * @return void
 */
void print_charlist(charlist_t* list);

/* Print the charlist_t data structure to a string.
 *
 * Iterates through the whole list and prints all characters
 * in the list including any '\0' to a string.
 *
 * @param str Destination string
 * @param list Input list of characters.
 * @return void
 */
void sprint_charlist(char* str, charlist_t* list);

/* Get next character sequence.
 *
 * It treats characters as numbers (0-255). Function tries to
 * increment character in the first position. If it fails,
 * new character is added to the back of the list.
 *
 * It's basicaly a number with base = 256.
 *
 * @param list A pointer to charlist_t.
 * @return void
 */
void next(charlist_t* list);