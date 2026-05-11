/*
 * File: set.h
 * Description: Header file containing definitions, constants, data structures,
 * and function prototypes for the Set Calculator program.
 * This file serves as the interface for the set operations library.
 */
 
/* --- Header Guard: Prevents recursive inclusion of this file --- */
#ifndef SET_H
#define SET_H

/* --- Library Imports --- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* --- Constants --- */
#define LEN 16 /* Size of the internal byte array (16 bytes * 8 bits = 128 bits) */
#define BIT 8 /* Number of bits in a byte */
#define MAX_PER_LINE 16 /* Number of the numbers to print per line */

#define NUM_SETS 6 /* Number of sets */
#define MAX_COMMAND_LINE_LENGTH 1000 /* Buffer size for input line */
#define MIN_VAL 0 /* Minimum valid set member value */
#define MAX_VAL 127 /* Maximum valid set member value */
#define SET_MAP_SIZE 7 /* Size of set mapping array + terminator */
#define CMD_MAP_SIZE 8 /* Size of command mapping array + terminator */
#define DECIMAL_BASE 10 /* Base for strtol conversion */
#define MAX_TEMP_WORD_LEN 32 /* Buffer size for temporary word storage in validation */
#define TRUE 1          /* Represents a boolean True value */
#define FALSE 0         /* Represents a boolean False value */
#define OK 0  /* Return code for success */
#define ERROR 1         /* Return code indicating a general failure or error */
#define DATA_TERMINATOR -1  /* Sentinel value indicating the end of the input list */

/* --- Data Structures --- */

/* 
 * Structure: set
 * Purpose: Represents a mathematical set using a bit-array for memory efficiency.
 */
typedef struct {
 unsigned char arr [LEN];
} set;

/*
 * Structure: SetMap
 * Purpose: Maps a string name (e.g., "SETA") to a set pointer.
 */
typedef struct {
    char *name;
    set *setPtr;
} SetMap;

/*
 * Structure: CmdMap
 * Purpose: Maps a command string to a handler function pointer.
 */
typedef struct {
    char *name;
    void (*func)(SetMap *map); 
} CmdMap;

/* --- Function Prototypes --- */

/*
 * Function: read_set
 * Description: Copies data from a temporary source set to the destination set.
 * Input: 
 * - dest: Pointer to the real set to update.
 * - source: Pointer to the temporary set containing valid data.
 * Output: None.
 */
void read_set(set * setPtr, set *source);

/*
 * Function: print_set
 * Purpose: Prints the contents of a set to standard output.
 * Input: 
 * - setPtr: Pointer to the set to print.
 * Output: None.
 */
void print_set(set * setPtr);

/*
 * Function: union_set
 * Purpose: Computes the union of two sets.
 * Input: 
 * - setA, setB: Source sets.
 * - setC: Destination set where the result will be stored.
 * Output: None.
 */
void union_set(set * setA, set * setB, set * setC);

/*
 * Function: intersect_set
 * Purpose: Computes the intersection of two sets.
 * Input: 
 * - setA, setB: Source sets.
 * - setC: Destination set where the result will be stored.
 * Output: None.
 */
void intersect_set(set * setA, set * setB, set * setC);

/*
 * Function: sub_set
 * Purpose: Computes set subtraction (A minus B).
 * Input: 
 * - setA: The set to subtract from.
 * - setB: The set to remove.
 * - setC: Destination set where the result will be stored.
 * Output: None.
 */
void sub_set(set * setA, set * setB, set * setC);

/*
 * Function: symdiff_set
 * Purpose: Computes symmetric difference (A XOR B).
 * Input: 
 * - setA, setB: Source sets.
 * - setC: Destination set where the result will be stored.
 * Output: None.
 */
void symdiff_set(set * setA, set * setB, set * setC);

/*
 * Function: stop
 * Purpose: Terminates the program execution.
 * Input: None.
 * Output: Does not return.
 */
void stop();

/* ---  Parsing Helper Functions --- */

/*
 * Function: remove_newline
 * Purpose: Strips trailing newline character from a string.
 * Input: String pointer.
 * Output: None.
 */
void remove_newline(char *line);

/*
 * Function: get_set_by_name
 * Purpose: Finds a set pointer by its name string.
 * Input: 
 * - map: The mapping array.
 * - name: The name string to search for.
 * Output: Pointer to set if found, NULL otherwise.
 */
set* get_set_by_name(SetMap *map, char *name);

/*
 * Function: check_extra_text
 * Purpose: Verifies no extra tokens exist after a command.
 * Input: None (uses internal tokenizer state).
 * Output: OK if clean, !OK if text found.
 */
int check_extra_text();

/*
 * Function: validate_input
 * Purpose: Validates command syntax (commas, spacing) before execution.
 * Input: 
 * - line: Raw line string.
 * - map: SetMap for context checks.
 * Output: OK if valid, !OK otherwise.
 */
int validate_input(char *line, SetMap *map);

/*
 * Function: get_set_arg
 * Purpose: Parses the next token as a set argument.
 * Input: SetMap.
 * Output: Pointer to set or NULL.
 */
set* get_set_arg(SetMap *map);

/*
 * Function: get_3_sets
 * Purpose: Helper to fetch 3 set arguments for binary operations.
 * Input: 
 * - map: SetMap.
 * - p1, p2, p3: Pointers to store the resulting set pointers.
 * Output: OK or !OK.
 */
int get_3_sets(SetMap *map, set **p1, set **p2, set **p3);

/* --- Command Handlers (Wrappers) --- */

/*
 * Function: help_read
 * Purpose: Wrapper handler for 'read_set' command.
 * Input: SetMap.
 * Output: None.
 */
void help_read(SetMap *map);

/*
 * Function: help_print
 * Purpose: Wrapper handler for 'print_set' command.
 * Input: SetMap.
 * Output: None.
 */
void help_print(SetMap *map);

/*
 * Function: help_union
 * Purpose: Wrapper handler for 'union_set' command.
 * Input: SetMap.
 * Output: None.
 */
void help_union(SetMap *map);

/*
 * Function: help_intersect
 * Purpose: Wrapper handler for 'intersect_set' command.
 * Input: SetMap.
 * Output: None.
 */
void help_intersect(SetMap *map);

/*
 * Function: help_sub
 * Purpose: Wrapper handler for 'sub_set' command.
 * Input: SetMap.
 * Output: None.
 */
void help_sub(SetMap *map);

/*
 * Function: help_symdiff
 * Purpose: Wrapper handler for 'symdiff_set' command.
 * Input: SetMap.
 * Output: None.
 */
void help_symdiff(SetMap *map);

/*
 * Function: help_stop
 * Purpose: Wrapper handler for 'stop' command.
 * Input: SetMap.
 * Output: None.
 */
void help_stop(SetMap *map);

#endif/* SET_H */
