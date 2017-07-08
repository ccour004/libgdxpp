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
#include "Interpolation.h"

/** Encapsulates a general vector. Allows chaining operations by returning a reference to itself in all modification methods. See
 * {@link Vector2} and {@link Vector3} for specific implementations.
 * @author Xoppa */
template <class T>
class Vector {
	/** @return a copy of this vector */
	virtual T cpy () = 0;

	/** @return The euclidean length */
	virtual float len () = 0;

	/** This method is faster than {@link Vector#len()} because it avoids calculating a square root. It is useful for comparisons,
	 * but not for getting exact lengths, as the return value is the square of the actual length.
	 * @return The squared euclidean length */
	virtual float len2 () = 0;

	/** Limits the length of this vector, based on the desired maximum length.
	 * @param limit desired maximum length for this vector
	 * @return this vector for chaining */
	virtual T& limit (float limit) = 0;

	/** Limits the length of this vector, based on the desired maximum length squared.
	 * <p />
	 * This method is slightly faster than limit().
	 * @param limit2 squared desired maximum length for this vector
	 * @return this vector for chaining
	 * @see #len2() */
	virtual T& limit2 (float limit2) = 0;

	/** Sets the length of this vector. Does nothing is this vector is zero.
	 * @param len desired length for this vector
	 * @return this vector for chaining */
	virtual T& setLength (float len) = 0;

	/** Sets the length of this vector, based on the square of the desired length. Does nothing is this vector is zero.
	 * <p />
	 * This method is slightly faster than setLength().
	 * @param len2 desired square of the length for this vector
	 * @return this vector for chaining
	 * @see #len2() */
	virtual T& setLength2 (float len2) = 0;

	/** Clamps this vector's length to given min and max values
	 * @param min Min length
	 * @param max Max length
	 * @return This vector for chaining */
	virtual T& clamp (float min, float max) = 0;

	/** Sets this vector from the given vector
	 * @param v The vector
	 * @return This vector for chaining */
	virtual T& set (const T& v) = 0;

	/** Subtracts the given vector from this vector.
	 * @param v The vector
	 * @return This vector for chaining */
	virtual T& sub (const T& v) = 0;

	/** Normalizes this vector. Does nothing if it is zero.
	 * @return This vector for chaining */
	virtual T& nor () = 0;

	/** Adds the given vector to this vector
	 * @param v The vector
	 * @return This vector for chaining */
	virtual T& add (const T& v) = 0;

	/** @param v The other vector
	 * @return The dot product between this and the other vector */
	virtual float dot (const T& v) = 0;

	/** Scales this vector by a scalar
	 * @param scalar The scalar
	 * @return This vector for chaining */
	virtual T& scl (float scalar) = 0;

	/** Scales this vector by another vector
	 * @return This vector for chaining */
	virtual T& scl (const T& v) = 0;

	/** @param v The other vector
	 * @return the distance between this and the other vector */
	virtual float dst (const T& v) = 0;

	/** This method is faster than {@link Vector#dst(Vector)} because it avoids calculating a square root. It is useful for
	 * comparisons, but not for getting accurate distances, as the return value is the square of the actual distance.
	 * @param v The other vector
	 * @return the squared distance between this and the other vector */
	virtual float dst2 (const T& v) = 0;

	/** Linearly interpolates between this vector and the target vector by alpha which is in the range [0,1]. The result is stored
	 * in this vector.
	 * @param target The target vector
	 * @param alpha The interpolation coefficient
	 * @return This vector for chaining. */
	virtual T& lerp (const T& target, float alpha) = 0;

	/** Interpolates between this vector and the given target vector by alpha (within range [0,1]) using the given Interpolation
	 * method. the result is stored in this vector.
	 * @param target The target vector
	 * @param alpha The interpolation coefficient
	 * @param interpolator An Interpolation object describing the used interpolation method
	 * @return This vector for chaining. */
	virtual T& interpolate (const T& target, float alpha, Interpolation& interpolator) = 0;

	/** Sets this vector to the unit vector with a random direction
	 * @return This vector for chaining */
	virtual T& setToRandomDirection () = 0;

	/** @return Whether this vector is a unit length vector */
	virtual bool isUnit () = 0;

	/** @return Whether this vector is a unit length vector within the given margin. */
	virtual bool isUnit (const float margin) = 0;

	/** @return Whether this vector is a zero vector */
	virtual bool isZero () = 0;

	/** @return Whether the length of this vector is smaller than the given margin */
	virtual bool isZero (const float margin) = 0;

	/** @return true if this vector is in line with the other vector (either in the same or the opposite direction) */
	virtual bool isOnLine (const T& other, float epsilon) = 0;

	/** @return true if this vector is in line with the other vector (either in the same or the opposite direction) */
	virtual bool isOnLine (const T& other) = 0;

	/** @return true if this vector is collinear with the other vector ({@link #isOnLine(Vector, float)} &&
	 *         {@link #hasSameDirection(Vector)}). */
	virtual bool isCollinear (const T& other, float epsilon) = 0;

	/** @return true if this vector is collinear with the other vector ({@link #isOnLine(Vector)} &&
	 *         {@link #hasSameDirection(Vector)}). */
	virtual bool isCollinear (const T& other) = 0;

	/** @return true if this vector is opposite collinear with the other vector ({@link #isOnLine(Vector, float)} &&
	 *         {@link #hasOppositeDirection(Vector)}). */
	virtual bool isCollinearOpposite (const T& other, float epsilon) = 0;

	/** @return true if this vector is opposite collinear with the other vector ({@link #isOnLine(Vector)} &&
	 *         {@link #hasOppositeDirection(Vector)}). */
	virtual bool isCollinearOpposite (const T& other) = 0;

	/** @return Whether this vector is perpendicular with the other vector. True if the dot product is 0. */
	virtual bool isPerpendicular (const T& other) = 0;

	/** @return Whether this vector is perpendicular with the other vector. True if the dot product is 0.
	 * @param epsilon a positive small number close to zero */
	virtual bool isPerpendicular (const T& other, float epsilon) = 0;

	/** @return Whether this vector has similar direction compared to the other vector. True if the normalized dot product is > 0. */
	virtual bool hasSameDirection (const T& other) = 0;

	/** @return Whether this vector has opposite direction compared to the other vector. True if the normalized dot product is < 0. */
	virtual bool hasOppositeDirection (const T& other) = 0;

	/** Compares this vector with the other vector, using the supplied epsilon for fuzzy equality testing.
	 * @param other
	 * @param epsilon
	 * @return whether the vectors have fuzzy equality. */
	virtual bool epsilonEquals (const T& other, float epsilon) = 0;

	/** First scale a supplied vector, then add it to this vector.
	 * @param v addition vector
	 * @param scalar for scaling the addition vector */
	virtual T& mulAdd (const T& v, float scalar) = 0;

	/** First scale a supplied vector, then add it to this vector.
	 * @param v addition vector
	 * @param mulVec vector by whose values the addition vector will be scaled */
	virtual T& mulAdd (const T& v, const T& mulVec) = 0;

	/** Sets the components of this vector to 0
	 * @return This vector for chaining */
	virtual T& setZero () = 0;
};

