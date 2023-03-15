#pragma once
#include <cmath>
#include <cassert>
#include <iostream>
#include "global.h"

template<int n> class vec
{
public:
	vec() = default;
	Float& operator[](const int i) 	   { assert(i >= 0 && i < n); return data[i]; }
	Float operator[](const int i) const { assert(i >= 0 && i < n); return data[i]; }
	Float norm2() const { return *this * *this; }
	Float norm()  const { return std::sqrt(norm2()); }
private:
	Float data[n] = {0};
};

template<int n> vec<n> operator*(const vec<n>& lhs, const vec<n>& rhs) {
	vec<n> ret;
	for (int i = n; i--; ret[i] = lhs[i] * rhs[i]);
	return ret;
}

template<int n> vec<n> operator*(const vec<n>& lhs, const Float & rhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] *= rhs);
	return ret;
}

template<int n> vec<n> operator*(const Float & lhs, const vec<n>& rhs) {
	vec<n> ret = rhs;
	for (int i = n; i--; ret[i] *= lhs);
	return ret;
}

template<int n> vec<n> operator/(const vec<n>& lhs, const Float rhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] /= rhs);
	return ret;
}

template<int n> vec<n> operator+(const vec<n>& lhs, const vec<n>& rhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] += rhs[i]);
	return ret;
}

template<int n> vec<n> operator+(const vec<n>& lhs, const Float & rhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] += rhs);
	return ret;
}

template<int n> vec<n> operator+(const Float & lhs, const vec<n>& rhs) {
	vec<n> ret = rhs;
	for (int i = n; i--; ret[i] += lhs);
	return ret;
}

template<int n> vec<n> operator-(const vec<n>& lhs, const vec<n>& rhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] -= rhs[i]);
	return ret;
}

template<int n> std::ostream& operator<<(std::ostream& out, const vec<n> rhs) {
	for (int i = 0; i < n; ++i)
		out << rhs[i] << ' ';
	return out;
}

template<int n1, int n2> vec<n1> embed(const vec<n2> v, Float fill = 1) {
	vec<n1> ret;
	for (int i = n1; i--; ret[i] = (i < n2) ? v[i] : fill);
	return ret;
}

template<int n1, int n2> vec<n1> proj(const vec<n2> v) {
	vec<n1> ret;
	for (int i = n1; i--; ret[i] = v[i]);
	return ret;
}

template<> class vec<2>
{
public:
	Float x, y;
	vec() = default;
	vec(Float val) : x(val), y(val) {}
	vec(Float x, Float y): x(x), y(y) {}
	Float & operator[](const int i)	   { assert(i == 0 || i == 1); return i == 0 ? x : y;}
	Float operator[](const int i) const { assert(i == 0 || i == 1); return i == 0 ? x : y;}
	vec   operator-() { return vec(-x, -y); }
	vec&  operator=(const vec& v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	Float norm2() const { return x * x + y * y; }
	Float norm()  const { return std::sqrt(norm2()); }
	vec normalize() const { return *this / norm();}
};

template<> class vec<3>
{
public:
	Float x, y, z;
	vec() = default;
	explicit vec(Float val) : x(val), y(val), z(val) {}
	vec(Float x, Float y, Float z): x(x), y(y), z(z) {}
	vec(const vec<4>& v);
	Float & operator[](const int i) 	   { assert(i >= 0 && i < 3); return i ? (1 == i ? y : z) : x;}
	Float operator[](const int i) const { assert(i >= 0 && i < 3); return i ? (1 == i ? y : z) : x;}
	vec   operator-() const { return vec(-x, -y, -z); }
	vec&  operator=(const vec& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	vec& operator+=(const vec& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	vec& operator-=(const vec& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	Float norm2() const { return x * x + y * y + z * z; }
	Float norm()  const { return std::sqrt(norm2()); }
	vec normalize() const { return *this / norm();}
};

template<> class vec<4>
{
public:
	Float x, y, z, w;
	vec() = default;
	vec(Float val) : x(val), y(val), z(val), w(val) {}
	vec(Float x, Float y, Float z, Float w): x(x), y(y), z(z), w(w) {}
	vec(const vec<3> &v, Float w_): x(v.x), y(v.y), z(v.z), w(w_) {}
	Float & operator[](const int i) {
		 assert(i >= 0 && i < 4);  
		 if (i == 0)	return x;
		 else if (i == 1)	return y;
		 else if (i == 2)	return z;
		 else	return w;
	 }
	Float operator[](const int i) const { 
		assert(i >= 0 && i < 4); 
		 if (i == 0)	return x;
		 else if (i == 1)	return y;
		 else if (i == 2)	return z;
		 else	return w;
	}
	vec   operator-() { return vec(-x, -y, -z, -w); }
	vec&  operator=(const vec& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}
	Float norm2() const { return x*x + y*y + z*z + w*w; }
	Float norm()  const { return std::sqrt(norm2()); }
	vec normalize() const { return *this / norm(); }
};

using vec2 = vec<2>;
using vec3 = vec<3>;
using vec4 = vec<4>;

Float dot(const vec2& v1, const vec2& v2);
Float dot(const vec3& v1, const vec3& v2);
Float dot(const vec4& v1, const vec4& v2);
vec3 cross(const vec3& v1, const vec3& v2);


template<int n> struct dt;

template<int nrows, int ncols> class mat
{
public:
		  vec<ncols> & operator[](const int i)       { assert(i >= 0 && i < nrows); return rows[i]; }
	const vec<ncols> & operator[](const int i) const { assert(i >= 0 && i < nrows); return rows[i]; }
	vec<nrows> col(const int idx) const {
		assert(idx >= 0 && idx < ncols);
		vec<nrows> ret;
		for (int i = nrows; i--; ret[i] = rows[i][idx]);
		return ret;
	}
	void set_col(const int idx, vec<nrows> v) {
		assert(idx >= 0 && idx < ncols);
		for (int i = nrows; i--; rows[i][idx] = v[i]);
	}

	/**
	 * @brief return a identity matrix
	 * 
	 * @return mat<nrows, ncols> 
	 */
	static mat<nrows, ncols> identity() {
		mat<nrows, ncols> ret;
		for (int i = nrows; i--; )
			for (int j = ncols; j--; ret[i][j] = (i == j));
		return ret;
	}

	Float det() const {
		return dt<ncols>::det(*this);
	}

	mat<nrows-1, ncols-1> get_minor(const int row, const int col) const {
		mat<nrows-1, ncols-1> ret;
		for (int i = nrows - 1; i--; )
			for (int j = ncols - 1; j--; ret[i][j] = rows[i<row?i:(i+1)][j<col?j:(j+1)]);
		return ret;
	}

	// 代数余子式
	Float cofactor(const int row, const int col) const {
		return get_minor(row, col).det() * ((row + col) % 2 ? -1 : 1);
	}

	// 伴随矩阵的转置
	// adj(A^T)_{ij} = C_{ji};
	mat<nrows, ncols> adjugate() const {
		mat<nrows, ncols> ret;
		for (int i = nrows; i--;)
			for (int j = ncols; j--; ret[i][j] = cofactor(i, j));
		return ret;		
	}

	mat<ncols, nrows> invert_transpose() const {
		mat<ncols, nrows> ret = adjugate();
		return ret / (dot(ret[0], rows[0])); 	// dot(ret[0], rows[0]) == det(A)
	}

	// 逆矩阵
	mat<nrows, ncols> invert() const {
		return invert_transpose().transpose();
	}

	// 转置矩阵
	mat<ncols, nrows> transpose() const {
		mat<ncols, nrows> ret;
		for (int i = ncols; i--; ret[i] = this->col(i));
		return ret;
	}

private:
	vec<ncols> rows[nrows] = {{}};
};

template<int R1, int C1, int C2> 
mat<R1,C2> operator*(const mat<R1, C1>& lhs, const mat<C1, C2>& rhs) {
	mat<R1, C2> ret;
	for (int i = R1; i--;)
		for (int j = C2; j--; ret[i][j] = dot(lhs[i], rhs.col(j)));
	return ret;
}

template<int nrows, int ncols>
vec<nrows> operator*(const mat<nrows, ncols>& lhs, const vec<ncols>& rhs) {
	vec<nrows> ret;
	for (int i = nrows; i--; ret[i] = dot(lhs[i], rhs));
	return ret;
}

template<int nrows, int ncols>
mat<nrows, ncols> operator*(const mat<nrows, ncols>& lhs, const Float & val) {
	mat<nrows, ncols> ret;
	for (int i = nrows; i--; ret[i] = lhs[i] * val);
	return ret;
}

template<int nrows, int ncols>
mat<nrows, ncols> operator*(const Float & val,  const mat<nrows, ncols>& rhs) {
	mat<nrows, ncols> ret;
	for (int i = nrows; i--; ret[i] = rhs[i] * val);
	return ret;
}


template<int nrows, int ncols>
mat<nrows, ncols> operator/(const mat<nrows, ncols>& lhs, const Float & val) {
	mat<nrows, ncols> ret;
	for (int i = nrows; i--; ret[i] = lhs[i] / val);
	return ret;
}

template<int nrows, int ncols>
mat<nrows, ncols> operator+(const mat<nrows, ncols>& lhs, const mat<nrows, ncols>& rhs) {
	mat<nrows, ncols> ret;
	for (int i = nrows; i--; ret[i] = lhs[i] + rhs[i]);
	return ret;
}

template<int nrows, int ncols>
mat<nrows, ncols> operator-(const mat<nrows, ncols>& lhs, const mat<nrows, ncols>& rhs) {
	mat<nrows, ncols> ret;
	for (int i = nrows; i--; ret[i] = lhs[i] - rhs[i]);
	return ret;
}

template<int nrows, int ncols>
std::ostream& operator<<(std::ostream& out, const mat<nrows, ncols>& m) {
	for (int i = 0; i < nrows; ++i)
		out << m[i] << "\n";
	return out;
}

using mat4 = mat<4, 4>;
using mat3 = mat<3, 3>;

template<int n> struct dt 
{
	static Float det(const mat<n, n>& m) {
		Float ret = 0;
		for (int i = n; i--; ret += (m[0][i] * m.cofactor(0, i)) );
		return ret;
	}
};

template<> struct dt<1>
{
	static Float det(const mat<1,1>& m) {
		return m[0][0];
	}
};

inline vec<3>::vec(const vec<4> &v): x(v.x), y(v.y), z(v.z) {}

class Point2 {
public:
	Float x, y;
	Point2() : x(0), y(0) {}
	Point2(Float v) : x(v), y(v) {}
	Point2(Float xx, Float yy) : x(xx), y(yy) {}

	Point2 operator+=(const vec2& v) {
		x += v.x; y += v.y;
		return *this;
	}

	Point2 operator*=(Float v) {
		x *= v; y *= v;
		return *this;
	}

	Point2 operator/=(Float v) {
		x /= v; y /= v;
		return *this;
	}
private:
};

inline Point2 operator+(const Point2& p1, const Point2& p2) {
	return Point2(p1.x + p2.x, p1.y + p2.y);
}

inline Point2 operator+(const Point2& p, const vec2& v) {
	return Point2(p.x + v.x, p.y + v.y);
}

inline Point2 operator+(const vec2& v, const Point2& p) {
	return Point2(p.x + v.x, p.y + v.y);
}

inline vec2 operator-(const Point2& p1, const Point2& p2) {
	return vec2(p1.x - p2.x, p1.y - p2.y);
}

inline Point2 operator*(const Point2& p1, Float v) {
	return Point2(p1.x * v, p1.y * v);
}

inline Point2 operator*(Float v, const Point2& p1) {
	return Point2(p1.x * v, p1.y * v);
}

inline Point2 operator/(const Point2& p1, Float v) {
	Float inv = (Float)1 / v;
	return Point2(p1.x * inv, p1.y * inv);
}

class Point3 {
public:
	Float x, y, z;
	Point3() : x(0), y(0), z(0) {}
	explicit Point3(Float v) : x(v), y(v), z(v) {}
	Point3(Float xx, Float yy, Float zz)
		: x(xx), y(yy), z(zz) {}

	static Float distance(const Point3& p1, const Point3& p2) {
		vec3 v(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
		return v.norm();
	}

	static Float distance_squard(const Point3& p1, const Point3& p2) {
		vec3 v(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
		return v.norm2();
	}

	Float operator[](int idx) const {
		if (idx < 0 || idx > 2)
			std::abort();
		if (idx == 0) return x;
		if (idx == 1) return y;
		return z;
	}
	
	Float& operator[](int idx) {
		if (idx < 0 || idx > 2)
			std::abort();
		if (idx == 0) return x;
		if (idx == 1) return y;
		return z;
	}

	Point3 operator+=(const vec3& v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	Point3 operator*=(Float v) {
		x *= v; y *= v; z *= v;
		return *this;
	}

	Point3 operator/=(Float v) {
		x /= v; y /= v; z /= v;
		return *this;
	}

private:
};

inline Point3 operator+(const Point3& p1, const Point3& p2) {
	return Point3(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

inline Point3 operator+(const Point3& p, const vec3& v) {
	return Point3(p.x + v.x, p.y + v.y, p.z + v.z);
}

inline Point3 operator+(const vec3& v, const Point3& p) {
	return Point3(p.x + v.x, p.y + v.y, p.z + v.z);
}

inline vec3 operator-(const Point3& p1, const Point3& p2) {
	return vec3(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

inline Point3 operator*(const Point3& p1, Float v) {
	return Point3(p1.x * v, p1.y * v, p1.z * v);
}

inline Point3 operator*(Float v, const Point3& p1) {
	return Point3(p1.x * v, p1.y * v, p1.z * v);
}

inline Point3 operator/(const Point3& p1, Float v) {
	Float inv = (Float)1 / v;
	return Point3(p1.x * inv, p1.y * inv, p1.z * inv);
}