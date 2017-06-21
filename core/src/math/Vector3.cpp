#include "Vector3.h"

	Vector3::Vector3 (const Vector2& vector, float z) {
		this->set(vector.x, vector.y, z);
	}
    
	Vector3& Vector3::set (const Vector2& vector, float z) {
		return this->set(vector.x, vector.y, z);
	}
    
	Vector3& Vector3::setFromSpherical (float azimuthalAngle, float polarAngle) {
		float cosPolar = MathUtils::cos(polarAngle);
		float sinPolar = MathUtils::sin(polarAngle);

		float cosAzim = MathUtils::cos(azimuthalAngle);
		float sinAzim = MathUtils::sin(azimuthalAngle);

		return this->set(cosAzim * sinPolar, sinAzim * sinPolar, cosPolar);
	}
    
	Vector3& Vector3::setToRandomDirection () {
		float u = random();
		float v = random();

		float theta = MathUtils::PI2 * u; // azimuthal angle
		float phi = acos(2.0f * v - 1.0f); // polar angle

		return this->setFromSpherical(theta, phi);
	}
    
	Vector3& Vector3::mul (const Matrix3& matrix) {
		std::vector<float> l_mat = matrix.val;
		return set(x * l_mat[Matrix3::M00] + y * l_mat[Matrix3::M01] + z * l_mat[Matrix3::M02], x * l_mat[Matrix3::M10] + y
			* l_mat[Matrix3::M11] + z * l_mat[Matrix3::M12], x * l_mat[Matrix3::M20] + y * l_mat[Matrix3::M21] + z * l_mat[Matrix3::M22]);
	}

	Vector3& Vector3::mul (const Matrix4& matrix) {
		std::vector<float> l_mat = matrix.val;
		return this->set(x * l_mat[Matrix4::M00] + y * l_mat[Matrix4::M01] + z * l_mat[Matrix4::M02] + l_mat[Matrix4::M03], x
			* l_mat[Matrix4::M10] + y * l_mat[Matrix4::M11] + z * l_mat[Matrix4::M12] + l_mat[Matrix4::M13], x * l_mat[Matrix4::M20] + y
			* l_mat[Matrix4::M21] + z * l_mat[Matrix4::M22] + l_mat[Matrix4::M23]);
	}
    
	Vector3& Vector3::mul (Quaternion& quat) {
		return quat.transform(*this);
	}
    
	Vector3& Vector3::traMul (const Matrix3& matrix) {
		std::vector<float> l_mat = matrix.val;
		return set(x * l_mat[Matrix3::M00] + y * l_mat[Matrix3::M10] + z * l_mat[Matrix3::M20], x * l_mat[Matrix3::M01] + y
			* l_mat[Matrix3::M11] + z * l_mat[Matrix3::M21], x * l_mat[Matrix3::M02] + y * l_mat[Matrix3::M12] + z * l_mat[Matrix3::M22]);
	}
    
	Vector3& Vector3::traMul (const Matrix4& matrix) {
		std::vector<float> l_mat = matrix.val;
		return this->set(x * l_mat[Matrix4::M00] + y * l_mat[Matrix4::M10] + z * l_mat[Matrix4::M20] + l_mat[Matrix4::M30], x
			* l_mat[Matrix4::M01] + y * l_mat[Matrix4::M11] + z * l_mat[Matrix4::M21] + l_mat[Matrix4::M31], x * l_mat[Matrix4::M02] + y
			* l_mat[Matrix4::M12] + z * l_mat[Matrix4::M22] + l_mat[Matrix4::M32]);
	}
    
	Vector3& Vector3::rot (const Matrix4& matrix) {
		std::vector<float> l_mat = matrix.val;
		return this->set(x * l_mat[Matrix4::M00] + y * l_mat[Matrix4::M01] + z * l_mat[Matrix4::M02], x * l_mat[Matrix4::M10] + y
			* l_mat[Matrix4::M11] + z * l_mat[Matrix4::M12], x * l_mat[Matrix4::M20] + y * l_mat[Matrix4::M21] + z * l_mat[Matrix4::M22]);
	}
    
	Vector3& Vector3::unrotate (const Matrix4& matrix) {
		std::vector<float> l_mat = matrix.val;
		return this->set(x * l_mat[Matrix4::M00] + y * l_mat[Matrix4::M10] + z * l_mat[Matrix4::M20], x * l_mat[Matrix4::M01] + y
			* l_mat[Matrix4::M11] + z * l_mat[Matrix4::M21], x * l_mat[Matrix4::M02] + y * l_mat[Matrix4::M12] + z * l_mat[Matrix4::M22]);
	}
    
	Vector3& Vector3::untransform (const Matrix4& matrix) {
		std::vector<float> l_mat = matrix.val;
		x -= l_mat[Matrix4::M03];
		y -= l_mat[Matrix4::M03];
		z -= l_mat[Matrix4::M03];
		return this->set(x * l_mat[Matrix4::M00] + y * l_mat[Matrix4::M10] + z * l_mat[Matrix4::M20], x * l_mat[Matrix4::M01] + y
			* l_mat[Matrix4::M11] + z * l_mat[Matrix4::M21], x * l_mat[Matrix4::M02] + y * l_mat[Matrix4::M12] + z * l_mat[Matrix4::M22]);
	}
    
	Vector3& Vector3::rotateRad (float radians, float axisX, float axisY, float axisZ) {
		return this->mul(tmpMat.setToRotationRad(axisX, axisY, axisZ, radians));
	}

	Vector3& Vector3::rotate (const Vector3& axis, float degrees) {
		tmpMat.setToRotation(axis, degrees);
		return this->mul(tmpMat);
	}
    
	Vector3& Vector3::rotateRad (const Vector3& axis, float radians) {
		tmpMat.setToRotationRad(axis, radians);
		return this->mul(tmpMat);
	}
    
	Vector3& Vector3::rotate (float degrees, float axisX, float axisY, float axisZ) {
		return this->mul(tmpMat.setToRotation(axisX, axisY, axisZ, degrees));
	}
    
	Vector3& Vector3::prj (const Matrix4& matrix) {
		std::vector<float> l_mat = matrix.val;
		float l_w = 1.0f / (x * l_mat[Matrix4::M30] + y * l_mat[Matrix4::M31] + z * l_mat[Matrix4::M32] + l_mat[Matrix4::M33]);
		return this->set((x * l_mat[Matrix4::M00] + y * l_mat[Matrix4::M01] + z * l_mat[Matrix4::M02] + l_mat[Matrix4::M03]) * l_w, (x
			* l_mat[Matrix4::M10] + y * l_mat[Matrix4::M11] + z * l_mat[Matrix4::M12] + l_mat[Matrix4::M13])
			* l_w, (x * l_mat[Matrix4::M20] + y * l_mat[Matrix4::M21] + z * l_mat[Matrix4::M22] + l_mat[Matrix4::M23]) * l_w);
	}

Vector3 Vector3::X =  Vector3(1, 0, 0);
Vector3 Vector3::Y =  Vector3(0, 1, 0);
Vector3 Vector3::Z =  Vector3(0, 0, 1);
Vector3 Vector3::Zero =  Vector3(0, 0, 0);
Matrix4 Vector3::tmpMat;