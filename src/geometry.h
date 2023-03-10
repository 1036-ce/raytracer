#pragma once
#include <cmath>
#include <cassert>
#include <iostream>

template<int n> class vec
{
public:
	vec() = default;
	double & operator[](const int i) 	   { assert(i >= 0 && i < n); return data[i]; }
	double   operator[](const int i) const { assert(i >= 0 && i < n); return data[i]; }
	double 	 norm2() const { return *this * *this; }
	double 	 norm()  const { return std::sqrt(norm2()); }
private:
	double data[n] = {0};
};

template<int n> vec<n> operator*(const vec<n>& lhs, const vec<n>& rhs) {
	vec<n> ret;
	for (int i = n; i--; ret[i] = lhs[i] * rhs[i]);
	return ret;
}

template<int n> vec<n> operator*(const vec<n>& lhs, const double& rhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] *= rhs);
	return ret;
}

template<int n> vec<n> operator*(const double& lhs, const vec<n>& rhs) {
	vec<n> ret = rhs;
	for (int i = n; i--; ret[i] *= lhs);
	return ret;
}

template<int n> vec<n> operator/(const vec<n>& lhs, const double rhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] /= rhs);
	return ret;
}

template<int n> vec<n> operator+(const vec<n>& lhs, const vec<n>& rhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] += rhs[i]);
	return ret;
}

template<int n> vec<n> operator+(const vec<n>& lhs, const double& rhs) {
	vec<n> ret = lhs;
	for (int i = n; i--; ret[i] += rhs);
	return ret;
}

template<int n> vec<n> operator+(const double& lhs, const vec<n>& rhs) {
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

template<int n1, int n2> vec<n1> embed(const vec<n2> v, double fill = 1) {
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
	double x, y;
	vec() = default;
	vec(double val) : x(val), y(val) {}
	vec(double x, double y): x(x), y(y) {}
	double & operator[](const int i)	   { assert(i == 0 || i == 1); return i == 0 ? x : y;}
	double 	 operator[](const int i) const { assert(i == 0 || i == 1); return i == 0 ? x : y;}
	vec   operator-() { return vec(-x, -y); }
	vec&  operator=(const vec& v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	double norm2() const { return x * x + y * y; }
	double norm()  const { return std::sqrt(norm2()); }
	vec normalize() const { return *this / norm();}
};

template<> class vec<3>
{
public:
	double x, y, z;
	vec() = default;
	explicit vec(double val) : x(val), y(val), z(val) {}
	vec(double x, double y, double z): x(x), y(y), z(z) {}
	vec(const vec<4>& v);
	double & operator[](const int i) 	   { assert(i >= 0 && i < 3); return i ? (1 == i ? y : z) : x;}
	double 	 operator[](const int i) const { assert(i >= 0 && i < 3); return i ? (1 == i ? y : z) : x;}
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
	double norm2() const { return x * x + y * y + z * z; }
	double norm()  const { return std::sqrt(norm2()); }
	vec normalize() const { return *this / norm();}
};

template<> class vec<4>
{
public:
	double x, y, z, w;
	vec() = default;
	vec(double val) : x(val), y(val), z(val), w(val) {}
	vec(double x, double y, double z, double w): x(x), y(y), z(z), w(w) {}
	vec(const vec<3> &v, double w_): x(v.x), y(v.y), z(v.z), w(w_) {}
	double & operator[](const int i) {
		 assert(i >= 0 && i < 4);  
		 if (i == 0)	return x;
		 else if (i == 1)	return y;
		 else if (i == 2)	return z;
		 else	return w;
	 }
	double 	 operator[](const int i) const { 
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
	double norm2() const { return x*x + y*y + z*z + w*w; }
	double norm()  const { return std::sqrt(norm2()); }
	vec normalize() const { return *this / norm(); }
};

using vec2 = vec<2>;
using vec3 = vec<3>;
using vec4 = vec<4>;

double dot(const vec2& v1, const vec2& v2);
double dot(const vec3& v1, const vec3& v2);
double dot(const vec4& v1, const vec4& v2);
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

	double det() const {
		return dt<ncols>::det(*this);
	}

	mat<nrows-1, ncols-1> get_minor(const int row, const int col) const {
		mat<nrows-1, ncols-1> ret;
		for (int i = nrows - 1; i--; )
			for (int j = ncols - 1; j--; ret[i][j] = rows[i<row?i:(i+1)][j<col?j:(j+1)]);
		return ret;
	}

	// ???????????????
	double cofactor(const int row, const int col) const {
		return get_minor(row, col).det() * ((row + col) % 2 ? -1 : 1);
	}

	// ?????????????????????
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

	// ?????????
	mat<nrows, ncols> invert() const {
		return invert_transpose().transpose();
	}

	// ????????????
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
mat<nrows, ncols> operator*(const mat<nrows, ncols>& lhs, const double& val) {
	mat<nrows, ncols> ret;
	for (int i = nrows; i--; ret[i] = lhs[i] * val);
	return ret;
}

template<int nrows, int ncols>
mat<nrows, ncols> operator*(const double& val,  const mat<nrows, ncols>& rhs) {
	mat<nrows, ncols> ret;
	for (int i = nrows; i--; ret[i] = rhs[i] * val);
	return ret;
}


template<int nrows, int ncols>
mat<nrows, ncols> operator/(const mat<nrows, ncols>& lhs, const double& val) {
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
	static double det(const mat<n, n>& m) {
		double ret = 0;
		for (int i = n; i--; ret += (m[0][i] * m.cofactor(0, i)) );
		return ret;
	}
};

template<> struct dt<1>
{
	static double det(const mat<1,1>& m) {
		return m[0][0];
	}
};

inline vec<3>::vec(const vec<4> &v): x(v.x), y(v.y), z(v.z) {}
