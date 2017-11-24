#include <math.h>

class vector2f {
public:
	vector2f() {}
	vector2f(const vector2f &_v): x(_v.x), y(_v.y) {}
	vector2f(float _x, float _y): x(_x), y(_y) {}

	vector2f operator +(const vector2f b) const {
		return vector2f(x + b.x, y + b.y);
	}

	vector2f& operator +=(const vector2f b) {
		x += b.x;
		y += b.y;
		return *this;
	}

	vector2f operator -(const vector2f b) const {
		return vector2f(x - b.x, y - b.y);
	}

	vector2f& operator -=(const vector2f b) {
		x -= b.x;
		y -= b.y;
		return *this;
	}
	
	vector2f operator *(float lambda) const { //scalar mult
		return vector2f(x * lambda, y * lambda);
	}

	vector2f& operator *=(float lambda) {
		x *= lambda;
		y *= lambda;
		return *this;
	}

	float operator *(const vector2f b) const { //dot product
		return x * b.x + y * b.y;
	}

	float Length() {
		return sqrt((*this) * (*this));
	}

	vector2f& Normalize() {
		return (*this) *= 1.f / Length();
	}

	float x, y;
};
