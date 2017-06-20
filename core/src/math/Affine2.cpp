#include "Affine2.h"

Affine2& Affine2::set (const Matrix3& matrix) {
		std::vector<float> other = matrix.val;

		m00 = other[Matrix3.M00];
		m01 = other[Matrix3.M01];
		m02 = other[Matrix3.M02];
		m10 = other[Matrix3.M10];
		m11 = other[Matrix3.M11];
		m12 = other[Matrix3.M12];
		return *this;
	}
    
    Affine2& Affine2::set (const Matrix4& matrix) {
		std::vector<float> other = matrix.val;

		m00 = other[Matrix4.M00];
		m01 = other[Matrix4.M01];
		m02 = other[Matrix4.M03];
		m10 = other[Matrix4.M10];
		m11 = other[Matrix4.M11];
		m12 = other[Matrix4.M13];
		return *this;
	}
    
    Affine2& Affine2::setToTranslation (const Vector2& trn) {
		return setToTranslation(trn.x, trn.y);
	}
    
    Affine2& Affine2::setToScaling (const Vector2& scale) {
		return setToScaling(scale.x, scale.y);
	}
    
    Affine2& Affine2::setToShearing (const Vector2& shear) {
		return setToShearing(shear.x, shear.y);
	}
    
    Affine2& Affine2::setToTrnRotScl (const Vector2& trn, float degrees, const Vector2& scale) {
		return setToTrnRotScl(trn.x, trn.y, degrees, scale.x, scale.y);
	}
    
    Affine2& Affine2::setToTrnRotRadScl (const Vector2& trn, float radians, const Vector2& scale) {
		return setToTrnRotRadScl(trn.x, trn.y, radians, scale.x, scale.y);
	}
    
    Affine2& Affine2::setToTrnScl (const Vector2& trn, const Vector2& scale) {
		return setToTrnScl(trn.x, trn.y, scale.x, scale.y);
	}
    
    Affine2& Affine2::translate (const Vector2& trn) {
		return translate(trn.x, trn.y);
	}

    Affine2& Affine2::preTranslate (const Vector2& trn) {
		return preTranslate(trn.x, trn.y);
	}
    
    Affine2& Affine2::scale (const Vector2& scale) {
		return scale(scale.x, scale.y);
	}
    
    Affine2& Affine2::preScale (const Vector2& scale) {
		return preScale(scale.x, scale.y);
	}
    
    Affine2& Affine2::shear (const Vector2& shear) {
		return shear(shear.x, shear.y);
	}
    
    Affine2& Affine2::preShear (const Vector2& shear) {
		return preShear(shear.x, shear.y);
	}
    
    Vector2& Affine2::getTranslation (const Vector2& position) {
		position.x = m02;
		position.y = m12;
		return position;
	}
    
    void Affine2::applyTo (const Vector2& point) {
		float x = point.x;
		float y = point.y;
		point.x = m00 * x + m01 * y + m02;
		point.y = m10 * x + m11 * y + m12;
	}