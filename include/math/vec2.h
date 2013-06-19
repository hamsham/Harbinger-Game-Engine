/*
 * 2d vector structure
 * recommended for use with non-integral types

 * NOTES:
 * Orientation is as follows:
 * ----------
 *		X
 *		Y
 * ----------
*/

#ifndef __HL_MATH_VEC2_H__
#define __HL_MATH_VEC2_H__

#include "../utils/assert.h"

namespace hamLibs {
namespace math {

template <typename numType>
struct vec2_t {
	union {
		numType v[2];
		struct {
			numType	x, y;
		} index;
	};

	//constructors
	vec2_t			();
	vec2_t			( numType n );
	vec2_t			(const vec2_t<numType>&);
	vec2_t			(vec2_t<numType>&&);
	vec2_t			(numType inX, numType inY);
	~vec2_t			() {}

	//array operators
	numType			operator		[]		(int) const;
	numType&		operator		[]		(int);

	//vector-vector operators
	vec2_t&			operator		= 		(const vec2_t<numType>&);
	vec2_t&			operator		= 		(vec2_t<numType>&&);
	vec2_t			operator 		+ 		(const vec2_t<numType>&) const;
	vec2_t			operator 		- 		(const vec2_t<numType>&) const;
	vec2_t			operator 		- 		() const;
	vec2_t			operator 		* 		(const vec2_t<numType>&) const;
	vec2_t			operator 		/ 		(const vec2_t<numType>&) const;
	vec2_t&			operator 		+= 		(const vec2_t<numType>&);
	vec2_t&			operator 		-= 		(const vec2_t<numType>&);
	vec2_t&			operator 		*= 		(const vec2_t<numType>&);
	vec2_t&			operator 		/= 		(const vec2_t<numType>&);
	vec2_t&			operator 		++		(); //prefix operators
	vec2_t&			operator 		--		();
	vec2_t			operator 		++		(int); //postfix operators
	vec2_t			operator 		--		(int);
	bool			operator		==		(const vec2_t<numType>& compare) const; //comparisons
	bool			operator		!=		(const vec2_t<numType>& compare) const;
	bool			operator		<		(const vec2_t<numType>& compare) const;
	bool			operator		>		(const vec2_t<numType>& compare) const;
	bool			operator		<=		(const vec2_t<numType>& compare) const;
	bool			operator		>=		(const vec2_t<numType>& compare) const;

	//vector-matrix operators
	mat2_t<numType>	operator		+		(const mat2_t<numType>&) const;
	mat2_t<numType>	operator		-		(const mat2_t<numType>&) const;
	vec2_t<numType>	operator		*		(const mat2_t<numType>&) const;
	vec2_t&			operator		*=		(const mat2_t<numType>&);

	//vector-scalar operators
	vec2_t			operator 		= 		(numType);
	vec2_t			operator 		+ 		(numType) const;
	vec2_t			operator 		- 		(numType) const;
	vec2_t			operator 		* 		(numType) const;
	vec2_t			operator 		/ 		(numType) const;
	vec2_t&			operator 		+= 		(numType);
	vec2_t&			operator 		-= 		(numType);
	vec2_t&			operator 		*= 		(numType);
	vec2_t&			operator 		/= 		(numType);
};

//---------------------------------------------------------------------
//	Vector Constructors
//---------------------------------------------------------------------
template <typename numType> inline
vec2_t<numType>::vec2_t() :
	vec2_t(
		numType(0), numType(0)
	)
{}
template <typename numType> inline
vec2_t<numType>::vec2_t( numType n ) :
	vec2_t( n, n )
{}

template <typename numType> inline
vec2_t<numType>::vec2_t(const vec2_t<numType>& input) :
	vec2_t(
		input.v[0], input.v[1]
	)
{}

template <typename numType> inline
vec2_t<numType>::vec2_t(vec2_t<numType>&& input) :
	vec2_t(
		input.v[0], input.v[1]
	)
{}

template <typename numType> inline
vec2_t<numType>::vec2_t(numType inX, numType inY) :
	v{ inX, inY }
{}

//---------------------------------------------------------------------
//	Array Operators
//---------------------------------------------------------------------
template <typename numType> inline
numType vec2_t<numType>::operator[](const int index) const {
	HL_DEBUG_ASSERT((index == 0) || (index == 1));
	return v[ index ];
}

template <typename numType> inline
numType& vec2_t<numType>::operator[](const int index) {
	HL_DEBUG_ASSERT((index == 0) || (index == 1));
	return v[ index ];
}

//---------------------------------------------------------------------
//	Vector-Vector Math Operations
//---------------------------------------------------------------------
template <typename numType> inline
vec2_t<numType>& vec2_t<numType>::operator = (const vec2_t<numType>& input) {
	v[0] = input.v[0];
	v[1] = input.v[1];
	return *this;
}

template <typename numType> inline
vec2_t<numType>& vec2_t<numType>::operator = (vec2_t<numType>&& input) {
	v[0] = input.v[0];
	v[1] = input.v[1];
	return *this;
}

template <typename numType> inline
vec2_t<numType> vec2_t<numType>::operator + (const vec2_t<numType>& input) const {
	return vec2_t<numType>(
		v[0] + input.v[0],
		v[1] + input.v[1]
	);
}

template <typename numType> inline
vec2_t<numType> vec2_t<numType>::operator - (const vec2_t<numType>& input) const {
	return vec2_t<numType>(
		v[0] - input.v[0],
		v[1] - input.v[1]
	);
}

//for operations like "vectA = -vectB"
template <typename numType> inline
vec2_t<numType> vec2_t<numType>::operator - () const {
	return vec2_t<numType>(-v[0], -v[1]);
}

template <typename numType> inline
vec2_t<numType> vec2_t<numType>::operator * (const vec2_t<numType>& input) const {
	return vec2_t<numType>(
		v[0] * input.v[0],
		v[1] * input.v[1]
	);
}

template <typename numType> inline
vec2_t<numType> vec2_t<numType>::operator / (const vec2_t<numType>& input) const {
	return vec2_t<numType>(
		v[0] / input.v[0],
		v[1] / input.v[1]
	);
}

template <typename numType> inline
vec2_t<numType>& vec2_t<numType>::operator += (const vec2_t<numType>& input) {
	v[0] += input.v[0];
	v[1] += input.v[1];
	return *this;
}

template <typename numType> inline
vec2_t<numType>& vec2_t<numType>::operator -= (const vec2_t<numType>& input) {
	v[0] -= input.v[0];
	v[1] -= input.v[1];
	return *this;
}

template <typename numType> inline
vec2_t<numType>& vec2_t<numType>::operator *= (const vec2_t<numType>& input) {
	v[0] *= input.v[0];
	v[1] *= input.v[1];
	return *this;
}

template <typename numType> inline
vec2_t<numType>& vec2_t<numType>::operator /= (const vec2_t<numType>& input) {
	v[0] /= input.v[0];
	v[1] /= input.v[1];
	return *this;
}

// prefix operations
template <typename numType> inline
vec2_t<numType>& vec2_t<numType>::operator++ () {
	++v[0];
	++v[1];
	return *this;
}
template <typename numType> inline
vec2_t<numType>& vec2_t<numType>::operator-- () {
	--v[0];
	--v[1];
	return *this;
}
//postfix operations
template <typename numType> inline
vec2_t<numType> vec2_t<numType>::operator++ (int) {
	return vec2_t<numType>(
		++v[0],
		++v[1]
	);
}
template <typename numType> inline
vec2_t<numType> vec2_t<numType>::operator-- (int) {
	return vec2_t<numType>(
		--v[0],
		--v[1]
	);
}

//comparisons
template <typename numType> inline
bool vec2_t<numType>::operator== (const vec2_t<numType>& compare) const {
	return (	v[0] == compare.v[0] &&
				v[1] == compare.v[1]
			);
}

template <typename numType> inline
bool vec2_t<numType>::operator!= (const vec2_t<numType>& compare) const {
	return (	v[0] != compare.v[0] &&
				v[1] != compare.v[1]
			);
}

template <typename numType> inline
bool vec2_t<numType>::operator< (const vec2_t<numType>& compare) const {
	return (	v[0] < compare.v[0] &&
				v[1] < compare.v[1]
			);
}

template <typename numType> inline
bool vec2_t<numType>::operator> (const vec2_t<numType>& compare) const {
	return (	v[0] > compare.v[0] &&
				v[1] > compare.v[1]
			);
}

template <typename numType> inline
bool vec2_t<numType>::operator<= (const vec2_t<numType>& compare) const {
	return (	v[0] <= compare.v[0] &&
				v[1] <= compare.v[1]
			);
}

template <typename numType> inline
bool vec2_t<numType>::operator>= (const vec2_t<numType>& compare) const {
	return (	v[0] >= compare.v[0] &&
				v[1] >= compare.v[1]
			);
}

//---------------------------------------------------------------------
//	Vector-Matrix Math Operations
//---------------------------------------------------------------------
template <typename numType> inline
mat2_t<numType> vec2_t<numType>::operator + (const mat2_t<numType>& m) const {
	return mat2_t<numType>(
		v[0] + m.m[0][0], v[0] + m.m[0][1],
		v[1] + m.m[1][0], v[1] + m.m[1][1]
	);
}

template <typename numType> inline
mat2_t<numType> vec2_t<numType>::operator - (const mat2_t<numType>& m) const {
	return mat2_t<numType>(
		v[0] - m.m[0][0], v[0] - m.m[0][1],
		v[1] - m.m[1][0], v[1] - m.m[1][1]
	);
}

template <typename numType> inline
vec2_t<numType> vec2_t<numType>::operator * (const mat2_t<numType>& m) const {
	return vec2_t<numType>(
		(m.m[0][0] * v[0]) + (m.m[0][1] * v[1]),
		(m.m[1][0] * v[0]) + (m.m[1][1] * v[1])
	);
}

template <typename numType> inline
vec2_t<numType>& vec2_t<numType>::operator *= (const mat2_t<numType>& m) {
	*this = *this * m;
	return *this;
}

//---------------------------------------------------------------------
//	Vector-Scalar Math Operations
//---------------------------------------------------------------------
template <typename numType> inline
vec2_t<numType> vec2_t<numType>::operator = (numType input) {
	v[0] = input;
	v[1] = input;
	return *this;
}
template <typename numType> inline
vec2_t<numType> vec2_t<numType>::operator + (numType input) const {
	return vec2_t<numType>(
		v[0] + input,
		v[1] + input
	);
}
template <typename numType> inline
vec2_t<numType> vec2_t<numType>::operator - (numType input) const {
	return vec2_t<numType>(
		v[0] - input,
		v[1] - input
	);
}
template <typename numType> inline
vec2_t<numType> vec2_t<numType>::operator * (numType input) const {
	return vec2_t<numType>(
		v[0] * input,
		v[1] * input
	);
}
template <typename numType> inline
vec2_t<numType> vec2_t<numType>::operator / (numType input) const {
	return vec2_t<numType>(
		v[0] / input,
		v[1] / input
	);
}
template <typename numType> inline
vec2_t<numType>& vec2_t<numType>::operator += (numType input) {
    v[0] += input;
    v[1] += input;
	return *this;
}
template <typename numType> inline
vec2_t<numType>& vec2_t<numType>::operator -= (numType input) {
    v[0] -= input;
    v[1] -= input;
	return *this;
}
template <typename numType> inline
vec2_t<numType>& vec2_t<numType>::operator *= (numType input) {
    v[0] *= input;
    v[1] *= input;
	return *this;
}
template <typename numType> inline
vec2_t<numType>& vec2_t<numType>::operator /= (numType input) {
    v[0] /= input;
    v[1] /= input;
	return *this;
}

} //end math namespace
} //end hamlibs namespace

#endif /* __HL_MATH_VEC2_H__*/
