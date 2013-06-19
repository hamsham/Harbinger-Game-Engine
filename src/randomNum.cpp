
#include <ctime>
#include "../include/utils/randomNum.h"
#include "../include/utils/assert.h"

constexpr unsigned long getMaxULong() { return 0UL - 1; }

namespace hamLibs {
namespace utils {

void randomNum::seed( unsigned long s ) {
    for ( unsigned int i = 0; i < 16; ++i ) {
        state[i] = s;
        genRandNum(); // initializing the state to random bits
    }
    index = 0;
}

void randomNum::seed() {
    unsigned long s = time( NULL );
    for ( unsigned int i = 0; i < 16; ++i ) {
        state[i] = s;
        genRandNum();
    }
    index = 0;
}

unsigned long randomNum::genRandNum() {
    unsigned long a, b, c, d;
    a = state[index];
    c = state[(index+13)&15];
    b = a^c^(a<<16)^(c<<15);
    c = state[(index+9)&15];
    c ^= (c>>11);
    a = state[index] = b^c;
    d = a^((a<<5)&0xDA442D24UL);
    index = (index + 15)&15;
    a = state[index];
    state[index] = a^b^d^(a<<2)^(b<<18)^(c<<28);
    return state[index];
}

float randomNum::randRangeF( float low, float high ) {
    HL_ASSERT( low < high );
    return low + ((float)genRandNum() / ( (float)getMaxULong() / (high-low) ));
}

int randomNum::randRangeI( int low, int high ) {
    HL_ASSERT( low < high );
    return static_cast<int>(randRangeF(
        static_cast<float>(low),
        static_cast<float>(high)
    ));
}

} // end utils namespace
} // end hamlibs namespace
        