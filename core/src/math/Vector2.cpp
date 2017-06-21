#include "Vector2.h"
#include "MathUtils.h"

Vector2 Vector2::X =  Vector2(1, 0);
Vector2 Vector2::Y =  Vector2(0, 1);
Vector2 Vector2::Zero =  Vector2(0, 0);

	 Vector2& Vector2::mul (const Matrix3& mat) {
		float x = this->x * mat.val[0] + this->y * mat.val[3] + mat.val[6];
		float y = this->x * mat.val[1] + this->y * mat.val[4] + mat.val[7];
		this->x = x;
		this->y = y;
		return *this;
	}
    
    float Vector2::angle () {
		float angle = MathUtils::atan2(y, x) * MathUtils::radiansToDegrees;
		if (angle < 0) angle += 360;
		return angle;
	}
    
float Vector2::angle (const Vector2& reference) {
		return MathUtils::atan2(crs(reference), dot(reference)) * MathUtils::radiansToDegrees;
	}
    
	 float Vector2::angleRad () {
		return MathUtils::atan2(y, x);
	}
    
	 float Vector2::angleRad (const Vector2& reference) {
		return MathUtils::atan2(crs(reference), dot(reference));
	}
    
	 Vector2& Vector2::setAngle (float degrees) {
		return setAngleRad(degrees * MathUtils::degreesToRadians);
	}
    
	 Vector2& Vector2::setAngleRad (float radians) {
		this->set(len(), 0.0f);
		this->rotateRad(radians);

		return *this;
	}
    
	 Vector2& Vector2::rotate (float degrees) {
		return rotateRad(degrees * MathUtils::degreesToRadians);
	}
    
	 Vector2& Vector2::rotateRad (float radians) {
		float cos = MathUtils::cos(radians);
		float sin = MathUtils::sin(radians);

		float X = this->x * cos - this->y * sin;
		float Y = this->x * sin + this->y * cos;

		this->x = X;
		this->y = Y;

		return *this;
	}
    
	 Vector2& Vector2::setToRandomDirection () {
		float theta = MathUtils::random(0.0f, MathUtils::PI2);
		return this->set(cos(theta), sin(theta));
	}
    
	 bool Vector2::isOnLine (const Vector2& other) {
		return MathUtils::isZero(x * other.y - y * other.x);
	}
    
	 bool Vector2::isOnLine (const Vector2& other, float epsilon) {
		return MathUtils::isZero(x * other.y - y * other.x, epsilon);
	}
    
    	 bool Vector2::isCollinear (const Vector2& other) {
		return isOnLine(other) && dot(other) > 0.0f;
	}
    
	 bool Vector2::isCollinear (const Vector2& other, float epsilon) {
		return isOnLine(other, epsilon) && dot(other) > 0.0f;
	}
    
	 bool Vector2::isCollinearOpposite (const Vector2& other, float epsilon) {
		return isOnLine(other, epsilon) && dot(other) < 0.0f;
	}
    
	 bool Vector2::isCollinearOpposite (const Vector2& other) {
		return isOnLine(other) && dot(other) < 0.0f;
	}
    
	 bool Vector2::isPerpendicular (const Vector2& vector) {
		return MathUtils::isZero(dot(vector));
	}
    
	 bool Vector2::isPerpendicular (const Vector2& vector, float epsilon) {
		return MathUtils::isZero(dot(vector), epsilon);
	}
    
	 bool Vector2::hasSameDirection (const Vector2& vector) {
		return dot(vector) > 0;
	}
    
    	 bool Vector2::hasOppositeDirection (const Vector2& vector) {
		return dot(vector) < 0;
	}
    
	 Vector2& Vector2::setZero () {
		this->x = 0;
		this->y = 0;
		return *this;
	}
