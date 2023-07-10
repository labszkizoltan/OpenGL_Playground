#pragma once
#include <iostream>

struct Vec3D
{
	float x, y, z;
	Vec3D();
	Vec3D(float _x, float _y, float _z);

	Vec3D& Add(const Vec3D& vec);
	Vec3D& Subtract(const Vec3D& vec);
	Vec3D& Multiply(const Vec3D& vec);
	Vec3D& Divide(const Vec3D& vec);

	friend Vec3D& operator+(Vec3D& v1, const Vec3D& v2);
	friend Vec3D& operator-(Vec3D& v1, const Vec3D& v2);
	friend Vec3D& operator*(Vec3D& v1, const Vec3D& v2);
	friend Vec3D& operator/(Vec3D& v1, const Vec3D& v2);

	Vec3D& operator+=(const Vec3D& vec);
	Vec3D& operator-=(const Vec3D& vec);
	Vec3D& operator*=(const Vec3D& vec);
	Vec3D& operator/=(const Vec3D& vec);
	
	
};

std::ostream& operator<<(std::ostream& stream, const Vec3D& vec)
{
	stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return stream;
}


Vec3D::Vec3D()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Vec3D::Vec3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

Vec3D& Vec3D::Add(const Vec3D& vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;
	return *this;
}

Vec3D& Vec3D::Subtract(const Vec3D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;
	return *this;
}

Vec3D& Vec3D::Multiply(const Vec3D& vec)
{
	this->x *= vec.x;
	this->y *= vec.y;
	this->z *= vec.z;
	return *this;
}

Vec3D& Vec3D::Divide(const Vec3D& vec)
{
	this->x /= vec.x;
	this->y /= vec.y;
	this->z /= vec.z;
	return *this;
}

Vec3D& operator+(Vec3D& v1, const Vec3D& v2)
{
	return v1.Add(v2);
}

Vec3D& operator-(Vec3D& v1, const Vec3D& v2)
{
	return v1.Subtract(v2);
}

Vec3D& operator*(Vec3D& v1, const Vec3D& v2)
{
	return v1.Multiply(v2);
}

Vec3D& operator/(Vec3D& v1, const Vec3D& v2)
{
	return v1.Divide(v2);
}

Vec3D& Vec3D::operator+=(const Vec3D& vec)
{
	return this->Add(vec);
}

Vec3D& Vec3D::operator-=(const Vec3D& vec)
{
	return this->Subtract(vec);
}

Vec3D& Vec3D::operator*=(const Vec3D& vec)
{
	return this->Multiply(vec);
}

Vec3D& Vec3D::operator/=(const Vec3D& vec)
{
	return this->Divide(vec);
}









