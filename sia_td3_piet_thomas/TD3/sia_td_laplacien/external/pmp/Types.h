// Copyright 2011-2020 the Polygon Mesh Processing Library developers.
// Distributed under a MIT-style license, see LICENSE.txt for details.

#pragma once

#include <cstdint>

#include <Eigen/Core>
#include <Eigen/Geometry>

//=============================================================================
namespace Eigen {
  template<typename A>
  inline typename internal::traits<A>::Scalar
  norm(const MatrixBase<A>& a) {
    return a.norm();
  }

  template<typename A,typename B>
  inline typename internal::traits<A>::Scalar
  dot(const MatrixBase<A>& a,const MatrixBase<B>& b) {
    return a.dot(b);
  }

  template<typename A,typename B>
  inline typename MatrixBase<A>::template cross_product_return_type<B>::type
  cross(const MatrixBase<A>& a,const MatrixBase<B>& b) {
    return a.cross(b);
  }

  template<typename A>
  inline const typename MatrixBase<A>::PlainObject
  normalize(const MatrixBase<A>& a) {
    return a.normalized();
  }

  template<typename A,typename B>
  inline typename internal::traits<A>::Scalar
  distance(const MatrixBase<A>& a,const MatrixBase<B>& b) {
    return (a-b).norm();
  }
}

//! \def PMP_ASSERT(x)
//! Custom assert macro that allows to silence unused variable warnings with no
//! overhead. Generates no code in release mode since if the argument to
//! sizeof() is an expression it is not evaluated. In debug mode we just fall
//! back to the default assert().
#ifdef NDEBUG
#define PMP_ASSERT(x)    \
    do                   \
    {                    \
        (void)sizeof(x); \
    } while (0)
#else
#define PMP_ASSERT(x) assert(x)
#endif

//! \def PMP_SHOW(x)
//! A simple pretty-printing debug macro. Prints the expression and its value.
#define PMP_SHOW(x) std::cerr << #x " = '" << x << "'" << std::endl;

//! The pmp-library namespace
namespace pmp {

//! \addtogroup core
//! @{

//! Scalar type
#ifdef PMP_SCALAR_TYPE_64
typedef double Scalar;
#else
typedef float Scalar;
#endif

typedef Eigen::Matrix<Scalar, 3, 1> vec3;
typedef Eigen::Matrix<Scalar, 2, 1> vec2;

//! Point type
typedef vec3 Point;

//! Normal type
typedef vec3 Normal;

//! Color type
typedef vec3 Color;

//! Texture coordinate type
typedef vec2 TexCoord;

// define index type to be used
#ifdef PMP_INDEX_TYPE_64
typedef std::uint_least64_t IndexType;
#define PMP_MAX_INDEX UINT_LEAST64_MAX
#else
typedef std::uint_least32_t IndexType;
#define PMP_MAX_INDEX UINT_LEAST32_MAX
#endif

//! Common IO flags for reading and writing
struct IOFlags
{
    IOFlags() {}
    bool use_binary = false;             //!< read / write binary format
    bool use_vertex_normals = false;     //!< read / write vertex normals
    bool use_vertex_colors = false;      //!< read / write vertex colors
    bool use_vertex_texcoords = false;   //!< read / write vertex texcoords
    bool use_face_normals = false;       //!< read / write face normals
    bool use_face_colors = false;        //!< read / write face colors
    bool use_halfedge_texcoords = false; //!< read / write halfedge texcoords
};

//! @}

//! \defgroup core core
//! \brief Core data structure and utilities.

//! \defgroup algorithms algorithms
//! \brief Geometry processing algorithms.

//! \defgroup visualization visualization
//! \brief Visualization tools using OpenGL.

} // namespace pmp
