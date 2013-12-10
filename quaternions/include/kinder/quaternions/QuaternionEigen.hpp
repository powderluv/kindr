/*
 * Copyright (c) 2013, Christian Gehring, Hannes Sommer, Paul Furgale, Remo Diethelm
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Autonomous Systems Lab, ETH Zurich nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Christian Gehring, Hannes Sommer, Paul Furgale,
 * Remo Diethelm BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/
#ifndef KINDER_QUATERNIONS_QUATERNIONEIGEN_HPP_
#define KINDER_QUATERNIONS_QUATERNIONEIGEN_HPP_

#include <Eigen/Geometry>

#include "kinder/common/common.hpp"
#include "kinder/common/assert_macros_eigen.hpp"
#include "kinder/quaternions/QuaternionBase.hpp"

namespace kinder {
namespace quaternions {
//! Implementation based on the C++ Eigen library
namespace eigen_implementation {


//! Implementation of a Quaternion based on Eigen::Quaternion
/*!
 * The Hamiltonian convention is used, where
 * Q = w + x*i + y*j + z*k and i*i=j*j=k*k=ijk=-1.
 *
 * The following two typedefs are provided for convenience:
 *   - QuaternionF for float
 *   - QuaternionD for double
 * \ingroup quaternions
 * \see rm::quaternions::eigen_implementation::UnitQuaternion for an implementation of a unit quaternion
 * \see rm::rotations::eigen_implementation::RotationQuaternion for quaternions that represent a rotation
 */
template<typename PrimType_>
class Quaternion : public QuaternionBase<Quaternion<PrimType_>>, private Eigen::Quaternion<PrimType_> {
 private:
  typedef Eigen::Quaternion<PrimType_> Base;
 public:
  //! the implementation type, i.e., Eigen::Quaternion<>
  typedef Base Implementation;
  //! the scalar type, i.e., the type of the coefficients
  typedef PrimType_ Scalar;
  //! the imaginary type, i.e., Eigen::Quaternion<>
  typedef Eigen::Matrix<PrimType_,3,1> Imaginary;
  //! quaternion as 4x1 matrix: [w; x; y; z]
  typedef Eigen::Matrix<PrimType_,4,1> Vector4;

  //! Default constructor creates a quaternion with all coefficients equal to zero
  Quaternion()
    : Base(Implementation(0,0,0,0)) {
  }

  Quaternion(const PrimType_& w, const PrimType_& x, const PrimType_& y, const PrimType_& z)
    : Base(w,x,y,z) {
  }

  Quaternion(const PrimType_& w, const Imaginary& imag)
    : Base(w,imag(0),imag(1),imag(2)) {
  }

  // create from Eigen::Quaternion
  explicit Quaternion(const Base& other)
    : Base(other) {
  }

  Quaternion(const Vector4& vector4)
    : Base(vector4(0),vector4(1),vector4(2),vector4(3)) {
  }

  /*! \returns the inverse of the quaternion
    */
  Quaternion inverted() const {
    return Quaternion(Implementation::inverse());
  }

  /*! \inverts the quaternion
    */
  Quaternion& invert() {
    *this = Quaternion(Implementation::inverse());
    return *this;
  }

  /*! \returns the conjugate of the quaternion
    */
  Quaternion conjugated() const {
    return Quaternion(Implementation::conjugate());
  }

  /*! \conjugates the quaternion
    */
  Quaternion& conjugate() {
    *this = Quaternion(Implementation::conjugate());
    return *this;
  }

  Quaternion& operator =(const Quaternion<PrimType_>& other) {
    this->w() = other.w();
    this->x() = other.x();
    this->y() = other.y();
    this->z() = other.z();
    return *this;
  }

  Quaternion& operator =(const UnitQuaternion<PrimType_>& other) {
    *this = Quaternion(other.toImplementation());
    return *this;
  }

//  bool operator ==(const Quaternion<PrimType_>& other) {
//	  return this->isEqual(other);
//  }

  template<typename PrimType_In>
  Quaternion& operator ()(const Quaternion<PrimType_In>& other) {
//	*this = other.template cast<PrimType_>();
	this->w() = static_cast<PrimType_>(other.w());
	this->x() = static_cast<PrimType_>(other.x());
	this->y() = static_cast<PrimType_>(other.y());
	this->z() = static_cast<PrimType_>(other.z());
	return *this;
  }

  template<typename PrimType_In>
  Quaternion& operator ()(const UnitQuaternion<PrimType_In>& other) {
//	*this = other.uq.template cast<PrimType_>(); // uq is private
	this->w() = static_cast<PrimType_>(other.w());
	this->x() = static_cast<PrimType_>(other.x());
	this->y() = static_cast<PrimType_>(other.y());
	this->z() = static_cast<PrimType_>(other.z());
	return *this;
  }

  inline Implementation& toImplementation() {
    return static_cast<Implementation&>(*this);
  }
  inline const Implementation& toImplementation() const {
    return static_cast<const Implementation&>(*this);
  }

  using QuaternionBase<Quaternion<PrimType_>>::operator==;
  using QuaternionBase<Quaternion<PrimType_>>::operator*;

  inline PrimType_ w() const {
    return Base::w();
  }

  inline PrimType_ x() const {
    return Base::x();
  }

  inline PrimType_ y() const {
    return Base::y();
  }

  inline PrimType_ z() const {
    return Base::z();
  }

  inline PrimType_& w() { // todo: attention: no assertion for unitquaternions!
    return Base::w();
  }

  inline PrimType_& x() {
    return Base::x();
  }

  inline PrimType_& y() {
    return Base::y();
  }

  inline PrimType_& z() {
    return Base::z();
  }

  inline PrimType_ getReal() const {
    return Base::w();
  }

  inline Imaginary getImaginary() const {
    return Imaginary(Base::x(),Base::y(),Base::z());
  }

  inline Vector4 getVector4() const {
    Vector4 vector4;
    vector4 << Base::w(), Base::x(), Base::y(), Base::z();
    return vector4;
  }



  inline PrimType_ norm() const {
    return Base::norm();
  }

  Quaternion normalized() const {
    return Quaternion(this->Base::normalized());
  }

  Quaternion& normalize() {
	  this->Base::normalize();
	  return *this;
  }

  Quaternion& setZero() {
    this->Base::setZero();
    return *this;
  }

  UnitQuaternion<PrimType_> toUnitQuaternion() const {
    return UnitQuaternion<PrimType_>(this->Base::normalized());
  }


};

//! Quaternion using double
typedef Quaternion<double> QuaternionD;
//! Quaternion using float
typedef Quaternion<float> QuaternionF;

//! Implementation of a unit quaternion based on Eigen::Quaternion
/*! The Hamiltonian convention is used, where
 * Q = w + x*i + y*j + z*k and i*i=j*j=k*k=ijk=-1.
 *
 * The following two typedefs are provided for convenience:
 *   - UnitQuaternionF for float
 *   - UnitQuaternionD for double
 * \ingroup quaternions
 * \see rm::quaternions::eigen_implementation::Quaternion for an implementation of a generic quaternion
 * \see rm::rotations::eigen_implementation::RotationQuaternion for quaternions that represent a rotation
 */
template<typename PrimType_>
class UnitQuaternion : public UnitQuaternionBase<UnitQuaternion<PrimType_>> {
 private:
  Quaternion<PrimType_> unitQuternion_;
  typedef UnitQuaternionBase<UnitQuaternion<PrimType_>> Base;
 public:
  //! the implementation type, i.e., Eigen::Quaternion<>
  typedef typename Quaternion<PrimType_>::Implementation Implementation;
  //! the scalar type, i.e., the type of the coefficients
  typedef PrimType_ Scalar;
  //! the imaginary type, i.e., Eigen::Quaternion<>
  typedef Eigen::Matrix<PrimType_,3,1> Imaginary;
  //! quaternion as 4x1 matrix: [w; x; y; z]
  typedef Eigen::Matrix<PrimType_,4,1> Vector4;

  //! Default Constructor initializes the unit quaternion to identity
  UnitQuaternion()
    : unitQuternion_(Implementation::Identity()) {
  }

  //! Constructor to create unit quaternion from coefficients
  /*! Q = w + x*i + y*j + z*k
   * \param   w   scalar
   * \param   x   vector index 1
   * \param   y   vector index 2
   * \param   z   vector index 3
   */
  UnitQuaternion(const PrimType_& w, const PrimType_& x, const PrimType_& y, const PrimType_& z)
    : unitQuternion_(w,x,y,z) {
    KINDER_ASSERT_SCALAR_NEAR_DBG(std::runtime_error, norm(), 1, 1e-4, "Input quaternion has not unit length.");
  }

  UnitQuaternion(const PrimType_& w, const Imaginary& imag)
    : unitQuternion_(w,imag) {
    KINDER_ASSERT_SCALAR_NEAR_DBG(std::runtime_error, norm(), 1, 1e-4, "Input quaternion has not unit length.");
  }

  UnitQuaternion(const Vector4& vector4)
    : unitQuternion_(vector4(0),vector4(1),vector4(2),vector4(3)) {
    KINDER_ASSERT_SCALAR_NEAR_DBG(std::runtime_error, norm(), 1, 1e-4, "Input quaternion has not unit length.");
  }

  //! Constructor to create unit quaternion from Quaternion
  explicit UnitQuaternion(const Quaternion<PrimType_>& other)
    : unitQuternion_(other.toImplementation()) {
    KINDER_ASSERT_SCALAR_NEAR_DBG(std::runtime_error, norm(), 1, 1e-4, "Input quaternion has not unit length.");
  }

  //! Constructor to create unit quaternion from Eigen::Quaternion
  /*!
   * \param other Eigen::Quaternion
   */
  explicit UnitQuaternion(const Implementation& other)
    : unitQuternion_(other) {
    KINDER_ASSERT_SCALAR_NEAR_DBG(std::runtime_error, norm(), 1, 1e-4, "Input quaternion has not unit length.");
  }

  UnitQuaternion& operator =(const UnitQuaternion<PrimType_>& other) {
	    this->w() = other.w();
	    this->x() = other.x();
	    this->y() = other.y();
	    this->z() = other.z();
	    return *this;
  }

  template<typename PrimType_In>
  UnitQuaternion& operator ()(const UnitQuaternion<PrimType_In>& other) {
//	uq = other.uq;
	this->w() = static_cast<PrimType_>(other.w());
	this->x() = static_cast<PrimType_>(other.x());
	this->y() = static_cast<PrimType_>(other.y());
	this->z() = static_cast<PrimType_>(other.z());
	return *this;
  }

  template<typename PrimType_In>
  UnitQuaternion& operator ()(const Quaternion<PrimType_In>& other) {
//		*this = (UnitQuaternion)quat;
//	uq = other.template cast<PrimType_>();
	this->w() = static_cast<PrimType_>(other.w());
	this->x() = static_cast<PrimType_>(other.x());
	this->y() = static_cast<PrimType_>(other.y());
	this->z() = static_cast<PrimType_>(other.z());
    KINDER_ASSERT_SCALAR_NEAR_DBG(std::runtime_error, norm(), 1, 1e-4, "Input quaternion has not unit length.");
	return *this;
  }

//  UnitQuaternion<PrimType_> operator *(const UnitQuaternion<PrimType_>& other) {
//	  return UnitQuaternion<PrimType_>(this->uq * other.uq);
//  }
//
//  Quaternion<PrimType_> operator *(const Quaternion<PrimType_>& other) {
//	  return Quaternion<PrimType_>(this->uq * other);
//  }
//  bool operator ==(const UnitQuaternion<PrimType_>& other) {
//	  return this->uq == other.uq;
//  }
//
//  bool operator ==(const Quaternion<PrimType_>& other) {
//	  return this->uq == other.uq;
//  }

  inline PrimType_ w() const {
    return unitQuternion_.w();
  }

  inline PrimType_ x() const {
    return unitQuternion_.x();
  }

  inline PrimType_ y() const {
    return unitQuternion_.y();
  }

  inline PrimType_ z() const {
    return unitQuternion_.z();
  }

  inline PrimType_& w() { // todo: attention: no assertion for unitquaternions!
    return unitQuternion_.w();
  }

  inline PrimType_& x() {
    return unitQuternion_.x();
  }

  inline PrimType_& y() {
    return unitQuternion_.y();
  }

  inline PrimType_& z() {
    return unitQuternion_.z();
  }

  inline PrimType_ getReal() const {
    return unitQuternion_.w();
  }

  inline Imaginary getImaginary() const {
    return Imaginary(unitQuternion_.x(),unitQuternion_.y(),unitQuternion_.z());
  }

  inline Vector4 getVector4() const {
    Vector4 vector4;
    vector4 << Base::w(), Base::x(), Base::y(), Base::z();
    return vector4;
  }

//  using Base::operator*;

//  using UnitQuaternionBase<UnitQuaternion<PrimType_>>::conjugate;

  /*! \returns the conjugate of the quaternion
    */
  UnitQuaternion conjugated() const {
    return UnitQuaternion(unitQuternion_.conjugated());
  }

  /*! \conjugates the quaternion
    */
  UnitQuaternion& conjugate() {
    unitQuternion_.conjugate();
    return *this;
  }

//  using Base::inverted;
//  using Base::invert;

//  /*! \returns the inverse of the quaternion which is the conjugate for unit quaternions
//    */
//  UnitQuaternion inverse() const {
//    return UnitQuaternion(Base::conjugate());
//  }

  PrimType_ norm() const {
    return unitQuternion_.norm();
  }

  const Implementation& toImplementation() const {
    return unitQuternion_.toImplementation();
  }

  Implementation& toImplementation() {
    return unitQuternion_.toImplementation();
  }


};

//! Unit quaternion using double
typedef UnitQuaternion<double> UnitQuaternionD;
//! Unit quaternion using float
typedef UnitQuaternion<float> UnitQuaternionF;



//template<typename PrimType_>
//Quaternion<PrimType_> operator *(const Quaternion<PrimType_>& a, const Quaternion<PrimType_>& b) {
//  return internal::MultiplicationTraits<Quaternion<PrimType_>, Quaternion<PrimType_>>::mult(a, b);
//}
//
//template<typename PrimType_>
//bool operator ==(const Quaternion<PrimType_>& a, const Quaternion<PrimType_>& b) {
//  return internal::ComparisonTraits<Quaternion<PrimType_>>::isequal((Quaternion<PrimType_>)a, (Quaternion<PrimType_>)b);
//}


} // namespace eigen_implementation

namespace internal {


//template<typename PrimType_>
//class MultiplicationTraits<eigen_implementation::Quaternion<PrimType_>, eigen_implementation::Quaternion<PrimType_>> {
//public:
//  inline static eigen_implementation::Quaternion<PrimType_> mult(const eigen_implementation::Quaternion<PrimType_>& a, const eigen_implementation::Quaternion<PrimType_>& b) {
//    return eigen_implementation::Quaternion<PrimType_>(eigen_implementation::Quaternion<PrimType_>(eigen_implementation::Quaternion<PrimType_>(a).toImplementation()*eigen_implementation::Quaternion<PrimType_>(b).toImplementation()));
//  }
//};
//
//template<typename PrimType_>
//class MultiplicationTraits<eigen_implementation::Quaternion<PrimType_>, eigen_implementation::UnitQuaternion<PrimType_>> {
//public:
//  inline static eigen_implementation::Quaternion<PrimType_> mult(const eigen_implementation::Quaternion<PrimType_>& a, const eigen_implementation::UnitQuaternion<PrimType_>& b) {
//    return eigen_implementation::Quaternion<PrimType_>(eigen_implementation::Quaternion<PrimType_>(eigen_implementation::Quaternion<PrimType_>(a).toImplementation()*eigen_implementation::Quaternion<PrimType_>(b).toImplementation()));
//  }
//};
//
//template<typename PrimType_>
//class MultiplicationTraits<eigen_implementation::UnitQuaternion<PrimType_>, eigen_implementation::Quaternion<PrimType_>> {
//public:
//  inline static eigen_implementation::Quaternion<PrimType_> mult(const eigen_implementation::UnitQuaternion<PrimType_>& a, const eigen_implementation::Quaternion<PrimType_>& b) {
//    return eigen_implementation::Quaternion<PrimType_>(eigen_implementation::Quaternion<PrimType_>(eigen_implementation::Quaternion<PrimType_>(a).toImplementation()*eigen_implementation::Quaternion<PrimType_>(b).toImplementation()));
//  }
//};
//
//template<typename PrimType_>
//class MultiplicationTraits<eigen_implementation::UnitQuaternion<PrimType_>, eigen_implementation::UnitQuaternion<PrimType_>> {
//public:
//  inline static eigen_implementation::UnitQuaternion<PrimType_> mult(const eigen_implementation::UnitQuaternion<PrimType_>& a, const eigen_implementation::UnitQuaternion<PrimType_>& b) {
//    return eigen_implementation::Quaternion<PrimType_>(eigen_implementation::Quaternion<PrimType_>(eigen_implementation::Quaternion<PrimType_>(a).toImplementation()*eigen_implementation::Quaternion<PrimType_>(b).toImplementation()));
//  }
//};


//template<typename PrimType_>
//class ComparisonTraits<eigen_implementation::Quaternion<PrimType_>> {
// public:
//   inline static bool isEqual(const eigen_implementation::Quaternion<PrimType_>& a, const eigen_implementation::Quaternion<PrimType_>& b){
//     return (a.w() ==  b.w() &&
//             a.x() ==  b.x() &&
//             a.y() ==  b.y() &&
//             a.z() ==  b.z());
//   }
//};


} // namespace internal
} // namespace quaternions
} // namespace rm


#endif /* KINDER_QUATERNIONS_QUATERNIONEIGEN_HPP_ */
