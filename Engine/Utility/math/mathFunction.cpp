#include"mathFunction.h"

#include <limits>
#include <algorithm>
#include <numbers>
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};

	return result;
};
Matrix4x4 Multiply(const Matrix4x4 m1, const Matrix4x4 m2) {
	Matrix4x4 m4;
	m4.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	m4.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	m4.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	m4.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

	m4.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	m4.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	m4.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	m4.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

	m4.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	m4.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	m4.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	m4.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

	m4.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	m4.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	m4.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	m4.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];
	return m4;
};


// 6. 単位行列の作成
Matrix4x4 MakeIdentity4x4() {

	Matrix4x4 m4;
	m4.m[0][0] = 1;
	m4.m[0][1] = 0;
	m4.m[0][2] = 0;
	m4.m[0][3] = 0;

	m4.m[1][0] = 0;
	m4.m[1][1] = 1;
	m4.m[1][2] = 0;
	m4.m[1][3] = 0;

	m4.m[2][0] = 0;
	m4.m[2][1] = 0;
	m4.m[2][2] = 1;
	m4.m[2][3] = 0;

	m4.m[3][0] = 0;
	m4.m[3][1] = 0;
	m4.m[3][2] = 0;
	m4.m[3][3] = 1;
	return m4;
};

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 m4;
	Matrix4x4 m1 = MakeRotateXMatrix(rotate.x);

	Matrix4x4 m2 = MakeRotateYMatrix(rotate.y);

	Matrix4x4 m3 = MakeRotateZMatrix(rotate.z);

	Matrix4x4 xyz = Multiply(m1, Multiply(m2, m3));


	m4.m[0][0] = xyz.m[0][0] * scale.x;
	m4.m[0][1] = xyz.m[0][1] * scale.x;
	m4.m[0][2] = xyz.m[0][2] * scale.x;

	m4.m[1][0] = xyz.m[1][0] * scale.y;
	m4.m[1][1] = xyz.m[1][1] * scale.y;
	m4.m[1][2] = xyz.m[1][2] * scale.y;

	m4.m[2][0] = xyz.m[2][0] * scale.z;
	m4.m[2][1] = xyz.m[2][1] * scale.z;
	m4.m[2][2] = xyz.m[2][2] * scale.z;


	m4.m[3][0] = translate.x;
	m4.m[3][1] = translate.y;
	m4.m[3][2] = translate.z;
	m4.m[0][3] = 0;
	m4.m[1][3] = 0;
	m4.m[2][3] = 0;
	m4.m[3][3] = 1.0f;

	return m4;
}
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate)
{
	Matrix4x4 m4;
	

	Matrix4x4 xyz  = MakeRotateMatrix(rotate);


	m4.m[0][0] = xyz.m[0][0] * scale.x;
	m4.m[0][1] = xyz.m[0][1] * scale.x;
	m4.m[0][2] = xyz.m[0][2] * scale.x;

	m4.m[1][0] = xyz.m[1][0] * scale.y;
	m4.m[1][1] = xyz.m[1][1] * scale.y;
	m4.m[1][2] = xyz.m[1][2] * scale.y;

	m4.m[2][0] = xyz.m[2][0] * scale.z;
	m4.m[2][1] = xyz.m[2][1] * scale.z;
	m4.m[2][2] = xyz.m[2][2] * scale.z;


	m4.m[3][0] = translate.x;
	m4.m[3][1] = translate.y;
	m4.m[3][2] = translate.z;
	m4.m[0][3] = 0;
	m4.m[1][3] = 0;
	m4.m[2][3] = 0;
	m4.m[3][3] = 1.0f;

	return m4;
}
;

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrixBillboard(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 m4;
	Matrix4x4 m1 = MakeRotateXMatrix(rotate.x);

	Matrix4x4 m2 = MakeRotateYMatrix(rotate.y);

	Matrix4x4 m3 = MakeRotateZMatrix(rotate.z);

	Matrix4x4 xyz = Multiply(m1, Multiply(m2, m3));


	m4.m[0][0] = xyz.m[0][0] * scale.x;
	m4.m[0][1] = xyz.m[0][1] * scale.x;
	m4.m[0][2] = xyz.m[0][2] * scale.x;

	m4.m[1][0] = xyz.m[1][0] * scale.y;
	m4.m[1][1] = xyz.m[1][1] * scale.y;
	m4.m[1][2] = xyz.m[1][2] * scale.y;

	m4.m[2][0] = xyz.m[2][0] * scale.z;
	m4.m[2][1] = xyz.m[2][1] * scale.z;
	m4.m[2][2] = xyz.m[2][2] * scale.z;


	m4.m[3][0] = translate.x;
	m4.m[3][1] = translate.y;
	m4.m[3][2] = translate.z;
	m4.m[0][3] = 0;
	m4.m[1][3] = 0;
	m4.m[2][3] = 0;
	m4.m[3][3] = 1.0f;

	return m4;
};

// Quaternionから回転行列を求める
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion) {
	Matrix4x4 result;
	result.m[0][0] = quaternion.w * quaternion.w + quaternion.x * quaternion.x - quaternion.y * quaternion.y - quaternion.z * quaternion.z;
	result.m[0][1] = 2 * (quaternion.x * quaternion.y + quaternion.w * quaternion.z);
	result.m[0][2] = 2 * (quaternion.x * quaternion.z - quaternion.w * quaternion.y);
	result.m[0][3] = 0;

	result.m[1][0] = 2 * (quaternion.x * quaternion.y - quaternion.w * quaternion.z);
	result.m[1][1] = quaternion.w * quaternion.w - quaternion.x * quaternion.x + quaternion.y * quaternion.y - quaternion.z * quaternion.z;
	result.m[1][2] = 2 * (quaternion.y * quaternion.z + quaternion.w * quaternion.x);
	result.m[1][3] = 0;

	result.m[2][0] = 2 * (quaternion.x * quaternion.z + quaternion.w * quaternion.y);
	result.m[2][1] = 2 * (quaternion.y * quaternion.z - quaternion.w * quaternion.x);
	result.m[2][2] = quaternion.w * quaternion.w - quaternion.x * quaternion.x - quaternion.y * quaternion.y + quaternion.z * quaternion.z;
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;
	return result;
}

// 1. X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 m4;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m4.m[i][j] = 0;
		}
	}
	m4.m[0][0] = 1;
	m4.m[1][1] = std::cos(radian);
	m4.m[1][2] = std::sin(radian);
	m4.m[2][1] = -std::sin(radian);
	m4.m[2][2] = std::cos(radian);
	m4.m[3][3] = 1;
	return m4;
};

// 2. Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 m4;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m4.m[i][j] = 0;
		}
	}
	m4.m[0][0] = std::cos(radian);
	m4.m[1][1] = 1;
	m4.m[2][2] = std::cos(radian);
	m4.m[0][2] = -std::sin(radian);
	m4.m[2][0] = std::sin(radian);
	m4.m[3][3] = 1;
	return m4;
};

// 3. Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 m4;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m4.m[i][j] = 0;
		}
	}
	m4.m[0][0] = std::cos(radian);
	m4.m[2][2] = 1;
	m4.m[1][1] = std::cos(radian);
	m4.m[1][0] = -std::sin(radian);
	m4.m[0][1] = std::sin(radian);
	m4.m[3][3] = 1;
	return m4;
};;

// 3. 座標返還
Vector3 MakeTransform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
};


// 4. 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	float m4s;
	Matrix4x4 m4;
	m4s =
		m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]
		- m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]
		+ m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];
	m4.m[0][0] = 1 / m4s * (
		m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]
		);
	m4.m[0][1] = 1 / m4s * (
		-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);
	m4.m[0][2] = 1 / m4s * (
		m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);
	m4.m[0][3] = 1 / m4s * (
		-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);

	//2
	m4.m[1][0] = 1 / m4s * (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2]
		+ m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);
	m4.m[1][1] = 1 / m4s * (+m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);
	m4.m[1][2] = 1 / m4s * (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2]
		+ m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);
	m4.m[1][3] = 1 / m4s * (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);



	m4.m[2][0] = 1 / m4s * (
		m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1]
		- m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]
		);
	m4.m[2][1] = 1 / m4s * (
		-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1]
		+ m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);
	m4.m[2][2] = 1 / m4s * (
		m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1]
		- m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);
	m4.m[2][3] = 1 / m4s * (
		-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1]
		+ m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);


	m4.m[3][0] = 1 / m4s * (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1]
		+ m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);
	m4.m[3][1] = 1 / m4s * (+m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1]
		- m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);
	m4.m[3][2] = 1 / m4s * (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1]
		+ m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);
	m4.m[3][3] = 1 / m4s * (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1]
		- m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);

	return m4;

}

// 1. 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 m4;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m4.m[i][j] = 0;
		}
	}
	m4.m[0][0] = 1 / aspectRatio * (1 / std::tan(fovY / 2));
	m4.m[1][1] = 1 / std::tan(fovY / 2);
	m4.m[2][2] = farClip / (farClip - nearClip);
	m4.m[2][3] = 1;
	m4.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);


	return m4;
};

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
		Matrix4x4 m4;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m4.m[i][j] = 0;
			}
		}
		m4.m[0][0] = width / 2;
		m4.m[1][1] = -(height / 2);
		m4.m[2][2] = maxDepth - minDepth;
		m4.m[3][0] = left + (width / 2);
		m4.m[3][1] = top + (height / 2);
		m4.m[3][2] = minDepth;
		m4.m[3][3] = 1;

		return m4;
}

// 2. 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 m4;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m4.m[i][j] = 0;
		}
	}
	m4.m[0][0] = 2 / (right - left);
	m4.m[1][1] = 2 / (top - bottom);
	m4.m[2][2] = 1 / (farClip - nearClip);
	m4.m[3][0] = (left + right) / (left - right);
	m4.m[3][1] = (top + bottom) / (bottom - top);
	m4.m[3][2] = nearClip / (nearClip - farClip);
	m4.m[3][3] = 1;


	return m4;
};

// 2. 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {

	Matrix4x4 m4;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m4.m[i][j] = 0;
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				m4.m[i][j] = 1.0f;
			}
		}
	}
	m4.m[0][0] = scale.x;
	m4.m[1][1] = scale.y;
	m4.m[2][2] = scale.z;
	return m4;

};

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 m4;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m4.m[i][j] = 0;
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				m4.m[i][j] = 1.0f;
			}
		}
	}
	m4.m[3][0] = translate.x;
	m4.m[3][1] = translate.y;
	m4.m[3][2] = translate.z;
	return m4;

};

Vector3 Add(const Vector3& posa, const Vector3& posb) {
	Vector3 AddPos;

	AddPos.x = posa.x +
		posb.x;
	AddPos.y = posa.y + posb.y;
	AddPos.z = posa.z + posb.z;

	return AddPos;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

Vector2 Subtract(const Vector2& posa, const Vector2& posb)
{
	Vector2 subTractPos;
	subTractPos.x = posa.x - posb.x;
	subTractPos.y = posa.y - posb.y;

	return subTractPos;
}

Vector3 Subtract(const Vector3& posa, const Vector3& posb)
{
	Vector3 AddPos;

	AddPos.x = posa.x - posb.x;
	AddPos.y = posa.y - posb.y;
	AddPos.z = posa.z - posb.z;

	return AddPos;
}

Vector2 Normalize(const Vector2& v)
{
	Vector2 m2;
	float mag = 1 / sqrtf(v.x * v.x + v.y * v.y);
	m2 = { v.x * mag, v.y * mag };

	return m2;
}

// 正規化
Vector3 Normalize(const Vector3& v) {
	Vector3 m3;
	float mag = 1 / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	m3 = { v.x * mag, v.y * mag, v.z * mag };

	return m3;
}

Quaternion Normalize(Quaternion quaternion) {

	Quaternion m3;
	float mag = 1 / sqrtf(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
	m3 = { quaternion.x * mag,quaternion.y * mag,quaternion.z * mag,quaternion.w * mag };
	return m3;

}

float Lerp(const float& v1, const float& v2, float t) {
	float p;
	p = v1 + t * (v2 - v1);
	return p;
}
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 p;
	p.x = v1.x + t * (v2.x - v1.x);
	p.y = v1.y + t * (v2.y - v1.y);
	p.z = v1.z + t * (v2.z - v1.z);
	return p;
};
Quaternion Lerp(const Quaternion& v1, const Quaternion& v2, float t) {
	Quaternion p;
	p.x = v1.x + t * (v2.x - v1.x);
	p.y = v1.y + t * (v2.y - v1.y);
	p.z = v1.z + t * (v2.z - v1.z);
	p.w = v1.w + t * (v2.w - v1.w);
	return p;
};
// 内績
float Dot(const Vector3& v1, const Vector3& v2) {
	float m3;
	m3 = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return m3;
};
//内績
float DotQuaternion(const Quaternion& v1, const Quaternion& v2) {
	float m3;
	m3 = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;


	return m3;
};

Vector3 SLerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 p;

	Vector3 s;
	Vector3 e;

	s = Normalize(v1);
	e = Normalize(v2);
	float angle = acos(Dot(s, e));
	if (angle < std::numeric_limits<float>::epsilon()) {
		return s;
	}
	// SinΘ
	float SinTh = sin(angle);

	// 補間係数
	float Ps = sin(angle * (1 - t));
	float Pe = sin(angle * t);

	p.x = (Ps * s.x + Pe * e.x) / SinTh;
	p.y = (Ps * s.y + Pe * e.y) / SinTh;
	p.z = (Ps * s.z + Pe * e.z) / SinTh;

	p = Normalize(p);


	return p;
}
Quaternion SLerp(const Quaternion& v1, const Quaternion& v2, float t)
{

	Quaternion result;
	float dot = DotQuaternion(v1, v2);
	Quaternion q00 = v1;
	if (dot < 0) {
		q00 = { -v1.x,-v1.y ,-v1.z ,-v1.w };
		dot = -dot;
	}

	// なす角を求める
	float thate = std::acos(dot);
	if (dot >= 1.0f - std::numeric_limits<float>::epsilon()) {
		Quaternion eResult;
		eResult.x = (1.0f - t) * v1.x + t * v2.x;
		eResult.y = (1.0f - t) * v1.y + t * v2.y;
		eResult.z = (1.0f - t) * v1.z + t * v2.z;
		eResult.w = (1.0f - t) * v1.w + t * v2.w;

		return eResult;
	}

	result.x = (sin((1 - t) * thate) / sin(thate)) * q00.x + sin(t * thate) / sin(thate) * v2.x;
	result.y = (sin((1 - t) * thate) / sin(thate)) * q00.y + sin(t * thate) / sin(thate) * v2.y;
	result.z = (sin((1 - t) * thate) / sin(thate)) * q00.z + sin(t * thate) / sin(thate) * v2.z;
	result.w = (sin((1 - t) * thate) / sin(thate)) * q00.w + sin(t * thate) / sin(thate) * v2.w;
	


	return result;
}
;

Vector3 CalculateValue(const std::vector<KeyFrameVector3>& keyframes, float time)
{
	assert(!keyframes.empty());// キーがないものは返す値がわからないのでダメ
	if (keyframes.size() == 1 || time <=keyframes[0].time) { // キーが1つか、時刻がキーフレーム前なら最初の値とする
		return keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を保管する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}
	// ここまできた場合は一番後の時刻よりも後ろなので最後の値を返すことにする
	return (*keyframes.rbegin()).value;
};

Quaternion CalculateValue(const std::vector<KeyFrameQuaternion>& keyframes, float time)
{
	assert(!keyframes.empty());// キーがないものは返す値がわからないのでダメ
	if (keyframes.size() == 1 || time <= keyframes[0].time) { // キーが1つか、時刻がキーフレーム前なら最初の値とする
		return keyframes[0].value;
	}
	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
		size_t nextIndex = index + 1;
		// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を保管する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			return SLerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}
	// ここまできた場合は一番後の時刻よりも後ろなので最後の値を返すことにする
	return (*keyframes.rbegin()).value;
};

// 5. 転置行列
Matrix4x4 Transpose(const Matrix4x4 m) {

	Matrix4x4 m4;
	m4.m[0][0] = m.m[0][0];
	m4.m[0][1] = m.m[1][0];
	m4.m[0][2] = m.m[2][0];
	m4.m[0][3] = m.m[3][0];

	m4.m[1][0] = m.m[0][1];
	m4.m[1][1] = m.m[1][1];
	m4.m[1][2] = m.m[2][1];
	m4.m[1][3] = m.m[3][1];

	m4.m[2][0] = m.m[0][2];
	m4.m[2][1] = m.m[1][2];
	m4.m[2][2] = m.m[2][2];
	m4.m[2][3] = m.m[3][2];

	m4.m[3][0] = m.m[0][3];
	m4.m[3][1] = m.m[1][3];
	m4.m[3][2] = m.m[2][3];
	m4.m[3][3] = m.m[3][3];
	return m4;


};

// 3. 座標返還
Vector3 Transform1(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
		1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
		1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
		1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
		1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

//長さ（ノルム）
float Length(const Vector3& v) {

	float m3 = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);


	return m3;
};

bool IsCollisionAABB(const Vector3& AABBPos, const Vector3& AABBMin, const Vector3& AABBMax, const Vector3& spherePos,const float& radius) {
	bool g = false;
	AABBPos;
	// 最近接点を求める
	Vector3 closestPoint{
		std::clamp(spherePos.x,AABBMin.x ,AABBMax.x),
		std::clamp(spherePos.y,AABBMin.y, AABBMax.y),
		std::clamp(spherePos.z,AABBMin.z, AABBMax.z)
	};

	// 最近接点と弾の中心との距離を求める
	float distance = Length({
		closestPoint.x - spherePos.x,
		closestPoint.y - spherePos.y,
		closestPoint.z - spherePos.z });
	// 距離が半径よりも小さければ衝突
	if (distance <= radius) {
		g = true;
	}
	else { g = false; }
	return g;
}

float LerpShortAngle(float a, float b, float t)
{
	// 角度差分を求める
	float diff = b - a;

	float Pi2 = fmod(diff, (float)std::numbers::pi * 2.0f);

	if (Pi2 <= -(float)std::numbers::pi) {
		Pi2 += (float)std::numbers::pi * 2.0f;
	}
	else if ((float)std::numbers::pi <= Pi2) {
		Pi2 -= (float)std::numbers::pi * 2.0f;
	}

	float afterAngle = a + Pi2 * t;
	return afterAngle;
}

// from, toは正規化済みベクトルを想定
Matrix4x4 MakeRotateMatrixFromTo(const Vector3& from, const Vector3& to) {
	Vector3 v = {
		from.y * to.z - from.z * to.y,
		from.z * to.x - from.x * to.z,
		from.x * to.y - from.y * to.x
	};
	float s = sqrtf((1 + from.x * to.x + from.y * to.y + from.z * to.z) * 2.0f);
	if (s < 1e-6f) {
		// ほぼ逆向きの場合は180度回転
		// 任意の直交ベクトルで回転
		Vector3 ortho = { 1, 0, 0 };
		if (fabsf(from.x) > 0.99f) ortho = { 0, 1, 0 };
		v = {
			from.y * ortho.z - from.z * ortho.y,
			from.z * ortho.x - from.x * ortho.z,
			from.x * ortho.y - from.y * ortho.x
		};
		float vx = v.x, vy = v.y, vz = v.z;
		float vx2 = vx * vx, vy2 = vy * vy, vz2 = vz * vz;
		Matrix4x4 m = {};
		m.m[0][0] = 1 - 2 * (vy2 + vz2);
		m.m[0][1] = 2 * (vx * vy);
		m.m[0][2] = 2 * (vx * vz);
		m.m[0][3] = 0;
		m.m[1][0] = 2 * (vx * vy);
		m.m[1][1] = 1 - 2 * (vx2 + vz2);
		m.m[1][2] = 2 * (vy * vz);
		m.m[1][3] = 0;
		m.m[2][0] = 2 * (vx * vz);
		m.m[2][1] = 2 * (vy * vz);
		m.m[2][2] = 1 - 2 * (vx2 + vy2);
		m.m[2][3] = 0;
		m.m[3][0] = m.m[3][1] = m.m[3][2] = 0;
		m.m[3][3] = 1;
		return m;
	}
	float invs = 1.0f / s;
	float vx = v.x * invs, vy = v.y * invs, vz = v.z * invs;
	float c = (1 + from.x * to.x + from.y * to.y + from.z * to.z) * 0.5f;
	float k = 1.0f - c;
	Matrix4x4 m = {};
	m.m[0][0] = c + vx * vx * k;
	m.m[0][1] = vx * vy * k - vz * s * 0.5f;
	m.m[0][2] = vx * vz * k + vy * s * 0.5f;
	m.m[0][3] = 0;
	m.m[1][0] = vy * vx * k + vz * s * 0.5f;
	m.m[1][1] = c + vy * vy * k;
	m.m[1][2] = vy * vz * k - vx * s * 0.5f;
	m.m[1][3] = 0;
	m.m[2][0] = vz * vx * k - vy * s * 0.5f;
	m.m[2][1] = vz * vy * k + vx * s * 0.5f;
	m.m[2][2] = c + vz * vz * k;
	m.m[2][3] = 0;
	m.m[3][0] = m.m[3][1] = m.m[3][2] = 0;
	m.m[3][3] = 1;
	return m;
}

// 回転行列からXYZ順のオイラー角を抽出
Vector3 ExtractEulerAngles(const Matrix4x4& m) {
	Vector3 angles;
	// Y軸が±90度付近でGimbal Lock
	if (fabsf(m.m[2][0]) < 1.0f - 1e-6f) {
		angles.y = asinf(-m.m[2][0]);
		angles.x = atan2f(m.m[2][1], m.m[2][2]);
		angles.z = atan2f(m.m[1][0], m.m[0][0]);
	}
	else {
		// Gimbal Lock
		angles.y = asinf(-m.m[2][0]);
		angles.x = 0;
		angles.z = atan2f(-m.m[0][1], m.m[1][1]);
	}
	return angles;
}


// 演算子のオーバーロード
Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b)
{
	return Multiply(a, b);
}

Vector3 operator+(const Vector3& a, const Vector3& b) {
	Vector3 c = { a.x + b.x,a.y + b.y ,a.z + b.z };

	return c;
}

Vector3 operator+(const Vector3& a, const float& b) {
	Vector3 c = { a.x + b,a.y + b,a.z + b };

	return c;
}

Vector3 operator+(const float& a, const Vector3& b) {
	Vector3 c = { b.x + a, b.y + a, b.z + a };

	return c;
}

Vector3 operator-(const Vector3& a, const Vector3& b) {
	Vector3 c = { a.x - b.x,a.y - b.y,a.z - b.z };

	return c;
}

Vector3 operator-(const Vector3& a, const float& b) {
	Vector3 c = { a.x - b,a.y - b,a.z - b };

	return c;
}

Vector3 operator*(const float& a, const Vector3& b) {
	Vector3 c = { a * b.x,a * b.y,a * b.z };

	return c;
}

Vector3 operator*(const Vector3& a, const float& b)
{
	Vector3 c = { a.x * b, a.y * b, a.z * b };

	return c;
}

Vector3 operator/(const Vector3& a, const float& b) {
	Vector3 c = { a.x / b , a.y / b, a.z / b };

	return c;
}

Vector3 operator*(const Vector3& vec, const Matrix4x4& mat) {
	Vector4 result = {
		vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + mat.m[3][0],
		vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + mat.m[3][1],
		vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + mat.m[3][2],
		vec.x * mat.m[0][3] + vec.y * mat.m[1][3] + vec.z * mat.m[2][3] + mat.m[3][3]
	};

	return { result.x / result.w, result.y / result.w, result.z / result.w };
}

Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
	Vector2 result = {
		v1.x - v2.x,
		v1.y - v2.y
	};

	return result;
}

Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
	Vector2 c = { v1.x + v2.x, v1.y + v2.y };

	return c;
}

Vector2 operator*(const Vector2& v1, const float& a)
{
	Vector2 c = { v1.x * a, v1.y * a };

	return c;
}

Vector2 operator*(const float& a, const Vector2& v1)
{
	Vector2 c = { a * v1.x, a * v1.y };

	return c;
}
