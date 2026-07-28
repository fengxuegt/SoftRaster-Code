#include "math/math.h"

int main() {
	math::mat3f m33;
	auto m = m33 * 4.0f;
	m.printMatrix();

	auto n = m33;

	auto s = m * n;

	s.printMatrix();

	math::mat4f m41;
	math::mat4f m42;

	auto m43 = m41 * m42;
	m43.printMatrix();

	return 0;
}