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
#include "Quaternion.h"
#include "Vector3.h"
#include <string>
#include <sstream>
#include "MathUtils.h"

/** Encapsulates a <a href="http://en.wikipedia.org/wiki/Row-major_order#Column-major_order">column major</a> 4 by 4 matrix. Like
 * the {@link Vector3} class it allows the chaining of methods by returning a reference to itself. For example:
 * 
 * <pre>
 * Matrix4 mat = new Matrix4().trn(position).mul(camera.combined);
 * </pre>
 * 
 * @author badlogicgames@gmail.com */

class Matrix3;
class Vector3;
class Quaternion;
class Affine2;
class Matrix4:public Serializable {
    public:
	static const long serialVersionUID = -2717655254359579617L;
	/** XX: Typically the unrotated X component for scaling, also the cosine of the angle when rotated on the Y and/or Z axis. On
	 * Vector3 multiplication this value is multiplied with the source X component and added to the target X component. */
	static const int M00 = 0;
	/** XY: Typically the negative sine of the angle when rotated on the Z axis. On Vector3 multiplication this value is multiplied
	 * with the source Y component and added to the target X component. */
	static const int M01 = 4;
	/** XZ: Typically the sine of the angle when rotated on the Y axis. On Vector3 multiplication this value is multiplied with the
	 * source Z component and added to the target X component. */
	static const int M02 = 8;
	/** XW: Typically the translation of the X component. On Vector3 multiplication this value is added to the target X component. */
	static const int M03 = 12;
	/** YX: Typically the sine of the angle when rotated on the Z axis. On Vector3 multiplication this value is multiplied with the
	 * source X component and added to the target Y component. */
	static const int M10 = 1;
	/** YY: Typically the unrotated Y component for scaling, also the cosine of the angle when rotated on the X and/or Z axis. On
	 * Vector3 multiplication this value is multiplied with the source Y component and added to the target Y component. */
	static const int M11 = 5;
	/** YZ: Typically the negative sine of the angle when rotated on the X axis. On Vector3 multiplication this value is multiplied
	 * with the source Z component and added to the target Y component. */
	static const int M12 = 9;
	/** YW: Typically the translation of the Y component. On Vector3 multiplication this value is added to the target Y component. */
	static const int M13 = 13;
	/** ZX: Typically the negative sine of the angle when rotated on the Y axis. On Vector3 multiplication this value is multiplied
	 * with the source X component and added to the target Z component. */
	static const int M20 = 2;
	/** ZY: Typical the sine of the angle when rotated on the X axis. On Vector3 multiplication this value is multiplied with the
	 * source Y component and added to the target Z component. */
	static const int M21 = 6;
	/** ZZ: Typically the unrotated Z component for scaling, also the cosine of the angle when rotated on the X and/or Y axis. On
	 * Vector3 multiplication this value is multiplied with the source Z component and added to the target Z component. */
	static const int M22 = 10;
	/** ZW: Typically the translation of the Z component. On Vector3 multiplication this value is added to the target Z component. */
	static const int M23 = 14;
	/** WX: Typically the value zero. On Vector3 multiplication this value is ignored. */
	static const int M30 = 3;
	/** WY: Typically the value zero. On Vector3 multiplication this value is ignored. */
	static const int M31 = 7;
	/** WZ: Typically the value zero. On Vector3 multiplication this value is ignored. */
	static const int M32 = 11;
	/** WW: Typically the value one. On Vector3 multiplication this value is ignored. */
	static const int M33 = 15;

	static std::vector<float> tmp;
	std::vector<float>  val = std::vector<float>(16);

	/** Constructs an identity matrix */
	Matrix4 () {
		val[M00] = 1.0f;
		val[M11] = 1.0f;
		val[M22] = 1.0f;
		val[M33] = 1.0f;
	}

	/** Constructs a matrix from the given matrix.
	 * 
	 * @param matrix The matrix to copy. (This matrix is not modified) */
	Matrix4 (const Matrix4& matrix) {
		this->set(matrix);
	}

	/** Constructs a matrix from the given float array. The array must have at least 16 elements; the first 16 will be copied.
	 * @param values The float array to copy. Remember that this matrix is in <a
	 *           href="http://en.wikipedia.org/wiki/Row-major_order">column major</a> order. (The float array is not modified) */
	Matrix4 (const std::vector<float>& values) {
		this->set(values);
	}

	/** Constructs a rotation matrix from the given {@link Quaternion}.
	 * @param quaternion The quaternion to be copied. (The quaternion is not modified) */
	Matrix4 (const Quaternion& quaternion) {
		this->set(quaternion);
	}

	/** Construct a matrix from the given translation, rotation and scale.
	 * @param position The translation
	 * @param rotation The rotation, must be normalized
	 * @param scale The scale */
	Matrix4 (const Vector3& position, const Quaternion& rotation, const Vector3& scale) {
		set(position, rotation, scale);
	}

	/** Sets the matrix to the given matrix.
	 * 
	 * @param matrix The matrix that is to be copied. (The given matrix is not modified)
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& set (const Matrix4& matrix) {
		return this->set(matrix.val);
	}

	/** Sets the matrix to the given matrix as a float array. The float array must have at least 16 elements; the first 16 will be
	 * copied.
	 * 
	 * @param values The matrix, in float form, that is to be copied. Remember that this matrix is in <a
	 *           href="http://en.wikipedia.org/wiki/Row-major_order">column major</a> order.
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& set (const std::vector<float>& values) {
        val = values;
		return *this;
	}

	/** Sets the matrix to a rotation matrix representing the quaternion.
	 * 
	 * @param quaternion The quaternion that is to be used to set this matrix.
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& set (const Quaternion& quaternion);

	/** Sets the matrix to a rotation matrix representing the quaternion.
	 * 
	 * @param quaternionX The X component of the quaternion that is to be used to set this matrix.
	 * @param quaternionY The Y component of the quaternion that is to be used to set this matrix.
	 * @param quaternionZ The Z component of the quaternion that is to be used to set this matrix.
	 * @param quaternionW The W component of the quaternion that is to be used to set this matrix.
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& set (float quaternionX, float quaternionY, float quaternionZ, float quaternionW) {
		return set(0.0f, 0.0f, 0.0f, quaternionX, quaternionY, quaternionZ, quaternionW);
	}

	/** Set this matrix to the specified translation and rotation.
	 * @param position The translation
	 * @param orientation The rotation, must be normalized
	 * @return This matrix for chaining */
	Matrix4& set (const Vector3& position, const Quaternion& orientation);

	/** Sets the matrix to a rotation matrix representing the translation and quaternion.
	 * 
	 * @param translationX The X component of the translation that is to be used to set this matrix.
	 * @param translationY The Y component of the translation that is to be used to set this matrix.
	 * @param translationZ The Z component of the translation that is to be used to set this matrix.
	 * @param quaternionX The X component of the quaternion that is to be used to set this matrix.
	 * @param quaternionY The Y component of the quaternion that is to be used to set this matrix.
	 * @param quaternionZ The Z component of the quaternion that is to be used to set this matrix.
	 * @param quaternionW The W component of the quaternion that is to be used to set this matrix.
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& set (float translationX, float translationY, float translationZ, float quaternionX, float quaternionY,
		float quaternionZ, float quaternionW) {
		float xs = quaternionX * 2.0f, ys = quaternionY * 2.0f, zs = quaternionZ * 2.0f;
		float wx = quaternionW * xs, wy = quaternionW * ys, wz = quaternionW * zs;
		float xx = quaternionX * xs, xy = quaternionX * ys, xz = quaternionX * zs;
		float yy = quaternionY * ys, yz = quaternionY * zs, zz = quaternionZ * zs;

		val[M00] = (1.0f - (yy + zz));
		val[M01] = (xy - wz);
		val[M02] = (xz + wy);
		val[M03] = translationX;

		val[M10] = (xy + wz);
		val[M11] = (1.0f - (xx + zz));
		val[M12] = (yz - wx);
		val[M13] = translationY;

		val[M20] = (xz - wy);
		val[M21] = (yz + wx);
		val[M22] = (1.0f - (xx + yy));
		val[M23] = translationZ;

		val[M30] = 0.0f;
		val[M31] = 0.0f;
		val[M32] = 0.0f;
		val[M33] = 1.0f;
		return *this;
	}

	/** Set this matrix to the specified translation, rotation and scale.
	 * @param position The translation
	 * @param orientation The rotation, must be normalized
	 * @param scale The scale
	 * @return This matrix for chaining */
	Matrix4& set (const Vector3& position, const Quaternion& orientation, const Vector3& scale);

	/** Sets the matrix to a rotation matrix representing the translation and quaternion.
	 * 
	 * @param translationX The X component of the translation that is to be used to set this matrix.
	 * @param translationY The Y component of the translation that is to be used to set this matrix.
	 * @param translationZ The Z component of the translation that is to be used to set this matrix.
	 * @param quaternionX The X component of the quaternion that is to be used to set this matrix.
	 * @param quaternionY The Y component of the quaternion that is to be used to set this matrix.
	 * @param quaternionZ The Z component of the quaternion that is to be used to set this matrix.
	 * @param quaternionW The W component of the quaternion that is to be used to set this matrix.
	 * @param scaleX The X component of the scaling that is to be used to set this matrix.
	 * @param scaleY The Y component of the scaling that is to be used to set this matrix.
	 * @param scaleZ The Z component of the scaling that is to be used to set this matrix.
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& set (float translationX, float translationY, float translationZ, float quaternionX, float quaternionY,
		float quaternionZ, float quaternionW, float scaleX, float scaleY, float scaleZ) {
		float xs = quaternionX * 2.0f, ys = quaternionY * 2.0f, zs = quaternionZ * 2.0f;
		float wx = quaternionW * xs, wy = quaternionW * ys, wz = quaternionW * zs;
		float xx = quaternionX * xs, xy = quaternionX * ys, xz = quaternionX * zs;
		float yy = quaternionY * ys, yz = quaternionY * zs, zz = quaternionZ * zs;

		val[M00] = scaleX * (1.0f - (yy + zz));
		val[M01] = scaleY * (xy - wz);
		val[M02] = scaleZ * (xz + wy);
		val[M03] = translationX;

		val[M10] = scaleX * (xy + wz);
		val[M11] = scaleY * (1.0f - (xx + zz));
		val[M12] = scaleZ * (yz - wx);
		val[M13] = translationY;

		val[M20] = scaleX * (xz - wy);
		val[M21] = scaleY * (yz + wx);
		val[M22] = scaleZ * (1.0f - (xx + yy));
		val[M23] = translationZ;

		val[M30] = 0.0f;
		val[M31] = 0.0f;
		val[M32] = 0.0f;
		val[M33] = 1.0f;
		return *this;
	}

	/** Sets the four columns of the matrix which correspond to the x-, y- and z-axis of the vector space this matrix creates as
	 * well as the 4th column representing the translation of any point that is multiplied by this matrix.
	 * 
	 * @param xAxis The x-axis.
	 * @param yAxis The y-axis.
	 * @param zAxis The z-axis.
	 * @param pos The translation vector. */
	Matrix4& set (const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis, const Vector3& pos);

	/** @return a copy of this matrix */
	Matrix4 cpy () {
		return Matrix4(*this);
	}

	/** Adds a translational component to the matrix in the 4th column. The other columns are untouched.
	 * 
	 * @param vector The translation vector to add to the current matrix. (This vector is not modified)
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& trn (const Vector3& vector);

	/** Adds a translational component to the matrix in the 4th column. The other columns are untouched.
	 * 
	 * @param x The x-component of the translation vector.
	 * @param y The y-component of the translation vector.
	 * @param z The z-component of the translation vector.
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& trn (float x, float y, float z) {
		val[M03] += x;
		val[M13] += y;
		val[M23] += z;
		return *this;
	}

	/** @return the backing float array */
	std::vector<float> getValues () {
		return val;
	}

	/** Postmultiplies this matrix with the given matrix, storing the result in this matrix. For example:
	 * 
	 * <pre>
	 * A.mul(B) results in A := AB.
	 * </pre>
	 * 
	 * @param matrix The other matrix to multiply by.
	 * @return This matrix for the purpose of chaining operations together. */
	Matrix4& mul (const Matrix4& matrix) {
		mul(val, matrix.val);
		return *this;
	}

	/** Premultiplies this matrix with the given matrix, storing the result in this matrix. For example:
	 * 
	 * <pre>
	 * A.mulLeft(B) results in A := BA.
	 * </pre>
	 * 
	 * @param matrix The other matrix to multiply by.
	 * @return This matrix for the purpose of chaining operations together. */
	Matrix4& mulLeft (const Matrix4& matrix) {
		tmpMat.set(matrix);
		mul(tmpMat.val, this->val);
		return set(tmpMat);
	}

	/** Transposes the matrix.
	 * 
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& tra () {
		tmp[M00] = val[M00];
		tmp[M01] = val[M10];
		tmp[M02] = val[M20];
		tmp[M03] = val[M30];
		tmp[M10] = val[M01];
		tmp[M11] = val[M11];
		tmp[M12] = val[M21];
		tmp[M13] = val[M31];
		tmp[M20] = val[M02];
		tmp[M21] = val[M12];
		tmp[M22] = val[M22];
		tmp[M23] = val[M32];
		tmp[M30] = val[M03];
		tmp[M31] = val[M13];
		tmp[M32] = val[M23];
		tmp[M33] = val[M33];
		return set(tmp);
	}

	/** Sets the matrix to an identity matrix.
	 * 
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& idt () {
		val[M00] = 1;
		val[M01] = 0;
		val[M02] = 0;
		val[M03] = 0;
		val[M10] = 0;
		val[M11] = 1;
		val[M12] = 0;
		val[M13] = 0;
		val[M20] = 0;
		val[M21] = 0;
		val[M22] = 1;
		val[M23] = 0;
		val[M30] = 0;
		val[M31] = 0;
		val[M32] = 0;
		val[M33] = 1;
		return *this;
	}

	/** Inverts the matrix. Stores the result in this matrix.
	 * 
	 * @return This matrix for the purpose of chaining methods together.
	 * @throws RuntimeException if the matrix is singular (not invertible) */
	Matrix4& inv () {
		float l_det = val[M30] * val[M21] * val[M12] * val[M03] - val[M20] * val[M31] * val[M12] * val[M03] - val[M30] * val[M11]
			* val[M22] * val[M03] + val[M10] * val[M31] * val[M22] * val[M03] + val[M20] * val[M11] * val[M32] * val[M03] - val[M10]
			* val[M21] * val[M32] * val[M03] - val[M30] * val[M21] * val[M02] * val[M13] + val[M20] * val[M31] * val[M02] * val[M13]
			+ val[M30] * val[M01] * val[M22] * val[M13] - val[M00] * val[M31] * val[M22] * val[M13] - val[M20] * val[M01] * val[M32]
			* val[M13] + val[M00] * val[M21] * val[M32] * val[M13] + val[M30] * val[M11] * val[M02] * val[M23] - val[M10] * val[M31]
			* val[M02] * val[M23] - val[M30] * val[M01] * val[M12] * val[M23] + val[M00] * val[M31] * val[M12] * val[M23] + val[M10]
			* val[M01] * val[M32] * val[M23] - val[M00] * val[M11] * val[M32] * val[M23] - val[M20] * val[M11] * val[M02] * val[M33]
			+ val[M10] * val[M21] * val[M02] * val[M33] + val[M20] * val[M01] * val[M12] * val[M33] - val[M00] * val[M21] * val[M12]
			* val[M33] - val[M10] * val[M01] * val[M22] * val[M33] + val[M00] * val[M11] * val[M22] * val[M33];
		if (l_det == 0.0f) throw "RuntimeException: non-invertible matrix";
		float inv_det = 1.0f / l_det;
		tmp[M00] = val[M12] * val[M23] * val[M31] - val[M13] * val[M22] * val[M31] + val[M13] * val[M21] * val[M32] - val[M11]
			* val[M23] * val[M32] - val[M12] * val[M21] * val[M33] + val[M11] * val[M22] * val[M33];
		tmp[M01] = val[M03] * val[M22] * val[M31] - val[M02] * val[M23] * val[M31] - val[M03] * val[M21] * val[M32] + val[M01]
			* val[M23] * val[M32] + val[M02] * val[M21] * val[M33] - val[M01] * val[M22] * val[M33];
		tmp[M02] = val[M02] * val[M13] * val[M31] - val[M03] * val[M12] * val[M31] + val[M03] * val[M11] * val[M32] - val[M01]
			* val[M13] * val[M32] - val[M02] * val[M11] * val[M33] + val[M01] * val[M12] * val[M33];
		tmp[M03] = val[M03] * val[M12] * val[M21] - val[M02] * val[M13] * val[M21] - val[M03] * val[M11] * val[M22] + val[M01]
			* val[M13] * val[M22] + val[M02] * val[M11] * val[M23] - val[M01] * val[M12] * val[M23];
		tmp[M10] = val[M13] * val[M22] * val[M30] - val[M12] * val[M23] * val[M30] - val[M13] * val[M20] * val[M32] + val[M10]
			* val[M23] * val[M32] + val[M12] * val[M20] * val[M33] - val[M10] * val[M22] * val[M33];
		tmp[M11] = val[M02] * val[M23] * val[M30] - val[M03] * val[M22] * val[M30] + val[M03] * val[M20] * val[M32] - val[M00]
			* val[M23] * val[M32] - val[M02] * val[M20] * val[M33] + val[M00] * val[M22] * val[M33];
		tmp[M12] = val[M03] * val[M12] * val[M30] - val[M02] * val[M13] * val[M30] - val[M03] * val[M10] * val[M32] + val[M00]
			* val[M13] * val[M32] + val[M02] * val[M10] * val[M33] - val[M00] * val[M12] * val[M33];
		tmp[M13] = val[M02] * val[M13] * val[M20] - val[M03] * val[M12] * val[M20] + val[M03] * val[M10] * val[M22] - val[M00]
			* val[M13] * val[M22] - val[M02] * val[M10] * val[M23] + val[M00] * val[M12] * val[M23];
		tmp[M20] = val[M11] * val[M23] * val[M30] - val[M13] * val[M21] * val[M30] + val[M13] * val[M20] * val[M31] - val[M10]
			* val[M23] * val[M31] - val[M11] * val[M20] * val[M33] + val[M10] * val[M21] * val[M33];
		tmp[M21] = val[M03] * val[M21] * val[M30] - val[M01] * val[M23] * val[M30] - val[M03] * val[M20] * val[M31] + val[M00]
			* val[M23] * val[M31] + val[M01] * val[M20] * val[M33] - val[M00] * val[M21] * val[M33];
		tmp[M22] = val[M01] * val[M13] * val[M30] - val[M03] * val[M11] * val[M30] + val[M03] * val[M10] * val[M31] - val[M00]
			* val[M13] * val[M31] - val[M01] * val[M10] * val[M33] + val[M00] * val[M11] * val[M33];
		tmp[M23] = val[M03] * val[M11] * val[M20] - val[M01] * val[M13] * val[M20] - val[M03] * val[M10] * val[M21] + val[M00]
			* val[M13] * val[M21] + val[M01] * val[M10] * val[M23] - val[M00] * val[M11] * val[M23];
		tmp[M30] = val[M12] * val[M21] * val[M30] - val[M11] * val[M22] * val[M30] - val[M12] * val[M20] * val[M31] + val[M10]
			* val[M22] * val[M31] + val[M11] * val[M20] * val[M32] - val[M10] * val[M21] * val[M32];
		tmp[M31] = val[M01] * val[M22] * val[M30] - val[M02] * val[M21] * val[M30] + val[M02] * val[M20] * val[M31] - val[M00]
			* val[M22] * val[M31] - val[M01] * val[M20] * val[M32] + val[M00] * val[M21] * val[M32];
		tmp[M32] = val[M02] * val[M11] * val[M30] - val[M01] * val[M12] * val[M30] - val[M02] * val[M10] * val[M31] + val[M00]
			* val[M12] * val[M31] + val[M01] * val[M10] * val[M32] - val[M00] * val[M11] * val[M32];
		tmp[M33] = val[M01] * val[M12] * val[M20] - val[M02] * val[M11] * val[M20] + val[M02] * val[M10] * val[M21] - val[M00]
			* val[M12] * val[M21] - val[M01] * val[M10] * val[M22] + val[M00] * val[M11] * val[M22];
		val[M00] = tmp[M00] * inv_det;
		val[M01] = tmp[M01] * inv_det;
		val[M02] = tmp[M02] * inv_det;
		val[M03] = tmp[M03] * inv_det;
		val[M10] = tmp[M10] * inv_det;
		val[M11] = tmp[M11] * inv_det;
		val[M12] = tmp[M12] * inv_det;
		val[M13] = tmp[M13] * inv_det;
		val[M20] = tmp[M20] * inv_det;
		val[M21] = tmp[M21] * inv_det;
		val[M22] = tmp[M22] * inv_det;
		val[M23] = tmp[M23] * inv_det;
		val[M30] = tmp[M30] * inv_det;
		val[M31] = tmp[M31] * inv_det;
		val[M32] = tmp[M32] * inv_det;
		val[M33] = tmp[M33] * inv_det;
		return *this;
	}

	/** @return The determinant of this matrix */
	float det () {
		return val[M30] * val[M21] * val[M12] * val[M03] - val[M20] * val[M31] * val[M12] * val[M03] - val[M30] * val[M11]
			* val[M22] * val[M03] + val[M10] * val[M31] * val[M22] * val[M03] + val[M20] * val[M11] * val[M32] * val[M03] - val[M10]
			* val[M21] * val[M32] * val[M03] - val[M30] * val[M21] * val[M02] * val[M13] + val[M20] * val[M31] * val[M02] * val[M13]
			+ val[M30] * val[M01] * val[M22] * val[M13] - val[M00] * val[M31] * val[M22] * val[M13] - val[M20] * val[M01] * val[M32]
			* val[M13] + val[M00] * val[M21] * val[M32] * val[M13] + val[M30] * val[M11] * val[M02] * val[M23] - val[M10] * val[M31]
			* val[M02] * val[M23] - val[M30] * val[M01] * val[M12] * val[M23] + val[M00] * val[M31] * val[M12] * val[M23] + val[M10]
			* val[M01] * val[M32] * val[M23] - val[M00] * val[M11] * val[M32] * val[M23] - val[M20] * val[M11] * val[M02] * val[M33]
			+ val[M10] * val[M21] * val[M02] * val[M33] + val[M20] * val[M01] * val[M12] * val[M33] - val[M00] * val[M21] * val[M12]
			* val[M33] - val[M10] * val[M01] * val[M22] * val[M33] + val[M00] * val[M11] * val[M22] * val[M33];
	}

	/** @return The determinant of the 3x3 upper left matrix */
	float det3x3 () {
		return val[M00] * val[M11] * val[M22] + val[M01] * val[M12] * val[M20] + val[M02] * val[M10] * val[M21] - val[M00]
			* val[M12] * val[M21] - val[M01] * val[M10] * val[M22] - val[M02] * val[M11] * val[M20];
	}

	/** Sets the matrix to a projection matrix with a near- and far plane, a field of view in degrees and an aspect ratio. Note that
	 * the field of view specified is the angle in degrees for the height, the field of view for the width will be calculated
	 * according to the aspect ratio.
	 * 
	 * @param near The near plane
	 * @param far The far plane
	 * @param fovy The field of view of the height in degrees
	 * @param aspectRatio The "width over height" aspect ratio
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& setToProjection (float near, float far, float fovy, float aspectRatio) {
		idt();
		float l_fd = (float)(1.0 / tan((fovy * (M_PI / 180)) / 2.0));
		float l_a1 = (far + near) / (near - far);
		float l_a2 = (2 * far * near) / (near - far);
		val[M00] = l_fd / aspectRatio;
		val[M10] = 0;
		val[M20] = 0;
		val[M30] = 0;
		val[M01] = 0;
		val[M11] = l_fd;
		val[M21] = 0;
		val[M31] = 0;
		val[M02] = 0;
		val[M12] = 0;
		val[M22] = l_a1;
		val[M32] = -1;
		val[M03] = 0;
		val[M13] = 0;
		val[M23] = l_a2;
		val[M33] = 0;

		return *this;
	}

	/** Sets the matrix to a projection matrix with a near/far plane, and left, bottom, right and top specifying the points on the
	 * near plane that are mapped to the lower left and upper right corners of the viewport. This allows to create projection
	 * matrix with off-center vanishing point.
	 * 
	 * @param left
	 * @param right
	 * @param bottom
	 * @param top
	 * @param near The near plane
	 * @param far The far plane
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& setToProjection (float left, float right, float bottom, float top, float near, float far) {
		float x = 2.0f * near / (right - left);
		float y = 2.0f * near / (top - bottom);
		float a = (right + left) / (right - left);
		float b = (top + bottom) / (top - bottom);
		float l_a1 = (far + near) / (near - far);
		float l_a2 = (2 * far * near) / (near - far);
		val[M00] = x;
		val[M10] = 0;
		val[M20] = 0;
		val[M30] = 0;
		val[M01] = 0;
		val[M11] = y;
		val[M21] = 0;
		val[M31] = 0;
		val[M02] = a;
		val[M12] = b;
		val[M22] = l_a1;
		val[M32] = -1;
		val[M03] = 0;
		val[M13] = 0;
		val[M23] = l_a2;
		val[M33] = 0;

		return *this;
	}

	/** Sets this matrix to an orthographic projection matrix with the origin at (x,y) extending by width and height. The near plane
	 * is set to 0, the far plane is set to 1.
	 * 
	 * @param x The x-coordinate of the origin
	 * @param y The y-coordinate of the origin
	 * @param width The width
	 * @param height The height
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& setToOrtho2D (float x, float y, float width, float height) {
		setToOrtho(x, x + width, y, y + height, 0, 1);
		return *this;
	}

	/** Sets this matrix to an orthographic projection matrix with the origin at (x,y) extending by width and height, having a near
	 * and far plane.
	 * 
	 * @param x The x-coordinate of the origin
	 * @param y The y-coordinate of the origin
	 * @param width The width
	 * @param height The height
	 * @param near The near plane
	 * @param far The far plane
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& setToOrtho2D (float x, float y, float width, float height, float near, float far) {
		setToOrtho(x, x + width, y, y + height, near, far);
		return *this;
	}

	/** Sets the matrix to an orthographic projection like glOrtho (http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml) following
	 * the OpenGL equivalent
	 * 
	 * @param left The left clipping plane
	 * @param right The right clipping plane
	 * @param bottom The bottom clipping plane
	 * @param top The top clipping plane
	 * @param near The near clipping plane
	 * @param far The far clipping plane
	 * @return This matrix for the purpose of chaining methods together. */
	 Matrix4& setToOrtho (float left, float right, float bottom, float top, float near, float far) {

		this->idt();
		float x_orth = 2 / (right - left);
		float y_orth = 2 / (top - bottom);
		float z_orth = -2 / (far - near);

		float tx = -(right + left) / (right - left);
		float ty = -(top + bottom) / (top - bottom);
		float tz = -(far + near) / (far - near);

		val[M00] = x_orth;
		val[M10] = 0;
		val[M20] = 0;
		val[M30] = 0;
		val[M01] = 0;
		val[M11] = y_orth;
		val[M21] = 0;
		val[M31] = 0;
		val[M02] = 0;
		val[M12] = 0;
		val[M22] = z_orth;
		val[M32] = 0;
		val[M03] = tx;
		val[M13] = ty;
		val[M23] = tz;
		val[M33] = 1;

		return *this;
	}

	/** Sets the 4th column to the translation vector.
	 * 
	 * @param vector The translation vector
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& setTranslation (const Vector3& vector);

	/** Sets the 4th column to the translation vector.
	 * 
	 * @param x The X coordinate of the translation vector
	 * @param y The Y coordinate of the translation vector
	 * @param z The Z coordinate of the translation vector
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& setTranslation (float x, float y, float z) {
		val[M03] = x;
		val[M13] = y;
		val[M23] = z;
		return *this;
	}

	/** Sets this matrix to a translation matrix, overwriting it first by an identity matrix and then setting the 4th column to the
	 * translation vector.
	 * 
	 * @param vector The translation vector
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4 setToTranslation (const Vector3& vector);

	/** Sets this matrix to a translation matrix, overwriting it first by an identity matrix and then setting the 4th column to the
	 * translation vector.
	 * 
	 * @param x The x-component of the translation vector.
	 * @param y The y-component of the translation vector.
	 * @param z The z-component of the translation vector.
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& setToTranslation (float x, float y, float z) {
		idt();
		val[M03] = x;
		val[M13] = y;
		val[M23] = z;
		return *this;
	}

	/** Sets this matrix to a translation and scaling matrix by first overwriting it with an identity and then setting the
	 * translation vector in the 4th column and the scaling vector in the diagonal.
	 * 
	 * @param translation The translation vector
	 * @param scaling The scaling vector
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& setToTranslationAndScaling (const Vector3& translation, const Vector3& scaling);

	/** Sets this matrix to a translation and scaling matrix by first overwriting it with an identity and then setting the
	 * translation vector in the 4th column and the scaling vector in the diagonal.
	 * 
	 * @param translationX The x-component of the translation vector
	 * @param translationY The y-component of the translation vector
	 * @param translationZ The z-component of the translation vector
	 * @param scalingX The x-component of the scaling vector
	 * @param scalingY The x-component of the scaling vector
	 * @param scalingZ The x-component of the scaling vector
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& setToTranslationAndScaling (float translationX, float translationY, float translationZ, float scalingX,
		float scalingY, float scalingZ) {
		idt();
		val[M03] = translationX;
		val[M13] = translationY;
		val[M23] = translationZ;
		val[M00] = scalingX;
		val[M11] = scalingY;
		val[M22] = scalingZ;
		return *this;
	}

	static Quaternion quat;
	static Quaternion quat2;

	/** Sets the matrix to a rotation matrix around the given axis.
	 * 
	 * @param axis The axis
	 * @param degrees The angle in degrees
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& setToRotation (const Vector3& axis, float degrees);

	/** Sets the matrix to a rotation matrix around the given axis.
	 * 
	 * @param axis The axis
	 * @param radians The angle in radians
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& setToRotationRad (const Vector3& axis, float radians);

	/** Sets the matrix to a rotation matrix around the given axis.
	 * 
	 * @param axisX The x-component of the axis
	 * @param axisY The y-component of the axis
	 * @param axisZ The z-component of the axis
	 * @param degrees The angle in degrees
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& setToRotation (float axisX, float axisY, float axisZ, float degrees);

	/** Sets the matrix to a rotation matrix around the given axis.
	 * 
	 * @param axisX The x-component of the axis
	 * @param axisY The y-component of the axis
	 * @param axisZ The z-component of the axis
	 * @param radians The angle in radians
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& setToRotationRad (float axisX, float axisY, float axisZ, float radians);

	/** Set the matrix to a rotation matrix between two vectors.
	 * @param v1 The base vector
	 * @param v2 The target vector
	 * @return This matrix for the purpose of chaining methods together */
	Matrix4& setToRotation (Vector3& v1, Vector3& v2);

	/** Set the matrix to a rotation matrix between two vectors.
	 * @param x1 The base vectors x value
	 * @param y1 The base vectors y value
	 * @param z1 The base vectors z value
	 * @param x2 The target vector x value
	 * @param y2 The target vector y value
	 * @param z2 The target vector z value
	 * @return This matrix for the purpose of chaining methods together */
	Matrix4& setToRotation (float x1,float y1, float z1, float x2, float y2, float z2);

	/** Sets this matrix to a rotation matrix from the given euler angles.
	 * @param yaw the yaw in degrees
	 * @param pitch the pitch in degrees
	 * @param roll the roll in degrees
	 * @return This matrix */
	Matrix4& setFromEulerAngles (float yaw, float pitch, float roll);
	
	/** Sets this matrix to a rotation matrix from the given euler angles.
	 * @param yaw the yaw in radians
	 * @param pitch the pitch in radians
	 * @param roll the roll in radians
	 * @return This matrix */
	Matrix4& setFromEulerAnglesRad (float yaw, float pitch, float roll);

	/** Sets this matrix to a scaling matrix
	 * 
	 * @param vector The scaling vector
	 * @return This matrix for chaining. */
	Matrix4& setToScaling (const Vector3& vector);

	/** Sets this matrix to a scaling matrix
	 * 
	 * @param x The x-component of the scaling vector
	 * @param y The y-component of the scaling vector
	 * @param z The z-component of the scaling vector
	 * @return This matrix for chaining. */
	Matrix4& setToScaling (float x, float y, float z) {
		idt();
		val[M00] = x;
		val[M11] = y;
		val[M22] = z;
		return *this;
	}

	static Vector3 l_vez;
	static Vector3 l_vex;
	static Vector3 l_vey;

	/** Sets the matrix to a look at matrix with a direction and an up vector. Multiply with a translation matrix to get a camera
	 * model view matrix.
	 * 
	 * @param direction The direction vector
	 * @param up The up vector
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& setToLookAt (const Vector3& direction, const Vector3& up);
    
    static Vector3 tmpVec;
	static Matrix4 tmpMat;

	/** Sets this matrix to a look at matrix with the given position, target and up vector.
	 * 
	 * @param position the position
	 * @param target the target
	 * @param up the up vector
	 * @return This matrix */
	Matrix4& setToLookAt (const Vector3& position, const Vector3& target, const Vector3& up);

	static Vector3 right;
	static Vector3 tmpForward;
	static Vector3 tmpUp;

	Matrix4& setToWorld (const Vector3& position, const Vector3& forward, const Vector3& up);

	std::string toString () {
        std::stringstream ss;
		ss<< "[" << val[M00] << "|" << val[M01] << "|" << val[M02] << "|" << val[M03] << "]\n" << "[" << val[M10] <<"|" << val[M11] << "|"
			<< val[M12] << "|" << val[M13] << "]\n" << "[" << val[M20] << "|" << val[M21] << "|" << val[M22] << "|" << val[M23] << "]\n" << "["
			<< val[M30] << "|" << val[M31] << "|" << val[M32] << "|" << val[M33] << "]\n";
        return ss.str();
	}

	/** Linearly interpolates between this matrix and the given matrix mixing by alpha
	 * @param matrix the matrix
	 * @param alpha the alpha value in the range [0,1]
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& lerp (const Matrix4& matrix, float alpha) {
		for (int i = 0; i < 16; i++)
			this->val[i] = this->val[i] * (1 - alpha) + matrix.val[i] * alpha;
		return *this;
	}

	/** Averages the given transform with this one and stores the result in this matrix. Translations and scales are lerped while
	 * rotations are slerped.
	 * @param other The other transform
	 * @param w Weight of this transform; weight of the other transform is (1 - w)
	 * @return This matrix for chaining */
	Matrix4& avg (Matrix4& other, float w);

	/** Averages the given transforms and stores the result in this matrix. Translations and scales are lerped while rotations are
	 * slerped. Does not destroy the data contained in t.
	 * @param t List of transforms
	 * @return This matrix for chaining */
	Matrix4& avg (std::vector<Matrix4>& t);

	/** Averages the given transforms with the given weights and stores the result in this matrix. Translations and scales are
	 * lerped while rotations are slerped. Does not destroy the data contained in t or w; Sum of w_i must be equal to 1, or
	 * unexpected results will occur.
	 * @param t List of transforms
	 * @param w List of weights
	 * @return This matrix for chaining */
	Matrix4& avg (std::vector<Matrix4>& t, std::vector<float> w);

	/** Sets this matrix to the given 3x3 matrix. The third column of this matrix is set to (0,0,1,0).
	 * @param mat the matrix */
	Matrix4& set (const Matrix3& mat);

	/** Sets this matrix to the given affine matrix. The values are mapped as follows:
	 *
	 * <pre>
	 *      [  M00  M01   0   M02  ]
	 *      [  M10  M11   0   M12  ]
	 *      [   0    0    1    0   ]
	 *      [   0    0    0    1   ]
	 * </pre>
	 * @param affine the affine matrix
	 * @return This matrix for chaining */
	Matrix4& set (const Affine2& affine);

	/** Assumes that this matrix is a 2D affine transformation, copying only the relevant components. The values are mapped as
	 * follows:
	 *
	 * <pre>
	 *      [  M00  M01   _   M02  ]
	 *      [  M10  M11   _   M12  ]
	 *      [   _    _    _    _   ]
	 *      [   _    _    _    _   ]
	 * </pre>
	 * @param affine the source matrix
	 * @return This matrix for chaining */
	Matrix4& setAsAffine (const Affine2& affine);

	/** Assumes that both matrices are 2D affine transformations, copying only the relevant components. The copied values are:
	 *
	 * <pre>
	 *      [  M00  M01   _   M03  ]
	 *      [  M10  M11   _   M13  ]
	 *      [   _    _    _    _   ]
	 *      [   _    _    _    _   ]
	 * </pre>
	 * @param mat the source matrix
	 * @return This matrix for chaining */
	Matrix4& setAsAffine (const Matrix4& mat) {
		val[M00] = mat.val[M00];
		val[M10] = mat.val[M10];
		val[M01] = mat.val[M01];
		val[M11] = mat.val[M11];
		val[M03] = mat.val[M03];
		val[M13] = mat.val[M13];
		return *this;
	}

	Matrix4& scl (const Vector3& scale);

	Matrix4& scl (float x, float y, float z) {
		val[M00] *= x;
		val[M11] *= y;
		val[M22] *= z;
		return *this;
	}

	Matrix4& scl (float scale) {
		val[M00] *= scale;
		val[M11] *= scale;
		val[M22] *= scale;
		return *this;
	}

	Vector3& getTranslation (Vector3& position);

	/** Gets the rotation of this matrix.
	 * @param rotation The {@link Quaternion} to receive the rotation
	 * @param normalizeAxes True to normalize the axes, necessary when the matrix might also include scaling.
	 * @return The provided {@link Quaternion} for chaining. */
	Quaternion& getRotation (Quaternion& rotation, bool normalizeAxes);

	/** Gets the rotation of this matrix.
	 * @param rotation The {@link Quaternion} to receive the rotation
	 * @return The provided {@link Quaternion} for chaining. */
	Quaternion& getRotation (Quaternion& rotation);

	/** @return the squared scale factor on the X axis */
	float getScaleXSquared () {
		return val[Matrix4::M00] * val[Matrix4::M00] + val[Matrix4::M01] * val[Matrix4::M01] + val[Matrix4::M02] * val[Matrix4::M02];
	}

	/** @return the squared scale factor on the Y axis */
	float getScaleYSquared () {
		return val[Matrix4::M10] * val[Matrix4::M10] + val[Matrix4::M11] * val[Matrix4::M11] + val[Matrix4::M12] * val[Matrix4::M12];
	}

	/** @return the squared scale factor on the Z axis */
	float getScaleZSquared () {
		return val[Matrix4::M20] * val[Matrix4::M20] + val[Matrix4::M21] * val[Matrix4::M21] + val[Matrix4::M22] * val[Matrix4::M22];
	}

	/** @return the scale factor on the X axis (non-negative) */
	float getScaleX () {
		return (MathUtils::isZero(val[Matrix4::M01]) && MathUtils::isZero(val[Matrix4::M02])) ? abs(val[Matrix4::M00])
			: sqrt(getScaleXSquared());
	}

	/** @return the scale factor on the Y axis (non-negative) */
	float getScaleY () {
		return (MathUtils::isZero(val[Matrix4::M10]) && MathUtils::isZero(val[Matrix4::M12])) ? abs(val[Matrix4::M11])
			: sqrt(getScaleYSquared());
	}

	/** @return the scale factor on the X axis (non-negative) */
	float getScaleZ () {
		return (MathUtils::isZero(val[Matrix4::M20]) && MathUtils::isZero(val[Matrix4::M21])) ? abs(val[Matrix4::M22])
			: sqrt(getScaleZSquared());
	}

	/** @param scale The vector which will receive the (non-negative) scale components on each axis.
	 * @return The provided vector for chaining. */
	Vector3& getScale (Vector3 scale);

	/** removes the translational part and transposes the matrix. */
	Matrix4& toNormalMatrix () {
		val[M03] = 0;
		val[M13] = 0;
		val[M23] = 0;
		return inv().tra();
	}

	// @on
	/** Postmultiplies this matrix by a translation matrix. Postmultiplication is also used by OpenGL ES'
	 * glTranslate/glRotate/glScale
	 * @param translation
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& translate (const Vector3& translation);

	/** Postmultiplies this matrix by a translation matrix. Postmultiplication is also used by OpenGL ES' 1.x
	 * glTranslate/glRotate/glScale.
	 * @param x Translation in the x-axis.
	 * @param y Translation in the y-axis.
	 * @param z Translation in the z-axis.
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& translate (float x, float y, float z) {
		tmp[M00] = 1;
		tmp[M01] = 0;
		tmp[M02] = 0;
		tmp[M03] = x;
		tmp[M10] = 0;
		tmp[M11] = 1;
		tmp[M12] = 0;
		tmp[M13] = y;
		tmp[M20] = 0;
		tmp[M21] = 0;
		tmp[M22] = 1;
		tmp[M23] = z;
		tmp[M30] = 0;
		tmp[M31] = 0;
		tmp[M32] = 0;
		tmp[M33] = 1;

		mul(val, tmp);
		return *this;
	}

	/** Postmultiplies this matrix with a (counter-clockwise) rotation matrix. Postmultiplication is also used by OpenGL ES' 1.x
	 * glTranslate/glRotate/glScale.
	 * 
	 * @param axis The vector axis to rotate around.
	 * @param degrees The angle in degrees.
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& rotate (const Vector3& axis, float degrees);

	/** Postmultiplies this matrix with a (counter-clockwise) rotation matrix. Postmultiplication is also used by OpenGL ES' 1.x
	 * glTranslate/glRotate/glScale.
	 * 
	 * @param axis The vector axis to rotate around.
	 * @param radians The angle in radians.
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& rotateRad (const Vector3& axis, float radians);

	/** Postmultiplies this matrix with a (counter-clockwise) rotation matrix. Postmultiplication is also used by OpenGL ES' 1.x
	 * glTranslate/glRotate/glScale
	 * @param axisX The x-axis component of the vector to rotate around.
	 * @param axisY The y-axis component of the vector to rotate around.
	 * @param axisZ The z-axis component of the vector to rotate around.
	 * @param degrees The angle in degrees
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& rotate (float axisX, float axisY, float axisZ, float degrees);

	/** Postmultiplies this matrix with a (counter-clockwise) rotation matrix. Postmultiplication is also used by OpenGL ES' 1.x
	 * glTranslate/glRotate/glScale
	 * @param axisX The x-axis component of the vector to rotate around.
	 * @param axisY The y-axis component of the vector to rotate around.
	 * @param axisZ The z-axis component of the vector to rotate around.
	 * @param radians The angle in radians
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& rotateRad (float axisX, float axisY, float axisZ, float radians);

	/** Postmultiplies this matrix with a (counter-clockwise) rotation matrix. Postmultiplication is also used by OpenGL ES' 1.x
	 * glTranslate/glRotate/glScale.
	 * 
	 * @param rotation
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& rotate (Quaternion& rotation);

	/** Postmultiplies this matrix by the rotation between two vectors.
	 * @param v1 The base vector
	 * @param v2 The target vector
	 * @return This matrix for the purpose of chaining methods together */
	Matrix4& rotate (Vector3& v1, Vector3& v2);

	/** Postmultiplies this matrix with a scale matrix. Postmultiplication is also used by OpenGL ES' 1.x
	 * glTranslate/glRotate/glScale.
	 * @param scaleX The scale in the x-axis.
	 * @param scaleY The scale in the y-axis.
	 * @param scaleZ The scale in the z-axis.
	 * @return This matrix for the purpose of chaining methods together. */
	Matrix4& scale (float scaleX, float scaleY, float scaleZ) {
		tmp[M00] = scaleX;
		tmp[M01] = 0;
		tmp[M02] = 0;
		tmp[M03] = 0;
		tmp[M10] = 0;
		tmp[M11] = scaleY;
		tmp[M12] = 0;
		tmp[M13] = 0;
		tmp[M20] = 0;
		tmp[M21] = 0;
		tmp[M22] = scaleZ;
		tmp[M23] = 0;
		tmp[M30] = 0;
		tmp[M31] = 0;
		tmp[M32] = 0;
		tmp[M33] = 1;

		mul(val, tmp);
		return *this;
	}

	/** Copies the 4x3 upper-left sub-matrix into float array. The destination array is supposed to be a column major matrix.
	 * @param dst the destination matrix */
	void extract4x3Matrix (std::vector<float> dst) {
		dst[0] = val[M00];
		dst[1] = val[M10];
		dst[2] = val[M20];
		dst[3] = val[M01];
		dst[4] = val[M11];
		dst[5] = val[M21];
		dst[6] = val[M02];
		dst[7] = val[M12];
		dst[8] = val[M22];
		dst[9] = val[M03];
		dst[10] = val[M13];
		dst[11] = val[M23];
	}

	/** @return True if this matrix has any rotation or scaling, false otherwise */
	bool hasRotationOrScaling () {
		return !(MathUtils::isEqual(val[M00], 1) && MathUtils::isEqual(val[M11], 1) && MathUtils::isEqual(val[M22], 1)
			&& MathUtils::isZero(val[M01]) && MathUtils::isZero(val[M02]) && MathUtils::isZero(val[M10]) && MathUtils::isZero(val[M12])
			&& MathUtils::isZero(val[M20]) && MathUtils::isZero(val[M21]));
	}
    
    static void mul(std::vector<float>& mata, const std::vector<float>& matb) {
		std::vector<float> tmp = std::vector<float>(16);
		tmp[M00] = mata[M00] * matb[M00] + mata[M01] * matb[M10] + mata[M02] * matb[M20] + mata[M03] * matb[M30];
		tmp[M01] = mata[M00] * matb[M01] + mata[M01] * matb[M11] + mata[M02] * matb[M21] + mata[M03] * matb[M31];
		tmp[M02] = mata[M00] * matb[M02] + mata[M01] * matb[M12] + mata[M02] * matb[M22] + mata[M03] * matb[M32];
		tmp[M03] = mata[M00] * matb[M03] + mata[M01] * matb[M13] + mata[M02] * matb[M23] + mata[M03] * matb[M33];
		tmp[M10] = mata[M10] * matb[M00] + mata[M11] * matb[M10] + mata[M12] * matb[M20] + mata[M13] * matb[M30];
		tmp[M11] = mata[M10] * matb[M01] + mata[M11] * matb[M11] + mata[M12] * matb[M21] + mata[M13] * matb[M31];
		tmp[M12] = mata[M10] * matb[M02] + mata[M11] * matb[M12] + mata[M12] * matb[M22] + mata[M13] * matb[M32];
		tmp[M13] = mata[M10] * matb[M03] + mata[M11] * matb[M13] + mata[M12] * matb[M23] + mata[M13] * matb[M33];
		tmp[M20] = mata[M20] * matb[M00] + mata[M21] * matb[M10] + mata[M22] * matb[M20] + mata[M23] * matb[M30];
		tmp[M21] = mata[M20] * matb[M01] + mata[M21] * matb[M11] + mata[M22] * matb[M21] + mata[M23] * matb[M31];
		tmp[M22] = mata[M20] * matb[M02] + mata[M21] * matb[M12] + mata[M22] * matb[M22] + mata[M23] * matb[M32];
		tmp[M23] = mata[M20] * matb[M03] + mata[M21] * matb[M13] + mata[M22] * matb[M23] + mata[M23] * matb[M33];
		tmp[M30] = mata[M30] * matb[M00] + mata[M31] * matb[M10] + mata[M32] * matb[M20] + mata[M33] * matb[M30];
		tmp[M31] = mata[M30] * matb[M01] + mata[M31] * matb[M11] + mata[M32] * matb[M21] + mata[M33] * matb[M31];
		tmp[M32] = mata[M30] * matb[M02] + mata[M31] * matb[M12] + mata[M32] * matb[M22] + mata[M33] * matb[M32];
		tmp[M33] = mata[M30] * matb[M03] + mata[M31] * matb[M13] + mata[M32] * matb[M23] + mata[M33] * matb[M33];
		mata = tmp;
	}
	
	static inline float det(std::vector<float>& val) {
		return val[M30] * val[M21] * val[M12] * val[M03] - val[M20] * val[M31] * val[M12] * val[M03] - val[M30] * val[M11]
				* val[M22] * val[M03] + val[M10] * val[M31] * val[M22] * val[M03] + val[M20] * val[M11] * val[M32] * val[M03] - val[M10]
				* val[M21] * val[M32] * val[M03] - val[M30] * val[M21] * val[M02] * val[M13] + val[M20] * val[M31] * val[M02] * val[M13]
				+ val[M30] * val[M01] * val[M22] * val[M13] - val[M00] * val[M31] * val[M22] * val[M13] - val[M20] * val[M01] * val[M32]
				* val[M13] + val[M00] * val[M21] * val[M32] * val[M13] + val[M30] * val[M11] * val[M02] * val[M23] - val[M10] * val[M31]
				* val[M02] * val[M23] - val[M30] * val[M01] * val[M12] * val[M23] + val[M00] * val[M31] * val[M12] * val[M23] + val[M10]
				* val[M01] * val[M32] * val[M23] - val[M00] * val[M11] * val[M32] * val[M23] - val[M20] * val[M11] * val[M02] * val[M33]
				+ val[M10] * val[M21] * val[M02] * val[M33] + val[M20] * val[M01] * val[M12] * val[M33] - val[M00] * val[M21] * val[M12]
				* val[M33] - val[M10] * val[M01] * val[M22] * val[M33] + val[M00] * val[M11] * val[M22] * val[M33];
	}
	
	static inline bool inv(std::vector<float>& val) {
		std::vector<float> tmp(16);
		float l_det = det(val);
		if (l_det == 0) return false;
		tmp[M00] = val[M12] * val[M23] * val[M31] - val[M13] * val[M22] * val[M31] + val[M13] * val[M21] * val[M32] - val[M11]
			* val[M23] * val[M32] - val[M12] * val[M21] * val[M33] + val[M11] * val[M22] * val[M33];
		tmp[M01] = val[M03] * val[M22] * val[M31] - val[M02] * val[M23] * val[M31] - val[M03] * val[M21] * val[M32] + val[M01]
			* val[M23] * val[M32] + val[M02] * val[M21] * val[M33] - val[M01] * val[M22] * val[M33];
		tmp[M02] = val[M02] * val[M13] * val[M31] - val[M03] * val[M12] * val[M31] + val[M03] * val[M11] * val[M32] - val[M01]
			* val[M13] * val[M32] - val[M02] * val[M11] * val[M33] + val[M01] * val[M12] * val[M33];
		tmp[M03] = val[M03] * val[M12] * val[M21] - val[M02] * val[M13] * val[M21] - val[M03] * val[M11] * val[M22] + val[M01]
			* val[M13] * val[M22] + val[M02] * val[M11] * val[M23] - val[M01] * val[M12] * val[M23];
		tmp[M10] = val[M13] * val[M22] * val[M30] - val[M12] * val[M23] * val[M30] - val[M13] * val[M20] * val[M32] + val[M10]
			* val[M23] * val[M32] + val[M12] * val[M20] * val[M33] - val[M10] * val[M22] * val[M33];
		tmp[M11] = val[M02] * val[M23] * val[M30] - val[M03] * val[M22] * val[M30] + val[M03] * val[M20] * val[M32] - val[M00]
			* val[M23] * val[M32] - val[M02] * val[M20] * val[M33] + val[M00] * val[M22] * val[M33];
		tmp[M12] = val[M03] * val[M12] * val[M30] - val[M02] * val[M13] * val[M30] - val[M03] * val[M10] * val[M32] + val[M00]
			* val[M13] * val[M32] + val[M02] * val[M10] * val[M33] - val[M00] * val[M12] * val[M33];
		tmp[M13] = val[M02] * val[M13] * val[M20] - val[M03] * val[M12] * val[M20] + val[M03] * val[M10] * val[M22] - val[M00]
			* val[M13] * val[M22] - val[M02] * val[M10] * val[M23] + val[M00] * val[M12] * val[M23];
		tmp[M20] = val[M11] * val[M23] * val[M30] - val[M13] * val[M21] * val[M30] + val[M13] * val[M20] * val[M31] - val[M10]
			* val[M23] * val[M31] - val[M11] * val[M20] * val[M33] + val[M10] * val[M21] * val[M33];
		tmp[M21] = val[M03] * val[M21] * val[M30] - val[M01] * val[M23] * val[M30] - val[M03] * val[M20] * val[M31] + val[M00]
			* val[M23] * val[M31] + val[M01] * val[M20] * val[M33] - val[M00] * val[M21] * val[M33];
		tmp[M22] = val[M01] * val[M13] * val[M30] - val[M03] * val[M11] * val[M30] + val[M03] * val[M10] * val[M31] - val[M00]
			* val[M13] * val[M31] - val[M01] * val[M10] * val[M33] + val[M00] * val[M11] * val[M33];
		tmp[M23] = val[M03] * val[M11] * val[M20] - val[M01] * val[M13] * val[M20] - val[M03] * val[M10] * val[M21] + val[M00]
			* val[M13] * val[M21] + val[M01] * val[M10] * val[M23] - val[M00] * val[M11] * val[M23];
		tmp[M30] = val[M12] * val[M21] * val[M30] - val[M11] * val[M22] * val[M30] - val[M12] * val[M20] * val[M31] + val[M10]
			* val[M22] * val[M31] + val[M11] * val[M20] * val[M32] - val[M10] * val[M21] * val[M32];
		tmp[M31] = val[M01] * val[M22] * val[M30] - val[M02] * val[M21] * val[M30] + val[M02] * val[M20] * val[M31] - val[M00]
			* val[M22] * val[M31] - val[M01] * val[M20] * val[M32] + val[M00] * val[M21] * val[M32];
		tmp[M32] = val[M02] * val[M11] * val[M30] - val[M01] * val[M12] * val[M30] - val[M02] * val[M10] * val[M31] + val[M00]
			* val[M12] * val[M31] + val[M01] * val[M10] * val[M32] - val[M00] * val[M11] * val[M32];
		tmp[M33] = val[M01] * val[M12] * val[M20] - val[M02] * val[M11] * val[M20] + val[M02] * val[M10] * val[M21] - val[M00]
			* val[M12] * val[M21] - val[M01] * val[M10] * val[M22] + val[M00] * val[M11] * val[M22];

		float inv_det = 1.0f / l_det;
		val[M00] = tmp[M00] * inv_det;
		val[M01] = tmp[M01] * inv_det;
		val[M02] = tmp[M02] * inv_det;
		val[M03] = tmp[M03] * inv_det;
		val[M10] = tmp[M10] * inv_det;
		val[M11] = tmp[M11] * inv_det;
		val[M12] = tmp[M12] * inv_det;
		val[M13] = tmp[M13] * inv_det;
		val[M20] = tmp[M20] * inv_det;
		val[M21] = tmp[M21] * inv_det;
		val[M22] = tmp[M22] * inv_det;
		val[M23] = tmp[M23] * inv_det;
		val[M30] = tmp[M30] * inv_det;
		val[M31] = tmp[M31] * inv_det;
		val[M32] = tmp[M32] * inv_det;
		val[M33] = tmp[M33] * inv_det;
		return true;
	}

	static inline void mulVec(const std::vector<float>& mat, std::vector<float>& vec) {
		float x = vec[0] * mat[M00] + vec[1] * mat[M01] + vec[2] * mat[M02] + mat[M03];
		float y = vec[0] * mat[M10] + vec[1] * mat[M11] + vec[2] * mat[M12] + mat[M13];
		float z = vec[0] * mat[M20] + vec[1] * mat[M21] + vec[2] * mat[M22] + mat[M23];
		vec[0] = x;
		vec[1] = y;
		vec[2] = z;
	}
	
	static inline void proj(const std::vector<float>& mat, std::vector<float>& vec) {
		float inv_w = 1.0f / (vec[0] * mat[M30] + vec[1] * mat[M31] + vec[2] * mat[M32] + mat[M33]);
		float x = (vec[0] * mat[M00] + vec[1] * mat[M01] + vec[2] * mat[M02] + mat[M03]) * inv_w;
		float y = (vec[0] * mat[M10] + vec[1] * mat[M11] + vec[2] * mat[M12] + mat[M13]) * inv_w; 
		float z = (vec[0] * mat[M20] + vec[1] * mat[M21] + vec[2] * mat[M22] + mat[M23]) * inv_w;
		vec[0] = x;
		vec[1] = y;
		vec[2] = z;
	}
	
	static inline void rot(const std::vector<float>& mat, std::vector<float>& vec) {
		float x = vec[0] * mat[M00] + vec[1] * mat[M01] + vec[2] * mat[M02];
		float y = vec[0] * mat[M10] + vec[1] * mat[M11] + vec[2] * mat[M12];
		float z = vec[0] * mat[M20] + vec[1] * mat[M21] + vec[2] * mat[M22];
		vec[0] = x;
		vec[1] = y;
		vec[2] = z;
	}
};