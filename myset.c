/*
Program: Set Calculator (myset)

Description:
This program functions as an interactive calculator for mathematical set operations.
It manages 6 predefined sets (SETA..SETF) and allows the user to perform operations
like read, print, union, intersection, subtraction and symmetric difference.

Data Model & Algorithms:
1) Set Representation: 
   Sets are stored as bit-arrays (128 bits total, 16 bytes). 
   This ensures O(1) memory usage and fast bitwise operations.
   
2) Input Parsing: 
   The program reads input line-by-line using fgets.
   It employs a two-step processing strategy:
   - Step A: Validate syntax (validate_input) to catch errors like missing commas.
   - Step B: Parse and execute (strtok) to perform the command.

3) Command Dispatching:
   A 'CmdMap' array maps command strings to function pointers, allowing
   for clean and extensible command execution.

Assumptions:
- Maximum line length is 1024 characters.
- Valid set members are integers in range [0..127].
- Commands are case-sensitive.
*/

#include "set.h" 

/*
Function: main
Purpose: The main entry point of the program. Manages the main loop.
Input: None (Reads from stdin).
Output: Returns OK (0) on success.
Algorithm:
- Initialize the 6 sets and the mapping arrays.
- Enter an infinite loop to read commands until EOF or 'stop'.
- For each line:
  1. Skip empty lines.
  2. Validate syntax using validate_input().
  3. Parse the command name using strtok().
  4. Search for the command in the CmdMap.
  5. Execute the corresponding helper function.
*/
int main() 
{
/* --- Variable Declarations --- */

/* Predefined Sets: Initialized to 0 (Empty Sets) */
  set SETA ={0};
  set SETB ={0};
  set SETC ={0};
  set SETD ={0};
  set SETE ={0};
  set SETF ={0};
 
/* Buffers for Input Processing */
  char command_line[MAX_COMMAND_LINE_LENGTH]; /* Stores the raw input from user */
  char command_copy[MAX_COMMAND_LINE_LENGTH]; /* Copy of input for printing */
  char *command_name; /* Pointer to the first token (command) */
  int i; /* Index */
  int was_stopped = FALSE; /* Flag: set to 1 when 'stop' is executed */
  
  /* Mapping Arrays */
  SetMap set_map[SET_MAP_SIZE]; /* Maps string names to set pointers */ 
  CmdMap cmd[CMD_MAP_SIZE]; /* Maps command names to functions */
  
  /* --- Initialization of Maps --- */
  /* Mapping Set Names to Memory Addresses */
  set_map[0].name = "SETA"; set_map[0].setPtr = &SETA;
  set_map[1].name = "SETB"; set_map[1].setPtr = &SETB;
  set_map[2].name = "SETC"; set_map[2].setPtr = &SETC;
  set_map[3].name = "SETD"; set_map[3].setPtr = &SETD;
  set_map[4].name = "SETE"; set_map[4].setPtr = &SETE;
  set_map[5].name = "SETF"; set_map[5].setPtr = &SETF;
  set_map[6].name = NULL;   set_map[6].setPtr = NULL; /* Null Terminator */
  
  /* Mapping Command Names to Helper Functions */
  cmd[0].name = "read_set";      cmd[0].func = help_read;
  cmd[1].name = "print_set";     cmd[1].func = help_print;
  cmd[2].name = "union_set";     cmd[2].func = help_union;
  cmd[3].name = "intersect_set"; cmd[3].func = help_intersect;
  cmd[4].name = "sub_set";       cmd[4].func = help_sub;
  cmd[5].name = "symdiff_set";   cmd[5].func = help_symdiff;
  cmd[6].name = "stop";          cmd[6].func = help_stop;
  cmd[7].name = NULL;            cmd[7].func = NULL; /* Null Terminator */


  printf("Please enter your command:\n"); /* Initial prompt */

  /* --- Main Processing Loop --- */
  /* Reads lines until EOF (End Of File) or 'stop' command */
  while (fgets(command_line, sizeof(command_line), stdin) != NULL)
  {
    char *p = command_line;/* Handle Empty Lines or Whitespace-only lines */
    while (*p && isspace(*p)) /* Skip whitespace chars */
    {
      p++;
    }
    if (*p == '\0') /* If line is empty */
    {
      printf("\nPlease enter next command:\n"); /* just prompt again without printing "The command is" */
      continue;
    }
    strcpy(command_copy, command_line); /* Make a copy to preserve original line */
    remove_newline(command_copy); /* Remove \n for clean printing */
    printf("The command is: %s\n", command_copy); /* Print the command as required by assignment spec */
    if (validate_input(command_line, set_map) == OK) /* Validate Syntax, Checks for commas, spaces, and structural integrity before parsing */
    {
      command_name = strtok(command_line, " \t\n\r,"); /* Extract Command Name, split the string using spaces, tabs, and commas as delimiters */
      if (command_name != NULL) /* Search for the command in the CmdMap */
      {  
        i = 0;
        while (cmd[i].name != NULL && strcmp(command_name, cmd[i].name) != 0)/* Search for the command in the CmdMap */ 
        {
          i++;
        }
        if (cmd[i].name != NULL) /* If command found, execute it */
        {
          if (strcmp(command_name, "stop") == 0) /* Check if it's the stop command to update flag */
          {
            was_stopped = TRUE;
          }
          (*(cmd[i].func))(set_map); /* Execute the function via pointer */
        } 
        else
        {
          printf("Undefined command name\n");/* Command not found in map */
        }
      }
    }
    if (!was_stopped) /* Prompt for next command if program wasn't stopped */
    {
    printf("\nPlease enter next command:\n");\
    }
  }
    if (!was_stopped)
    {
      printf("The program is not terminated correctly (missing stop command)\n");
      return ERROR; /* Return error code indicating failure */
    }
  return OK; /* Return success code if no text was found */
}

/*
Function: read_set
Purpose: Copies the data from a temporary source set to the destination set.
Algorithm:
- Uses struct assignment to copy the entire bit-array in one operation.
*/
void read_set(set * setPtr, set *source)
{
  *setPtr = *source; /* Copy the group */
}

/*
Function: print_set
Purpose: Prints the numbers present in the set.
Input:
- set *setPtr: Pointer to the set to print.
Output: None (Prints to stdout).
Algorithm:
- Iterate through all bytes (0-15) and all bits (0-7).
- If a bit is ON (1), calculate the number (byte*8 + bit).
- Print the number.
- Manage formatting to print max 16 numbers per line.
*/
void print_set(set * setPtr)
{
  int i, j; /* Index */
  int lineCount=0; /* Counter for numbers printed on current line */
  int is_empty = TRUE; /* Flag: assumes empty until a bit is found */
  for(i = 0; i < LEN; i++) /* Iterate bytes */
  {
    for(j = 0; j < BIT; j++) /* Iterate bits in byte */
    {
      if(setPtr -> arr[i] & (1 << j)) /* Check if the j-th bit in the i-th byte is ON */
      {
        is_empty = FALSE; /* Set is not empty */
        if(lineCount == MAX_PER_LINE) /* Handle line break every 16 numbers */
        {
          printf("\n");
          lineCount=0;
        }
        printf("%d ", BIT * i + j); /* Calculate number value and print */
        lineCount++;
      }
    }
  }
  if(is_empty) /* Check if no bits were found */
  {
    printf("The set is empty\n");
  }
  else
  {
    printf("\n"); /* End with newline */
  }
}

/*
Function: remove_newline
Purpose: Strips the trailing newline character from a string (if present).
Input: char *line - String to modify.
Output: None.
Algorithm:
- Search for '\n' using strchr.
- If found, replace it with null terminator '\0'.
*/
void remove_newline(char *line) 
{
  char *p;
  p = strchr(line, '\n'); /* Find first occurrence of newline */
  if(p != NULL) /* If found \n */
  *p = '\0'; /* Replace with null terminator */
}

/*
Function: get_set_by_name
Purpose: Finds a set pointer given its name string.
Input:
- SetMap *map: The mapping array.
- char *name: The name string to search (e.g., "SETA").
Output: Pointer to set if found, NULL otherwise.
Algorithm:
- Linear search through the map array.
- Compare strings using strcmp.
*/
set* get_set_by_name(SetMap *map, char *name)
{
  int i = 0; /* Index */
  while (map[i].name != NULL) /* Iterate until the NULL terminator of the map */
  {
    if (strcmp(name, map[i].name) == 0) /* If we found the grop name */
    {
      return map[i].setPtr; /* Call the appropriate wrapper function */
    }
    i++;
  }
  return NULL; /* Not found */
}

/*
Function: check_extra_text
Purpose: Checks if there are any remaining tokens in the input buffer.
Input: None (Uses strtok's static internal state with NULL).
Output: 
- OK (0): If the line is clean.
- ERROR (1): If extraneous text exists.
Algorithm:
- Attempt to fetch one more token using strtok.
- If a token exists, print error and return failure code.
*/
int check_extra_text()
{
  char *token = strtok(NULL, " \t\n\r"); /* Try to extract one more token from the string */
  if (token != NULL) /* Check if a token was actually found */
  {
    printf("Extraneous text after end of command\n"); /* Print error message for extraneous text */
    return ERROR; /* Return error code indicating failure */
  }
  return OK; /* Return success code if no text was found */
}


/*
Function: get_set_arg
Purpose: Helper function to parse the next token as a set argument.
Input: 
- SetMap *map: The mapping array used to validate the set name.
Output: 
- set*: Pointer to the found set structure.
- NULL: If parsing fails or set is undefined.
Algorithm:
- Extract next token.
- Validate token existence (Missing parameter).
- Validate set name existence in map (Undefined set).
*/
set *get_set_arg(SetMap *map)
{
  char *name; /* Variable to hold the parsed name string */
  set *s; /* Variable to hold the resulting set pointer */

  name = strtok(NULL, " \t,\n\r"); /* Get the next token using delimiters (space, tab, comma, newline) */
  if (name == NULL) /* if the token is NULL (end of line reached prematurely) */
  {
    printf("Missing parameter\n"); /* Print error message for missing parameter */
    return NULL; /* Return NULL to indicate error */
  }
  s = get_set_by_name(map, name); /* Search for the set name in the provided map */
  if (s == NULL) /* if the set was not found (s is NULL) */
  {
    printf("Undefined set name\n");/* Print error message for undefined set name */
    return NULL; /* Return NULL to indicate error */
  }
    return s; /* Return the valid set pointer */
}

/*
Function: validate_input
Purpose: Performs comprehensive syntax validation on the input line.
         Detects illegal commas, missing commas, and consecutive commas.
Input:
- char *line: The raw input string to validate.
- SetMap *map: Used for context-aware checks (smart missing comma detection).
Output: 
- OK (0): If syntax is valid.
- ERROR (1): If a syntax error is detected.
Algorithm:
1. Skip leading whitespace.
2. Skip command name.
3. Check for illegal commas around the command.
4. Iterate through arguments checking for comma correctness.
5. Use backtracking to distinguish "missing comma" from "undefined set".
*/
int validate_input(char *line, SetMap *map)
{
  char *ptr = line; /* Pointer to traverse the string */
  int in_word = FALSE;  /* Flag: Indicates if we are inside a word */     
  int after_word_space = FALSE; /* Flag: Indicates we finished a word and saw a space */
    
  while (*ptr && isspace(*ptr)) /* Skip all leading whitespace characters */
  {
    ptr++;
  }
  if (*ptr == '\0') /* Check if line is empty (end of string) */
  {
    return OK; /* Return success code */ 
  }
  while (*ptr && !isspace(*ptr) && *ptr != ',') /* Skip over the command name (alphanumeric characters) */
  {
    ptr++;
  } 
  if (*ptr == ',') /* Check for an illegal comma immediately after the command name */
  {
    printf("Illegal comma\n"); 
    return ERROR;/* Return error code indicating failure */
  }
  while (*ptr && isspace(*ptr))/* Skip spaces between the command and the first argument */
  {
    ptr++;
  }
  if (*ptr == ',')
  {
    printf("Illegal comma\n"); 
    return ERROR; /* Return error code indicating failure */
  }
  while (*ptr)
  {
    if (*ptr == ',') /* Case 1: Current character is a comma */
    {
      char *next = ptr + 1;/* Create a look-ahead pointer to check the next character */
      while (*next && isspace(*next)) /* Skip whitespace after the comma */
      {
        next++;
      } 
      if (*next == '\0')/* Check if line ends immediately after comma (Extraneous text error) */
      {
        printf("Extraneous text after end of command\n");
        return ERROR;/* Return error code indicating failure */
      }
      if (*next == ',') /* Check if the next character is also a comma (Multiple consecutive commas) */
      {
        printf("Multiple consecutive commas\n");
        return ERROR;/* Return error code indicating failure */
      } 
      /* Reset flags because a comma satisfies the separation requirement */
      in_word = FALSE;
      after_word_space = FALSE;
    }
    else if (isspace(*ptr)) /* Case 2: Current character is a whitespace */
    {
      if (in_word) /* We are no longer in a word */ 
      {
        in_word = FALSE;
        after_word_space = TRUE; /* We are now in the space after a word */
      }
    }
    else /* Case 3: Current character is a regular character (start of a new word) */
    {
      if (after_word_space) 
      {
        char *back = ptr - 1; /* Pointer to backtrack */
        char *end; /* Pointer to end of previous word */
        char temp[MAX_TEMP_WORD_LEN]; /* Buffer to hold previous word */
        int len; /* Length of previous word */

        while (back > line && isspace(*back))/* Backtrack: skip spaces backwards to find end of previous word */ 
        {
          back--; 
        } 
        end = back; /* Mark the end of the word */              
        while (back >= line && !isspace(*back) && *back != ',') /* Backtrack: go to the start of the previous word */
        {
          back--;
        }  
        back++; /* Adjust pointer to start of word */
        len = (int)(end - back + 1); /* Calculate length of the previous word */
        if (len < MAX_TEMP_WORD_LEN - 1) /* Copy the word to a temp buffer if it fits */
        {
          strncpy(temp, back, len);
          temp[len] = '\0';/* Null terminate the string */                 
          if (get_set_by_name(map, temp) == NULL && !isdigit(temp[0]) && temp[0] != '-')/*  If prev word is unknown (not a set, not a number), we suppress the 'Missing comma' error to allow 'Undefined set' error later */
          {
            in_word = TRUE; /* Treat as new word */
            after_word_space = FALSE; /* Reset space flag */
            ptr++; /* Move to next char */
            continue; /* Skip the error reporting */
          }
        }   
        printf("Missing comma\n");/* If we reached here, a comma is strictly missing */
        return ERROR; /* Return error code indicating failure */
      }
      in_word = TRUE; /* Mark that we are currently inside a word */
    }
    ptr++; /* Move to the next character in the string */
  }
  return OK; /* If loop finishes without errors, return OK */
}

/*
Function: get_3_sets
Purpose: Wrapper utility to fetch exactly 3 set arguments from input.
         Used by union, intersection, sub, and symdiff functions.
Input: 
- SetMap *map: Context for set lookup.
- set **p1, **p2, **p3: Output parameters to store retrieved set pointers.
Output: 
- OK (0): If all 3 sets were retrieved successfully.
- ERROR (1): If any error occurred.
Algorithm:
- Call get_set_arg for the first set.
- Call get_set_arg for the second set.
- Call get_set_arg for the third set.
- Check for extra text at the end.
*/
int get_3_sets(SetMap *map, set **p1, set **p2, set **p3)
{
  *p1 = get_set_arg(map); /* Get the first set argument */
  if (*p1 == NULL) /* Check for error (NULL) */
  {
    return ERROR; /* Return error code indicating failure */
  }
  *p2 = get_set_arg(map); /* Get the second set argument */
  if (*p2 == NULL) /* Check for error (NULL) */
  {
    return ERROR;/* Return error code indicating failure */
  }
  *p3 = get_set_arg(map); /* Get the third set argument */
  if (*p3 == NULL) /* Check for error (NULL) */
  {
    return ERROR;/* Return error code indicating failure */
  }
  if (check_extra_text() == ERROR) /* Check if there is any garbage text left in the line */
  {
    return ERROR;/* Return error code indicating failure */
  }
  return OK; /* Return success code*/
}

/*
Function: help_read
Purpose: Handles the 'read_set' command logic.
         Reads a set name and a list of integers to populate the set.
Input: 
- SetMap *map: Context for set lookup.
Output: None.
Algorithm:
1. Parse set name.
2. Create a local TEMPORARY set (stack memory) initialized to 0.
3. Loop to parse integers.
4. For each valid integer, calculate bit/byte index and update the TEMPORARY set.
5. If all valid, call read_set to copy temporary set to real set.
*/
void help_read(SetMap *map)
{
  char *name; /* Pointer for set name token */
  char *token; /* Pointer for number tokens */
  set *s; /* Pointer to the target set */
  set temp_set = {0}; /* Temporary array to hold values Effectively*/
  int num; /* Current parsed number */
  int running = TRUE; /* Flag to control the parsing loop */
  char *endptr; /* Pointer for strtol error checking */
  int byte_index, bit_index; /* Calculated indices for bitwise op */
  int i = 0; /* Loop index for initialization */
  
  for(i = 0; i < LEN; i++) 
  {
    temp_set.arr[i] = 0;
  }
  name = strtok(NULL, " \t,\n\r"); /* Get the set name token */
  if (name == NULL) /* Check if name is missing */
  {
    printf("Missing parameter\n");
    return;
  }
  s = get_set_by_name(map, name); /* Look up the set by name */
  if (s == NULL) /* Check if set is undefined */
  {
    printf("Undefined set name\n");
    return;
  }
  while (running) /* Start loop to parse the list of numbers */
  {
    token = strtok(NULL, " \t,\n\r"); /* Get the next number token */
    if (token == NULL) /* Check if list ended abruptly without -1 */
    {
      printf("List of set members is not terminated correctly\n");
      return;
    }
    num = (int)strtol(token, &endptr, DECIMAL_BASE); /* Convert the string token to an integer */
    if (*endptr != '\0') /* Check if conversion failed (not an integer) */
    {
      printf("Invalid set member - not an integer\n");
      return;
    }
    if (num == DATA_TERMINATOR) /* Check for the terminator value (-1) */
    {
      running = FALSE; /* Terminator found */
    }
    else if (num < MIN_VAL || num > MAX_VAL) /* Check if number is out of valid range (0-127) */
    {
      printf("Invalid set member - value out of range\n");
      return;
    }
    else 
    {
     /* Translate to bits for the temp set */
      byte_index = num / BIT;
      bit_index = num % BIT;
      /* Turn on the specific bit in the temporary set */
      temp_set.arr[byte_index] |= (1 << bit_index);
    }
  }
  if (check_extra_text() == ERROR) /* Verify no extra text exists after the -1 terminator */
  {
    return;
  }
  read_set(s, &temp_set); /* Update the actual set using the temporary array */
}

/*
Function: help_print
Purpose: Handles the 'print_set' command.
         Parses the set name and calls the print function.
Input: 
- SetMap *map: Context for set lookup.
Output: None.
Algorithm:
- Parse set name.
- Validate set existence.
- Check for extra text.
- Call print_set().
*/
void help_print(SetMap *map)
{
  char *name; /* Pointer for set name */
  set *s; /* Pointer to the set */
  name = strtok(NULL, " \t,\n\r"); /* Get the set name token */
  if (name == NULL) /* Check if parameter is missing */
  {
    printf("Missing parameter\n");
    return;
  }
  s = get_set_by_name(map, name); /* Look up set by name */
  if (s == NULL) /* Check if set exists */
  {
    printf("Undefined set name\n");
    return;
  }
  if (check_extra_text() != OK) /* Check for extraneous text */
  {
    return;
  }
  print_set(s); /* Perform the print operation */
}

/*
Function: help_union
Purpose: Handler for 'union_set' command.
         Orchestrates the Union operation between 3 sets.
Input: 
- SetMap *map: Context for set lookup.
Output: None.
Algorithm:
- Use get_3_sets helper to parse and validate 3 set arguments.
- If successful, call union_set() with the retrieved pointers.
*/
void help_union(SetMap *map)
{
  set *s1, *s2, *s3; /* Pointers for the 3 sets */
  if (get_3_sets(map, &s1, &s2, &s3) == OK) 
  {
    union_set(s1, s2, s3);/* Perform the Union operation: s3 = s1 OR s2 */
  }
}

/*
Function: help_intersect
Purpose: Handler for 'intersect_set' command.
         Orchestrates the Intersection operation between 3 sets.
Input: 
- SetMap *map: Context for set lookup.
Output: None.
Algorithm:
- Use get_3_sets helper to parse and validate 3 set arguments.
- If successful, call intersect_set() with the retrieved pointers.
*/
void help_intersect(SetMap *map)
{
  set *s1, *s2, *s3; /* Pointers for the 3 sets */
  if (get_3_sets(map, &s1, &s2, &s3) == OK) /* Attempt to retrieve 3 valid sets from input */
  {
    intersect_set(s1, s2, s3); /* Perform the Intersection operation: s3 = s1 AND s2 */
  }
}

/*
Function: help_sub
Purpose: Handler for 'sub_set' command.
         Orchestrates the Subtraction operation between 3 sets.
Input: 
- SetMap *map: Context for set lookup.
Output: None.
Algorithm:
- Use get_3_sets helper to parse and validate 3 set arguments.
- If successful, call sub_set() with the retrieved pointers.
*/
void help_sub(SetMap *map)
{
  set *s1, *s2, *s3; /* Pointers for the 3 sets */
  if (get_3_sets(map, &s1, &s2, &s3) == OK) /* Attempt to retrieve 3 valid sets from input */
  {
    sub_set(s1, s2, s3); /* Perform the Subtraction operation: s3 = s1 - s2 */ 
  }
}

/*
Function: help_symdiff
Purpose: Handler for 'symdiff_set' command.
         Orchestrates the Symmetric Difference operation between 3 sets.
Input: 
- SetMap *map: Context for set lookup.
Output: None.
Algorithm:
- Use get_3_sets helper to parse and validate 3 set arguments.
- If successful, call symdiff_set() with the retrieved pointers.
*/
void help_symdiff(SetMap *map)
{
  set *s1, *s2, *s3;/* Pointers for the 3 sets */
  if (get_3_sets(map, &s1, &s2, &s3) == OK) /* Attempt to retrieve 3 valid sets from input */
  {
    symdiff_set(s1, s2, s3);/* Perform the Symmetric Difference operation: s3 = s1 XOR s2 */
  }
}

/*
Function: help_stop
Purpose: Handler for 'stop' command.
         Ensures clean termination of the program.
Input: 
- SetMap *map: Not used, but required to match function pointer signature.
Output: None (Calls exit).
Algorithm:
- Check for extraneous text after the 'stop' command.
- If clean, call stop() function to exit.
*/
void help_stop(SetMap *map)
{
  if (check_extra_text() != OK) /* Verify there is no text after the command "stop" */
  {
    return;
  }
  stop();
}





