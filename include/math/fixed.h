/* 
 * File:   fixed.h
 * Author: hammy
 *
 * Created on May 17, 2013, 6:02 AM
 */

#ifndef __HL_FIXED_H__
#define	__HL_FIXED_H__

namespace hamLibs {
namespace math {

template <typename fixedBase, unsigned fracDigits>
class fixed_t {
    
    private:
        fixedBase number;
        
    public:
        constexpr fixed_t       ()                     : number( 0 ) {}
        constexpr fixed_t       ( const fixed_t& f )   : number( f.number ) {}
        constexpr fixed_t       ( fixedBase f )        : number( f ) {}
        constexpr fixed_t       ( float f )            : number( (fixedBase(1) << fracDigits) * f ) {}
        ~fixed_t                () = default;
        
        fixed_t&    operator    ++  ()                                      { return *this = fixed_t( ++number ); }
        fixed_t&    operator    --  ()                                      { return *this = fixed_t( --number ); }
        
        fixed_t     operator    ++  ( int )                                 { return fixed_t( ++number ); }
        fixed_t     operator    --  ( int )                                 { return fixed_t( --number ); }
        
        constexpr   bool        operator    !   () const                    { return !number; }
        
        constexpr   bool        operator    ==  ( const fixed_t& f ) const  { return number == f.number; }
        constexpr   bool        operator    !=  ( const fixed_t& f ) const  { return number != f.number; }
        constexpr   bool        operator    >=  ( const fixed_t& f ) const  { return number >= f.number; }
        constexpr   bool        operator    <=  ( const fixed_t& f ) const  { return number <= f.number; }
        constexpr   bool        operator    >   ( const fixed_t& f ) const  { return number > f.number; }
        constexpr   bool        operator    <   ( const fixed_t& f ) const  { return number < f.number; }
        
        constexpr   fixed_t     operator    +   ( const fixed_t& f ) const  { return fixed_t( number + f.number ); }
        constexpr   fixed_t     operator    -   ( const fixed_t& f ) const  { return fixed_t( number - f.number ); }
        constexpr   fixed_t     operator    -   () const                    { return fixed_t( -number ); }
        constexpr   fixed_t     operator    *   ( const fixed_t& f ) const  { return fixed_t( number * f.number ); }
        constexpr   fixed_t     operator    /   ( const fixed_t& f ) const  { return fixed_t( number / f.number ); }
        constexpr   fixed_t     operator    %   ( const fixed_t& f ) const  { return fixed_t( number % f.number ); }
        constexpr   fixed_t     operator    &   ( const fixed_t& f ) const  { return fixed_t( number & f.number ); }
        constexpr   fixed_t     operator    |   ( const fixed_t& f ) const  { return fixed_t( number | f.number ); }
        constexpr   fixed_t     operator    ^   ( const fixed_t& f ) const  { return fixed_t( number ^ f.number ); }
        constexpr   fixed_t     operator    ~   () const                    { return fixed_t( ~number ); }
        constexpr   fixed_t     operator    >>  ( const fixed_t& f ) const  { return fixed_t( number >> f.number ); }
        constexpr   fixed_t     operator    <<  ( const fixed_t& f ) const  { return fixed_t( number << f.number ); }
        
        constexpr   fixed_t&    operator    =    ( const fixed_t& f )       { number = f.number; return *this; }
        fixed_t&                operator    +=   ( const fixed_t& f )       { number += f.number; return *this; }
        fixed_t&                operator    -=   ( const fixed_t& f )       { number -= f.number; return *this; }
        fixed_t&                operator    *=   ( const fixed_t& f )       { number *= f.number; return *this; }
        fixed_t&                operator    /=   ( const fixed_t& f )       { number /= f.number; return *this; }
        fixed_t&                operator    %=   ( const fixed_t& f )       { number %= f.number; return *this; }
        fixed_t&                operator    &=   ( const fixed_t& f )       { number &= f.number; return *this; }
        fixed_t&                operator    |=   ( const fixed_t& f )       { number |= f.number; return *this; }
        fixed_t&                operator    ^=   ( const fixed_t& f )       { number ^= f.number; return *this; }
        fixed_t&                operator    >>=  ( const fixed_t& f )       { number >>= f.number; return *this; }
        fixed_t&                operator    <<=  ( const fixed_t& f )       { number <<= f.number; return *this; }
        
        constexpr   operator    float       ()          { return float(1.f / (fixedBase(1) << fracDigits)) * number; }
        constexpr   fixed_t&    operator =  ( float f ) { return *this = fixed_t( f ); }
};

}//end math namespace
}//end hamlibs namespace

#endif	/* __HL_FIXED_H__ */

