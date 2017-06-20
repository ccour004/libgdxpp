/*******************************************************************************
 * Copyright 2011 See AUTHORS file.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/
 
#pragma once

#include "../Serializable.h"
#include "MathUtils.h"

/** A simple quaternion class.
 * @see <a href="http://en.wikipedia.org/wiki/Quaternion">http://en.wikipedia.org/wiki/Quaternion</a>
 * @author badlogicgames@gmail.com
 * @author vesuvio
 * @author xoppa */
class Matrix3;
class Matrix4;
class Vector3;
class Quaternion:public Serializable {
    public:
	 static const long serialVersionUID = -7661875440774897168L;
	 static Quaternion tmp1;
	 static Quaternion tmp2;

	 float x;
	 float y;
	 float z;
	 float w;

	/** Constructor, sets the four components of the quaternion.
	 * @param x The x-component
	 * @param y The y-component
	 * @param z The z-component
	 * @param w The w-component */
	 Quaternion (float x, float y, float z, float w) {
		this->set(x, y, z, w);
	}

	 Quaternion () {
		idt();
	}

	/** Constructor, sets the quaternion components from the given quaternion.
	 * 
	 * @param quaternion The quaternion to copy. */
	 Quaternion (const Quaternion& quaternion) {
		this->set(quaternion);
	}

	/** Constructor, sets the quaternion from the given axis vector and the angle around that axis in degrees.
	 * 
	 * @param axis The axis
	 * @param angle The angle in degrees. */
	 Quaternion (const Vector3& axis, float angle) {
		this->set(axis, angle);
	}

	/** Sets the components of the quaternion
	 * @param x The x-component
	 * @param y The y-component
	 * @param z The z-component
	 * @param w The w-component
	 * @return This quaternion for chaining */
	 Quaternion& set (float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		return *this;
	}

	/** Sets the quaternion components from the given quaternion.
	 * @param quaternion The quaternion.
	 * @return This quaternion for chaining. */
	 Quaternion& set (const Quaternion& quaternion) {
		return this->set(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	}

	/** Sets the quaternion components from the given axis and angle around that axis.
	 * 
	 * @param axis The axis
	 * @param angle The angle in degrees
	 * @return This quaternion for chaining. */
	 Quaternion& set (const Vector3& axis, float angle);

	/** @return a copy of this quaternion */
	 Quaternion cpy () {
		return Quaternion(*this);
	}

	/** @return the euclidean length of the specified quaternion */
	 const static float len (const float x, const float y, const float z, const float w) {
		return sqrt(x * x + y * y + z * z + w * w);
	}

	/** @return the euclidean length of this quaternion */
	 float len () {
		return sqrt(x * x + y * y + z * z + w * w);
	}

	
	 std::string toString () {
         std::stringstream ss;
         ss<<"["<<x<<"|"<<y <<"|"<<z <<"|"<< w <<"]";
		return ss.str();
	}

	/** Sets the quaternion to the given euler angles in degrees.
	 * @param yaw the rotation around the y axis in degrees
	 * @param pitch the rotation around the x axis in degrees
	 * @param roll the rotation around the z axis degrees
	 * @return this quaternion */
	 Quaternion& setEulerAngles (float yaw, float pitch, float roll) {
		return setEulerAnglesRad(yaw * MathUtils::degreesToRadians, pitch * MathUtils::degreesToRadians, roll
			* MathUtils::degreesToRadians);
	}

	/** Sets the quaternion to the given euler angles in radians.
	 * @param yaw the rotation around the y axis in radians
	 * @param pitch the rotation around the x axis in radians
	 * @param roll the rotation around the z axis in radians
	 * @return this quaternion */
	 Quaternion& setEulerAnglesRad (float yaw, float pitch, float roll) {
		const float hr = roll * 0.5f;
		const float shr = sin(hr);
		const float chr = cos(hr);
		const float hp = pitch * 0.5f;
		const float shp = sin(hp);
		const float chp = cos(hp);
		const float hy = yaw * 0.5f;
		const float shy = sin(hy);
		const float chy = cos(hy);
		const float chy_shp = chy * shp;
		const float shy_chp = shy * chp;
		const float chy_chp = chy * chp;
		const float shy_shp = shy * shp;

		x = (chy_shp * chr) + (shy_chp * shr); // cos(yaw/2) * sin(pitch/2) * cos(roll/2) + sin(yaw/2) * cos(pitch/2) * sin(roll/2)
		y = (shy_chp * chr) - (chy_shp * shr); // sin(yaw/2) * cos(pitch/2) * cos(roll/2) - cos(yaw/2) * sin(pitch/2) * sin(roll/2)
		z = (chy_chp * shr) - (shy_shp * chr); // cos(yaw/2) * cos(pitch/2) * sin(roll/2) - sin(yaw/2) * sin(pitch/2) * cos(roll/2)
		w = (chy_chp * chr) + (shy_shp * shr); // cos(yaw/2) * cos(pitch/2) * cos(roll/2) + sin(yaw/2) * sin(pitch/2) * sin(roll/2)
		return *this;
	}

	/** Get the pole of the gimbal lock, if any.
	 * @return positive (+1) for north pole, negative (-1) for south pole, zero (0) when no gimbal lock */
	 int getGimbalPole () {
		const float t = y * x + z * w;
		return t > 0.499f ? 1 : (t < -0.499f ? -1 : 0);
	}

	/** Get the roll euler angle in radians, which is the rotation around the z axis. Requires that this quaternion is normalized.
	 * @return the rotation around the z axis in radians (between -PI and +PI) */
	 float getRollRad () {
		const int pole = getGimbalPole();
		return pole == 0 ? atan2(2.0f * (w * z + y * x), 1.0f - 2.0f * (x * x + z * z)) : (float)pole * 2.0f
			* atan2(y, w);
	}

	/** Get the roll euler angle in degrees, which is the rotation around the z axis. Requires that this quaternion is normalized.
	 * @return the rotation around the z axis in degrees (between -180 and +180) */
	 float getRoll () {
		return getRollRad() * MathUtils::radiansToDegrees;
	}

	/** Get the pitch euler angle in radians, which is the rotation around the x axis. Requires that this quaternion is normalized.
	 * @return the rotation around the x axis in radians (between -(PI/2) and +(PI/2)) */
	 float getPitchRad () {
		const int pole = getGimbalPole();
		return pole == 0 ? asin(MathUtils::clamp(2.0f * (w * x - z * y), -1.0f, 1.0f)) : (float)pole * M_PI * 0.5f;
	}

	/** Get the pitch euler angle in degrees, which is the rotation around the x axis. Requires that this quaternion is normalized.
	 * @return the rotation around the x axis in degrees (between -90 and +90) */
	 float getPitch () {
		return getPitchRad() * MathUtils::radiansToDegrees;
	}

	/** Get the yaw euler angle in radians, which is the rotation around the y axis. Requires that this quaternion is normalized.
	 * @return the rotation around the y axis in radians (between -PI and +PI) */
	 float getYawRad () {
		return getGimbalPole() == 0 ? atan2(2.0f * (y * w + x * z), 1.0f - 2.0f * (y * y + x * x)) : 0.0f;
	}

	/** Get the yaw euler angle in degrees, which is the rotation around the y axis. Requires that this quaternion is normalized.
	 * @return the rotation around the y axis in degrees (between -180 and +180) */
	 float getYaw () {
		return getYawRad() * MathUtils::radiansToDegrees;
	}

	 const static float len2 (const float x, const float y, const float z, const float w) {
		return x * x + y * y + z * z + w * w;
	}

	/** @return the length of this quaternion without square root */
	 float len2 () {
		return x * x + y * y + z * z + w * w;
	}

	/** Normalizes this quaternion to unit length
	 * @return the quaternion for chaining */
	 Quaternion& nor () {
		float len = len2();
		if (len != 0.f && !MathUtils::isEqual(len, 1.0f)) {
			len = sqrt(len);
			w /= len;
			x /= len;
			y /= len;
			z /= len;
		}
		return *this;
	}

	/** Conjugate the quaternion.
	 * 
	 * @return This quaternion for chaining */
	 Quaternion& conjugate () {
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	// TODO : this would better fit into the vector3 class
	/** Transforms the given vector using this quaternion
	 * 
	 * @param v Vector to transform */
	 Vector3& transform (Vector3& v);

	/** Multiplies this quaternion with another one in the form of this = this * other
	 * 
	 * @param other Quaternion to multiply with
	 * @return This quaternion for chaining */
	 Quaternion& mul (const Quaternion& other) {
		const float X = this->w * other.x + this->x * other.w + this->y * other.z - this->z * other.y;
		const float Y = this->w * other.y + this->y * other.w + this->z * other.x - this->x * other.z;
		const float Z = this->w * other.z + this->z * other.w + this->x * other.y - this->y * other.x;
		const float W = this->w * other.w - this->x * other.x - this->y * other.y - this->z * other.z;
		this->x = X;
		this->y = Y;
		this->z = Z;
		this->w = W;
		return *this;
	}

	/** Multiplies this quaternion with another one in the form of this = this * other
	 * 
	 * @param x the x component of the other quaternion to multiply with
	 * @param y the y component of the other quaternion to multiply with
	 * @param z the z component of the other quaternion to multiply with
	 * @param w the w component of the other quaternion to multiply with
	 * @return This quaternion for chaining */
	 Quaternion& mul (const float x, const float y, const float z, const float w) {
		const float X = this->w * x + this->x * w + this->y * z - this->z * y;
		const float Y = this->w * y + this->y * w + this->z * x - this->x * z;
		const float Z = this->w * z + this->z * w + this->x * y - this->y * x;
		const float W = this->w * w - this->x * x - this->y * y - this->z * z;
		this->x = X;
		this->y = Y;
		this->z = Z;
		this->w = W;
		return *this;
	}

	/** Multiplies this quaternion with another one in the form of this = other * this
	 * 
	 * @param other Quaternion to multiply with
	 * @return This quaternion for chaining */
	 Quaternion& mulLeft (const Quaternion& other) {
		const float X = other.w * this->x + other.x * this->w + other.y * this->z - other.z * y;
		const float Y = other.w * this->y + other.y * this->w + other.z * this->x - other.x * z;
		const float Z = other.w * this->z + other.z * this->w + other.x * this->y - other.y * x;
		const float W = other.w * this->w - other.x * this->x - other.y * this->y - other.z * z;
		this->x = X;
		this->y = Y;
		this->z = Z;
		this->w = W;
		return *this;
	}

	/** Multiplies this quaternion with another one in the form of this = other * this
	 * 
	 * @param x the x component of the other quaternion to multiply with
	 * @param y the y component of the other quaternion to multiply with
	 * @param z the z component of the other quaternion to multiply with
	 * @param w the w component of the other quaternion to multiply with
	 * @return This quaternion for chaining */
	 Quaternion& mulLeft (const float x, const float y, const float z, const float w) {
		const float X = w * this->x + x * this->w + y * this->z - z * this->y;
		const float Y = w * this->y + y * this->w + z * this->x - x * this->z;
		const float Z = w * this->z + z * this->w + x * this->y - y * this->x;
		const float W = w * this->w - x * this->x - y * this->y - z * this->z;
		this->x = X;
		this->y = Y;
		this->z = Z;
		this->w = W;
		return *this;
	}

	/** Add the x,y,z,w components of the passed in quaternion to the ones of this quaternion */
	 Quaternion& add (const Quaternion& quaternion) {
		this->x += quaternion.x;
		this->y += quaternion.y;
		this->z += quaternion.z;
		this->w += quaternion.w;
		return *this;
	}

	/** Add the x,y,z,w components of the passed in quaternion to the ones of this quaternion */
	 Quaternion& add (float qx, float qy, float qz, float qw) {
		this->x += qx;
		this->y += qy;
		this->z += qz;
		this->w += qw;
		return *this;
	}

	// TODO : the matrix4 set(quaternion) doesnt set the last row+col of the matrix to 0,0,0,1 so... that's why there is this
// method
	/** Fills a 4x4 matrix with the rotation matrix represented by this quaternion.
	 * 
	 * @param matrix Matrix to fill */
	 void toMatrix (std::vector<float>& matrix);

	/** Sets the quaternion to an identity Quaternion
	 * @return this quaternion for chaining */
	 Quaternion& idt () {
		return this->set(0, 0, 0, 1);
	}

	/** @return If this quaternion is an identity Quaternion */
	 bool isIdentity () {
		return MathUtils::isZero(x) && MathUtils::isZero(y) && MathUtils::isZero(z) && MathUtils::isEqual(w, 1.0f);
	}

	/** @return If this quaternion is an identity Quaternion */
	 bool isIdentity (const float tolerance) {
		return MathUtils::isZero(x, tolerance) && MathUtils::isZero(y, tolerance) && MathUtils::isZero(z, tolerance)
			&& MathUtils::isEqual(w, 1.0f, tolerance);
	}

	// todo : the setFromAxis(v3,float) method should replace the set(v3,float) method
	/** Sets the quaternion components from the given axis and angle around that axis.
	 * 
	 * @param axis The axis
	 * @param degrees The angle in degrees
	 * @return This quaternion for chaining. */
	 Quaternion& setFromAxis (const Vector3& axis, const float degrees);

	/** Sets the quaternion components from the given axis and angle around that axis.
	 * 
	 * @param axis The axis
	 * @param radians The angle in radians
	 * @return This quaternion for chaining. */
	 Quaternion& setFromAxisRad (const Vector3& axis, const float radians);
     
	/** Sets the quaternion components from the given axis and angle around that axis.
	 * @param x X direction of the axis
	 * @param y Y direction of the axis
	 * @param z Z direction of the axis
	 * @param degrees The angle in degrees
	 * @return This quaternion for chaining. */
	 Quaternion& setFromAxis (const float x, const float y, const float z, const float degrees) {
		return setFromAxisRad(x, y, z, degrees * MathUtils::degreesToRadians);
	}

	/** Sets the quaternion components from the given axis and angle around that axis.
	 * @param x X direction of the axis
	 * @param y Y direction of the axis
	 * @param z Z direction of the axis
	 * @param radians The angle in radians
	 * @return This quaternion for chaining. */
	 Quaternion& setFromAxisRad (const float x, const float y, const float z, const float radians);

	/** Sets the Quaternion from the given matrix, optionally removing any scaling. */
	 Quaternion& setFromMatrix (bool normalizeAxes, const Matrix4& matrix);

	/** Sets the Quaternion from the given rotation matrix, which must not contain scaling. */
	 Quaternion& setFromMatrix (const Matrix4& matrix) {
		return setFromMatrix(false, matrix);
	}

	/** Sets the Quaternion from the given matrix, optionally removing any scaling. */
	 Quaternion& setFromMatrix (bool normalizeAxes, const Matrix3& matrix);

	/** Sets the Quaternion from the given rotation matrix, which must not contain scaling. */
	 Quaternion& setFromMatrix (const Matrix3& matrix) {
		return setFromMatrix(false, matrix);
	}

	/** <p>
	 * Sets the Quaternion from the given x-, y- and z-axis which have to be orthonormal.
	 * </p>
	 * 
	 * <p>
	 * Taken from Bones framework for JPCT, see http://www.aptalkarga.com/bones/ which in turn took it from Graphics Gem code at
	 * ftp://ftp.cis.upenn.edu/pub/graphics/shoemake/quatut.ps.Z.
	 * </p>
	 * 
	 * @param xx x-axis x-coordinate
	 * @param xy x-axis y-coordinate
	 * @param xz x-axis z-coordinate
	 * @param yx y-axis x-coordinate
	 * @param yy y-axis y-coordinate
	 * @param yz y-axis z-coordinate
	 * @param zx z-axis x-coordinate
	 * @param zy z-axis y-coordinate
	 * @param zz z-axis z-coordinate */
	 Quaternion& setFromAxes (float xx, float xy, float xz, float yx, float yy, float yz, float zx, float zy, float zz) {
		return setFromAxes(false, xx, xy, xz, yx, yy, yz, zx, zy, zz);
	}

	/** <p>
	 * Sets the Quaternion from the given x-, y- and z-axis.
	 * </p>
	 * 
	 * <p>
	 * Taken from Bones framework for JPCT, see http://www.aptalkarga.com/bones/ which in turn took it from Graphics Gem code at
	 * ftp://ftp.cis.upenn.edu/pub/graphics/shoemake/quatut.ps.Z.
	 * </p>
	 * 
	 * @param normalizeAxes whether to normalize the axes (necessary when they contain scaling)
	 * @param xx x-axis x-coordinate
	 * @param xy x-axis y-coordinate
	 * @param xz x-axis z-coordinate
	 * @param yx y-axis x-coordinate
	 * @param yy y-axis y-coordinate
	 * @param yz y-axis z-coordinate
	 * @param zx z-axis x-coordinate
	 * @param zy z-axis y-coordinate
	 * @param zz z-axis z-coordinate */
	 Quaternion& setFromAxes (bool normalizeAxes, float xx, float xy, float xz, float yx, float yy, float yz, float zx,
		float zy, float zz);

	/** Set this quaternion to the rotation between two vectors.
	 * @param v1 The base vector, which should be normalized.
	 * @param v2 The target vector, which should be normalized.
	 * @return This quaternion for chaining */
	 Quaternion setFromCross (const Vector3& v1, const Vector3& v2);

	/** Set this quaternion to the rotation between two vectors.
	 * @param x1 The base vectors x value, which should be normalized.
	 * @param y1 The base vectors y value, which should be normalized.
	 * @param z1 The base vectors z value, which should be normalized.
	 * @param x2 The target vector x value, which should be normalized.
	 * @param y2 The target vector y value, which should be normalized.
	 * @param z2 The target vector z value, which should be normalized.
	 * @return This quaternion for chaining */
	 Quaternion setFromCross (const float x1, const float y1, const float z1, const float x2, const float y2, const float z2);

	/** Spherical linear interpolation between this quaternion and the other quaternion, based on the alpha value in the range
	 * [0,1]. Taken from Bones framework for JPCT, see http://www.aptalkarga.com/bones/
	 * @param end the end quaternion
	 * @param alpha alpha in the range [0,1]
	 * @return this quaternion for chaining */
	 Quaternion& slerp (const Quaternion& end, float alpha) {
		const float d = this->x * end.x + this->y * end.y + this->z * end.z + this->w * end.w;
		float absDot = d < 0.f ? -d : d;

		// Set the first and second scale for the interpolation
		float scale0 = 1.0f - alpha;
		float scale1 = alpha;

		// Check if the angle between the 2 quaternions was big enough to
		// warrant such calculations
		if ((1 - absDot) > 0.1) {// Get the angle between the 2 quaternions,
			// and then store the sin() of that angle
			const float angle = acos(absDot);
			const float invSinTheta = 1.0f / sin(angle);

			// Calculate the scale for q1 and q2, according to the angle and
			// it's sine value
			scale0 = (sin((1.0f - alpha) * angle) * invSinTheta);
			scale1 = (sin((alpha * angle)) * invSinTheta);
		}

		if (d < 0.f) scale1 = -scale1;

		// Calculate the x, y, z and w values for the quaternion by using a
		// special form of linear interpolation for quaternions.
		x = (scale0 * x) + (scale1 * end.x);
		y = (scale0 * y) + (scale1 * end.y);
		z = (scale0 * z) + (scale1 * end.z);
		w = (scale0 * w) + (scale1 * end.w);

		// Return the interpolated quaternion
		return *this;
	}

	/** Spherical linearly interpolates multiple quaternions and stores the result in this Quaternion. Will not destroy the data
	 * previously inside the elements of q. result = (q_1^w_1)*(q_2^w_2)* ... *(q_n^w_n) where w_i=1/n.
	 * @param q List of quaternions
	 * @return This quaternion for chaining */
	 Quaternion& slerp (const std::vector<Quaternion>& q) {

		// Calculate exponents and multiply everything from left to right
		const float w = 1.0f / q.size();
		set(q[0]).exp(w);
		for (int i = 1; i < q.size(); i++)
			mul(tmp1.set(q[i]).exp(w));
		nor();
		return *this;
	}

	/** Spherical linearly interpolates multiple quaternions by the given weights and stores the result in this Quaternion. Will not
	 * destroy the data previously inside the elements of q or w. result = (q_1^w_1)*(q_2^w_2)* ... *(q_n^w_n) where the sum of w_i
	 * is 1. Lists must be equal in length.
	 * @param q List of quaternions
	 * @param w List of weights
	 * @return This quaternion for chaining */
	 Quaternion& slerp (const std::vector<Quaternion>& q, const std::vector<float>& w) {

		// Calculate exponents and multiply everything from left to right
		set(q[0]).exp(w[0]);
		for (int i = 1; i < q.size(); i++)
			mul(tmp1.set(q[i]).exp(w[i]));
		nor();
		return *this;
	}

	/** Calculates (this quaternion)^alpha where alpha is a real number and stores the result in this quaternion. See
	 * http://en.wikipedia.org/wiki/Quaternion#Exponential.2C_logarithm.2C_and_power
	 * @param alpha Exponent
	 * @return This quaternion for chaining */
	 Quaternion& exp (float alpha) {

		// Calculate |q|^alpha
		float norm = len();
		float normExp = pow(norm, alpha);

		// Calculate theta
		float theta = acos(w / norm);

		// Calculate coefficient of basis elements
		float coeff = 0;
		if (abs(theta) < 0.001f) // If theta is small enough, use the limit of sin(alpha*theta) / sin(theta) instead of actual
// value
			coeff = normExp * alpha / norm;
		else
			coeff = (float)(normExp * sin(alpha * theta) / (norm * sin(theta)));

		// Write results
		w = (float)(normExp * cos(alpha * theta));
		x *= coeff;
		y *= coeff;
		z *= coeff;

		// Fix any possible discrepancies
		nor();

		return *this;
	}

	
	 int hashCode () {
		const int prime = 31;
		int result = 1;
		result = prime * result + std::hash<float>{}(w);
		result = prime * result + std::hash<float>{}(x);
		result = prime * result + std::hash<float>{}(y);
		result = prime * result + std::hash<float>{}(z);
		return result;
	}

	
	 bool operator== (const Quaternion& lhs) {
		return lhs.x == x && lhs.y == y && lhs.z == z && lhs.w == w;
	}

	/** Get the dot product between the two quaternions (commutative).
	 * @param x1 the x component of the first quaternion
	 * @param y1 the y component of the first quaternion
	 * @param z1 the z component of the first quaternion
	 * @param w1 the w component of the first quaternion
	 * @param x2 the x component of the second quaternion
	 * @param y2 the y component of the second quaternion
	 * @param z2 the z component of the second quaternion
	 * @param w2 the w component of the second quaternion
	 * @return the dot product between the first and second quaternion. */
	 const static float dot (const float x1, const float y1, const float z1, const float w1, const float x2, const float y2,
		const float z2, const float w2) {
		return x1 * x2 + y1 * y2 + z1 * z2 + w1 * w2;
	}

	/** Get the dot product between this and the other quaternion (commutative).
	 * @param other the other quaternion.
	 * @return the dot product of this and the other quaternion. */
	 float dot (const Quaternion& other) {
		return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
	}

	/** Get the dot product between this and the other quaternion (commutative).
	 * @param x the x component of the other quaternion
	 * @param y the y component of the other quaternion
	 * @param z the z component of the other quaternion
	 * @param w the w component of the other quaternion
	 * @return the dot product of this and the other quaternion. */
	 float dot (const float x, const float y, const float z, const float w) {
		return this->x * x + this->y * y + this->z * z + this->w * w;
	}

	/** Multiplies the components of this quaternion with the given scalar.
	 * @param scalar the scalar.
	 * @return this quaternion for chaining. */
	 Quaternion& mul (float scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		this->w *= scalar;
		return *this;
	}

	/** Get the axis angle representation of the rotation in degrees. The supplied vector will receive the axis (x, y and z values)
	 * of the rotation and the value returned is the angle in degrees around that axis. Note that this method will alter the
	 * supplied vector, the existing value of the vector is ignored. </p> This will normalize this quaternion if needed. The
	 * received axis is a unit vector. However, if this is an identity quaternion (no rotation), then the length of the axis may be
	 * zero.
	 * 
	 * @param axis vector which will receive the axis
	 * @return the angle in degrees
	 * @see <a href="http://en.wikipedia.org/wiki/Axis%E2%80%93angle_representation">wikipedia</a>
	 * @see <a href="http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle">calculation</a> */
	 float getAxisAngle (const Vector3& axis) {
		return getAxisAngleRad(axis) * MathUtils::radiansToDegrees;
	}

	/** Get the axis-angle representation of the rotation in radians. The supplied vector will receive the axis (x, y and z values)
	 * of the rotation and the value returned is the angle in radians around that axis. Note that this method will alter the
	 * supplied vector, the existing value of the vector is ignored. </p> This will normalize this quaternion if needed. The
	 * received axis is a unit vector. However, if this is an identity quaternion (no rotation), then the length of the axis may be
	 * zero.
	 * 
	 * @param axis vector which will receive the axis
	 * @return the angle in radians
	 * @see <a href="http://en.wikipedia.org/wiki/Axis%E2%80%93angle_representation">wikipedia</a>
	 * @see <a href="http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle">calculation</a> */
	 float getAxisAngleRad (const Vector3& axis);

	/** Get the angle in radians of the rotation this quaternion represents. Does not normalize the quaternion. Use
	 * {@link #getAxisAngleRad(Vector3)} to get both the axis and the angle of this rotation. Use
	 * {@link #getAngleAroundRad(Vector3)} to get the angle around a specific axis.
	 * @return the angle in radians of the rotation */
	 float getAngleRad () {
		return (float)(2.0 * acos((this->w > 1) ? (this->w / len()) : this->w));
	}

	/** Get the angle in degrees of the rotation this quaternion represents. Use {@link #getAxisAngle(Vector3)} to get both the axis
	 * and the angle of this rotation. Use {@link #getAngleAround(Vector3)} to get the angle around a specific axis.
	 * @return the angle in degrees of the rotation */
	 float getAngle () {
		return getAngleRad() * MathUtils::radiansToDegrees;
	}

	/** Get the swing rotation and twist rotation for the specified axis. The twist rotation represents the rotation around the
	 * specified axis. The swing rotation represents the rotation of the specified axis itself, which is the rotation around an
	 * axis perpendicular to the specified axis. </p> The swing and twist rotation can be used to reconstruct the original
	 * quaternion: this = swing * twist
	 * 
	 * @param axisX the X component of the normalized axis for which to get the swing and twist rotation
	 * @param axisY the Y component of the normalized axis for which to get the swing and twist rotation
	 * @param axisZ the Z component of the normalized axis for which to get the swing and twist rotation
	 * @param swing will receive the swing rotation: the rotation around an axis perpendicular to the specified axis
	 * @param twist will receive the twist rotation: the rotation around the specified axis
	 * @see <a href="http://www.euclideanspace.com/maths/geometry/rotations/for/decomposition">calculation</a> */
	 void getSwingTwist (const float axisX, const float axisY, const float axisZ, const Quaternion& swing,
		const Quaternion& twist);

	/** Get the swing rotation and twist rotation for the specified axis. The twist rotation represents the rotation around the
	 * specified axis. The swing rotation represents the rotation of the specified axis itself, which is the rotation around an
	 * axis perpendicular to the specified axis. </p> The swing and twist rotation can be used to reconstruct the original
	 * quaternion: this = swing * twist
	 * 
	 * @param axis the normalized axis for which to get the swing and twist rotation
	 * @param swing will receive the swing rotation: the rotation around an axis perpendicular to the specified axis
	 * @param twist will receive the twist rotation: the rotation around the specified axis
	 * @see <a href="http://www.euclideanspace.com/maths/geometry/rotations/for/decomposition">calculation</a> */
	 void getSwingTwist (const Vector3& axis, const Quaternion& swing, const Quaternion& twist);

	/** Get the angle in radians of the rotation around the specified axis. The axis must be normalized.
	 * @param axisX the x component of the normalized axis for which to get the angle
	 * @param axisY the y component of the normalized axis for which to get the angle
	 * @param axisZ the z component of the normalized axis for which to get the angle
	 * @return the angle in radians of the rotation around the specified axis */
	 float getAngleAroundRad (const float axisX, const float axisY, const float axisZ);

	/** Get the angle in radians of the rotation around the specified axis. The axis must be normalized.
	 * @param axis the normalized axis for which to get the angle
	 * @return the angle in radians of the rotation around the specified axis */
	 float getAngleAroundRad (const Vector3& axis);

	/** Get the angle in degrees of the rotation around the specified axis. The axis must be normalized.
	 * @param axisX the x component of the normalized axis for which to get the angle
	 * @param axisY the y component of the normalized axis for which to get the angle
	 * @param axisZ the z component of the normalized axis for which to get the angle
	 * @return the angle in degrees of the rotation around the specified axis */
	 float getAngleAround (const float axisX, const float axisY, const float axisZ) {
		return getAngleAroundRad(axisX, axisY, axisZ) * MathUtils::radiansToDegrees;
	}

	/** Get the angle in degrees of the rotation around the specified axis. The axis must be normalized.
	 * @param axis the normalized axis for which to get the angle
	 * @return the angle in degrees of the rotation around the specified axis */
	 float getAngleAround (const Vector3& axis);
};