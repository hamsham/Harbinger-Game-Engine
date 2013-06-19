/* 
 * File:   endian.h
 * Author: hammy
 *
 * Created on March 13, 2013, 11:17 PM
 */

#ifndef __HL_ENDIAN_H__
#define __HL_ENDIAN_H__

#include <cstdint>

enum e_endianness : int {
    HL_UNKNOWN_ENDIAN  = -1,
    HL_LITTLE_ENDIAN   = 0,
    HL_BIG_ENDIAN      = 1,
    HL_PDP_ENDIAN      = 2
};

union endianValue {
    uint8_t     lsBit[ sizeof( uint32_t ) ] = {0,1,2,3};
    uint32_t    byteOrder;
};

constexpr int checkEndianness() {
    return ( endianValue().byteOrder == 0x03020100 )	// If Little Endian Byte Order,
        ? HL_LITTLE_ENDIAN                          	// return 0 for little endian.
        : ( endianValue().byteOrder == 0x00010203 )     // Else if Big Endian Byte Order,
            ? HL_BIG_ENDIAN                         	// return 1 for big endian.
            : ( endianValue().byteOrder == 0x01000302 ) // Else if PDP Endian Byte Order,
                ? HL_PDP_ENDIAN                         // return 2 for pdp endian.
                : HL_UNKNOWN_ENDIAN;                    // Else return -1 for wtf endian.
}

const int HL_ENDIANNESS_CHECK = checkEndianness();

/*
 * Only doing this due to IDE syntax-highlighting
 * Creates a macro named HL_ENDIANNESS for a compile-time constant
 */
#if HL_ENDIANNESS_CHECK == HL_LITTLE_ENDIAN
    #define HL_ENDIANNESS HL_LITTLE_ENDIAN
	
#elif HL_ENDIANNESS_CHECK == HL_BIG_ENDIAN
    #define HL_ENDIANNESS HL_BIG_ENDIAN
	
#elif HL_ENDIANNESS_CHECK == HL_PDP_ENDIAN
    #define HL_ENDIANNESS HL_PDP_ENDIAN
	
#else
    #define HL_ENDIANNESS HL_UNKNOWN_ENDIAN
	
#endif

// remove compiler warning about a variable being set but not used
inline int hlGetEndianness() {
    return HL_ENDIANNESS_CHECK;
}

#endif /* __HL_ENDIAN_H__ */
