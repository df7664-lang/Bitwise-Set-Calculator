/*
 * File: set.c
 * Description: Implementation of set operations using bitwise logic.
 * This file contains the core logic for Union, Intersect, Sub, etc.
 * The implementation uses an array of bits to represent set members efficiently.
 */
#include "set.h"

/*
Function: union_set
Purpose: Computes the union of two sets (A U B) and stores the result in C.
Input:
- set *setA: Pointer to the first source set.
- set *setB: Pointer to the second source set.
- set *setC: Pointer to the destination set.
Output: None.
Algorithm:
- Iterate through the byte array from index 0 to LEN-1.
- Perform bitwise OR operation: dest[i] = sourceA[i] | sourceB[i].
  (If a bit is 1 in either A OR B, it becomes 1 in C).
*/
void union_set(set *setA, set * setB, set * setC)
{
  int i; /* Index for iterating over the bytes. */
  for(i = 0; i < LEN; i++) /* Iterate over all 16 bytes of the set */
  {
    setC -> arr[i] = (setA -> arr[i]) | (setB -> arr[i]); /* Apply Bitwise OR (|) operation byte-by-byte */
  }
}

/*
Function: intersect_set
Purpose: Computes the intersection of two sets (A n B).
Input:
- set *setA: Pointer to the first source set.
- set *setB: Pointer to the second source set.
- set *setC: Pointer to the destination set.
Output: None.
Algorithm:
- Iterate through the byte array.
- Perform bitwise AND operation: dest[i] = sourceA[i] & sourceB[i].
  (Only if a bit is 1 in BOTH A AND B, it becomes 1 in C).
*/
void intersect_set(set * setA, set * setB, set * setC)
{
  int i; /* Index for iterating over the bytes. */
  for(i = 0; i < LEN; i++) /* Iterate over all 16 bytes of the set */
  {
    setC -> arr[i] = (setA -> arr[i]) & (setB -> arr[i]); /* Apply Bitwise AND (&) operation */
  }
}

/*
Function: sub_set
Purpose: Computes the difference of two sets (A \ B).
Input:
- set *setA: Pointer to the set to subtract from (Minuend).
- set *setB: Pointer to the set to subtract (Subtrahend).
- set *setC: Pointer to the destination set.
Output: None.
Algorithm:
- Iterate through the byte array.
- Perform bitwise AND with NOT: dest[i] = sourceA[i] & (~sourceB[i]).
  (Keep elements that are in A AND are NOT in B).
*/
 void sub_set(set * setA, set * setB, set * setC)
{
  int i; /* Index for iterating over the bytes. */
  for(i = 0; i < LEN; i++) /* Iterate over all 16 bytes of the set */
  {
    /* Apply Logic: A AND (NOT B) */
    /* ~setB->arr[i] inverts the bits of B */
    setC -> arr[i] = (setA -> arr[i]) & (~setB -> arr[i]); 
  }
}

/*
Function: symdiff_set
Purpose: Computes the symmetric difference of two sets (A XOR B).
Input:
- set *setA: Pointer to the first source set.
- set *setB: Pointer to the second source set.
- set *setC: Pointer to the destination set.
Output: None.
Algorithm:
- Iterate through the byte array.
- Perform bitwise XOR operation: dest[i] = sourceA[i] ^ sourceB[i].
  (Result contains elements present in either A or B, but NOT in both).
*/
void symdiff_set(set * setA, set * setB, set * setC)
{
  int i; /* Index for iterating over the bytes. */
  for(i = 0; i < LEN; i++) /* Iterate over all 16 bytes of the set */
  {
    setC -> arr[i] = (setA -> arr[i]) ^ (setB -> arr[i]); /* Apply Bitwise Exclusive OR (XOR) logic */
  }
}

/*
Function: stop
Purpose: Terminates the program execution immediately.
Input: None.
Output: Does not return (exits to OS).
Algorithm:
- Calls the standard library function exit() with success code (0).
*/
void stop()
{
  exit(OK);
}
