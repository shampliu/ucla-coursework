//////////////////////////////////////////////////////////////////////////////
//
//  --- vecm.h ---
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ANGEL_VEC_H__
#define __ANGEL_VEC_H__

#include <iostream>
#include <cmath>

//  Define M_PI in the case it's not defined in the math header file
#ifndef M_PI
#  define M_PI  3.14159265358979323846
#endif

//  Defined constant for when numbers are too small to be used in the
//    denominator of a division operation.  This is only used if the
//    DEBUG macro is defined.
const float  DivideByZeroTolerance = float(1.0e-07);

//  Degrees-to-radians constant 
const float  DegreesToRadians = float(M_PI / 180.0);

//////////////////////////////////////////////////////////////////////////////
//
//  vec2.h - 2D vector
//

struct vec2 {

    float  x;
    float  y;

    //
    //  --- Constructors and Destructors ---
    //

    vec2( float s = float(0.0) ) :
    x(s), y(s) {}

    vec2( float x, float y ) :
    x(x), y(y) {}

    vec2( const vec2& v )
    { x = v.x;  y = v.y;  }

    //
    //  --- Indexing Operator ---
    //

    float& operator [] ( int i ) { return *(&x + i); }
    const float operator [] ( int i ) const { return *(&x + i); }

    //
    //  --- (non-modifying) Arithematic Operators ---
    //

    vec2 operator - () const // unary minus operator
    { return vec2( -x, -y ); }

    vec2 operator + ( const vec2& v ) const
    { return vec2( x + v.x, y + v.y ); }

    vec2 operator - ( const vec2& v ) const
    { return vec2( x - v.x, y - v.y ); }

    vec2 operator * ( const float s ) const
    { return vec2( s*x, s*y ); }

    vec2 operator * ( const vec2& v ) const
    { return vec2( x*v.x, y*v.y ); }

    friend vec2 operator * ( const float s, const vec2& v )
    { return v * s; }

    vec2 operator / ( const float s ) const {
#ifdef DEBUG
        if ( std::fabs(s) < DivideByZeroTolerance ) {
            std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                << "Division by zero" << std::endl;
            return vec2();
        }
#endif // DEBUG

        float r = float(1.0) / s;
        return *this * r;
    }

    //
    //  --- (modifying) Arithematic Operators ---
    //

    vec2& operator += ( const vec2& v )
    { x += v.x;  y += v.y;   return *this; }

    vec2& operator -= ( const vec2& v )
    { x -= v.x;  y -= v.y;  return *this; }

    vec2& operator *= ( const float s )
    { x *= s;  y *= s;   return *this; }

    vec2& operator *= ( const vec2& v )
    { x *= v.x;  y *= v.y; return *this; }

    vec2& operator /= ( const float s ) {
#ifdef DEBUG
        if ( std::fabs(s) < DivideByZeroTolerance ) {
            std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                << "Division by zero" << std::endl;
        }
#endif // DEBUG

        float r = float(1.0) / s;
        *this *= r;

        return *this;
    }

    //
    //  --- Insertion and Extraction Operators ---
    //

    friend std::ostream& operator << ( std::ostream& os, const vec2& v ) {
        return os << "( " << v.x << ", " << v.y <<  " )";
    }

    friend std::istream& operator >> ( std::istream& is, vec2& v )
    { return is >> v.x >> v.y ; }

    //
    //  --- Conversion Operators ---
    //

    operator const float* () const
    { return static_cast<const float*>( &x ); }

    operator float* ()
    { return static_cast<float*>( &x ); }
};

//----------------------------------------------------------------------------
//
//  Non-class vec2 Methods
//

inline
float dot( const vec2& u, const vec2& v ) {
    return u.x * v.x + u.y * v.y;
}

inline
float length( const vec2& v ) {
    return std::sqrt( dot(v,v) );
}

inline
vec2 normalize( const vec2& v ) {
    return v / length(v);
}

//////////////////////////////////////////////////////////////////////////////
//
//  vec3.h - 3D vector
//
//////////////////////////////////////////////////////////////////////////////

struct vec3 {

    float  x;
    float  y;
    float  z;

    //
    //  --- Constructors and Destructors ---
    //

    vec3( float s = float(0.0) ) :
    x(s), y(s), z(s) {}

    vec3( float x, float y, float z ) :
    x(x), y(y), z(z) {}

    vec3( const vec3& v ) { x = v.x;  y = v.y;  z = v.z; }

    vec3( const vec2& v, const float f ) { x = v.x;  y = v.y;  z = f; }

    //
    //  --- Indexing Operator ---
    //

    float& operator [] ( int i ) { return *(&x + i); }
    const float operator [] ( int i ) const { return *(&x + i); }

    //
    //  --- (non-modifying) Arithematic Operators ---
    //

    vec3 operator - () const  // unary minus operator
    { return vec3( -x, -y, -z ); }

    vec3 operator + ( const vec3& v ) const
    { return vec3( x + v.x, y + v.y, z + v.z ); }

    vec3 operator - ( const vec3& v ) const
    { return vec3( x - v.x, y - v.y, z - v.z ); }

    vec3 operator * ( const float s ) const
    { return vec3( s*x, s*y, s*z ); }

    vec3 operator * ( const vec3& v ) const
    { return vec3( x*v.x, y*v.y, z*v.z ); }

    friend vec3 operator * ( const float s, const vec3& v )
    { return v * s; }

    vec3 operator / ( const float s ) const {
#ifdef DEBUG
        if ( std::fabs(s) < DivideByZeroTolerance ) {
            std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                << "Division by zero" << std::endl;
            return vec3();
        }
#endif // DEBUG

        float r = float(1.0) / s;
        return *this * r;
    }

    //
    //  --- (modifying) Arithematic Operators ---
    //

    vec3& operator += ( const vec3& v )
    { x += v.x;  y += v.y;  z += v.z;  return *this; }

    vec3& operator -= ( const vec3& v )
    { x -= v.x;  y -= v.y;  z -= v.z;  return *this; }

    vec3& operator *= ( const float s )
    { x *= s;  y *= s;  z *= s;  return *this; }

    vec3& operator *= ( const vec3& v )
    { x *= v.x;  y *= v.y;  z *= v.z;  return *this; }

    vec3& operator /= ( const float s ) {
#ifdef DEBUG
        if ( std::fabs(s) < DivideByZeroTolerance ) {
            std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                << "Division by zero" << std::endl;
        }
#endif // DEBUG

        float r = float(1.0) / s;
        *this *= r;

        return *this;
    }

    //
    //  --- Insertion and Extraction Operators ---
    //

    friend std::ostream& operator << ( std::ostream& os, const vec3& v ) {
        return os << "( " << v.x << ", " << v.y << ", " << v.z <<  " )";
    }

    friend std::istream& operator >> ( std::istream& is, vec3& v )
    { return is >> v.x >> v.y >> v.z ; }

    //
    //  --- Conversion Operators ---
    //

    operator const float* () const
    { return static_cast<const float*>( &x ); }

    operator float* ()
    { return static_cast<float*>( &x ); }
};

//----------------------------------------------------------------------------
//
//  Non-class vec3 Methods
//

inline
float dot( const vec3& u, const vec3& v ) {
    return u.x*v.x + u.y*v.y + u.z*v.z ;
}

inline
float length( const vec3& v ) {
    return std::sqrt( dot(v,v) );
}

inline
vec3 normalize( const vec3& v ) {
    return v / length(v);
}

inline
vec3 cross(const vec3& a, const vec3& b )
{
    return vec3( a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x );
}


//////////////////////////////////////////////////////////////////////////////
//
//  vec4 - 4D vector
//
//////////////////////////////////////////////////////////////////////////////

struct vec4 {

    float  x;
    float  y;
    float  z;
    float  w;

    //
    //  --- Constructors and Destructors ---
    //

    vec4( float s = float(0.0) ) :
    x(s), y(s), z(s), w(s) {}

    vec4( float x, float y, float z, float w ) :
    x(x), y(y), z(z), w(w) {}

    vec4( const vec4& v ) { x = v.x;  y = v.y;  z = v.z;  w = v.w; }

    vec4( const vec3& v, const float w = 1.0 ) : w(w)
    { x = v.x;  y = v.y;  z = v.z; }

    vec4( const vec2& v, const float z, const float w ) : z(z), w(w)
    { x = v.x;  y = v.y; }

    //
    //  --- Indexing Operator ---
    //

    float& operator [] ( int i ) { return *(&x + i); }
    const float operator [] ( int i ) const { return *(&x + i); }

    //
    //  --- (non-modifying) Arithematic Operators ---
    //

    vec4 operator - () const  // unary minus operator
    { return vec4( -x, -y, -z, -w ); }

    vec4 operator + ( const vec4& v ) const
    { return vec4( x + v.x, y + v.y, z + v.z, w + v.w ); }

    vec4 operator - ( const vec4& v ) const
    { return vec4( x - v.x, y - v.y, z - v.z, w - v.w ); }

    vec4 operator * ( const float s ) const
    { return vec4( s*x, s*y, s*z, s*w ); }

    vec4 operator * ( const vec4& v ) const
    { return vec4( x*v.x, y*v.y, z*v.z, w*v.z ); }

    friend vec4 operator * ( const float s, const vec4& v )
    { return v * s; }

    vec4 operator / ( const float s ) const {
#ifdef DEBUG
        if ( std::fabs(s) < DivideByZeroTolerance ) {
            std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                << "Division by zero" << std::endl;
            return vec4();
        }
#endif // DEBUG

        float r = float(1.0) / s;
        return *this * r;
    }

    //
    //  --- (modifying) Arithematic Operators ---
    //

    vec4& operator += ( const vec4& v )
    { x += v.x;  y += v.y;  z += v.z;  w += v.w;  return *this; }

    vec4& operator -= ( const vec4& v )
    { x -= v.x;  y -= v.y;  z -= v.z;  w -= v.w;  return *this; }

    vec4& operator *= ( const float s )
    { x *= s;  y *= s;  z *= s;  w *= s;  return *this; }

    vec4& operator *= ( const vec4& v )
    { x *= v.x, y *= v.y, z *= v.z, w *= v.w;  return *this; }

    vec4& operator /= ( const float s ) {
#ifdef DEBUG
        if ( std::fabs(s) < DivideByZeroTolerance ) {
            std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                << "Division by zero" << std::endl;
        }
#endif // DEBUG

        float r = float(1.0) / s;
        *this *= r;

        return *this;
    }

    //
    //  --- Insertion and Extraction Operators ---
    //

    friend std::ostream& operator << ( std::ostream& os, const vec4& v ) {
        return os << "( " << v.x << ", " << v.y
            << ", " << v.z << ", " << v.w << " )";
    }

    friend std::istream& operator >> ( std::istream& is, vec4& v )
    { return is >> v.x >> v.y >> v.z >> v.w; }

    //
    //  --- Conversion Operators ---
    //

    operator const float* () const
    { return static_cast<const float*>( &x ); }

    operator float* ()
    { return static_cast<float*>( &x ); }
};

//----------------------------------------------------------------------------
//
//  Non-class vec4 Methods
//

inline
float dot( const vec4& u, const vec4& v ) {
    return u.x*v.x + u.y*v.y + u.z*v.z + u.w*v.w;
}

inline
float length( const vec4& v ) {
    return std::sqrt( dot(v,v) );
}

inline
vec4 normalize( const vec4& v ) {
    return v / length(v);
}

inline
vec3 cross(const vec4& a, const vec4& b )
{
    return vec3( a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x );
}

//----------------------------------------------------------------------------

#endif // __ANGEL_VEC_H__
