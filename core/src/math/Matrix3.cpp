#include "Matrix3.h"

    Matrix3& Matrix3::setToRotation (const Vector3& axis, float cos, float sin) {
		float oc = 1.0f - cos;
		val[M00] = oc * axis.x * axis.x + cos;
		val[M10] = oc * axis.x * axis.y - axis.z * sin;
		val[M20] = oc * axis.z * axis.x + axis.y * sin;
		val[M01] = oc * axis.x * axis.y + axis.z * sin;
		val[M11] = oc * axis.y * axis.y + cos;
		val[M21] = oc * axis.y * axis.z - axis.x * sin;
		val[M02] = oc * axis.z * axis.x - axis.y * sin;
		val[M12] = oc * axis.y * axis.z + axis.x * sin;
		val[M22] = oc * axis.z * axis.z + cos;
		return *this;
	}
    
	 Matrix3& Matrix3::setToTranslation (const Vector2& translation) {
		val[M00] = 1;
		val[M10] = 0;
		val[M20] = 0;

		val[M01] = 0;
		val[M11] = 1;
		val[M21] = 0;

		val[M02] = translation.x;
		val[M12] = translation.y;
		val[M22] = 1;

		return *this;
	}
    
	 Matrix3& Matrix3::setToScaling (const Vector2& scale) {
		val[M00] = scale.x;
		val[M10] = 0;
		val[M20] = 0;
		val[M01] = 0;
		val[M11] = scale.y;
		val[M21] = 0;
		val[M02] = 0;
		val[M12] = 0;
		val[M22] = 1;
		return *this;
	}
    
	 Matrix3& Matrix3::trn (const Vector2& vector) {
		val[M02] += vector.x;
		val[M12] += vector.y;
		return *this;
	}
    
	 Matrix3& Matrix3::trn (const Vector3& vector) {
		val[M02] += vector.x;
		val[M12] += vector.y;
		return *this;
	}
    
	 Matrix3& Matrix3::translate (const Vector2& translation) {
		tmp[M00] = 1;
		tmp[M10] = 0;
		tmp[M20] = 0;

		tmp[M01] = 0;
		tmp[M11] = 1;
		tmp[M21] = 0;

		tmp[M02] = translation.x;
		tmp[M12] = translation.y;
		tmp[M22] = 1;
		mul(val, tmp);
		return *this;
	}
    
	 Matrix3& Matrix3::scale (const Vector2& scale) {
		tmp[M00] = scale.x;
		tmp[M10] = 0;
		tmp[M20] = 0;
		tmp[M01] = 0;
		tmp[M11] = scale.y;
		tmp[M21] = 0;
		tmp[M02] = 0;
		tmp[M12] = 0;
		tmp[M22] = 1;
		mul(val, tmp);
		return *this;
	}
    
	 Vector2& Matrix3::getTranslation (const Vector2& position) {
		position.x = val[M02];
		position.y = val[M12];
		return position;
	}
    
	 Vector2& Matrix3::getScale (const Vector2& scale) {
		scale.x = sqrt(val[M00] * val[M00] + val[M01] * val[M01]);
		scale.y = sqrt(val[M10] * val[M10] + val[M11] * val[M11]);
		return scale;
	}
    
	 Matrix3& Matrix3::scl (const Vector2& scale) {
		val[M00] *= scale.x;
		val[M11] *= scale.y;
		return *this;
	}
    
	 Matrix3& Matrix3::scl (const Vector3& scale) {
		val[M00] *= scale.x;
		val[M11] *= scale.y;
		return *this;
	}
