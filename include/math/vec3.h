/*
 * 3d vector structure
 * recommended for use with non-integral types

 * NOTES:
 * Orientation is as follows:
 * ----------
 *		X
 *		Y
 *		Z
 * ----------
*/

#ifndef __HL_MATH_VEC3_H__
#define __HL_MATH_VEC3_H__

#include "../utils/assert.h"

namespace hamLibs {
namespace math {

template <typename numType>
struct vec3_t {
	union {
		numType v[3];
		struct {
			numType x, y, z;
		} index;
	};

	//constructors
	vec3_t			();
	vec3_t			( numType n );
	vec3_t			(const vec3_t<numType>&);
	vec3_t			(vec3_t<numType>&&);
	vec3_t			(numType inX, numType inY, numType inZ);
	~vec3_t			(){}

	//array operators
	numType			operator		[]		(int) const;
	numType&		operator		[]		(int);

	//vector-vector operators
	vec3_t&			operator		= 		(const vec3_t<numType>&);
	vec3_t&			operator		= 		(vec3_t<numType>&&);
	vec3_t			operator 		+ 		(const vec3_t<numType>&) const;
	vec3_t			operator 		- 		(const vec3_t<numType>&) const;
	vec3_t			operator 		- 		() const;
	vec3_t			operator 		* 		(const vec3_t<numType>&) const;
	vec3_t			operator 		/ 		(const vec3_t<numType>&) const;
	vec3_t&			operator 		+= 		(const vec3_t<numType>&);
	vec3_t&			operator 		-= 		(const vec3_t<numType>&);
	vec3_t&			operator 		*= 		(const vec3_t<numType>&);
	vec3_t&			operator 		/= 		(const vec3_t<numType>&);
	vec3_t&			operator 		++		(); //prefix operators
	vec3_t&			operator 		--		();
	vec3_t			operator 		++		(int); //postfix operators
	vec3_t			operator 		--		(int);
	bool			operator		==		(const vec3_t<numType>& compare) const; //comparisons
	bool			operator		!=		(const vec3_t<numType>& compare) const;
	bool			operator		<		(const vec3_t<numType>& compare) const;
	bool			operator		>		(const vec3_t<numType>& compare) const;
	bool			operator		<=		(const vec3_t<numType>& compare) const;
	bool			operator		>=		(const vec3_t<numType>& compare) const;

	//vector-matrix operators
	mat3_t<numType>	operator		+		(const mat3_t<numType>&) const;
	mat3_t<numType>	operator		-		(const mat3_t<numType>&) const;
	vec3_t<numType>	operator		*		(const mat3_t<numType>&) const;
	vec3_t&			operator		*=		(const mat3_t<numType>&);

	//vector-scalar operators
	vec3_t			operator 		= 		(numType);
	vec3_t			operator 		+ 		(numType) const;
	vec3_t			operator 		- 		(numType) const;
	vec3_t			operator 		* 		(numType) const;
	vec3_t			operator 		/ 		(numType) const;
	vec3_t&			operator 		+= 		(numType);
	vec3_t&			operator 		-= 		(numType);
	vec3_t&			operator 		*= 		(numType);
	vec3_t&			operator 		/= 		(numType);
};

//---------------------------------------------------------------------
//	Vector Constructors
//---------------------------------------------------------------------
template <typename numType> inline
vec3_t<numType>::vec3_t() :
	vec3_t(
		numType(0), numType(0), numType(0)
	)
{}
template <typename numType> inline
vec3_t<numType>::vec3_t( numType n ) :
	vec3_t( n, n, n )
{}

template <typename numType> inline
vec3_t<numType>::vec3_t(const vec3_t<numType>& input) :
	vec3_t(
		input.v[0], input.v[1], input.v[2]
	)
{}

template <typename numType> inline
vec3_t<numType>::vec3_t(vec3_t<numType>&& input) :
	vec3_t(
		input.v[0], input.v[1], input.v[2]
	)
{}

template <typename numType> inline
vec3_t<numType>::vec3_t(numType inX, numType inY, numType inZ) :
	v{ inX, inY, inZ }
{}

//---------------------------------------------------------------------
//	Array Operators
//---------------------------------------------------------------------
template <typename numType> inline
numType vec3_t<numType>::operator[](const int index) const {
	HL_DEBUG_ASSERT((index >= 0) && (index < 3));
	return v[ index ];
}

template <typename numType> inline
numType& vec3_t<numType>::operator[](const int index) {
	HL_DEBUG_ASSERT((index >= 0) && (index < 3));
	return v[ index ];
}

//---------------------------------------------------------------------
//	Vector-Vector Math Operations
//---------------------------------------------------------------------
template <typename numType> inline
vec3_t<numType>& vec3_t<numType>::operator = (const vec3_t<numType>& input) {
	v[0] = input.v[0];
	v[1] = input.v[1];
	v[2] = input.v[2];
	return *this;
}

template <typename numType> inline
vec3_t<numType>& vec3_t<numType>::operator = (vec3_t<numType>&& input) {
	v[0] = input.v[0];
	v[1] = input.v[1];
	v[2] = input.v[2];
	return *this;
}

template <typename numType> inline
vec3_t<numType> vec3_t<numType>::operator + (const vec3_t<numType>& input) const {
	return vec3_t<numType>(
		v[0] + input.v[0],
		v[1] + input.v[1],
		v[2] + input.v[2]
	);
}

template <typename numType> inline
vec3_t<numType> vec3_t<numType>::operator - (const vec3_t<numType>& input) const {
	return vec3_t<numType>(
		v[0] - input.v[0],
		v[1] - input.v[1],
		v[2] - input.v[2]
	);
}

//for operations like "vectA = -vectB"
template <typename numType> inline
vec3_t<numType> vec3_t<numType>::operator - () const {
	return vec3_t<numType>(-v[0], -v[1], -v[2]);
}

template <typename numType> inline
vec3_t<numType> vec3_t<numType>::operator * (const vec3_t<numType>& input) const {
	return vec3_t<numType>(
		v[0] * input.v[0],
		v[1] * input.v[1],
		v[2] * input.v[2]
	);
}

template <typename numType> inline
vec3_t<numType> vec3_t<numType>::operator / (const vec3_t<numType>& input) const {
	return vec3_t<numType>(
		v[0] / input.v[0],
		v[1] / input.v[1],
		v[2] / input.v[2]
	);
}

template <typename numType> inline
vec3_t<numType>& vec3_t<numType>::operator += (const vec3_t<numType>& input) {
	v[0] += input.v[0];
	v[1] += input.v[1];
	v[2] += input.v[2];
	return *this;
}

template <typename numType> inline
vec3_t<numType>& vec3_t<numType>::operator -= (const vec3_t<numType>& input) {
	v[0] -= input.v[0];
	v[1] -= input.v[1];
	v[2] -= input.v[2];
	return *this;
}

template <typename numType> inline
vec3_t<numType>& vec3_t<numType>::operator *= (const vec3_t<numType>& input) {
	v[0] *= input.v[0];
	v[1] *= input.v[1];
	v[2] *= input.v[2];
	return *this;
}

template <typename numType> inline
vec3_t<numType>& vec3_t<numType>::operator /= (const vec3_t<numType>& input) {
	v[0] /= input.v[0];
	v[1] /= input.v[1];
	v[2] /= input.v[2];
	return *this;
}

// prefix operations
template <typename numType> inline
vec3_t<numType>& vec3_t<numType>::operator++ () {
	++v[0];
	++v[1];
	++v[2];
	return *this;
}
template <typename numType> inline
vec3_t<numType>& vec3_t<numType>::operator-- () {
	--v[0];
	--v[1];
	--v[2];
	return *this;
}
//postfix operations
template <typename numType> inline
vec3_t<numType> vec3_t<numType>::operator++ (int) {
	return vec3_t<numType>(
		++v[0],
		++v[1],
		++v[2]
	);
}
template <typename numType> inline
vec3_t<numType> vec3_t<numType>::operator-- (int) {
	return vec3_t<numType>(
		--v[0],
		--v[1],
		--v[2]
	);
}

//comparisons
template <typename numType> inline
bool vec3_t<numType>::operator== (const vec3_t<numType>& compare) const {
	return (	v[0] == compare.v[0] &&
				v[1] == compare.v[1] &&
				v[2] == compare.v[2]
			);
}

template <typename numType> inline
bool vec3_t<numType>::operator!= (const vec3_t<numType>& compare) const {
	return (	v[0] != compare.v[0] &&
				v[1] != compare.v[1] &&
				v[2] != compare.v[2]
			);
}

template <typename numType> inline
bool vec3_t<numType>::operator< (const vec3_t<numType>& compare) const {
	return (	v[0] < compare.v[0] &&
				v[1] < compare.v[1] &&
				v[2] < compare.v[2]
			);
}

template <typename numType> inline
bool vec3_t<numType>::operator> (const vec3_t<numType>& compare) const {
	return (	v[0] > compare.v[0] &&
				v[1] > compare.v[1] &&
				v[2] > compare.v[2]
			);
}

template <typename numType> inline
bool vec3_t<numType>::operator<= (const vec3_t<numType>& compare) const {
	return (	v[0] <= compare.v[0] &&
				v[1] <= compare.v[1] &&
				v[2] <= compare.v[2]
			);
}

template <typename numType> inline
bool vec3_t<numType>::operator>= (const vec3_t<numType>& compare) const {
	return (	v[0] >= compare.v[0] &&
				v[1] >= compare.v[1] &&
				v[2] >= compare.v[2]
			);
}

//---------------------------------------------------------------------
//	Vector-Matrix Math Operations
//---------------------------------------------------------------------
template <typename numType> inline
mat3_t<numType> vec3_t<numType>::operator + (const mat3_t<numType>& m) const {
	return mat3_t<numType>(
		v[0] + m.m[0][0], v[0] + m.m[0][1], v[0] + m.m[0][2],
		v[1] + m.m[1][0], v[1] + m.m[1][1], v[1] + m.m[1][2],
		v[2] + m.m[2][0], v[2] + m.m[2][1], v[2] + m.m[2][2]
	);
}

template <typename numType> inline
mat3_t<numType> vec3_t<numType>::operator - (const mat3_t<numType>& m) const {
	return mat3_t<numType>(
		v[0] - m.m[0][0], v[0] - m.m[0][1], v[0] - m.m[0][2],
		v[1] - m.m[1][0], v[1] - m.m[1][1], v[1] - m.m[1][2],
		v[2] - m.m[2][0], v[2] - m.m[2][1], v[2] - m.m[2][2]
	);
}

template <typename numType> inline
vec3_t<numType> vec3_t<numType>::operator * (const mat3_t<numType>& m) const {
	return vec3_t<numType>(
		(m.m[0][0] * v[0]) + (m.m[0][1] * v[1]) + (m.m[0][2] * v[2]),
		(m.m[1][0] * v[0]) + (m.m[1][1] * v[1]) + (m.m[1][2] * v[2]),
		(m.m[2][0] * v[0]) + (m.m[2][1] * v[1]) + (m.m[2][2] * v[2])
	);
}

template <typename numType> inline
vec3_t<numType>& vec3_t<numType>::operator *= (const mat3_t<numType>& m) {
	*this = *this * m;
	return *this;
}

//---------------------------------------------------------------------
//	Vector-Scalar Math Operations
//---------------------------------------------------------------------
template <typename numType> inline
vec3_t<numType> vec3_t<numType>::operator = (numType input) {
	v[0] = input;
	v[1] = input;
	v[2] = input;
	return *this;
}
template <typename numType> inline
vec3_t<numType> vec3_t<numType>::operator + (numType input) const {
	return vec3_t<numType>(
		v[0] + input,
		v[1] + input,
		v[2] + input
	);
}
template <typename numType> inline
vec3_t<numType> vec3_t<numType>::operator - (numType input) const {
	return vec3_t<numType>(
		v[0] - input,
		v[1] - input,
		v[2] - input
	);
}
template <typename numType> inline
vec3_t<numType> vec3_t<numType>::operator * (numType input) const {
	return vec3_t<numType>(
		v[0] * input,
		v[1] * input,
		v[2] * input
	);
}
template <typename numType> inline
vec3_t<numType> vec3_t<numType>::operator / (numType input) const {
	return vec3_t<numType>(
		v[0] / input,
		v[1] / input,
		v[2] / input
	);
}
template <typename numType> inline
vec3_t<numType>& vec3_t<numType>::operator += (numType input) {
	v[0] += input;
	v[1] += input;
	v[2] += input;
	return *this;
}
template <typename numType> inline
vec3_t<numType>& vec3_t<numType>::operator -= (numType input) {
	v[0] -= input;
	v[1] -= input;
	v[2] -= input;
	return *this;
}
template <typename numType> inline
vec3_t<numType>& vec3_t<numType>::operator *= (numType input) {
	v[0] *= input;
	v[1] *= input;
	v[2] *= input;
	return *this;
}
template <typename numType> inline
vec3_t<numType>& vec3_t<numType>::operator /= (numType input) {
	v[0] /= input;
	v[1] /= input;
	v[2] /= input;
	return *this;
}

} //end math namespace
} //end hamlibs namespace

#endif /* __HL_MATH_VEC3_H__*/
