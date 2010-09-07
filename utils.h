#ifndef UTILS_H
#define UTILS_H

extern const char HEXTABLE[];

//convert a nibble to an int
int HexToInt( const char input );

//convert two nibbles to an int
int HexToInt( const char msB, const char lsB );

//find rightmost set bit
int FindOnBitUShort( unsigned short b );

#endif
