#include "Quaternion.h"
#include "Vector3.h"
#include "MathUtils.h"

Quaternion Quaternion::tmp1 =  Quaternion(0, 0, 0, 0);
Quaternion Quaternion::tmp2 =  Quaternion(0, 0, 0, 0);

	 bool Quaternion::isIdentity () {
		return MathUtils::isZero(x) && MathUtils::isZero(y) && MathUtils::isZero(z) && MathUtils::isEqual(w, 1.0f);
	}
    
	 bool Quaternion::isIdentity (const float tolerance) {
		return MathUtils::isZero(x, tolerance) && MathUtils::isZero(y, tolerance) && MathUtils::isZero(z, tolerance)
			&& MathUtils::isEqual(w, 1.0f, tolerance);
	}

    Quaternion& Quaternion::set (const Vector3& axis, float angle) {
		return setFromAxis(axis.x, axis.y, axis.z, angle);
	}
    
	 Quaternion& Quaternion::mul (float scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		this->w *= scalar;
		return *this;
	}
    
	 float Quaternion::getAngleRad () {
		return (float)(2.0f * acos((this->w > 1) ? (this->w / len()) : this->w));
	}
    
	 float Quaternion::getAngleAround (const float axisX, const float axisY, const float axisZ) {
		return getAngleAroundRad(axisX, axisY, axisZ) * MathUtils::radiansToDegrees;
	}
    
	 Quaternion& Quaternion::setFromAxis (const float x, const float y, const float z, const float degrees) {
		return setFromAxisRad(x, y, z, degrees * MathUtils::degreesToRadians);
	}
    
	 float Quaternion::getAngle () {
		return getAngleRad() * MathUtils::radiansToDegrees;
	}
    
	 float Quaternion::getAxisAngle (Vector3& axis) {
		return getAxisAngleRad(axis) * MathUtils::radiansToDegrees;
	}
    
    Vector3& Quaternion::transform (Vector3& v) {
		tmp2.set(*this);
		tmp2.conjugate();
		tmp2.mulLeft(tmp1.set(v.x, v.y, v.z, 0)).mulLeft(*this);

		v.x = tmp2.x;
		v.y = tmp2.y;
		v.z = tmp2.z;
		return v;
	}
    
	 float Quaternion::getRoll () {
		return getRollRad() * MathUtils::radiansToDegrees;
	}
    
	 float Quaternion::getPitchRad () {
		const int pole = getGimbalPole();
		return pole == 0 ? asin(MathUtils::clamp(2.0f * (w * x - z * y), -1.0f, 1.0f)) : (float)pole * M_PI * 0.5f;
	}
    
	 float Quaternion::getPitch () {
		return getPitchRad() * MathUtils::radiansToDegrees;
	}
    
	 float Quaternion::getYawRad () {
		return getGimbalPole() == 0 ? atan2(2.0f * (y * w + x * z), 1.0f - 2.0f * (y * y + x * x)) : 0.0f;
	}
    
	 float Quaternion::getYaw () {
		return getYawRad() * MathUtils::radiansToDegrees;
	}
    
	 Quaternion& Quaternion::nor () {
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
    
	 Quaternion& Quaternion::setEulerAngles (float yaw, float pitch, float roll) {
		return setEulerAnglesRad(yaw * MathUtils::degreesToRadians, pitch * MathUtils::degreesToRadians, roll
			* MathUtils::degreesToRadians);
	}
    
    Quaternion& Quaternion::setFromAxis (const Vector3& axis, const float degrees) {
		return setFromAxis(axis.x, axis.y, axis.z, degrees);
	}

    Quaternion& Quaternion::setFromAxisRad (const Vector3& axis, const float radians) {
		return setFromAxisRad(axis.x, axis.y, axis.z, radians);
	}
    
    Quaternion& Quaternion::setFromAxisRad (const float x, const float y, const float z, const float radians) {
		float d = Vector3::len(x, y, z);
		if (d == 0.0f) return idt();
		d = 1.0f / d;
		float l_ang = radians < 0 ? MathUtils::PI2 - fmod(-radians,MathUtils::PI2) : fmod(radians,MathUtils::PI2);
		float l_sin = sin(l_ang / 2);
		float l_cos = cos(l_ang / 2);
		return this->set(d * x * l_sin, d * y * l_sin, d * z * l_sin, l_cos).nor();
	}
    
    Quaternion& Quaternion::setFromMatrix (bool normalizeAxes, const Matrix4& matrix) {
		return setFromAxes(normalizeAxes, matrix.val[Matrix4::M00], matrix.val[Matrix4::M01], matrix.val[Matrix4::M02],
			matrix.val[Matrix4::M10], matrix.val[Matrix4::M11], matrix.val[Matrix4::M12], matrix.val[Matrix4::M20],
			matrix.val[Matrix4::M21], matrix.val[Matrix4::M22]);
	}
    
    Quaternion& Quaternion::setFromMatrix (bool normalizeAxes, const Matrix3& matrix) {
		return setFromAxes(normalizeAxes, matrix.val[Matrix3::M00], matrix.val[Matrix3::M01], matrix.val[Matrix3::M02],
			matrix.val[Matrix3::M10], matrix.val[Matrix3::M11], matrix.val[Matrix3::M12], matrix.val[Matrix3::M20],
			matrix.val[Matrix3::M21], matrix.val[Matrix3::M22]);
	}
    
    Quaternion& Quaternion::setFromAxes (bool normalizeAxes, float xx, float xy, float xz, float yx, float yy, float yz, float zx,
		float zy, float zz) {
		if (normalizeAxes) {
			const float lx = 1.0f / Vector3::len(xx, xy, xz);
			const float ly = 1.0f / Vector3::len(yx, yy, yz);
			const float lz = 1.0f / Vector3::len(zx, zy, zz);
			xx *= lx;
			xy *= lx;
			xz *= lx;
			yx *= ly;
			yy *= ly;
			yz *= ly;
			zx *= lz;
			zy *= lz;
			zz *= lz;
		}
		// the trace is the sum of the diagonal elements; see
		// http://mathworld.wolfram.com/MatrixTrace.html
		const float t = xx + yy + zz;

		// we protect the division by s by ensuring that s>=1
		if (t >= 0) { // |w| >= .5
			float s = sqrt(t + 1); // |s|>=1 ...
			w = 0.5f * s;
			s = 0.5f / s; // so this division isn't bad
			x = (zy - yz) * s;
			y = (xz - zx) * s;
			z = (yx - xy) * s;
		} else if ((xx > yy) && (xx > zz)) {
			float s = sqrt(1.0f + xx - yy - zz); // |s|>=1
			x = s * 0.5f; // |x| >= .5
			s = 0.5f / s;
			y = (yx + xy) * s;
			z = (xz + zx) * s;
			w = (zy - yz) * s;
		} else if (yy > zz) {
			float s = sqrt(1.0f + yy - xx - zz); // |s|>=1
			y = s * 0.5f; // |y| >= .5
			s = 0.5f / s;
			x = (yx + xy) * s;
			z = (zy + yz) * s;
			w = (xz - zx) * s;
		} else {
			float s = sqrt(1.0f + zz - xx - yy); // |s|>=1
			z = s * 0.5f; // |z| >= .5
			s = 0.5f / s;
			x = (xz + zx) * s;
			y = (zy + yz) * s;
			w = (yx - xy) * s;
		}

		return *this;
	}
    
    Quaternion& Quaternion::setFromCross (Vector3& v1, Vector3& v2) {
		const float dot = MathUtils::clamp(v1.dot(v2), -1.0f, 1.0f);
		const float angle = acos(dot);
		return setFromAxisRad(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x, angle);
	}
    
    Quaternion& Quaternion::setFromCross (const float x1, const float y1, const float z1, const float x2, const float y2, const float z2) {
		const float dot = MathUtils::clamp(Vector3::dot(x1, y1, z1, x2, y2, z2), -1.0f, 1.0f);
		const float angle = acos(dot);
		return setFromAxisRad(y1 * z2 - z1 * y2, z1 * x2 - x1 * z2, x1 * y2 - y1 * x2, angle);
	}
    
    float Quaternion::getAxisAngleRad (Vector3& axis) {
		if (this->w > 1) this->nor(); // if w>1 acos and sqrt will produce errors, this cant happen if quaternion is normalised
		float angle = (float)(2.0 * acos(this->w));
		double s = sqrt(1 - this->w * this->w); // assuming quaternion normalised then w is less than 1, so term always positive.
		if (s < MathUtils::FLOAT_ROUNDING_ERROR) { // test to avoid divide by zero, s is always positive due to sqrt
			// if s close to zero then direction of axis not important
			axis.x = this->x; // if it is important that axis is normalised then replace with x=1; y=z=0;
			axis.y = this->y;
			axis.z = this->z;
		} else {
			axis.x = (float)(this->x / s); // normalise axis
			axis.y = (float)(this->y / s);
			axis.z = (float)(this->z / s);
		}

		return angle;
	}
    
    void Quaternion::getSwingTwist (const float axisX, const float axisY, const float axisZ, Quaternion& swing,
		Quaternion& twist) {
		const float d = Vector3::dot(this->x, this->y, this->z, axisX, axisY, axisZ);
		twist.set(axisX * d, axisY * d, axisZ * d, this->w).nor();
		if (d < 0) twist.mul(-1.0f);
		swing.set(twist).conjugate().mulLeft(*this);
	}
    
    void Quaternion::getSwingTwist (const Vector3& axis, Quaternion& swing, Quaternion& twist) {
		getSwingTwist(axis.x, axis.y, axis.z, swing, twist);
	}
    
    float Quaternion::getAngleAroundRad (const float axisX, const float axisY, const float axisZ) {
		const float d = Vector3::dot(this->x, this->y, this->z, axisX, axisY, axisZ);
		const float l2 = Quaternion::len2(axisX * d, axisY * d, axisZ * d, this->w);
		return MathUtils::isZero(l2) ? 0.0f : (float)(2.0f * acos(MathUtils::clamp(
			(float)((d < 0 ? -this->w : this->w) / sqrt(l2)), -1.0f, 1.0f)));
	}
    
    float Quaternion::getAngleAroundRad (const Vector3& axis) {
		return getAngleAroundRad(axis.x, axis.y, axis.z);
	}
    
    float Quaternion::getAngleAround (const Vector3& axis) {
		return getAngleAround(axis.x, axis.y, axis.z);
	}
    
    void Quaternion::toMatrix (std::vector<float>& matrix) {
		const float xx = x * x;
		const float xy = x * y;
		const float xz = x * z;
		const float xw = x * w;
		const float yy = y * y;
		const float yz = y * z;
		const float yw = y * w;
		const float zz = z * z;
		const float zw = z * w;
		// Set matrix from quaternion
		matrix[Matrix4::M00] = 1 - 2 * (yy + zz);
		matrix[Matrix4::M01] = 2 * (xy - zw);
		matrix[Matrix4::M02] = 2 * (xz + yw);
		matrix[Matrix4::M03] = 0;
		matrix[Matrix4::M10] = 2 * (xy + zw);
		matrix[Matrix4::M11] = 1 - 2 * (xx + zz);
		matrix[Matrix4::M12] = 2 * (yz - xw);
		matrix[Matrix4::M13] = 0;
		matrix[Matrix4::M20] = 2 * (xz - yw);
		matrix[Matrix4::M21] = 2 * (yz + xw);
		matrix[Matrix4::M22] = 1 - 2 * (xx + yy);
		matrix[Matrix4::M23] = 0;
		matrix[Matrix4::M30] = 0;
		matrix[Matrix4::M31] = 0;
		matrix[Matrix4::M32] = 0;
		matrix[Matrix4::M33] = 1;
	}

