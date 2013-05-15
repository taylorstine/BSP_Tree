/**
 * @file vector.hpp
 * @brief Vector classes.
 *
 * @author Eric Butler (edbutler)
 */

#ifndef _462_MATH_VECTOR_HPP_
#define _462_MATH_VECTOR_HPP_

#include "math/math.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>


/*
This file defines 3 different vector classes: 2D, 3D, and 4D.
Each class overrides most sensible operators and have functions
for other operators such as dot product, length, and normalization.

All named functions do not mutate their arguments but rather return
a new vector. So, for example, you must do
    v = normalize( v );
in order to make v a unit vector. Some operators (e.g., +=) mutate
their first argument, as with built-in numerical types.

Most operations are inline functions because it results in a noticeable
speed increase in most applications.
*/

/**
 * A 2d vector.
 */
class Vector2
{
public:

    /**
     * The number of dimensions.
     */
    static const size_t DIM = 2;

    /**
     * The zero vector.
     */
    static const Vector2 Zero() {
        return Vector2(0.0, 0.0);
    }

    /**
     * The vector (1,1).
     */
    static const Vector2 Ones() {
        return Vector2(1.0, 1.0);
    }

    /**
     * The vector (1,0).
     */
    static const Vector2 UnitX() {
        return Vector2(1.0, 0.0);
    }

    /**
     * The vector (0,1).
     */
    static const Vector2 UnitY() {
        return Vector2(0.0, 1.0);
    }

    /**
     * Components of this vector.
     */
    float x, y;

    /**
     * Default constructor. Leaves values unitialized.
     */
    Vector2() {}

    /**
     * Create a vector with the given values.
     */
    Vector2( float x, float y )
        : x( x ), y( y ) {}

    // also uses default copy and assignment

    Vector2 operator+( const Vector2& rhs ) const {
        return Vector2( x + rhs.x, y + rhs.y );
    }

    Vector2& operator+=( const Vector2& rhs ) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2 operator-( const Vector2& rhs ) const {
        return Vector2( x - rhs.x, y - rhs.y );
    }

    Vector2& operator-=( const Vector2& rhs ) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vector2 operator*( float s ) const {
        return Vector2( x * s, y * s );
    }

    Vector2& operator*=( float s ) {
        x *= s;
        y *= s;
        return *this;
    }

    Vector2 operator/( float s ) const {
        float inv = 1.0 / s;
        return Vector2( x * inv, y * inv );
    }

    Vector2& operator/=( float s ) {
        float inv = 1.0 / s;
        x *= inv;
        y *= inv;
        return *this;
    }

    Vector2 operator-() const {
        return Vector2( -x, -y );
    }

    /**
     * @remark No bounds checking.
     */
    const float& operator[]( size_t i ) const {
        // assumes all members are in a contiguous block
        assert( i < DIM );
        return ( &x )[i];
    }

    /**
     * @remark No bounds checking.
     */
    float& operator[]( size_t i ) {
        // assumes all members are in a contiguous block
        assert( i < DIM );
        return ( &x )[i];
    }

    bool operator==( const Vector2& rhs ) const {
        return x == rhs.x && y == rhs.y;
    }

    bool operator!=( const Vector2& rhs ) const {
        return !operator==( rhs );
    }

    void to_array( float arr[DIM] ) const {
        arr[0] = float( x );
        arr[1] = float( y );
    }
};

/**
 * Returns the dot product of two vectors
 */
inline float dot( const Vector2& lhs, const Vector2& rhs ) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

/**
 * Efficiency function: does not require square root operation.
 */
inline float squared_length( const Vector2& v ) {
    return v.x * v.x + v.y * v.y;
}

/**
 * Returns the length of a vector.
 */
inline float length( const Vector2& v ) {
    return sqrt( squared_length( v ) );
}

/**
 * Calculate the positive distance between two vectors.
 */
inline float distance( const Vector2& lhs, const Vector2& rhs ) {
    return length( lhs - rhs );
}

/**
 * Efficiency function: does not require square root operation.
 */
inline float squared_distance( const Vector2& lhs, const Vector2& rhs ) {
    return squared_length( lhs - rhs );
}

/**
 * Returns the unit vector pointing in the same direction as this vector.
 */
inline Vector2 normalize( const Vector2& v ) {
    return v / length( v );
}

/**
 * Returns a vector whose elements are the absolute values of all the
 * elements of this vector.
 */
inline Vector2 vabs( const Vector2& v ) {
    return Vector2( fabs( v.x ), fabs( v.y ) );
}

/**
 * Returns the element-wise maximum of the two vectors.
 */
inline Vector2 vmax( const Vector2& lhs, const Vector2& rhs ) {
    return Vector2(
        std::max( lhs.x, rhs.x ),
        std::max( lhs.y, rhs.y )
    );
}

/**
 * Returns the element-wise minimum of the two vectors.
 */
inline Vector2 vmin( const Vector2& lhs, const Vector2& rhs ) {
    return Vector2(
        std::min( lhs.x, rhs.x ),
        std::min( lhs.y, rhs.y )
    );
}

inline Vector2 operator*( float s, const Vector2& rhs ) {
    return rhs * s;
}

/**
 * Outputs a vector text formatted as "(x,y,z)".
 */
std::ostream& operator<<( std::ostream& os, const Vector2& rhs );

/**
 * A 3d vector.
 */
class Vector3
{
public:

    /**
     * The number of dimensions.
     */
    static const size_t DIM = 3;

    /**
     * The zero vector.
     */
    static const Vector3 Zero() {
        return Vector3(0.0, 0.0, 0.0);
    }

    /**
     * The vector (1,1,1).
     */
    static const Vector3 Ones() {
        return Vector3(1.0, 1.0, 1.0);
    }

    /**
     * The vector (1,0,0)
     */
    static const Vector3 UnitX() {
        return Vector3(1.0, 0.0, 0.0);
    }

    /**
     * The vector (0,1,0)
     */
    static const Vector3 UnitY() {
        return Vector3(0.0, 1.0, 0.0);
    }

    /**
     * The vector (0,0,1)
     */
    static const Vector3 UnitZ() {
        return Vector3(0.0, 0.0, 1.0);
    }

    /**
     * Components of this vector.
     */
    float x, y, z;

    /**
     * Default constructor. Leaves values unitialized.
     */
    Vector3() {}

    /**
     * Create a vector with the given values.
     */
    Vector3( float x, float y, float z )
        : x( x ), y( y ), z( z ) {}

    /**
     * Create a vector from a 2d vector.
     */
    Vector3( const Vector2& v, float z )
        : x( v.x ), y( v.y ), z( z ) { }

    /**
     * Create a vector from a float array.
     */
    explicit Vector3( const float arr[3] )
        : x( arr[0] ), y( arr[1] ), z( arr[2] ) { }

    // also uses default copy and assignment

    Vector3 operator+( const Vector3& rhs ) const {
        return Vector3( x + rhs.x, y + rhs.y, z + rhs.z );
    }

    Vector3& operator+=( const Vector3& rhs ) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vector3 operator-( const Vector3& rhs ) const {
        return Vector3( x - rhs.x, y - rhs.y, z - rhs.z );
    }

    Vector3& operator-=( const Vector3& rhs ) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vector3 operator*( float s ) const {
        return Vector3( x * s, y * s, z * s );
    }

    Vector3& operator*=( float s ) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }


    Vector3 operator/( float s ) const {
        float inv = 1.0 / s;
        return Vector3( x * inv, y * inv, z * inv );
    }

    Vector3& operator/=( float s ) {
        float inv = 1.0 / s;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    Vector3 operator-() const {
        return Vector3( -x, -y, -z );
    }

    /**
     * @remark No bounds checking.
     */
    const float& operator[]( size_t i ) const {
        // assumes all members are in a contiguous block
        assert( i < DIM );
        return ( &x )[i];
    }

    /**
     * @remark No bounds checking.
     */
    float& operator[]( size_t i ) {
        // assumes all members are in a contiguous block
        assert( i < DIM );
        return ( &x )[i];
    }

    bool operator==( const Vector3& rhs ) const {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    bool operator!=( const Vector3& rhs ) const {
        return !operator==( rhs );
    }

    void to_array( float arr[DIM] ) const {
        arr[0] = float( x );
        arr[1] = float( y );
        arr[2] = float( z );
    }
};

/**
 * Returns the dot product of two vectors
 */
inline float dot( const Vector3& lhs, const Vector3& rhs ) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

/**
 * Returns the cross product of two vectors
 */
inline Vector3 cross( const Vector3& lhs, const Vector3& rhs ) {
    return Vector3(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}

/**
 * Efficiency function: does not require square root operation.
 */
inline float squared_length( const Vector3& v ) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

/**
 * Returns the length of a vector.
 */
inline float length( const Vector3& v ) {
    return sqrt( squared_length( v ) );
}

/**
 * Calculate the positive distance between two vectors.
 */
inline float distance( const Vector3& lhs, const Vector3& rhs ) {
    return length( lhs - rhs );
}

/**
 * Efficiency function: does not require square root operation.
 */
inline float squared_distance( const Vector3& lhs, const Vector3& rhs ) {
    return squared_length( lhs - rhs );
}

/**
 * Returns the unit vector pointing in the same direction as this vector.
 */
inline Vector3 normalize( const Vector3& v ) {
    return v / length( v );
}

/**
 * Returns a vector whose elements are the absolute values of all the
 * elements of this vector.
 */
inline Vector3 vabs( const Vector3& v ) {
    return Vector3( fabs( v.x ), fabs( v.y ), fabs( v.z ) );
}

/**
 * Returns the element-wise maximum of the two vectors.
 */
inline Vector3 vmax( const Vector3& lhs, const Vector3& rhs ) {
    return Vector3(
        std::max( lhs.x, rhs.x ),
        std::max( lhs.y, rhs.y ),
        std::max( lhs.z, rhs.z )
    );
}

/**
 * Returns the element-wise minimum of the two vectors.
 */
inline Vector3 vmin( const Vector3& lhs, const Vector3& rhs ) {
    return Vector3(
        std::min( lhs.x, rhs.x ),
        std::min( lhs.y, rhs.y ),
        std::min( lhs.z, rhs.z )
    );
}

inline Vector3 operator*( float s, const Vector3& rhs ) {
    return rhs * s;
}

/**
 * Outputs a vector text formatted as "(x,y,z)".
 */
std::ostream& operator<<( std::ostream& os, const Vector3& rhs );

/**
 * A 4d vector.
 */
class Vector4
{
public:

    /**
     * The number of dimensions.
     */
    static const size_t DIM = 4;

    /**
     * The zero vector.
     */
    static const Vector4 Zero() {
        return Vector4(0.0, 0.0, 0.0, 0.0);
    }

    /**
     * The vector (1,1,1,1).
     */
    static const Vector4 Ones() {
        return Vector4(1.0, 1.0, 1.0, 1.0);
    }

    /**
     * The vector (1,0,0,0)
     */
    static const Vector4 UnitX() {
        return Vector4(1.0, 0.0, 0.0, 0.0);
    }

    /**
     * The vector (0,1,0,0)
     */
    static const Vector4 UnitY() {
        return Vector4(0.0, 1.0, 0.0, 0.0);
    }

    /**
     * The vector (0,0,1,0)
     */
    static const Vector4 UnitZ() {
        return Vector4(0.0, 0.0, 1.0, 0.0);
    }

    /**
     * The vector (0,0,0,1)
     */
    static const Vector4 UnitW() {
        return Vector4(0.0, 0.0, 0.0, 1.0);
    }

    /**
     * Components of this vector.
     */
    float x, y, z, w;

    /**
     * Default constructor. Leaves values unitialized.
     */
    Vector4() {}

    /**
     * Create a vector with the given values.
     */
    Vector4( float x, float y, float z, float w )
        : x( x ), y( y ), z( z ), w( w ) {}

    /**
     * Create the vector (v.x, v.y, v.z, w).
     */
    Vector4( const Vector3& v, float w )
        : x( v.x ), y( v.y ), z( v.z ), w( w ) {}

    // also uses default copy and assignment

    Vector4 operator+( const Vector4& rhs ) const {
        return Vector4( x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w );
    }

    Vector4& operator+=( const Vector4& rhs ) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Vector4 operator-( const Vector4& rhs ) const {
        return Vector4( x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w );
    }

    Vector4& operator-=( const Vector4& rhs ) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    Vector4 operator*( float s ) const {
        return Vector4( x * s, y * s, z * s, w * s );
    }

    Vector4& operator*=( float s ) {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }

    Vector4 operator/( float s ) const {
        float inv = 1.0 / s;
        return Vector4( x * inv, y * inv, z * inv, w * inv );
    }

    Vector4& operator/=( float s ) {
        float inv = 1.0 / s;
        x *= inv;
        y *= inv;
        z *= inv;
        w *= inv;
        return *this;
    }

    Vector4 operator-() const {
        return Vector4( -x, -y, -z, -w );
    }

    /**
     * @remark No bounds checking.
     */
    const float& operator[]( size_t i ) const {
        // assumes all members are in a contiguous block
        assert( i < DIM );
        return ( &x )[i];
    }

    /**
     * @remark No bounds checking.
     */
    float& operator[]( size_t i ) {
        // assumes all members are in a contiguous block
        assert( i < DIM );
        return ( &x )[i];
    }

    bool operator==( const Vector4& rhs ) const {
        return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
    }

    bool operator!=( const Vector4& rhs ) const {
        return !operator==( rhs );
    }

    /**
     * Returns the first three components, ignoring the fourth
     */
    Vector3 xyz() const {
        return Vector3( x, y, z );
    }

    void to_array( float arr[DIM] ) const {
        arr[0] = float( x );
        arr[1] = float( y );
        arr[2] = float( z );
        arr[3] = float( w );
    }
};

/**
 * Returns the 3d vector corresponding to this 4d vector.
 * @remark If w==0, returns (x,y,z).
 */
inline Vector3 project( const Vector4& v ) {
    float winv = v.w == 0.0 ? 1.0 : 1.0 / v.w;
    return Vector3( v.x * winv, v.y * winv, v.z * winv );
}

/**
 * Returns the dot product of two vectors
 */
inline float dot( const Vector4& lhs, const Vector4& rhs ) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

/**
 * Efficiency function: does not require square root operation.
 */
inline float squared_length( const Vector4& v ) {
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

/**
 * Returns the length of a vector.
 */
inline float length( const Vector4& v ) {
    return sqrt( squared_length( v ) );
}

/**
 * Calculate the positive distance between two vectors.
 */
inline float distance( const Vector4& lhs, const Vector4& rhs ) {
    return length( lhs - rhs );
}

/**
 * Efficiency function: does not require square root operation.
 */
inline float squared_distance( const Vector4& lhs, const Vector4& rhs ) {
    return squared_length( lhs - rhs );
}

/**
 * Returns the unit vector pointing in the same direction as this vector.
 */
inline Vector4 normalize( const Vector4& v ) {
    return v / length( v );
}

/**
 * Returns a vector whose elements are the absolute values of all the
 * elements of this vector.
 */
inline Vector4 vabs( const Vector4& v ) {
    return Vector4( fabs( v.x ), fabs( v.y ), fabs( v.z ), fabs( v.w ) );
}

/**
 * Returns the element-wise maximum of the two vectors.
 */
inline Vector4 vmax( const Vector4& lhs, const Vector4& rhs ) {
    return Vector4(
        std::max( lhs.x, rhs.x ),
        std::max( lhs.y, rhs.y ),
        std::max( lhs.z, rhs.z ),
        std::max( lhs.w, rhs.w )
    );
}

/**
 * Returns the element-wise minimum of the two vectors.
 */
inline Vector4 vmin( const Vector4& lhs, const Vector4& rhs ) {
    return Vector4(
        std::min( lhs.x, rhs.x ),
        std::min( lhs.y, rhs.y ),
        std::min( lhs.z, rhs.z ),
        std::min( lhs.w, rhs.w )
    );
}

inline Vector4 operator*( float s, const Vector4& rhs ) {
    return rhs * s;
}

/**
 * Outputs a vector text formatted as "(x,y,z)".
 */
std::ostream& operator<<( std::ostream& os, const Vector4& rhs );



#endif /* _462_MATH_VECTOR_HPP_ */

