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
#include "Vector.h"
#include "Vector2.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "../Serializable.h"

/** Encapsulates a 3D vector. Allows chaining operations by returning a reference to itself in all modification methods.
 * @author badlogicgames@gmail.com */

template<typename T>
class Vector3:public Serializable, public Vector<T> {
	static const long serialVersionUID = 3840054589595372522L;

	/** the x-component of this vector **/
	float x;
	/** the y-component of this vector **/
	float y;
	/** the z-component of this vector **/
	float z;

	 Vector3 X =  Vector3(1, 0, 0);
	 Vector3 Y =  Vector3(0, 1, 0);
	 Vector3 Z =  Vector3(0, 0, 1);
	 Vector3 Zero =  Vector3(0, 0, 0);

	static Matrix4 tmpMat;
public:
	/** Constructs a vector at (0,0,0) */
	Vector3 () {
	}

	/** Creates a vector with the given components
	 * @param x The x-component
	 * @param y The y-component
	 * @param z The z-component */
	Vector3 (float x, float y, float z) {
		this->set(x, y, z);
	}

	/** Creates a vector from the given vector
	 * @param vector The vector */
	Vector3 (const Vector3& vector) {
		this->set(vector);
	}

	/** Creates a vector from the given array. The array must have at least 3 elements.
	 *
	 * @param values The array */
	Vector3 (const float* values) {
		this->set(values[0], values[1], values[2]);
	}

	/** Creates a vector from the given vector and z-component
	 *
	 * @param vector The vector
	 * @param z The z-component */
	Vector3 (const Vector2 vector, float z) {
		this->set(vector.x, vector.y, z);
	}

	/** Sets the vector to the given components
	 *
	 * @param x The x-component
	 * @param y The y-component
	 * @param z The z-component
	 * @return this vector for chaining */
	Vector3 set (float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
		return this;
	}

	
	Vector3 set (const Vector3 vector) {
		return this->set(vector.x, vector.y, vector.z);
	}

	/** Sets the components from the array. The array must have at least 3 elements
	 *
	 * @param values The array
	 * @return this vector for chaining */
	Vector3 set (const float* values) {
		return this->set(values[0], values[1], values[2]);
	}

	/** Sets the components of the given vector and z-component
	 *
	 * @param vector The vector
	 * @param z The z-component
	 * @return This vector for chaining */
	Vector3 set (const Vector2 vector, float z) {
		return this->set(vector.x, vector.y, z);
	}

	/** Sets the components from the given spherical coordinate
	 * @param azimuthalAngle The angle between x-axis in radians [0, 2pi]
	 * @param polarAngle The angle between z-axis in radians [0, pi]
	 * @return This vector for chaining */
	Vector3 setFromSpherical (float azimuthalAngle, float polarAngle) {
		float cosPolar = cos(polarAngle);
		float sinPolar = sin(polarAngle);

		float cosAzim = cos(azimuthalAngle);
		float sinAzim = sin(azimuthalAngle);

		return this->set(cosAzim * sinPolar, sinAzim * sinPolar, cosPolar);
	}

	
	Vector3 setToRandomDirection () {
		float u = random();
		float v = random();

		float theta = PI2 * u; // azimuthal angle
		float phi = acos(2f * v - 1f); // polar angle

		return this->setFromSpherical(theta, phi);
	}

	
	Vector3 cpy () {
		return new Vector3(this);
	}

	
	Vector3 add (const Vector3 vector) {
		return this->add(vector.x, vector.y, vector.z);
	}

	/** Adds the given vector to this component
	 * @param x The x-component of the other vector
	 * @param y The y-component of the other vector
	 * @param z The z-component of the other vector
	 * @return This vector for chaining. */
	Vector3 add (float x, float y, float z) {
		return this->set(this->x + x, this->y + y, this->z + z);
	}

	/** Adds the given value to all three components of the vector.
	 *
	 * @param values The value
	 * @return This vector for chaining */
	Vector3 add (float values) {
		return this->set(this->x + values, this->y + values, this->z + values);
	}

	
	Vector3 sub (const Vector3 a_vec) {
		return this->sub(a_vec.x, a_vec.y, a_vec.z);
	}

	/** Subtracts the other vector from this vector.
	 *
	 * @param x The x-component of the other vector
	 * @param y The y-component of the other vector
	 * @param z The z-component of the other vector
	 * @return This vector for chaining */
	Vector3 sub (float x, float y, float z) {
		return this->set(this->x - x, this->y - y, this->z - z);
	}

	/** Subtracts the given value from all components of this vector
	 *
	 * @param value The value
	 * @return This vector for chaining */
	Vector3 sub (float value) {
		return this->set(this->x - value, this->y - value, this->z - value);
	}

	
	Vector3 scl (float scalar) {
		return this->set(this->x * scalar, this->y * scalar, this->z * scalar);
	}

	
	Vector3 scl (const Vector3 other) {
		return this->set(x * other.x, y * other.y, z * other.z);
	}

	/** Scales this vector by the given values
	 * @param vx X value
	 * @param vy Y value
	 * @param vz Z value
	 * @return This vector for chaining */
	Vector3 scl (float vx, float vy, float vz) {
		return this->set(this->x * vx, this->y * vy, this->z * vz);
	}

	
	Vector3 mulAdd (Vector3 vec, float scalar) {
		this->x += vec.x * scalar;
		this->y += vec.y * scalar;
		this->z += vec.z * scalar;
		return this;
	}

	
	Vector3 mulAdd (Vector3 vec, Vector3 mulVec) {
		this->x += vec.x * mulVec.x;
		this->y += vec.y * mulVec.y;
		this->z += vec.z * mulVec.z;
		return this;
	}

	/** @return The euclidean length */
	static float len (const float x, const float y, const float z) {
		return sqrt(x * x + y * y + z * z);
	}

	
	float len () {
		return sqrt(x * x + y * y + z * z);
	}

	/** @return The squared euclidean length */
	static float len2 (const float x, const float y, const float z) {
		return x * x + y * y + z * z;
	}

	
	float len2 () {
		return x * x + y * y + z * z;
	}

	/** @param vector The other vector
	 * @return Whether this and the other vector are equal */
	bool idt (const Vector3 vector) {
		return x == vector.x && y == vector.y && z == vector.z;
	}

	/** @return The euclidean distance between the two specified vectors */
	static float dst (const float x1, const float y1, const float z1, const float x2, const float y2, const float z2) {
		float a = x2 - x1;
		float b = y2 - y1;
		float c = z2 - z1;
		return sqrt(a * a + b * b + c * c);
	}

	
	float dst (const Vector3 vector) {
		float a = vector.x - x;
		float b = vector.y - y;
		float c = vector.z - z;
		return sqrt(a * a + b * b + c * c);
	}

	/** @return the distance between this point and the given point */
	float dst (float x, float y, float z) {
		float a = x - this->x;
		float b = y - this->y;
		float c = z - this->z;
		return sqrt(a * a + b * b + c * c);
	}

	/** @return the squared distance between the given points */
	static float dst2 (const float x1, float y1, float z1, float x2, float y2, float z2) {
		float a = x2 - x1;
		float b = y2 - y1;
		float c = z2 - z1;
		return a * a + b * b + c * c;
	}

	
	float dst2 (Vector3 point) {
		float a = point.x - x;
		float b = point.y - y;
		float c = point.z - z;
		return a * a + b * b + c * c;
	}

	/** Returns the squared distance between this point and the given point
	 * @param x The x-component of the other point
	 * @param y The y-component of the other point
	 * @param z The z-component of the other point
	 * @return The squared distance */
	float dst2 (float x, float y, float z) {
		float a = x - this->x;
		float b = y - this->y;
		float c = z - this->z;
		return a * a + b * b + c * c;
	}

	
	Vector3 nor () {
		float len2 = this->len2();
		if (len2 == 0f || len2 == 1f) return this;
		return this->scl(1f / sqrt(len2));
	}

	/** @return The dot product between the two vectors */
	static float dot (float x1, float y1, float z1, float x2, float y2, float z2) {
		return x1 * x2 + y1 * y2 + z1 * z2;
	}

	
	float dot (const Vector3 vector) {
		return x * vector.x + y * vector.y + z * vector.z;
	}

	/** Returns the dot product between this and the given vector.
	 * @param x The x-component of the other vector
	 * @param y The y-component of the other vector
	 * @param z The z-component of the other vector
	 * @return The dot product */
	float dot (float x, float y, float z) {
		return this->x * x + this->y * y + this->z * z;
	}

	/** Sets this vector to the cross product between it and the other vector.
	 * @param vector The other vector
	 * @return This vector for chaining */
	Vector3 crs (const Vector3 vector) {
		return this->set(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
	}

	/** Sets this vector to the cross product between it and the other vector.
	 * @param x The x-component of the other vector
	 * @param y The y-component of the other vector
	 * @param z The z-component of the other vector
	 * @return This vector for chaining */
	Vector3 crs (float x, float y, float z) {
		return this->set(this->y * z - this->z * y, this->z * x - this->x * z, this->x * y - this->y * x);
	}

	/** Left-multiplies the vector by the given 4x3 column major matrix. The matrix should be composed by a 3x3 matrix representing
	 * rotation and scale plus a 1x3 matrix representing the translation.
	 * @param matrix The matrix
	 * @return This vector for chaining */
	Vector3 mul4x3 (float* matrix) {
		return set(x * matrix[0] + y * matrix[3] + z * matrix[6] + matrix[9], x * matrix[1] + y * matrix[4] + z * matrix[7]
			+ matrix[10], x * matrix[2] + y * matrix[5] + z * matrix[8] + matrix[11]);
	}

	/** Left-multiplies the vector by the given matrix, assuming the fourth (w) component of the vector is 1.
	 * @param matrix The matrix
	 * @return This vector for chaining */
	Vector3 mul (const Matrix4 matrix) {
		float l_mat[] = matrix.val;
		return this->set(x * l_mat[Matrix4.M00] + y * l_mat[Matrix4.M01] + z * l_mat[Matrix4.M02] + l_mat[Matrix4.M03], x
			* l_mat[Matrix4.M10] + y * l_mat[Matrix4.M11] + z * l_mat[Matrix4.M12] + l_mat[Matrix4.M13], x * l_mat[Matrix4.M20] + y
			* l_mat[Matrix4.M21] + z * l_mat[Matrix4.M22] + l_mat[Matrix4.M23]);
	}

	/** Multiplies the vector by the transpose of the given matrix, assuming the fourth (w) component of the vector is 1.
	 * @param matrix The matrix
	 * @return This vector for chaining */
	Vector3 traMul (const Matrix4 matrix) {
		float l_mat[] = matrix.val;
		return this->set(x * l_mat[Matrix4.M00] + y * l_mat[Matrix4.M10] + z * l_mat[Matrix4.M20] + l_mat[Matrix4.M30], x
			* l_mat[Matrix4.M01] + y * l_mat[Matrix4.M11] + z * l_mat[Matrix4.M21] + l_mat[Matrix4.M31], x * l_mat[Matrix4.M02] + y
			* l_mat[Matrix4.M12] + z * l_mat[Matrix4.M22] + l_mat[Matrix4.M32]);
	}

	/** Left-multiplies the vector by the given matrix.
	 * @param matrix The matrix
	 * @return This vector for chaining */
	Vector3 mul (Matrix3 matrix) {
		float l_mat[] = matrix.val;
		return set(x * l_mat[Matrix3.M00] + y * l_mat[Matrix3.M01] + z * l_mat[Matrix3.M02], x * l_mat[Matrix3.M10] + y
			* l_mat[Matrix3.M11] + z * l_mat[Matrix3.M12], x * l_mat[Matrix3.M20] + y * l_mat[Matrix3.M21] + z * l_mat[Matrix3.M22]);
	}

	/** Multiplies the vector by the transpose of the given matrix.
	 * @param matrix The matrix
	 * @return This vector for chaining */
	Vector3 traMul (Matrix3 matrix) {
		float l_mat[] = matrix.val;
		return set(x * l_mat[Matrix3.M00] + y * l_mat[Matrix3.M10] + z * l_mat[Matrix3.M20], x * l_mat[Matrix3.M01] + y
			* l_mat[Matrix3.M11] + z * l_mat[Matrix3.M21], x * l_mat[Matrix3.M02] + y * l_mat[Matrix3.M12] + z * l_mat[Matrix3.M22]);
	}

	/** Multiplies the vector by the given {@link Quaternion}.
	 * @return This vector for chaining */
	Vector3 mul (const Quaternion quat) {
		return quat.transform(this);
	}

	/** Multiplies this vector by the given matrix dividing by w, assuming the fourth (w) component of the vector is 1. This is
	 * mostly used to project/unproject vectors via a perspective projection matrix.
	 *
	 * @param matrix The matrix.
	 * @return This vector for chaining */
	Vector3 prj (const Matrix4 matrix) {
		float l_mat[] = matrix.val;
		float l_w = 1f / (x * l_mat[Matrix4.M30] + y * l_mat[Matrix4.M31] + z * l_mat[Matrix4.M32] + l_mat[Matrix4.M33]);
		return this->set((x * l_mat[Matrix4.M00] + y * l_mat[Matrix4.M01] + z * l_mat[Matrix4.M02] + l_mat[Matrix4.M03]) * l_w, (x
			* l_mat[Matrix4.M10] + y * l_mat[Matrix4.M11] + z * l_mat[Matrix4.M12] + l_mat[Matrix4.M13])
			* l_w, (x * l_mat[Matrix4.M20] + y * l_mat[Matrix4.M21] + z * l_mat[Matrix4.M22] + l_mat[Matrix4.M23]) * l_w);
	}

	/** Multiplies this vector by the first three columns of the matrix, essentially only applying rotation and scaling.
	 *
	 * @param matrix The matrix
	 * @return This vector for chaining */
	Vector3 rot (const Matrix4 matrix) {
		float l_mat[] = matrix.val;
		return this->set(x * l_mat[Matrix4.M00] + y * l_mat[Matrix4.M01] + z * l_mat[Matrix4.M02], x * l_mat[Matrix4.M10] + y
			* l_mat[Matrix4.M11] + z * l_mat[Matrix4.M12], x * l_mat[Matrix4.M20] + y * l_mat[Matrix4.M21] + z * l_mat[Matrix4.M22]);
	}

	/** Multiplies this vector by the transpose of the first three columns of the matrix. Note: only works for translation and
	 * rotation, does not work for scaling. For those, use {@link #rot(Matrix4)} with {@link Matrix4#inv()}.
	 * @param matrix The transformation matrix
	 * @return The vector for chaining */
	Vector3 unrotate (const Matrix4 matrix) {
		float l_mat[] = matrix.val;
		return this->set(x * l_mat[Matrix4.M00] + y * l_mat[Matrix4.M10] + z * l_mat[Matrix4.M20], x * l_mat[Matrix4.M01] + y
			* l_mat[Matrix4.M11] + z * l_mat[Matrix4.M21], x * l_mat[Matrix4.M02] + y * l_mat[Matrix4.M12] + z * l_mat[Matrix4.M22]);
	}

	/** Translates this vector in the direction opposite to the translation of the matrix and the multiplies this vector by the
	 * transpose of the first three columns of the matrix. Note: only works for translation and rotation, does not work for
	 * scaling. For those, use {@link #mul(Matrix4)} with {@link Matrix4#inv()}.
	 * @param matrix The transformation matrix
	 * @return The vector for chaining */
	Vector3 untransform (const Matrix4 matrix) {
		float l_mat[] = matrix.val;
		x -= l_mat[Matrix4.M03];
		y -= l_mat[Matrix4.M03];
		z -= l_mat[Matrix4.M03];
		return this->set(x * l_mat[Matrix4.M00] + y * l_mat[Matrix4.M10] + z * l_mat[Matrix4.M20], x * l_mat[Matrix4.M01] + y
			* l_mat[Matrix4.M11] + z * l_mat[Matrix4.M21], x * l_mat[Matrix4.M02] + y * l_mat[Matrix4.M12] + z * l_mat[Matrix4.M22]);
	}

	/** Rotates this vector by the given angle in degrees around the given axis.
	 *
	 * @param degrees the angle in degrees
	 * @param axisX the x-component of the axis
	 * @param axisY the y-component of the axis
	 * @param axisZ the z-component of the axis
	 * @return This vector for chaining */
	Vector3 rotate (float degrees, float axisX, float axisY, float axisZ) {
		return this->mul(tmpMat.setToRotation(axisX, axisY, axisZ, degrees));
	}

	/** Rotates this vector by the given angle in radians around the given axis.
	 *
	 * @param radians the angle in radians
	 * @param axisX the x-component of the axis
	 * @param axisY the y-component of the axis
	 * @param axisZ the z-component of the axis
	 * @return This vector for chaining */
	Vector3 rotateRad (float radians, float axisX, float axisY, float axisZ) {
		return this->mul(tmpMat.setToRotationRad(axisX, axisY, axisZ, radians));
	}

	/** Rotates this vector by the given angle in degrees around the given axis.
	 *
	 * @param axis the axis
	 * @param degrees the angle in degrees
	 * @return This vector for chaining */
	Vector3 rotate (const Vector3 axis, float degrees) {
		tmpMat.setToRotation(axis, degrees);
		return this->mul(tmpMat);
	}

	/** Rotates this vector by the given angle in radians around the given axis.
	 *
	 * @param axis the axis
	 * @param radians the angle in radians
	 * @return This vector for chaining */
	Vector3 rotateRad (const Vector3 axis, float radians) {
		tmpMat.setToRotationRad(axis, radians);
		return this->mul(tmpMat);
	}

	
	bool isUnit () {
		return isUnit(0.000000001f);
	}

	
	bool isUnit (const float margin) {
		return Math.abs(len2() - 1f) < margin;
	}

	
	bool isZero () {
		return x == 0 && y == 0 && z == 0;
	}

	
	bool isZero (const float margin) {
		return len2() < margin;
	}

	
	bool isOnLine (Vector3 other, float epsilon) {
		return len2(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x) <= epsilon;
	}

	
	bool isOnLine (Vector3 other) {
		return len2(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x) <= FLOAT_ROUNDING_ERROR;
	}

	
	bool isCollinear (Vector3 other, float epsilon) {
		return isOnLine(other, epsilon) && hasSameDirection(other);
	}

	
	bool isCollinear (Vector3 other) {
		return isOnLine(other) && hasSameDirection(other);
	}

	
	bool isCollinearOpposite (Vector3 other, float epsilon) {
		return isOnLine(other, epsilon) && hasOppositeDirection(other);
	}

	
	bool isCollinearOpposite (Vector3 other) {
		return isOnLine(other) && hasOppositeDirection(other);
	}

	
	bool isPerpendicular (Vector3 vector) {
		return isZero(dot(vector));
	}

	
	bool isPerpendicular (Vector3 vector, float epsilon) {
		return isZero(dot(vector), epsilon);
	}

	
	bool hasSameDirection (Vector3 vector) {
		return dot(vector) > 0;
	}

	
	bool hasOppositeDirection (Vector3 vector) {
		return dot(vector) < 0;
	}

	
	Vector3 lerp (const Vector3 target, float alpha) {
		x += alpha * (target.x - x);
		y += alpha * (target.y - y);
		z += alpha * (target.z - z);
		return this;
	}

	
	Vector3 interpolate (Vector3 target, float alpha, Interpolation& interpolator) {
		return lerp(target, interpolator.apply(0f, 1f, alpha));
	}

	/** Spherically interpolates between this vector and the target vector by alpha which is in the range [0,1]. The result is
	 * stored in this vector.
	 *
	 * @param target The target vector
	 * @param alpha The interpolation coefficient
	 * @return This vector for chaining. */
	Vector3 slerp (const Vector3 target, float alpha) {
		float dot = dot(target);
		// If the inputs are too close for comfort, simply linearly interpolate.
		if (dot > 0.9995 || dot < -0.9995) return lerp(target, alpha);

		// theta0 = angle between input vectors
		float theta0 = acos(dot);
		// theta = angle between this vector and result
		float theta = theta0 * alpha;

		float st = (float)Math.sin(theta);
		float tx = target.x - x * dot;
		float ty = target.y - y * dot;
		float tz = target.z - z * dot;
		float l2 = tx * tx + ty * ty + tz * tz;
		float dl = st * ((l2 < 0.0001f) ? 1f : 1f / sqrt(l2));

		return scl((float)Math.cos(theta)).add(tx * dl, ty * dl, tz * dl).nor();
	}

	/** Converts this {@code Vector3} to a string in the format {@code (x,y,z)}.
	 * @return a string representation of this object. */
	
	std::string toString () {
		return "(" + x + "," + y + "," + z + ")";
	}

	/** Sets this {@code Vector3} to the value represented by the specified string according to the format of {@link #toString()}.
	 * @param v the string.
	 * @return this vector for chaining */
	Vector3 fromString (std::string v) {
		int s0 = v.indexOf(',', 1);
		int s1 = v.indexOf(',', s0 + 1);
		if (s0 != -1 && s1 != -1 && v.charAt(0) == '(' && v.charAt(v.length() - 1) == ')') {
			try {
				float x = Float.parseFloat(v.substring(1, s0));
				float y = Float.parseFloat(v.substring(s0 + 1, s1));
				float z = Float.parseFloat(v.substring(s1 + 1, v.length() - 1));
				return this->set(x, y, z);
			} catch (NumberFormatException ex) {
				// Throw a GdxRuntimeException
			}
		}
		throw new GdxRuntimeException("Malformed Vector3: " + v);
	}

	
	Vector3 limit (float limit) {
		return limit2(limit * limit);
	}

	
	Vector3 limit2 (float limit2) {
		float len2 = len2();
		if (len2 > limit2) {
			scl(sqrt(limit2 / len2));
		}
		return this;
	}

	
	Vector3 setLength (float len) {
		return setLength2(len * len);
	}

	
	Vector3 setLength2 (float len2) {
		float oldLen2 = len2();
		return (oldLen2 == 0 || oldLen2 == len2) ? this : scl(sqrt(len2 / oldLen2));
	}

	
	Vector3 clamp (float min, float max) {
		float len2 = len2();
		if (len2 == 0f) return this;
		float max2 = max * max;
		if (len2 > max2) return scl(sqrt(max2 / len2));
		float min2 = min * min;
		if (len2 < min2) return scl(sqrt(min2 / len2));
		return this;
	}

	
	int hashCode () {
		int prime = 31;
		int result = 1;
		result = prime * result + NumberUtils.floatToIntBits(x);
		result = prime * result + NumberUtils.floatToIntBits(y);
		result = prime * result + NumberUtils.floatToIntBits(z);
		return result;
	}

	
	bool operator== (const Vector3& obj) {
		return x == obj.x && y == obj.y && z == obj.z;
	}

	
	bool epsilonEquals (const Vector3 other, float epsilon) {
		if (other == null) return false;
		if (abs(other.x - x) > epsilon) return false;
		if (abs(other.y - y) > epsilon) return false;
		if (abs(other.z - z) > epsilon) return false;
		return true;
	}

	/** Compares this vector with the other vector, using the supplied epsilon for fuzzy equality testing.
	 * @return whether the vectors are the same. */
	bool epsilonEquals (float x, float y, float z, float epsilon) {
		if (abs(x - this->x) > epsilon) return false;
		if (abs(y - this->y) > epsilon) return false;
		if (abs(z - this->z) > epsilon) return false;
		return true;
	}

	
	Vector3 setZero () {
		this->x = 0;
		this->y = 0;
		this->z = 0;
		return this;
	}
};


