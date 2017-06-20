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
#include "Vector2.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "../Serializable.h"

class Matrix3;
class Matrix4;
class Vector2;

/** A specialized 3x3 matrix that can represent sequences of 2D translations, scales, flips, rotations, and shears. <a
 * href="http://en.wikipedia.org/wiki/Affine_transformation">Affine transformations</a> preserve straight lines, and
 * parallel lines remain parallel after the transformation. Operations on affine matrices are faster because the last row can
 * always be assumed (0, 0, 1).
 *
 * @author vmilea */
 class Affine2:public Serializable {
     public:
	static const long serialVersionUID = 1524569123485049187L;

	 float m00 = 1, m01 = 0, m02 = 0;
	 float m10 = 0, m11 = 1, m12 = 0;

	// constant: m21 = 0, m21 = 1, m22 = 1

	/** Constructs an identity matrix. */
	 Affine2 () {
	}

	/** Constructs a matrix from the given affine matrix.
	 *
	 * @param other The affine matrix to copy. This matrix will not be modified. */
	 Affine2 (const Affine2& other) {
		set(other);
	}

	/** Sets this matrix to the identity matrix
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& idt () {
		m00 = 1;
		m01 = 0;
		m02 = 0;
		m10 = 0;
		m11 = 1;
		m12 = 0;
		return *this;
	}

	/** Copies the values from the provided affine matrix to this matrix.
	 * @param other The affine matrix to copy.
	 * @return This matrix for the purposes of chaining. */
	 Affine2& set (const Affine2& other) {
		m00 = other.m00;
		m01 = other.m01;
		m02 = other.m02;
		m10 = other.m10;
		m11 = other.m11;
		m12 = other.m12;
		return *this;
	}

	/** Copies the values from the provided matrix to this matrix.
	 * @param matrix The matrix to copy, assumed to be an affine transformation.
	 * @return This matrix for the purposes of chaining. */
	 Affine2& set (const Matrix3& matrix);

	/** Copies the 2D transformation components from the provided 4x4 matrix. The values are mapped as follows:
	 *
	 * <pre>
	 *      [  M00  M01  M03  ]
	 *      [  M10  M11  M13  ]
	 *      [   0    0    1   ]
	 * </pre>
	 * @param matrix The source matrix, assumed to be an affine transformation within XY plane. This matrix will not be modified.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& set (const Matrix4& matrix);

	/** Sets this matrix to a translation matrix.
	 * @param x The translation in x
	 * @param y The translation in y
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToTranslation (float x, float y) {
		m00 = 1;
		m01 = 0;
		m02 = x;
		m10 = 0;
		m11 = 1;
		m12 = y;
		return *this;
	}

	/** Sets this matrix to a translation matrix.
	 * @param trn The translation vector.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToTranslation (const Vector2& trn);

	/** Sets this matrix to a scaling matrix.
	 * @param scaleX The scale in x.
	 * @param scaleY The scale in y.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToScaling (float scaleX, float scaleY) {
		m00 = scaleX;
		m01 = 0;
		m02 = 0;
		m10 = 0;
		m11 = scaleY;
		m12 = 0;
		return *this;
	}

	/** Sets this matrix to a scaling matrix.
	 * @param scale The scale vector.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToScaling (const Vector2& scale);

	/** Sets this matrix to a rotation matrix that will rotate any vector in counter-clockwise direction around the z-axis.
	 * @param degrees The angle in degrees.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToRotation (float degrees) {
		float cos = MathUtils::cosDeg(degrees);
		float sin = MathUtils::sinDeg(degrees);

		m00 = cos;
		m01 = -sin;
		m02 = 0;
		m10 = sin;
		m11 = cos;
		m12 = 0;
		return *this;
	}

	/** Sets this matrix to a rotation matrix that will rotate any vector in counter-clockwise direction around the z-axis.
	 * @param radians The angle in radians.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToRotationRad (float radians) {
		float cosine = MathUtils::cos(radians);
		float sine = MathUtils::sin(radians);

		m00 = cosine;
		m01 = -sine;
		m02 = 0;
		m10 = sine;
		m11 = cosine;
		m12 = 0;
		return *this;
	}

	/** Sets this matrix to a rotation matrix that will rotate any vector in counter-clockwise direction around the z-axis.
	 * @param cos The angle cosine.
	 * @param sin The angle sine.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToRotation (float cos, float sin) {
		m00 = cos;
		m01 = -sin;
		m02 = 0;
		m10 = sin;
		m11 = cos;
		m12 = 0;
		return *this;
	}

	/** Sets this matrix to a shearing matrix.
	 * @param shearX The shear in x direction.
	 * @param shearY The shear in y direction.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToShearing (float shearX, float shearY) {
		m00 = 1;
		m01 = shearX;
		m02 = 0;
		m10 = shearY;
		m11 = 1;
		m12 = 0;
		return *this;
	}

	/** Sets this matrix to a shearing matrix.
	 * @param shear The shear vector.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToShearing (const Vector2& shear);

	/** Sets this matrix to a concatenation of translation, rotation and scale. It is a more efficient form for:
	 * <code>idt().translate(x, y).rotate(degrees).scale(scaleX, scaleY)</code>
	 * @param x The translation in x.
	 * @param y The translation in y.
	 * @param degrees The angle in degrees.
	 * @param scaleX The scale in y.
	 * @param scaleY The scale in x.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToTrnRotScl (float x, float y, float degrees, float scaleX, float scaleY) {
		m02 = x;
		m12 = y;

		if (degrees == 0) {
			m00 = scaleX;
			m01 = 0;
			m10 = 0;
			m11 = scaleY;
		} else {
			float sin = MathUtils::sinDeg(degrees);
			float cos = MathUtils::cosDeg(degrees);

			m00 = cos * scaleX;
			m01 = -sin * scaleY;
			m10 = sin * scaleX;
			m11 = cos * scaleY;
		}
		return *this;
	}

	/** Sets this matrix to a concatenation of translation, rotation and scale. It is a more efficient form for:
	 * <code>idt().translate(trn).rotate(degrees).scale(scale)</code>
	 * @param trn The translation vector.
	 * @param degrees The angle in degrees.
	 * @param scale The scale vector.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToTrnRotScl (const Vector2& trn, float degrees, const Vector2& scale);

	/** Sets this matrix to a concatenation of translation, rotation and scale. It is a more efficient form for:
	 * <code>idt().translate(x, y).rotateRad(radians).scale(scaleX, scaleY)</code>
	 * @param x The translation in x.
	 * @param y The translation in y.
	 * @param radians The angle in radians.
	 * @param scaleX The scale in y.
	 * @param scaleY The scale in x.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToTrnRotRadScl (float x, float y, float radians, float scaleX, float scaleY) {
		m02 = x;
		m12 = y;

		if (radians == 0) {
			m00 = scaleX;
			m01 = 0;
			m10 = 0;
			m11 = scaleY;
		} else {
			float sin = MathUtils::sin(radians);
			float cos = MathUtils::cos(radians);

			m00 = cos * scaleX;
			m01 = -sin * scaleY;
			m10 = sin * scaleX;
			m11 = cos * scaleY;
		}
		return *this;
	}

	/** Sets this matrix to a concatenation of translation, rotation and scale. It is a more efficient form for:
	 * <code>idt().translate(trn).rotateRad(radians).scale(scale)</code>
	 * @param trn The translation vector.
	 * @param radians The angle in radians.
	 * @param scale The scale vector.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToTrnRotRadScl (const Vector2& trn, float radians, const Vector2& scale);

	/** Sets this matrix to a concatenation of translation and scale. It is a more efficient form for:
	 * <code>idt().translate(x, y).scale(scaleX, scaleY)</code>
	 * @param x The translation in x.
	 * @param y The translation in y.
	 * @param scaleX The scale in y.
	 * @param scaleY The scale in x.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToTrnScl (float x, float y, float scaleX, float scaleY) {
		m00 = scaleX;
		m01 = 0;
		m02 = x;
		m10 = 0;
		m11 = scaleY;
		m12 = y;
		return *this;
	}

	/** Sets this matrix to a concatenation of translation and scale. It is a more efficient form for:
	 * <code>idt().translate(trn).scale(scale)</code>
	 * @param trn The translation vector.
	 * @param scale The scale vector.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToTrnScl (const Vector2& trn, const Vector2& scale);

	/** Sets this matrix to the product of two matrices.
	 * @param l Left matrix.
	 * @param r Right matrix.
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& setToProduct (const Affine2& l, const Affine2& r) {
		m00 = l.m00 * r.m00 + l.m01 * r.m10;
		m01 = l.m00 * r.m01 + l.m01 * r.m11;
		m02 = l.m00 * r.m02 + l.m01 * r.m12 + l.m02;
		m10 = l.m10 * r.m00 + l.m11 * r.m10;
		m11 = l.m10 * r.m01 + l.m11 * r.m11;
		m12 = l.m10 * r.m02 + l.m11 * r.m12 + l.m12;
		return *this;
	}

	/** Inverts this matrix given that the determinant is != 0.
	 * @return This matrix for the purpose of chaining operations.
	 * @throws GdxRuntimeException if the matrix is singular (not invertible) */
	 Affine2& inv () {
		float determinant = det();
		if (determinant == 0) throw "GdxRuntimeException: Can't invert a singular affine matrix";

		float invDet = 1.0f / determinant;

		float tmp00 = m11;
		float tmp01 = -m01;
		float tmp02 = m01 * m12 - m11 * m02;
		float tmp10 = -m10;
		float tmp11 = m00;
		float tmp12 = m10 * m02 - m00 * m12;

		m00 = invDet * tmp00;
		m01 = invDet * tmp01;
		m02 = invDet * tmp02;
		m10 = invDet * tmp10;
		m11 = invDet * tmp11;
		m12 = invDet * tmp12;
		return *this;
	}

	/** Postmultiplies this matrix with the provided matrix and stores the result in this matrix. For example:
	 *
	 * <pre>
	 * A.mul(B) results in A := AB
	 * </pre>
	 * @param other Matrix to multiply by.
	 * @return This matrix for the purpose of chaining operations together. */
	 Affine2& mul (const Affine2& other) {
		float tmp00 = m00 * other.m00 + m01 * other.m10;
		float tmp01 = m00 * other.m01 + m01 * other.m11;
		float tmp02 = m00 * other.m02 + m01 * other.m12 + m02;
		float tmp10 = m10 * other.m00 + m11 * other.m10;
		float tmp11 = m10 * other.m01 + m11 * other.m11;
		float tmp12 = m10 * other.m02 + m11 * other.m12 + m12;

		m00 = tmp00;
		m01 = tmp01;
		m02 = tmp02;
		m10 = tmp10;
		m11 = tmp11;
		m12 = tmp12;
		return *this;
	}

	/** Premultiplies this matrix with the provided matrix and stores the result in this matrix. For example:
	 *
	 * <pre>
	 * A.preMul(B) results in A := BA
	 * </pre>
	 * @param other The other Matrix to multiply by
	 * @return This matrix for the purpose of chaining operations. */
	 Affine2& preMul (const Affine2& other) {
		float tmp00 = other.m00 * m00 + other.m01 * m10;
		float tmp01 = other.m00 * m01 + other.m01 * m11;
		float tmp02 = other.m00 * m02 + other.m01 * m12 + other.m02;
		float tmp10 = other.m10 * m00 + other.m11 * m10;
		float tmp11 = other.m10 * m01 + other.m11 * m11;
		float tmp12 = other.m10 * m02 + other.m11 * m12 + other.m12;

		m00 = tmp00;
		m01 = tmp01;
		m02 = tmp02;
		m10 = tmp10;
		m11 = tmp11;
		m12 = tmp12;
		return *this;
	}

	/** Postmultiplies this matrix by a translation matrix.
	 * @param x The x-component of the translation vector.
	 * @param y The y-component of the translation vector.
	 * @return This matrix for the purpose of chaining. */
	 Affine2& translate (float x, float y) {
		m02 += m00 * x + m01 * y;
		m12 += m10 * x + m11 * y;
		return *this;
	}

	/** Postmultiplies this matrix by a translation matrix.
	 * @param trn The translation vector.
	 * @return This matrix for the purpose of chaining. */
	 Affine2& translate (const Vector2& trn);

	/** Premultiplies this matrix by a translation matrix.
	 * @param x The x-component of the translation vector.
	 * @param y The y-component of the translation vector.
	 * @return This matrix for the purpose of chaining. */
	 Affine2& preTranslate (float x, float y) {
		m02 += x;
		m12 += y;
		return *this;
	}

	/** Premultiplies this matrix by a translation matrix.
	 * @param trn The translation vector.
	 * @return This matrix for the purpose of chaining. */
	 Affine2& preTranslate (const Vector2& trn);

	/** Postmultiplies this matrix with a scale matrix.
	 * @param scaleX The scale in the x-axis.
	 * @param scaleY The scale in the y-axis.
	 * @return This matrix for the purpose of chaining. */
	 Affine2& scale (float scaleX, float scaleY) {
		m00 *= scaleX;
		m01 *= scaleY;
		m10 *= scaleX;
		m11 *= scaleY;
		return *this;
	}

	/** Postmultiplies this matrix with a scale matrix.
	 * @param scale The scale vector.
	 * @return This matrix for the purpose of chaining. */
	 Affine2& scale (const Vector2& scale);

	/** Premultiplies this matrix with a scale matrix.
	 * @param scaleX The scale in the x-axis.
	 * @param scaleY The scale in the y-axis.
	 * @return This matrix for the purpose of chaining. */
	 Affine2& preScale (float scaleX, float scaleY) {
		m00 *= scaleX;
		m01 *= scaleX;
		m02 *= scaleX;
		m10 *= scaleY;
		m11 *= scaleY;
		m12 *= scaleY;
		return *this;
	}

	/** Premultiplies this matrix with a scale matrix.
	 * @param scale The scale vector.
	 * @return This matrix for the purpose of chaining. */
	 Affine2& preScale (const Vector2& scale);

	/** Postmultiplies this matrix with a (counter-clockwise) rotation matrix.
	 * @param degrees The angle in degrees
	 * @return This matrix for the purpose of chaining. */
	 Affine2& rotate (float degrees) {
		if (degrees == 0) return *this;

		float cos = MathUtils::cosDeg(degrees);
		float sin = MathUtils::sinDeg(degrees);

		float tmp00 = m00 * cos + m01 * sin;
		float tmp01 = m00 * -sin + m01 * cos;
		float tmp10 = m10 * cos + m11 * sin;
		float tmp11 = m10 * -sin + m11 * cos;

		m00 = tmp00;
		m01 = tmp01;
		m10 = tmp10;
		m11 = tmp11;
		return *this;
	}

	/** Postmultiplies this matrix with a (counter-clockwise) rotation matrix.
	 * @param radians The angle in radians
	 * @return This matrix for the purpose of chaining. */
	 Affine2& rotateRad (float radians) {
		if (radians == 0) return *this;

		float cos = MathUtils::cos(radians);
		float sin = MathUtils::sin(radians);

		float tmp00 = m00 * cos + m01 * sin;
		float tmp01 = m00 * -sin + m01 * cos;
		float tmp10 = m10 * cos + m11 * sin;
		float tmp11 = m10 * -sin + m11 * cos;

		m00 = tmp00;
		m01 = tmp01;
		m10 = tmp10;
		m11 = tmp11;
		return *this;
	}

	/** Premultiplies this matrix with a (counter-clockwise) rotation matrix.
	 * @param degrees The angle in degrees
	 * @return This matrix for the purpose of chaining. */
	 Affine2& preRotate (float degrees) {
		if (degrees == 0) return *this;

		float cos = MathUtils::cosDeg(degrees);
		float sin = MathUtils::sinDeg(degrees);

		float tmp00 = cos * m00 - sin * m10;
		float tmp01 = cos * m01 - sin * m11;
		float tmp02 = cos * m02 - sin * m12;
		float tmp10 = sin * m00 + cos * m10;
		float tmp11 = sin * m01 + cos * m11;
		float tmp12 = sin * m02 + cos * m12;

		m00 = tmp00;
		m01 = tmp01;
		m02 = tmp02;
		m10 = tmp10;
		m11 = tmp11;
		m12 = tmp12;
		return *this;
	}

	/** Premultiplies this matrix with a (counter-clockwise) rotation matrix.
	 * @param radians The angle in radians
	 * @return This matrix for the purpose of chaining. */
	 Affine2& preRotateRad (float radians) {
		if (radians == 0) return *this;

		float cos = MathUtils::cos(radians);
		float sin = MathUtils::sin(radians);

		float tmp00 = cos * m00 - sin * m10;
		float tmp01 = cos * m01 - sin * m11;
		float tmp02 = cos * m02 - sin * m12;
		float tmp10 = sin * m00 + cos * m10;
		float tmp11 = sin * m01 + cos * m11;
		float tmp12 = sin * m02 + cos * m12;

		m00 = tmp00;
		m01 = tmp01;
		m02 = tmp02;
		m10 = tmp10;
		m11 = tmp11;
		m12 = tmp12;
		return *this;
	}

	/** Postmultiplies this matrix by a shear matrix.
	 * @param shearX The shear in x direction.
	 * @param shearY The shear in y direction.
	 * @return This matrix for the purpose of chaining. */
	 Affine2& shear (float shearX, float shearY) {
		float tmp0 = m00 + shearY * m01;
		float tmp1 = m01 + shearX * m00;
		m00 = tmp0;
		m01 = tmp1;

		tmp0 = m10 + shearY * m11;
		tmp1 = m11 + shearX * m10;
		m10 = tmp0;
		m11 = tmp1;
		return *this;
	}

	/** Postmultiplies this matrix by a shear matrix.
	 * @param shear The shear vector.
	 * @return This matrix for the purpose of chaining. */
	 Affine2& shear (const Vector2& shear);

	/** Premultiplies this matrix by a shear matrix.
	 * @param shearX The shear in x direction.
	 * @param shearY The shear in y direction.
	 * @return This matrix for the purpose of chaining. */
	 Affine2& preShear (float shearX, float shearY) {
		float tmp00 = m00 + shearX * m10;
		float tmp01 = m01 + shearX * m11;
		float tmp02 = m02 + shearX * m12;
		float tmp10 = m10 + shearY * m00;
		float tmp11 = m11 + shearY * m01;
		float tmp12 = m12 + shearY * m02;

		m00 = tmp00;
		m01 = tmp01;
		m02 = tmp02;
		m10 = tmp10;
		m11 = tmp11;
		m12 = tmp12;
		return *this;
	}

	/** Premultiplies this matrix by a shear matrix.
	 * @param shear The shear vector.
	 * @return This matrix for the purpose of chaining. */
	 Affine2& preShear (const Vector2& shear);

	/** Calculates the determinant of the matrix.
	 * @return The determinant of this matrix. */
	 float det () {
		return m00 * m11 - m01 * m10;
	}

	/** Get the x-y translation component of the matrix.
	 * @param position Output vector.
	 * @return Filled position. */
	 Vector2& getTranslation (const Vector2& position);

	/** Check if the this is a plain translation matrix.
	 * @return True if scale is 1 and rotation is 0. */
	 bool isTranslation () {
		return (m00 == 1 && m11 == 1 && m01 == 0 && m10 == 0);
	}

	/** Check if this is an indentity matrix.
	 * @return True if scale is 1 and rotation is 0. */
	 bool isIdt () {
		return (m00 == 1 && m02 == 0 && m12 == 0 && m11 == 1 && m01 == 0 && m10 == 0);
	}

	/** Applies the affine transformation on a vector. */
	 void applyTo (const Vector2& point);

	
	 std::string toString () {
        std::stringstream ss;
		ss<<"[" << m00 << "|" << m01 << "|" << m02 << "]\n[" << m10 << "|" << m11 << "|" << m12 << "]\n[0.0|0.0|0.1]";
        return ss.str();
	}
};

    