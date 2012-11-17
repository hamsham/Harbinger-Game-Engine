/* 
 * File:   miscellaneous.h
 * Author: hammy
 *
 * Created on November 16, 2012, 5:19 PM
 */

#ifndef MISCELLANEOUS_H
#define	MISCELLANEOUS_H

	
//------------------------------------------------------------------------
//	Object Pairs
//------------------------------------------------------------------------
template < typename T1, typename T2 >
struct s_pair {
	T1 a;
	T1 b;

	s_pair();
	s_pair( const s_pair& );
	~s_pair() {}
	
	s_pair& operator = ( const s_pair& pairCopy );
	bool operator == ( const s_pair& pairCopy );
	bool operator != ( const s_pair& pairCopy );
};

template < typename T1, typename T2 >
s_pair< T1, T2 >::s_pair() :
	a(),
	b()
{}

template < typename T1, typename T2 >
s_pair< T1, T2 >::s_pair( const s_pair& pairCopy ) :
	a( pairCopy.a ),
	b( pairCopy.b )
{}

template < typename T1, typename T2 >
s_pair< T1, T2 >::~s_pair() {}

template < typename T1, typename T2 >
s_pair< T1, T2 >& s_pair< T1, T2 >::operator =(const s_pair& pairCopy) {
	a = pairCopy.a;
	b = pairCopy.b;
}

template < typename T1, typename T2 >
bool s_pair< T1, T2 >::operator ==(const s_pair& pairCopy) {
	return ((a == pairCopy.a) && (b == pairCopy.b));
}

template < typename T1, typename T2 >
bool s_pair< T1, T2 >::operator !=(const s_pair& pairCopy) {
	return ((a != pairCopy.a) && (b != pairCopy.b));
}

#endif	/* MISCELLANEOUS_H */

