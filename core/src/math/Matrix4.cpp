#include "Matrix4.h"

    static std::vector<float> Matrix4::tmp(16);

	Matrix4& Matrix4::set (const Quaternion& quaternion) {
		return set(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	}
    
	Matrix4& Matrix4::set (const Vector3& position, const Quaternion& orientation) {
		return set(position.x, position.y, position.z, orientation.x, orientation.y, orientation.z, orientation.w);
	}
    
	Matrix4& Matrix4::set (const Vector3& position, const Quaternion& orientation, const Vector3& scale) {
		return set(position.x, position.y, position.z, orientation.x, orientation.y, orientation.z, orientation.w, scale.x,
			scale.y, scale.z);
	}
    
	Matrix4& Matrix4::set (const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis, const Vector3& pos) {
		val[M00] = xAxis.x;
		val[M01] = xAxis.y;
		val[M02] = xAxis.z;
		val[M10] = yAxis.x;
		val[M11] = yAxis.y;
		val[M12] = yAxis.z;
		val[M20] = zAxis.x;
		val[M21] = zAxis.y;
		val[M22] = zAxis.z;
		val[M03] = pos.x;
		val[M13] = pos.y;
		val[M23] = pos.z;
		val[M30] = 0;
		val[M31] = 0;
		val[M32] = 0;
		val[M33] = 1;
		return this;
	}

	Matrix4& Matrix4::trn (const Vector3& vector) {
		val[M03] += vector.x;
		val[M13] += vector.y;
		val[M23] += vector.z;
		return this;
	}
    
    Matrix4& Matrix4::setTranslation (const Vector3& vector) {
		val[M03] = vector.x;
		val[M13] = vector.y;
		val[M23] = vector.z;
		return this;
	}

	Matrix4 Matrix4::setToTranslation (const Vector3& vector) {
		idt();
		val[M03] = vector.x;
		val[M13] = vector.y;
		val[M23] = vector.z;
		return this;
	}
    
	Matrix4& Matrix4::setToTranslationAndScaling (const Vector3& translation, const Vector3& scaling) {
		idt();
		val[M03] = translation.x;
		val[M13] = translation.y;
		val[M23] = translation.z;
		val[M00] = scaling.x;
		val[M11] = scaling.y;
		val[M22] = scaling.z;
		return this;
	}
    
	Matrix4& Matrix4::setToRotation (const Vector3& axis, float degrees) {
		if (degrees == 0) {
			idt();
			return this;
		}
		return set(quat.set(axis, degrees));
	}
    
	Matrix4& Matrix4::setToRotationRad (const Vector3& axis, float radians) {
		if (radians == 0) {
			idt();
			return this;
		}
		return set(quat.setFromAxisRad(axis, radians));
	}
    
	Matrix4& Matrix4::setToRotation (float axisX, float axisY, float axisZ, float degrees) {
		if (degrees == 0) {
			idt();
			return this;
		}
		return set(quat.setFromAxis(axisX, axisY, axisZ, degrees));
	}
    
	Matrix4& Matrix4::setToRotationRad (float axisX, float axisY, float axisZ, float radians) {
		if (radians == 0) {
			idt();
			return this;
		}
		return set(quat.setFromAxisRad(axisX, axisY, axisZ, radians));
	}
    
	Matrix4& Matrix4::setToRotation (const Vector3& v1, const Vector3& v2) {
		return set(quat.setFromCross(v1, v2));
	}
    
	Matrix4& Matrix4::setToRotation (float x1,float y1, float z1, float x2, float y2, float z2) {
		return set(quat.setFromCross(x1, y1, z1, x2, y2, z2));
	}
    
	Matrix4& Matrix4::setFromEulerAngles (float yaw, float pitch, float roll) {
		quat.setEulerAngles(yaw, pitch, roll);
		return set(quat);
	}
    
	Matrix4& Matrix4::setFromEulerAnglesRad (float yaw, float pitch, float roll) {
		quat.setEulerAnglesRad(yaw, pitch, roll);
		return set(quat);
	}
    
	Matrix4& Matrix4::setToScaling (const Vector3& vector) {
		idt();
		val[M00] = vector.x;
		val[M11] = vector.y;
		val[M22] = vector.z;
		return this;
	}
    
	Matrix4& Matrix4::setToLookAt (const Vector3& direction, const Vector3& up) {
		l_vez.set(direction).nor();
		l_vex.set(direction).nor();
		l_vex.crs(up).nor();
		l_vey.set(l_vex).crs(l_vez).nor();
		idt();
		val[M00] = l_vex.x;
		val[M01] = l_vex.y;
		val[M02] = l_vex.z;
		val[M10] = l_vey.x;
		val[M11] = l_vey.y;
		val[M12] = l_vey.z;
		val[M20] = -l_vez.x;
		val[M21] = -l_vez.y;
		val[M22] = -l_vez.z;

		return this;
	}
    
	Matrix4& Matrix4::setToLookAt (const Vector3& position, const Vector3& target, const Vector3& up) {
		tmpVec.set(target).sub(position);
		setToLookAt(tmpVec, up);
		this.mul(tmpMat.setToTranslation(-position.x, -position.y, -position.z));

		return this;
	}
    
	Matrix4& Matrix4::setToWorld (const Vector3& position, const Vector3& forward, const Vector3& up) {
		tmpForward.set(forward).nor();
		right.set(tmpForward).crs(up).nor();
		tmpUp.set(right).crs(tmpForward).nor();

		this.set(right, tmpUp, tmpForward.scl(-1), position);
		return this;
	}
    
	Matrix4& Matrix4::avg (const Matrix4& other, float w) {
		getScale(tmpVec);
		other.getScale(tmpForward);

		getRotation(quat);
		other.getRotation(quat2);

		getTranslation(tmpUp);
		other.getTranslation(right);

		setToScaling(tmpVec.scl(w).add(tmpForward.scl(1 - w)));
		rotate(quat.slerp(quat2, 1 - w));
		setTranslation(tmpUp.scl(w).add(right.scl(1 - w)));

		return this;
	}
    
	Matrix4& Matrix4::avg (const std::vector<Matrix4>& t) {
		const float w = 1.0f / t.size();

		tmpVec.set(t[0].getScale(tmpUp).scl(w));
		quat.set(t[0].getRotation(quat2).exp(w));
		tmpForward.set(t[0].getTranslation(tmpUp).scl(w));

		for (int i = 1; i < t.size(); i++) {
			tmpVec.add(t[i].getScale(tmpUp).scl(w));
			quat.mul(t[i].getRotation(quat2).exp(w));
			tmpForward.add(t[i].getTranslation(tmpUp).scl(w));
		}
		quat.nor();

		setToScaling(tmpVec);
		rotate(quat);
		setTranslation(tmpForward);

		return this;
	}
    
	Matrix4& Matrix4::avg (const std::vector<Matrix4>& t, std::vector<float> w) {
		tmpVec.set(t[0].getScale(tmpUp).scl(w[0]));
		quat.set(t[0].getRotation(quat2).exp(w[0]));
		tmpForward.set(t[0].getTranslation(tmpUp).scl(w[0]));

		for (int i = 1; i < t.length; i++) {
			tmpVec.add(t[i].getScale(tmpUp).scl(w[i]));
			quat.mul(t[i].getRotation(quat2).exp(w[i]));
			tmpForward.add(t[i].getTranslation(tmpUp).scl(w[i]));
		}
		quat.nor();

		setToScaling(tmpVec);
		rotate(quat);
		setTranslation(tmpForward);

		return this;
	}
    
	Matrix4& Matrix4::set (const Matrix3& mat) {
		val[0] = mat.val[0];
		val[1] = mat.val[1];
		val[2] = mat.val[2];
		val[3] = 0;
		val[4] = mat.val[3];
		val[5] = mat.val[4];
		val[6] = mat.val[5];
		val[7] = 0;
		val[8] = 0;
		val[9] = 0;
		val[10] = 1;
		val[11] = 0;
		val[12] = mat.val[6];
		val[13] = mat.val[7];
		val[14] = 0;
		val[15] = mat.val[8];
		return this;
	}
    
	Matrix4& Matrix4::set (const Affine2& affine) {
		val[M00] = affine.m00;
		val[M10] = affine.m10;
		val[M20] = 0;
		val[M30] = 0;
		val[M01] = affine.m01;
		val[M11] = affine.m11;
		val[M21] = 0;
		val[M31] = 0;
		val[M02] = 0;
		val[M12] = 0;
		val[M22] = 1;
		val[M32] = 0;
		val[M03] = affine.m02;
		val[M13] = affine.m12;
		val[M23] = 0;
		val[M33] = 1;
		return this;
	}
    
	Matrix4& Matrix4::setAsAffine (const Affine2& affine) {
		val[M00] = affine.m00;
		val[M10] = affine.m10;
		val[M01] = affine.m01;
		val[M11] = affine.m11;
		val[M03] = affine.m02;
		val[M13] = affine.m12;
		return this;
	}
    
	Matrix4& Matrix4::scl (const Vector3& scale) {
		val[M00] *= scale.x;
		val[M11] *= scale.y;
		val[M22] *= scale.z;
		return this;
	}
    
	Vector3& Matrix4::getTranslation (Vector3& position) {
		position.x = val[M03];
		position.y = val[M13];
		position.z = val[M23];
		return position;
	}
    
	Quaternion& Matrix4::getRotation (Quaternion& rotation, bool normalizeAxes) {
		return rotation.setFromMatrix(normalizeAxes, this);
	}
    
	Quaternion& Matrix4::getRotation (Quaternion& rotation) {
		return rotation.setFromMatrix(this);
	}
    
	Vector3& Matrix4::getScale (Vector3 scale) {
		return scale.set(getScaleX(), getScaleY(), getScaleZ());
	}
    
	Matrix4& Matrix4::translate (const Vector3& translation) {
		return translate(translation.x, translation.y, translation.z);
	}
    
	Matrix4& Matrix4::rotate (const Vector3& axis, float degrees) {
		if (degrees == 0) return this;
		quat.set(axis, degrees);
		return rotate(quat);
	}
    
	Matrix4& Matrix4::rotateRad (const Vector3& axis, float radians) {
		if (radians == 0) return this;
		quat.setFromAxisRad(axis, radians);
		return rotate(quat);
	}
    
	Matrix4& Matrix4::rotate (float axisX, float axisY, float axisZ, float degrees) {
		if (degrees == 0) return this;
		quat.setFromAxis(axisX, axisY, axisZ, degrees);
		return rotate(quat);
	}
    
	Matrix4& Matrix4::rotateRad (float axisX, float axisY, float axisZ, float radians) {
		if (radians == 0) return this;
		quat.setFromAxisRad(axisX, axisY, axisZ, radians);
		return rotate(quat);
	}
    
	Matrix4& Matrix4::rotate (const Quaternion& rotation) {
		rotation.toMatrix(tmp);
		mul(val, tmp);
		return this;
	}
    
	Matrix4& Matrix4::rotate (const Vector3& v1, const Vector3& v2) {
		return rotate(quat.setFromCross(v1, v2));
	}