/* 
 * CS:APP Data Lab 
 * 
 * Chang Liu 704291153 
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {

 /* This function will look for location of first 1 or 0 for negative numbers
  *	and count the number of bits following that. Then we just add 1 to get
  * the amount of bits needed to represent that number. 
  */

 int a = (x >> 31) ^ x; 
 int m1 = 0x55;   /* 0x55555555 */
 int m2;          /* 0xAAAAAAAA */
 int m3 = 0x33;   /* 0x33333333 */
 int m4;          /* 0xCCCCCCCC */
 int m5 = 0x0F;   /* 0x0F0F0F0F */
 int m6;          /* 0xF0F0F0F0 */
 int m7;          /* 0x00FF00FF */
 int m8 = 0xFF;   /* 0xFF00FF00 */
 int m9 = 0xFF;   /* 0x0000FFFF */
 int m10;         /* 0xFFFF0000 */

 /* generate mask with all 1’s after position of first one */
 a = a | (a >> 1);
 a = a | (a >> 2);
 a = a | (a >> 4);
 a = a | (a >> 8);
 a = a | (a >> 16);

 /* count number of 1's and add 1 to get number of bits needed */ 

 m1 = (m1 << 8)  + m1;
 m1 = (m1 << 16) + m1;
 m2 = ~m1;
 m3 = (m3 << 8)  + m3;
 m3 = (m3 << 16) + m3;
 m4 = ~m3;
 m5 = (m5 << 8)  + m5;
 m5 = (m5 << 16) + m5;
 m6 = ~m5;
 m8 = (m8 << 8);
 m8 = (m8 << 16)  + m8;
 m7 = ~m8;
 m9 = (m9 << 8) + m9;
 m10 = ~m9;

 a = (a & m1) + ((a & m2) >> 1);
 a = (a & m3) + ((a & m4) >> 2);
 a = (a & m5) + ((a & m6) >> 4);
 a = (a & m7) + ((a & m8) >> 8);
 a = (a & m9) + ((a & m10) >> 16);

 return a + 1;
}
/* 
 * sm2tc - Convert from sign-magnitude to two's complement
 *   where the MSB is the sign bit
 *   Example: sm2tc(0x80000005) = -5.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 4
 */
int sm2tc(int x) {
/*
 * Does not change anything if the number was positive to begin with. 
 * If x was negative, it inverts the number, adds the signed bit and 1.  
 */ 

 int mask = x >> 31; 
 int msb = mask << 31; 
 int a = x ^ mask; 
 int b = mask & 1; 
 return msb + a + b; 
}
/* 
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3
 */
int isNonNegative(int x) {
/*
 *
 */
 return !(x & (1 << 31));
}

/*
 * rotateRight - Rotate x to the right by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateRight(0x87654321,4) = 0x18765432
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateRight(int x, int n) {
/*
 * I first saved the designated bits to rotate in a variable and shifted 
 * them to the left most side. I then right-shifted x by n bits to hide
 * those bits and made the new left bits all zeros. A bitwise OR operation on 
 * the saved leftmost bits would give the correct result.  
 */
 int leftBits = x << (32 + ~n + 1);
 return ((x >> n) & ((~(1<<31)) >> (n + ~0))) | leftBits;
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
/*
 * For positive numbers, dividing by 2^n is just an arithmetic right shift 
 * of n. For negative numbers, we need a bias of 2^n - 1 to be added before
 * the shift.
 */
 int sign = !!(x & (1 << 31)); 
 return ((x + ((sign << n) + ~sign + 1)) >> n);  
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
 /*
 * x will have all odd bits if x && 0xAAAAAAAA evaluates to 0xAAAAAAAA
 */
 int mask = 0xAA; 
 mask |= mask << 8; 
 mask |= mask << 16; 
 mask |= mask << 24; 
 return !((x & mask) ^ mask);
}
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
/*
 * In Boolean terms, A XOR B == (A | B) && ~(A & B) 
 * (DeMorgan's Law) == ~(~A & ~B) && ~(A & B)
 */
 return ~(~x & ~y) & ~(x & y); 

}
/*
 * isTmin - returns 1 if x is the minimum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmin(int x) {
/*
 * If x is Tmin, then x + x would overflow and yield 0. Negating 0 would then 
 * yield 1, proving that x is Tmin. 
 * Corner case of x = 0 will return 0 as desired. 
 */
 return !(x + x) ^ !x; 
}
