#ifndef MATHEMATICS_H_
#define MATHEMATICS_H_

#include <iostream>
#include <cmath>

using namespace std;

const float EPS=0.001f;

//----------------------------------------------------------------------//
//	Deklaracje klas.													//
//----------------------------------------------------------------------//

class Vec3;
class Vec4;
class Mat3;
class Mat4;
class Quat;
class SRT;

//----------------------------------------------------------------------//
//	Prototypy funkcji.													//
//----------------------------------------------------------------------//

inline float ReciprocalSqrt( float x );
inline float dot(Vec3 &v1, Vec3 &v2);
inline float dot(Quat &q1, Quat &q2);
inline Vec3 cross(Vec3 &v1, Vec3 &v2);
inline Vec3 slerp(Vec3 &v1, Vec3 &v2, float t);
inline Quat slerp(Quat v1, Quat v2, float t);

//----------------------------------------------------------------------//
//	Prototypy operatorow.												//
//----------------------------------------------------------------------//

inline Vec3 operator+(Vec3 &lhs, Vec3 &rhs);
inline Vec3 operator-(Vec3 &lhs, Vec3 &rhs);
inline Vec3 operator*(Vec3 &lhs, Quat &rhs);
inline Vec3 operator*(Vec3 &lhs, Mat3 &rhs);
inline Vec3 operator*(Vec3 &lhs, Mat4 &rhs);
inline Vec3 operator*(Vec3 &lhs, float rhs);
inline Vec3 operator/(Vec3 &lhs, float rhs);

inline bool operator==(Vec3 &lhs, Vec3 &rhs);
inline bool operator!=(Vec3 &lhs, Vec3 &rhs);
inline bool operator==(Quat &lhs, Quat &rhs);

inline Vec3 operator*(Mat3 &lhs, Vec3 &rhs);
inline Vec3 operator*(Mat4 &lhs, Vec3 &rhs);
inline Mat3 operator*(Mat3 &lhs, Mat3 &rhs);
inline Mat4 operator*(Mat4 &lhs, Mat4 &rhs);

inline Quat operator+(Quat &lhs, Quat &rhs);
inline Quat operator*(Quat &lhs, Quat &rhs);
inline Quat operator*(Quat &lhs, float rhs);

inline Vec3 operator*(Vec3 &lhs, SRT &rhs);
inline SRT  operator+(SRT &lhs, SRT &rhs);

//----------------------------------------------------------------------//
//	Prototypy klas.														//
//----------------------------------------------------------------------//

class Vec3
{
	public:
		inline Vec3();
		inline Vec3(const Vec3& pattern);
		inline Vec3(float *data);
		inline Vec3(float x, float y, float z);
		inline ~Vec3();

		inline void xyz(float x, float y, float z);
		inline void normalize();
		inline float length();
		inline void rotate(float xAngle, float yAngle, float zAngle);
		inline void rotate(float angle, Vec3 axis);
		inline void wrap(float *data);
		inline void operator+=(Vec3 &rhs);
		inline void operator-=(Vec3 &rhs);
		inline void operator*=(float rhs);
		inline void operator/=(float rhs);
		inline void operator*=(Quat rhs);

		inline float& operator[](int el);

		inline Vec3& operator=(const Vec3 &rhs);

		inline operator double*();
		inline operator float*();

		float *v;
		bool wrapper;
};

class Mat3
{
	public:
		inline Mat3();
		inline Mat3(const Mat3 &pattern);
		inline Mat3(const Mat4 &pattern);

		inline void identity();
		inline void ones();
		inline void zeros();
		inline void inverse();
		inline void transpose();

		inline void decompose(Vec3 &scale, Vec3 &axis, float &angle);
		inline float determinant();
		inline Quat toQuaternion();
		inline Quat toQuaternion2();

		inline void scaling(float x, float y, float z);
		inline void rotationX(float angle);
		inline void rotationY(float angle);
		inline void rotationZ(float angle);
		inline void rotationAxis(float angle, Vec3 axis);

		inline void operator+=(Mat3 &rhs);
		inline void operator-=(Mat3 &rhs);
		inline void operator*=(Mat3 &rhs);
		inline void operator*=(float rhs);
		inline void operator/=(float rhs);

		inline float& operator[](int el);

		inline Mat3& operator=(const Mat3 &rhs);
		inline Mat3& operator=(const Mat4 &rhs);

		float m[9];
};

class Mat4
{
	public:
		inline Mat4();
		inline Mat4(float *data);
		inline Mat4(const Mat3 &pattern);
		inline Mat4(const Mat4 &pattern);
		inline ~Mat4();

		inline void set(Mat4 &pattern);

		inline void identity();
		inline void ones();
		inline void zeros();
		inline void inverse();
		inline void transpose();

		inline float determinant();

		inline void translation(float x, float y, float z);
		inline void scaling(float x, float y, float z);
		inline void rotationX(float angle);
		inline void rotationY(float angle);
		inline void rotationZ(float angle);
		inline void rotationAxis(float angle, Vec3 axis);

		inline void operator+=(Mat4 &rhs);
		inline void operator-=(Mat4 &rhs);
		inline void operator*=(Mat4 &rhs);
		inline void operator*=(float rhs);
		inline void operator/=(float rhs);

		inline float& operator[](int el);

		inline Mat4& operator=(const Mat3 &rhs);
		inline Mat4& operator=(const Mat4 &rhs);

		float *m;
		bool wrapper;
};

class Quat
{
	public:
		inline Quat();
		inline Quat(float x, float y, float z, float w);
		inline Quat(float angle, Vec3 &axis);

		inline void xyzw(float x, float y, float z, float w);
		inline void rotationAxis(float angle, Vec3 axis);
		inline void conjugate();
		inline void normalize();
		inline float length();
		inline Mat3 toMatrix();
		inline Vec3 toEuler();
		inline void fromEuler(Vec3 angles);

		inline float& operator[](int el);

		inline void operator*=(float rhs);
		inline void operator*=(Quat &lhs);

		float q[4];
};

class SRT
{
	public:
		SRT() { s.xyz(1,1,1); }
		SRT(Vec3 &S, Quat &R, Vec3 &T) { s=S; r=R; t=T; }

		void set(Vec3 &S, Quat &R, Vec3 &T) { s=S; r=R; t=T; }
		void setS(Vec3 &S) { s=S; }
		void setR(Quat &R) { r=R; }
		void setT(Vec3 &T) { t=T; }

		void operator+=(SRT &rhs)
		{
			s.v[0]*=rhs.s.v[0]; s.v[1]*=rhs.s.v[1]; s.v[2]*=rhs.s.v[2];
			r*=rhs.r;
			t+=rhs.t;
		}
		void operator-=(SRT &rhs)
		{

		}

		Vec3 s;	//Skalowanie.
		Quat r;	//Rotacja.
		Vec3 t;	//Translacja.
};

//----------------------------------------------------------------------//
//	Definicje metod i operatorow klasy: Vec3.							//
//----------------------------------------------------------------------//
Vec3::Vec3()
{
	wrapper=false;
	v=new float[3];
	v[0]=v[1]=v[2]=0;
}

Vec3::Vec3(const Vec3& pattern)
{
	wrapper=false;
	v=new float[3];
	v[0]=pattern.v[0];
	v[1]=pattern.v[1];
	v[2]=pattern.v[2];
}

Vec3::Vec3(float *data)
{
	wrapper=true;
	v=data;
}

Vec3::Vec3(float x, float y, float z)
{
	wrapper=false;
	v=new float[3];
	v[0]=x; v[1]=y; v[2]=z;
}

Vec3::~Vec3()
{
	if(!wrapper) {delete[] v; v=NULL;}
}

void Vec3::xyz(float x=0, float y=0, float z=0)
{
	v[0]=x; v[1]=y; v[2]=z;
}

void Vec3::normalize()
{
	float len=length();
	if(!len) return;	//Unikniecie dzielenia przez 0.
	v[0]/=len;	v[1]/=len;	v[2]/=len;
}

float Vec3::length()
{
	return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

void Vec3::rotate(float xAngle, float yAngle, float zAngle)
{
	float s=0, c=0, x=0, y=0, z=0;

	if(xAngle)
	{
		xAngle*=0.0174533f;
		s=sin(xAngle);
		c=cos(xAngle);

		y = v[1]*c-v[2]*s;
		z = v[1]*s+v[2]*c;

		v[1]=y; v[2]=z;
	}

	if(yAngle)
	{
		yAngle*=0.0174533f;
		s=sin(yAngle);
		c=cos(yAngle);

		x = v[0]*c + v[2]*s;
		z = v[2]*c - v[0]*s;

		v[0]=x; v[2]=z;
	}

	if(zAngle)
	{
		zAngle*=0.0174533f;
		s = sin(zAngle);
		c = cos(zAngle);

		x = v[0]*c - v[1]*s;
		y = v[0]*s + v[1]*c;

		v[0]=x; v[1]=y;
	}
}

void Vec3::wrap(float *data)
{
	if(!wrapper)
	{
		delete[] v;
		v=data;
		wrapper=true;
	}
	else v=data;
}

void Vec3::rotate(float angle, Vec3 axis)
{
	float s=0, c=0, one_minus_c=0, x=0, y=0, z=0;

	angle*=0.0174533f;	//Zamiana na radiany.
	s=sin(angle);		//sin(angle).
	c=cos(angle);		//cos(angle).
	one_minus_c=1.0f-c;	//1-cos(angle).

	axis.normalize();	//Normalizacja osi obrotu.

	//Pierwsza kolumna.
	x=v[0]*(axis.v[0]*axis.v[0]*one_minus_c+c);				//xx(1-c)+c
	y=v[0]*(axis.v[0]*axis.v[1]*one_minus_c+axis.v[2]*s);	//xy(1-c)+zs
	z=v[0]*(axis.v[0]*axis.v[2]*one_minus_c-axis.v[1]*s);	//xz(1-c)-ys

	//Druga kolumna.
	x+=v[1]*(axis.v[0]*axis.v[1]*one_minus_c-axis.v[2]*s);	//yx(1-c)-zs
	y+=v[1]*(axis.v[1]*axis.v[1]*one_minus_c+c);				//yy(1-c)+c
	z+=v[1]*(axis.v[1]*axis.v[2]*one_minus_c+axis.v[0]*s);	//yz(1-c)+xs

	//Trzecia kolumna.
	x+=v[2]*(axis.v[0]*axis.v[2]*one_minus_c+axis.v[1]*s);	//zx(1-c)+ys
	y+=v[2]*(axis.v[1]*axis.v[2]*one_minus_c-axis.v[0]*s);	//zy(1-c)-ys
	z+=v[2]*(axis.v[2]*axis.v[2]*one_minus_c+c);				//zz(1-c)+c

	v[0]=x;
	v[1]=y;
	v[2]=z;
}

void Vec3::operator+=(Vec3 &rhs) { v[0]+=rhs.v[0]; v[1]+=rhs.v[1]; v[2]+=rhs.v[2]; }
void Vec3::operator-=(Vec3 &rhs) { v[0]-=rhs.v[0]; v[1]-=rhs.v[1]; v[2]-=rhs.v[2];}
void Vec3::operator*=(float rhs) { v[0]*=rhs; v[1]*=rhs; v[2]*=rhs; }
void Vec3::operator/=(float rhs) { v[0]/=rhs; v[1]/=rhs; v[2]/=rhs; }
float& Vec3::operator[](int el) {return v[el]; }
Vec3::operator float*() { return v; }
Vec3::operator double*() { return (double*)v; }

void Vec3::operator*=(Quat rhs)
{
	Quat res,vec(v[0],v[1],v[2],0);

	res=rhs*vec;
	rhs.conjugate();
	res*=rhs;

	v[0]=res.q[0];
	v[1]=res.q[1];
	v[2]=res.q[2];
}

Vec3& Vec3::operator=(const Vec3 &rhs)
{
	v[0]=rhs.v[0]; v[1]=rhs.v[1]; v[2]=rhs.v[2];
	return (*this);
}

//----------------------------------------------------------------------//
//	Definicje metod i operatorow klasy: Mat3.							//
//----------------------------------------------------------------------//

Mat3::Mat3()
{
	identity();
}

Mat3::Mat3(const Mat3 &pattern)
{
	m[0]=pattern.m[0]; m[3]=pattern.m[3]; m[6]=pattern.m[6];
	m[1]=pattern.m[1]; m[4]=pattern.m[4]; m[7]=pattern.m[7];
	m[2]=pattern.m[2]; m[5]=pattern.m[5]; m[8]=pattern.m[8];
}

Mat3::Mat3(const Mat4 &pattern)
{
	m[0]=pattern.m[0]; m[3]=pattern.m[4]; m[6]=pattern.m[8];
	m[1]=pattern.m[1]; m[4]=pattern.m[5]; m[7]=pattern.m[9];
	m[2]=pattern.m[2]; m[5]=pattern.m[6]; m[8]=pattern.m[10];
}

void Mat3::identity()
{
	m[0]=1; m[3]=0; m[6]=0;
	m[1]=0; m[4]=1; m[7]=0;
	m[2]=0; m[5]=0; m[8]=1;
}

void Mat3::ones()
{
	m[0]=1; m[3]=1; m[6]=1;
	m[1]=1; m[4]=1; m[7]=1;
	m[2]=1; m[5]=1; m[8]=1;
}

void Mat3::zeros()
{
	m[0]=0; m[3]=0; m[6]=0;
	m[1]=0; m[4]=0; m[7]=0;
	m[2]=0; m[5]=0; m[8]=0;
}

void Mat3::inverse()
{
	float det=determinant(); Mat3 res;
	res.m[0] =  (m[4]*m[8]-m[7]*m[5]) / det;
	res.m[1] = -(m[1]*m[8]-m[7]*m[2]) / det;
	res.m[2] =  (m[1]*m[5]-m[4]*m[2]) / det;
	res.m[3] = -(m[3]*m[8]-m[6]*m[5]) / det;
	res.m[4] =  (m[0]*m[8]-m[6]*m[2]) / det;
	res.m[5] = -(m[0]*m[5]-m[3]*m[2]) / det;
	res.m[6] =  (m[3]*m[7]-m[6]*m[4]) / det;
	res.m[7] = -(m[0]*m[7]-m[6]*m[1]) / det;
	res.m[8] =  (m[0]*m[4]-m[3]*m[1]) / det;
	*this=res;
}

void Mat3::transpose()
{
	Mat3 res;
	res.m[0]=m[0]; res.m[3]=m[1]; res.m[6]=m[2];
	res.m[1]=m[3]; res.m[4]=m[4]; res.m[7]=m[5];
	res.m[2]=m[6]; res.m[5]=m[7]; res.m[8]=m[8];
	(*this)=res;
}

float Mat3::determinant()
{
	return m[0]*m[4]*m[8]+m[3]*m[7]*m[2]+m[6]*m[1]*m[5]-m[2]*m[4]*m[6]-m[5]*m[7]*m[0]-m[8]*m[1]*m[3];
}

void Mat3::decompose(Vec3 &scale, Vec3 &axis, float &angle)
{
	//Obliczenie wspolczynnikow skalowania x, y, z.
	scale.v[0] = sqrt(m[0]*m[0] + m[3]*m[3] + m[6]*m[6]);
	scale.v[1] = sqrt(m[1]*m[1] + m[4]*m[4] + m[7]*m[7]);
	scale.v[2] = sqrt(m[2]*m[2] + m[5]*m[5] + m[8]*m[8]);

	//Pozbycie sie skalowania z macierzy zrodlowej.
	m[0]/=scale.v[0]; m[3]/=scale.v[0]; m[6]/=scale.v[0];
	m[1]/=scale.v[1]; m[4]/=scale.v[1]; m[7]/=scale.v[1];
	m[2]/=scale.v[2]; m[5]/=scale.v[2]; m[8]/=scale.v[2];

	//Sprawdzenie czy kat nie jest rowny 0 lub 180 stopni.
	if(fabs(m[3]-m[1])<EPS && fabs(m[6]-m[2])<EPS && fabs(m[7]-m[5])<EPS)
	{
		if(	fabs(m[3]+m[1])<EPS &&
			fabs(m[6]+m[2])<EPS &&
		 	fabs(m[7]+m[5])<EPS &&
		 	( m[0]+m[4]+m[8]-3<EPS && m[0]+m[4]+m[8]-3>-EPS) ) angle=0;
		else
		{
			angle=180.0f;

			axis.v[0] = sqrt((m[0]+1)/2);	//Tutaj znak nie jest pewny.
			axis.v[1] = sqrt((m[4]+1)/2);	//Tutaj znak nie jest pewny.
			axis.v[2] = sqrt((m[8]+1)/2);	//Tutaj znak nie jest pewny.

			if(fabs(axis.v[0])<EPS && fabs(axis.v[1])>EPS && fabs(axis.v[2])>EPS) {axis.v[1]*=-1;}
			else if(fabs(axis.v[1])<EPS && fabs(axis.v[2])>EPS) {axis.v[0]*=-1;}
			else if(fabs(axis.v[2])<EPS) axis.v[0]*=-1;
			else if(axis.v[0]*axis.v[0]>0 && axis.v[1]*axis.v[1]>0 && axis.v[2]*axis.v[2]>0)
			{
				axis.v[0]=fabs(axis.v[0]);
				axis.v[1]=fabs(axis.v[1]);
				axis.v[2]=fabs(axis.v[2]);
			}
			else if(axis.v[1]*axis.v[2]>0) axis.v[0]*=-1;
			else if(axis.v[0]*axis.v[2]>0) axis.v[1]*=-1;
			else if(axis.v[0]*axis.v[1]>0) axis.v[2]*=-1;
		}
	}
	else
	{
		angle = acos((m[0]+m[4]+m[8]-1)/2);
		angle*=57.29578f;

		float d=0;
		d+=( m[5]-m[7])*(m[5]-m[7] );
		d+=( m[6]-m[2])*(m[6]-m[2] );
		d+=( m[3]-m[1])*(m[3]-m[1] );
		d=sqrt(d);

		axis.v[0]= (m[5]-m[7])/d;
		axis.v[1]= (m[6]-m[2])/d;
		axis.v[2]= -(m[3]-m[1])/d;
	}
}

Quat Mat3::toQuaternion()
{
	float s0, s1, s2;
	int k0, k1, k2, k3;
	Quat res;
	if( m[0]+m[4]+m[8] > 0.0f )
	{
		k0 = 3;
		k1 = 2;
		k2 = 1;
		k3 = 0;
		s0 = 1.0f;
		s1 = 1.0f;
		s2 = 1.0f;
	}
	else if( m[0]>m[4] &&  m[0]>m[8])
	{
		k0 = 0;
		k1 = 1;
		k2 = 2;
		k3 = 3;
		s0 = 1.0f;
		s1 = -1.0f;
		s2 = -1.0f;
	}
	else if ( m[4] > m[8] )
	{
		k0 = 1;
		k1 = 0;
		k2 = 3;
		k3 = 2;
		s0 = -1.0f;
		s1 = 1.0f;
		s2 = -1.0f;
	}
	else
	{
		k0 = 2;
		k1 = 3;
		k2 = 0;
		k3 = 1;
		s0 = -1.0f;
		s1 = -1.0f;
		s2 = 1.0f;
	}
	float t = s0 * m[0] + s1 * m[4] + s2 * m[8] + 1.0f;
	float s = ReciprocalSqrt( t ) * 0.5f;
	res.q[k0] = s * t;
	res.q[k1] = ( m[3] - s2 * m[1] ) * s;
	res.q[k2] = ( m[2] - s1 * m[6] ) * s;
	res.q[k3] = ( m[7] - s0 * m[5] ) * s;
	//q[4] = m[0 * 4 + 3];
	//q[5] = m[1 * 4 + 3];
	//q[6] = m[2 * 4 + 3];
	//q[7] = 0.0f;
	return res;
}

Quat Mat3::toQuaternion2()
{
	float s0, s1, s2;
	int k0, k1, k2, k3;
	Quat res;
	if( m[0]+m[4]+m[8] > 0.0f )
	{
		k0 = 3;
		k1 = 2;
		k2 = 1;
		k3 = 0;
		s0 = 1.0f;
		s1 = 1.0f;
		s2 = 1.0f;
	}
	else if( m[0]>m[4] &&  m[0]>m[8])
	{
		k0 = 0;
		k1 = 1;
		k2 = 2;
		k3 = 3;
		s0 = 1.0f;
		s1 = -1.0f;
		s2 = -1.0f;
	}
	else if ( m[4] > m[8] )
	{
		k0 = 1;
		k1 = 0;
		k2 = 3;
		k3 = 2;
		s0 = -1.0f;
		s1 = 1.0f;
		s2 = -1.0f;
	}
	else
	{
		k0 = 2;
		k1 = 3;
		k2 = 0;
		k3 = 1;
		s0 = -1.0f;
		s1 = -1.0f;
		s2 = 1.0f;
	}
	float t = s0 * m[0] + s1 * m[4] + s2 * m[8] + 1.0f;
	float s = ReciprocalSqrt( t ) * 0.5f;
	res.q[k0] = s * t;
	res.q[k1] = ( m[3] - s2 * m[1] ) * s;
	res.q[k2] = ( m[2] - s1 * m[6] ) * s;
	res.q[k3] = ( m[7] - s0 * m[5] ) * s;
	//q[4] = m[0 * 4 + 3];
	//q[5] = m[1 * 4 + 3];
	//q[6] = m[2 * 4 + 3];
	//q[7] = 0.0f;
	return res;
}

void Mat3::scaling(float x, float y, float z)
{
	m[0]=x; m[3]=0; m[6]=0;
	m[1]=0; m[4]=y; m[7]=0;
	m[2]=0; m[5]=0; m[8]=z;
}

void Mat3::rotationX(float angle)
{
	angle*=0.0174533f;
	float s = sin(angle), c=cos(angle);

	m[0]=1; m[3]=0; m[6]=0;
	m[1]=0; m[4]=c; m[7]=s;
	m[2]=0; m[5]=-s; m[8]=c;
}

void Mat3::rotationY(float angle)
{
	angle*=0.0174533f;
	float s = sin(angle), c=cos(angle);

	m[0]=c;  m[3]=0; m[6]=-s;
	m[1]=0;  m[4]=1; m[7]=0;
	m[2]=s; m[5]=0; m[8]=c;
}

void Mat3::rotationZ(float angle)
{
	angle*=0.0174533f;
	float s = sin(angle), c=cos(angle);

	m[0]=c; m[3]=s; m[6]=0;
	m[1]=-s; m[4]=c;  m[7]=0;
	m[2]=0; m[5]=0;  m[8]=1;
}

void Mat3::rotationAxis(float angle, Vec3 axis)
{
	angle*=0.0174533f;
	float s = sin(angle), c=cos(angle), one_minus_c=1-c;
	axis.normalize();

	//Pierwsza kolumna.
	m[0]=axis.v[0]*axis.v[0]*one_minus_c+c;				//xx(1-c)+c
	m[1]=axis.v[0]*axis.v[1]*one_minus_c+axis.v[2]*s;	//xy(1-c)+zs
	m[2]=axis.v[0]*axis.v[2]*one_minus_c-axis.v[1]*s;	//xz(1-c)-ys

	//Druga kolumna.
	m[3]=axis.v[0]*axis.v[1]*one_minus_c-axis.v[2]*s;	//yx(1-c)-zs
	m[4]=axis.v[1]*axis.v[1]*one_minus_c+c;				//yy(1-c)+c
	m[5]=axis.v[1]*axis.v[2]*one_minus_c+axis.v[0]*s;	//yz(1-c)+xs

	//Trzecia kolumna.
	m[6]=axis.v[0]*axis.v[2]*one_minus_c+axis.v[1]*s;	//zx(1-c)+ys
	m[7]=axis.v[1]*axis.v[2]*one_minus_c-axis.v[0]*s;	//zy(1-c)-ys
	m[8]=axis.v[2]*axis.v[2]*one_minus_c+c;				//zz(1-c)+c
}

void Mat3::operator+=(Mat3 &rhs)
{
	m[0]+=rhs.m[0]; m[3]+=rhs.m[3]; m[6]+=rhs.m[6];
	m[1]+=rhs.m[1]; m[4]+=rhs.m[4]; m[7]+=rhs.m[7];
	m[2]+=rhs.m[2]; m[5]+=rhs.m[5]; m[8]+=rhs.m[8];
}

void Mat3::operator-=(Mat3 &rhs)
{
	m[0]-=rhs.m[0]; m[3]-=rhs.m[3]; m[6]-=rhs.m[6];
	m[1]-=rhs.m[1]; m[4]-=rhs.m[4]; m[7]-=rhs.m[7];
	m[2]-=rhs.m[2]; m[5]-=rhs.m[5]; m[8]-=rhs.m[8];
}

void Mat3::operator*=(Mat3 &rhs)
{
	(*this)=(*this)*rhs;
}

void Mat3::operator*=(float rhs)
{
	m[0]*=rhs; m[3]*=rhs; m[6]*=rhs;
	m[1]*=rhs; m[4]*=rhs; m[7]*=rhs;
	m[2]*=rhs; m[5]*=rhs; m[8]*=rhs;
}

void Mat3::operator/=(float rhs)
{
	m[0]/=rhs; m[3]/=rhs; m[6]/=rhs;
	m[1]/=rhs; m[4]/=rhs; m[7]/=rhs;
	m[2]/=rhs; m[5]/=rhs; m[8]/=rhs;
}

inline Mat3& Mat3::operator=(const Mat3 &rhs)
{
	m[0]=rhs.m[0]; m[3]=rhs.m[3]; m[6]=rhs.m[6];
	m[1]=rhs.m[1]; m[4]=rhs.m[4]; m[7]=rhs.m[7];
	m[2]=rhs.m[2]; m[5]=rhs.m[5]; m[8]=rhs.m[8];
	return (*this);
}

inline Mat3& Mat3::operator=(const Mat4 &rhs)
{
	m[0]=rhs.m[0]; m[3]=rhs.m[4]; m[6]=rhs.m[8];
	m[1]=rhs.m[1]; m[4]=rhs.m[5]; m[7]=rhs.m[9];
	m[2]=rhs.m[2]; m[5]=rhs.m[6]; m[8]=rhs.m[10];
	return (*this);
}

float& Mat3::operator[](int el) { return m[el]; }

//----------------------------------------------------------------------//
//	Definicje metod i operatorow klasy: Mat4.							//
//----------------------------------------------------------------------//

Mat4::Mat4()
{
	m=new float[16];
	m[0]=1; m[4]=0; m[8]=0;  m[12]=0;
	m[1]=0; m[5]=1; m[9]=0;  m[13]=0;
	m[2]=0; m[6]=0; m[10]=1; m[14]=0;
	m[3]=0; m[7]=0; m[11]=0; m[15]=1;
	wrapper=false;
}

Mat4::Mat4(float *data)
{
	m=data;
	wrapper=true;
}

Mat4::Mat4(const Mat3 &pattern)
{
	m=new float[16];
	m[0]=pattern.m[0]; m[4]=pattern.m[3]; m[8]=pattern.m[6];
	m[1]=pattern.m[1]; m[5]=pattern.m[4]; m[9]=pattern.m[7];
	m[2]=pattern.m[2]; m[6]=pattern.m[5]; m[10]=pattern.m[8];
}

Mat4::Mat4(const Mat4 &pattern)
{
	m=new float[16];
	m[0]=pattern.m[0]; m[4]=pattern.m[4]; m[8]=pattern.m[8];   m[12]=pattern.m[12];
	m[1]=pattern.m[1]; m[5]=pattern.m[5]; m[9]=pattern.m[9];   m[13]=pattern.m[13];
	m[2]=pattern.m[2]; m[6]=pattern.m[6]; m[10]=pattern.m[10]; m[14]=pattern.m[14];
	m[3]=pattern.m[3]; m[7]=pattern.m[7]; m[11]=pattern.m[11]; m[15]=pattern.m[15];
}

Mat4::~Mat4()
{
	if(!wrapper && m) delete[] m;
	m=NULL;
}

void Mat4::set(Mat4 &pattern)
{
	m[0]=pattern.m[0]; m[4]=pattern.m[4]; m[8]=pattern.m[8];   m[12]=pattern.m[12];
	m[1]=pattern.m[1]; m[5]=pattern.m[5]; m[9]=pattern.m[9];   m[13]=pattern.m[13];
	m[2]=pattern.m[2]; m[6]=pattern.m[6]; m[10]=pattern.m[10]; m[14]=pattern.m[14];
	m[3]=pattern.m[3]; m[7]=pattern.m[7]; m[11]=pattern.m[11]; m[15]=pattern.m[15];
}

void Mat4::identity()
{
	m[0]=1; m[4]=0; m[8]=0;  m[12]=0;
	m[1]=0; m[5]=1; m[9]=0;  m[13]=0;
	m[2]=0; m[6]=0; m[10]=1; m[14]=0;
	m[3]=0; m[7]=0; m[11]=0; m[15]=1;
}

void Mat4::ones()
{
	m[0]=1; m[4]=1; m[8]=1;  m[12]=1;
	m[1]=1; m[5]=1; m[9]=1;  m[13]=1;
	m[2]=1; m[6]=1; m[10]=1; m[14]=1;
	m[3]=1; m[7]=1; m[11]=1; m[15]=1;
}

void Mat4::zeros()
{
	m[0]=0; m[4]=0; m[8]=0;  m[12]=0;
	m[1]=0; m[5]=0; m[9]=0;  m[13]=0;
	m[2]=0; m[6]=0; m[10]=0; m[14]=0;
	m[3]=0; m[7]=0; m[11]=0; m[15]=0;
}

void Mat4::inverse()
{
	float x=determinant();
	if (x==0) return;

	Mat4 res;

	res[0] = m[9]*m[14]*m[7] - m[13]*m[10]*m[7] + m[13]*m[6]*m[11] - m[5]*m[14]*m[11] - m[9]*m[6]*m[15] + m[5]*m[10]*m[15];
	res[4] = m[12]*m[10]*m[7] - m[8]*m[14]*m[7] - m[12]*m[6]*m[11] + m[4]*m[14]*m[11] + m[8]*m[6]*m[15] - m[4]*m[10]*m[15];
	res[8] = m[8]*m[13]*m[7] - m[12]*m[9]*m[7] + m[12]*m[5]*m[11] - m[4]*m[13]*m[11] - m[8]*m[5]*m[15] + m[4]*m[9]*m[15];
	res[12] = m[12]*m[9]*m[6] - m[8]*m[13]*m[6] - m[12]*m[5]*m[10] + m[4]*m[13]*m[10] + m[8]*m[5]*m[14] - m[4]*m[9]*m[14];
	res[1] = m[13]*m[10]*m[3] - m[9]*m[14]*m[3] - m[13]*m[2]*m[11] + m[1]*m[14]*m[11] + m[9]*m[2]*m[15] - m[1]*m[10]*m[15];
	res[5] = m[8]*m[14]*m[3] - m[12]*m[10]*m[3] + m[12]*m[2]*m[11] - m[0]*m[14]*m[11] - m[8]*m[2]*m[15] + m[0]*m[10]*m[15];
	res[9] = m[12]*m[9]*m[3] - m[8]*m[13]*m[3] - m[12]*m[1]*m[11] + m[0]*m[13]*m[11] + m[8]*m[1]*m[15] - m[0]*m[9]*m[15];
	res[13] = m[8]*m[13]*m[2] - m[12]*m[9]*m[2] + m[12]*m[1]*m[10] - m[0]*m[13]*m[10] - m[8]*m[1]*m[14] + m[0]*m[9]*m[14];
	res[2] = m[5]*m[14]*m[3] - m[13]*m[6]*m[3] + m[13]*m[2]*m[7] - m[1]*m[14]*m[7] - m[5]*m[2]*m[15] + m[1]*m[6]*m[15];
	res[6] = m[12]*m[6]*m[3] - m[4]*m[14]*m[3] - m[12]*m[2]*m[7] + m[0]*m[14]*m[7] + m[4]*m[2]*m[15] - m[0]*m[6]*m[15];
	res[10] = m[4]*m[13]*m[3] - m[12]*m[5]*m[3] + m[12]*m[1]*m[7] - m[0]*m[13]*m[7] - m[4]*m[1]*m[15] + m[0]*m[5]*m[15];
	res[15] = m[12]*m[5]*m[2] - m[4]*m[13]*m[2] - m[12]*m[1]*m[6] + m[0]*m[13]*m[6] + m[4]*m[1]*m[14] - m[0]*m[5]*m[14];
	res[3] = m[9]*m[6]*m[3] - m[5]*m[10]*m[3] - m[9]*m[2]*m[7] + m[1]*m[10]*m[7] + m[5]*m[2]*m[11] - m[1]*m[6]*m[11];
	res[7] = m[4]*m[10]*m[3] - m[8]*m[6]*m[3] + m[8]*m[2]*m[7] - m[0]*m[10]*m[7] - m[4]*m[2]*m[11] + m[0]*m[6]*m[11];
	res[11] = m[8]*m[5]*m[3] - m[4]*m[9]*m[3] - m[8]*m[1]*m[7] + m[0]*m[9]*m[7] + m[4]*m[1]*m[11] - m[0]*m[5]*m[11];
	res[15] = m[4]*m[9]*m[2] - m[8]*m[5]*m[2] + m[8]*m[1]*m[6] - m[0]*m[9]*m[6] - m[4]*m[1]*m[10] + m[0]*m[5]*m[10];

	*this = res;

}

void Mat4::transpose()
{
	Mat4 res;
	res.m[0]=m[0];  res.m[4]=m[1];  res.m[8]=m[2];   res.m[12]=m[3];
	res.m[1]=m[4];  res.m[5]=m[5];  res.m[9]=m[6];   res.m[13]=m[7];
	res.m[2]=m[8];  res.m[6]=m[9];  res.m[10]=m[10]; res.m[14]=m[11];
	res.m[3]=m[12]; res.m[7]=m[13]; res.m[11]=m[14]; res.m[15]=m[15];
	(*this)=res;
	//for(int i=0; i<16; i++) m[i]=res.m[i];
}

float Mat4::determinant()
{
	/*
	float det=0;
	det=m[0]*(m[5]*m[10]*m[15]+m[9]*m[14]*m[7]+m[13]*m[6]*m[11]-m[7]*m[10]*m[13]-m[11]*m[14]*m[5]-m[15]*m[6]*m[9]);
	det-=m[1]*(m[4]*m[10]*m[15]+m[8]*m[14]*m[7]+m[12]*m[6]*m[11]-m[7]*m[10]*m[12]-m[11]*m[14]*m[4]-m[15]*m[6]*m[8]);
	det+=m[2]*(m[4]*m[9]*m[15]+m[8]*m[13]*m[7]+m[12]*m[5]*m[11]-m[7]*m[9]*m[12]-m[11]*m[13]*m[4]-m[15]*m[5]*m[8]);
	det-=m[3]*(m[4]*m[9]*m[14]+m[8]*m[13]*m[6]+m[12]*m[5]*m[10]-m[6]*m[9]*m[12]-m[10]*m[13]*m[4]-m[14]*m[5]*m[8]);
	return det;*/
	return
	m[12]*m[9]*m[6]*m[3]-m[8]*m[13]*m[6]*m[3]-m[12]*m[5]*m[10]*m[3]+
	m[4]*m[13]*m[10]*m[3]+m[8]*m[5]*m[14]*m[3]-m[4]*m[9]*m[14]*m[3]-
	m[12]*m[9]*m[2]*m[7]+m[8]*m[13]*m[2]*m[7]+m[12]*m[1]*m[10]*m[7]-
	m[0]*m[13]*m[10]*m[7]-m[8]*m[1]*m[14]*m[7]+m[0]*m[9]*m[14]*m[7]+
	m[12]*m[5]*m[2]*m[11]-m[4]*m[13]*m[2]*m[11]-m[12]*m[1]*m[6]*m[11]+
	m[0]*m[13]*m[6]*m[11]+m[4]*m[1]*m[14]*m[11]-m[0]*m[5]*m[14]*m[11]-
	m[8]*m[5]*m[2]*m[15]+m[4]*m[9]*m[2]*m[15]+m[8]*m[1]*m[6]*m[15]-
	m[0]*m[9]*m[6]*m[15]-m[4]*m[1]*m[10]*m[15]+m[0]*m[5]*m[10]*m[15];
}

void Mat4::translation(float x, float y, float z)
{
	m[0]=1; m[4]=0; m[8]=0;  m[12]=x;
	m[1]=0; m[5]=1; m[9]=0;  m[13]=y;
	m[2]=0; m[6]=0; m[10]=1; m[14]=z;
	m[3]=0; m[7]=0; m[11]=0; m[15]=1;
}

void Mat4::scaling(float x, float y, float z)
{
	m[0]=x; m[4]=0; m[8]=0;  m[12]=0;
	m[1]=0; m[5]=y; m[9]=0;  m[13]=0;
	m[2]=0; m[6]=0; m[10]=z; m[14]=0;
	m[3]=0; m[7]=0; m[11]=0; m[15]=1;
}

void Mat4::rotationX(float angle)
{
	angle*=0.0174533f;
	float s = sin(angle), c=cos(angle);

	m[0]=1; m[4]=0; m[8]=0;  m[12]=0;
	m[1]=0; m[5]=c; m[9]=-s; m[13]=0;
	m[2]=0; m[6]=s; m[10]=c; m[14]=0;
	m[3]=0; m[7]=0; m[11]=0; m[15]=1;
}

void Mat4::rotationY(float angle)
{
	angle*=0.0174533f;
	float s = sin(angle), c=cos(angle);

	m[0]=c;  m[4]=0; m[8]=s;  m[12]=0;
	m[1]=0;  m[5]=1; m[9]=0;  m[13]=0;
	m[2]=-s; m[6]=0; m[10]=c; m[14]=0;
	m[3]=0;  m[7]=0; m[11]=0; m[15]=1;
}

void Mat4::rotationZ(float angle)
{
	angle*=0.0174533f;
	float s = sin(angle), c=cos(angle);

	m[0]=c; m[4]=-s; m[8]=0;  m[12]=0;
	m[1]=s; m[5]=c;  m[9]=0;  m[13]=0;
	m[2]=0; m[6]=0;  m[10]=1; m[14]=0;
	m[3]=0; m[7]=0;  m[11]=0; m[15]=1;
}

void Mat4::rotationAxis(float angle, Vec3 axis)
{
	angle*=0.0174533f;
	float s = sin(angle), c=cos(angle);
	axis.normalize();

	m[0]=axis.v[0]*axis.v[0]*(1-c)+c;
	m[1]=axis.v[0]*axis.v[1]*(1-c)+axis.v[2]*s;
	m[2]=axis.v[0]*axis.v[2]*(1-c)-axis.v[1]*s;
	m[3]=0;
	m[4]=axis.v[0]*axis.v[1]*(1-c)-axis.v[2]*s;
	m[5]=axis.v[1]*axis.v[1]*(1-c)+c;
	m[6]=axis.v[1]*axis.v[2]*(1-c)+axis.v[0]*s;
	m[7]=0;
	m[8]=axis.v[0]*axis.v[2]*(1-c)+axis.v[1]*s;
	m[9]=axis.v[1]*axis.v[2]*(1-c)-axis.v[0]*s;
	m[10]=axis.v[2]*axis.v[2]*(1-c)+c;
	m[11]=0; m[12]=0; m[13]=0; m[14]=0; m[15]=1;
}

void Mat4::operator+=(Mat4 &rhs)
{
	m[0]+=rhs.m[0]; m[4]+=rhs.m[4]; m[8]+=rhs.m[8];   m[12]+=rhs.m[12];
	m[1]+=rhs.m[1]; m[5]+=rhs.m[5]; m[9]+=rhs.m[9];   m[13]+=rhs.m[13];
	m[2]+=rhs.m[2]; m[6]+=rhs.m[6]; m[10]+=rhs.m[10]; m[14]+=rhs.m[14];
	m[3]+=rhs.m[3]; m[7]+=rhs.m[7]; m[11]+=rhs.m[11]; m[15]+=rhs.m[15];
}

void Mat4::operator-=(Mat4 &rhs)
{
	m[0]-=rhs.m[0]; m[3]-=rhs.m[0]; m[6]-=rhs.m[0];
	m[1]-=rhs.m[0]; m[4]-=rhs.m[0]; m[7]-=rhs.m[0];
	m[2]-=rhs.m[0]; m[5]-=rhs.m[0]; m[8]-=rhs.m[0];
}

void Mat4::operator*=(Mat4 &rhs)
{
	Mat4 res;
	res.m[0]  = m[0]*rhs.m[0]  + m[4]*rhs.m[1] + m[8]*rhs.m[2]  + m[12]*rhs.m[3];
	res.m[4]  = m[0]*rhs.m[4]  + m[4]*rhs.m[5] + m[8]*rhs.m[6]  + m[12]*rhs.m[7];
	res.m[8]  = m[0]*rhs.m[8]  + m[4]*rhs.m[9] + m[8]*rhs.m[10] + m[12]*rhs.m[11];
	res.m[12] = m[0]*rhs.m[12] + m[4]*rhs.m[13]+ m[8]*rhs.m[14] + m[12]*rhs.m[15];
	res.m[1]  = m[1]*rhs.m[0]  + m[5]*rhs.m[1] + m[9]*rhs.m[2]  + m[13]*rhs.m[3];
	res.m[5]  = m[1]*rhs.m[4]  + m[5]*rhs.m[5] + m[9]*rhs.m[6]  + m[13]*rhs.m[7];
	res.m[9]  = m[1]*rhs.m[8]  + m[5]*rhs.m[9] + m[9]*rhs.m[10] + m[13]*rhs.m[11];
	res.m[13] = m[1]*rhs.m[12] + m[5]*rhs.m[13]+ m[9]*rhs.m[14] + m[13]*rhs.m[15];
	res.m[2]  = m[2]*rhs.m[0]  + m[6]*rhs.m[1] + m[10]*rhs.m[2] + m[14]*rhs.m[3];
	res.m[6]  = m[2]*rhs.m[4]  + m[6]*rhs.m[5] + m[10]*rhs.m[6] + m[14]*rhs.m[7];
	res.m[10] = m[2]*rhs.m[8]  + m[6]*rhs.m[9] + m[10]*rhs.m[10]+ m[14]*rhs.m[11];
	res.m[14] = m[2]*rhs.m[12] + m[6]*rhs.m[13]+ m[10]*rhs.m[14]+ m[14]*rhs.m[15];
	res.m[3]  = m[3]*rhs.m[0]  + m[7]*rhs.m[1] + m[11]*rhs.m[2] + m[15]*rhs.m[3];
	res.m[7]  = m[3]*rhs.m[4]  + m[7]*rhs.m[5] + m[11]*rhs.m[6] + m[15]*rhs.m[7];
	res.m[11] = m[3]*rhs.m[8]  + m[7]*rhs.m[9] + m[11]*rhs.m[10]+ m[15]*rhs.m[11];
	res.m[15] = m[3]*rhs.m[12] + m[7]*rhs.m[13]+ m[11]*rhs.m[14]+ m[15]*rhs.m[15];
	*this=res;
}

void Mat4::operator*=(float rhs)
{
	m[0]*=rhs; m[4]*=rhs; m[8]*=rhs;  m[12]*=rhs;
	m[1]*=rhs; m[5]*=rhs; m[9]*=rhs;  m[13]*=rhs;
	m[2]*=rhs; m[6]*=rhs; m[10]*=rhs; m[14]*=rhs;
	m[3]*=rhs; m[7]*=rhs; m[11]*=rhs; m[15]*=rhs;
}

void Mat4::operator/=(float rhs)
{
	m[0]/=rhs; m[4]/=rhs; m[8]/=rhs;  m[12]/=rhs;
	m[1]/=rhs; m[5]/=rhs; m[9]/=rhs;  m[13]/=rhs;
	m[2]/=rhs; m[6]/=rhs; m[10]/=rhs; m[14]/=rhs;
	m[3]/=rhs; m[7]/=rhs; m[11]/=rhs; m[15]/=rhs;
}

float& Mat4::operator[](int el) { return m[el]; }

Mat4& Mat4::operator=(const Mat3 &rhs)
{
	m[0]=rhs.m[0]; m[4]=rhs.m[3]; m[8]=rhs.m[6];
	m[1]=rhs.m[1]; m[5]=rhs.m[4]; m[9]=rhs.m[7];
	m[2]=rhs.m[2]; m[6]=rhs.m[5]; m[10]=rhs.m[8];
	return (*this);
}

Mat4& Mat4::operator=(const Mat4 &rhs)
{
	m[0]=rhs.m[0]; m[4]=rhs.m[4]; m[8]=rhs.m[8];   m[12]=rhs.m[12];
	m[1]=rhs.m[1]; m[5]=rhs.m[5]; m[9]=rhs.m[9];   m[13]=rhs.m[13];
	m[2]=rhs.m[2]; m[6]=rhs.m[6]; m[10]=rhs.m[10]; m[14]=rhs.m[14];
	m[3]=rhs.m[3]; m[7]=rhs.m[7]; m[11]=rhs.m[11]; m[15]=rhs.m[15];
	return (*this);
}

//----------------------------------------------------------------------//
//	Definicje metod i operatorow klasy: Quat.							//
//----------------------------------------------------------------------//

Quat::Quat()
{
	q[0]=q[1]=q[2]=0;
	q[3]=1;
}

Quat::Quat(float x, float y, float z, float w)
{
	q[0]=x; q[1]=y; q[2]=z; q[3]=w;
}

Quat::Quat(float angle, Vec3 &axis)
{
	rotationAxis(angle, axis);
}

void Quat::xyzw(float x, float y, float z, float w)
{
	q[0]=x; q[1]=y; q[2]=z; q[3]=w;
}

void Quat::rotationAxis(float angle, Vec3 axis)
{
	angle*=0.0174533f;	//Zamiana na radiany (pomnozenie przez PI/180).
	axis.normalize();	//Znormalizowanie osi.
	float s=sin(angle*0.5f);

	q[0] = axis.v[0] * s;
	q[1] = axis.v[1] * s;
	q[2] = axis.v[2] * s;
	q[3] = cos(angle*0.5f);
}

void Quat::normalize()
{
	float len=length();
	q[0]/=len;
	q[1]/=len;
	q[2]/=len;
	q[3]/=len;
}

void Quat::conjugate()
{
	q[0]=-q[0]; q[1]=-q[1]; q[2]=-q[2];
}

float Quat::length()
{
	return sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
}

Mat3 Quat::toMatrix()
{
	Mat3 res;
	res.m[0]=1-(2*q[1]*q[1])-(2*q[2]*q[2]); res.m[3]=(2*q[0]*q[1])-(2*q[3]*q[2]);   res.m[6]=(2*q[0]*q[2])+(2*q[3]*q[1]);
	res.m[1]=(2*q[0]*q[1])+(2*q[3]*q[2]);   res.m[4]=1-(2*q[0]*q[0])-(2*q[2]*q[2]); res.m[7]=(2*q[1]*q[2])-(2*q[3]*q[0]);
	res.m[2]=(2*q[0]*q[2])-(2*q[3]*q[1]);   res.m[5]=(2*q[1]*q[2])+(2*q[3]*q[0]);   res.m[8]=1-(2*q[0]*q[0])-(2*q[1]*q[1]);

	//res.m[0]=1-2*q[1]*q[1]-2*q[2]*q[2]; res.m[3]=2*q[0]*q[1]-2*q[3]*q[2];   res.m[6]=2*q[0]*q[2]+2*q[3]*q[1];
	//res.m[1]=2*q[0]*q[1]+2*q[3]*q[2];   res.m[4]=1-2*q[0]*q[0]-2*q[2]*q[2]; res.m[7]=2*q[1]*q[2]-2*q[3]*q[0];
	//res.m[2]=2*q[0]*q[2]-2*q[3]*q[1];   res.m[5]=2*q[1]*q[2]+2*q[3]*q[0];   res.m[8]=1-2*q[0]*q[0]-2*q[1]*q[1];
	return res;
}

Vec3 Quat::toEuler()
{
	Vec3 res;
	float test = q[0]*q[1] + q[2]*q[3];

	if( (test<0.5-EPS) && (test>0.5+EPS))
	{
		res[1] = 2*atan2(q[0],q[3]);
		res[2] = asin(2*q[0]*q[1] + 2*q[2]*q[3]);
		res[0] = 0;
	}
	else if( (test>-0.5-EPS) && (test<-0.5+EPS))
	{
		res[1] = -2*atan2(q[0],q[3]);
		res[2] = asin(2*q[0]*q[1] + 2*q[2]*q[3]);
		res[0] = 0;
	}
	else
	{
		res[1] = atan2(2*q[1]*q[3]-2*q[0]*q[2] , 1 - 2*q[1]*q[1] - 2*q[2]*q[2]);
		res[2] = asin(2*q[0]*q[1] + 2*q[2]*q[3]);
		res[0] = atan2(2*q[0]*q[3]-2*q[1]*q[2] , 1 - 2*q[0]*q[0] - 2*q[2]*q[2]);
	}

	if(res[0]) res[0]*=180.0f/3.141593f;
	if(res[1]) res[1]*=180.0f/3.141593f;
	if(res[2]) res[2]*=180.0f/3.141593f;

	return res;
}

void Quat::fromEuler(Vec3 a)
{
	a[0]*=3.141593f/180.0f;
	a[1]*=3.141593f/180.0f;
	a[2]*=3.141593f/180.0f;
	float c1 = cos(a[1] / 2);
	float c2 = cos(a[2] / 2);
	float c3 = cos(a[0] / 2);
	float s1 = sin(a[1] / 2);
	float s2 = sin(a[2] / 2);
	float s3 = sin(a[0] / 2);

	q[3] = c1*c2*c3 - s1*s2*s3;
	q[0] = s1*s2*c3 + c1*c2*s3;
	q[1] = s1*c2*c3 + c1*s2*s3;
	q[2] = c1*s2*c3 - s1*c2*s3;
}

float& Quat::operator[](int el) {return q[el];};

void Quat::operator*=(float rhs)
{
	q[0]*=rhs; q[1]*=rhs; q[2]*=rhs; q[3]*=rhs;
}

void Quat::operator*=(Quat &rhs)
{
	(*this)=(*this)*rhs;
}

//----------------------------------------------------------------------//
//	Definicje pozostalych operatorow									//
//----------------------------------------------------------------------//

Vec3 operator+(Vec3 &lhs, Vec3 &rhs)
{
	return Vec3(lhs.v[0]+rhs.v[0], lhs.v[1]+rhs.v[1], lhs.v[2]+rhs.v[2]);
}

Vec3 operator-(Vec3 &lhs, Vec3 &rhs)
{
	return Vec3(lhs.v[0]-rhs.v[0], lhs.v[1]-rhs.v[1], lhs.v[2]-rhs.v[2]);
}

Vec3 operator*(Vec3 &lhs, float rhs)
{
	return Vec3(lhs.v[0]*rhs, lhs.v[1]*rhs, lhs.v[2]*rhs);
}

inline Vec3 operator*(Vec3 &lhs, Quat &rhs)
{
	Vec3 uv, uuv;
	Vec3 qvec(rhs.q[0], rhs.q[1], rhs.q[2]);
	uv = cross(qvec,lhs);
	uuv = cross(qvec,uv);
	uv *= (2.0f * rhs.q[3]);
	uuv *= 2.0f;

	Vec3 res=lhs;
	res+=uv;
	res+=uuv;
	return res;
}

Vec3 operator/(Vec3 &lhs, float rhs)
{
	return Vec3(lhs.v[0]/rhs, lhs.v[1]/rhs, lhs.v[2]/rhs);
}

bool operator!=(Vec3 &lhs, Vec3 &rhs)
{
	if(lhs.v[0]-rhs.v[0]>EPS || lhs.v[0]-rhs.v[0]<-EPS) return true;
	if(lhs.v[1]-rhs.v[1]>EPS || lhs.v[1]-rhs.v[1]<-EPS) return true;
	if(lhs.v[2]-rhs.v[2]>EPS || lhs.v[2]-rhs.v[2]<-EPS) return true;
	return false;
}

bool operator==(Vec3 &lhs, Vec3 &rhs)
{
	if(lhs.v[0]-rhs.v[0]>EPS || lhs.v[0]-rhs.v[0]<-EPS) return false;
	if(lhs.v[1]-rhs.v[1]>EPS || lhs.v[1]-rhs.v[1]<-EPS) return false;
	if(lhs.v[2]-rhs.v[2]>EPS || lhs.v[2]-rhs.v[2]<-EPS) return false;
	return true;
}

bool operator==(Quat &lhs, Quat &rhs)
{
	if(lhs[0]-rhs[0]>EPS || lhs[0]-rhs[0]<-EPS) return false;
	if(lhs[1]-rhs[1]>EPS || lhs[1]-rhs[1]<-EPS) return false;
	if(lhs[2]-rhs[2]>EPS || lhs[2]-rhs[2]<-EPS) return false;
	if(lhs[3]-rhs[3]>EPS || lhs[3]-rhs[3]<-EPS) return false;
	return true;
}

Vec3 operator*(Vec3 &lhs, Mat3 &rhs)
{
	return Vec3(lhs.v[0]*rhs.m[0] + lhs.v[1]*rhs.m[1] + lhs.v[2]*rhs.m[2],
				lhs.v[0]*rhs.m[3] + lhs.v[1]*rhs.m[4] + lhs.v[2]*rhs.m[5],
				lhs.v[0]*rhs.m[6] + lhs.v[1]*rhs.m[7] + lhs.v[2]*rhs.m[8]);
}

Vec3 operator*(Vec3 &lhs, Mat4 &rhs)
{
	return Vec3(lhs.v[0]*rhs.m[0] + lhs.v[1]*rhs.m[1] + lhs.v[2]*rhs.m[2] + rhs.m[3],
				lhs.v[0]*rhs.m[4] + lhs.v[1]*rhs.m[5] + lhs.v[2]*rhs.m[6] + rhs.m[7],
				lhs.v[0]*rhs.m[8] + lhs.v[1]*rhs.m[9] + lhs.v[2]*rhs.m[10] + rhs.m[11]);
}

Vec3 operator*(Mat3 &lhs, Vec3 &rhs)
{
	return Vec3(lhs.m[0]*rhs.v[0]+lhs.m[3]*rhs.v[1]+lhs.m[6]*rhs.v[2],
				lhs.m[1]*rhs.v[0]+lhs.m[4]*rhs.v[1]+lhs.m[7]*rhs.v[2],
				lhs.m[2]*rhs.v[0]+lhs.m[5]*rhs.v[1]+lhs.m[8]*rhs.v[2]);
}

Vec3 operator*(Mat4 &lhs, Vec3 &rhs)
{
	return Vec3(lhs.m[0]*rhs.v[0]+lhs.m[4]*rhs.v[1]+lhs.m[8]*rhs.v[2] + lhs.m[12],
				lhs.m[1]*rhs.v[0]+lhs.m[5]*rhs.v[1]+lhs.m[9]*rhs.v[2] + lhs.m[13],
				lhs.m[2]*rhs.v[0]+lhs.m[6]*rhs.v[1]+lhs.m[10]*rhs.v[2]+ lhs.m[14]);
}

Mat3 operator*(Mat3 &lhs, Mat3 &rhs)
{
	Mat3 res;
	res.m[0]=lhs.m[0]*rhs.m[0] + lhs.m[3]*rhs.m[1] + lhs.m[6]*rhs.m[2];
	res.m[3]=lhs.m[0]*rhs.m[3] + lhs.m[3]*rhs.m[4] + lhs.m[6]*rhs.m[5];
	res.m[6]=lhs.m[0]*rhs.m[6] + lhs.m[3]*rhs.m[7] + lhs.m[6]*rhs.m[8];
	res.m[1]=lhs.m[1]*rhs.m[0] + lhs.m[4]*rhs.m[1] + lhs.m[7]*rhs.m[2];
	res.m[4]=lhs.m[1]*rhs.m[3] + lhs.m[4]*rhs.m[4] + lhs.m[7]*rhs.m[5];
	res.m[7]=lhs.m[1]*rhs.m[6] + lhs.m[4]*rhs.m[7] + lhs.m[7]*rhs.m[8];
	res.m[2]=lhs.m[2]*rhs.m[0] + lhs.m[5]*rhs.m[1] + lhs.m[8]*rhs.m[2];
	res.m[5]=lhs.m[2]*rhs.m[3] + lhs.m[5]*rhs.m[4] + lhs.m[8]*rhs.m[5];
	res.m[8]=lhs.m[2]*rhs.m[6] + lhs.m[5]*rhs.m[7] + lhs.m[8]*rhs.m[8];
	return res;
}

Mat4 operator*(Mat4 &lhs, Mat4 &rhs)
{
	Mat4 res;
	res.m[0]= lhs.m[0]*rhs.m[0] + lhs.m[4]*rhs.m[1] + lhs.m[8]*rhs.m[2]  + lhs.m[12]*rhs.m[3];
	res.m[4]= lhs.m[0]*rhs.m[4] + lhs.m[4]*rhs.m[5] + lhs.m[8]*rhs.m[6]  + lhs.m[12]*rhs.m[7];
	res.m[8]= lhs.m[0]*rhs.m[8] + lhs.m[4]*rhs.m[9] + lhs.m[8]*rhs.m[10] + lhs.m[12]*rhs.m[11];
	res.m[12]=lhs.m[0]*rhs.m[12]+ lhs.m[4]*rhs.m[13]+ lhs.m[8]*rhs.m[14] + lhs.m[12]*rhs.m[15];
	res.m[1]= lhs.m[1]*rhs.m[0] + lhs.m[5]*rhs.m[1] + lhs.m[9]*rhs.m[2]  + lhs.m[13]*rhs.m[3];
	res.m[5]= lhs.m[1]*rhs.m[4] + lhs.m[5]*rhs.m[5] + lhs.m[9]*rhs.m[6]  + lhs.m[13]*rhs.m[7];
	res.m[9]= lhs.m[1]*rhs.m[8] + lhs.m[5]*rhs.m[9] + lhs.m[9]*rhs.m[10] + lhs.m[13]*rhs.m[11];
	res.m[13]=lhs.m[1]*rhs.m[12]+ lhs.m[5]*rhs.m[13]+ lhs.m[9]*rhs.m[14] + lhs.m[13]*rhs.m[15];
	res.m[2]= lhs.m[2]*rhs.m[0] + lhs.m[6]*rhs.m[1] + lhs.m[10]*rhs.m[2] + lhs.m[14]*rhs.m[3];
	res.m[6]= lhs.m[2]*rhs.m[4] + lhs.m[6]*rhs.m[5] + lhs.m[10]*rhs.m[6] + lhs.m[14]*rhs.m[7];
	res.m[10]=lhs.m[2]*rhs.m[8] + lhs.m[6]*rhs.m[9] + lhs.m[10]*rhs.m[10]+ lhs.m[14]*rhs.m[11];
	res.m[14]=lhs.m[2]*rhs.m[12]+ lhs.m[6]*rhs.m[13]+ lhs.m[10]*rhs.m[14]+ lhs.m[14]*rhs.m[15];
	res.m[3]= lhs.m[3]*rhs.m[0] + lhs.m[7]*rhs.m[1] + lhs.m[11]*rhs.m[2] + lhs.m[15]*rhs.m[3];
	res.m[7]= lhs.m[3]*rhs.m[4] + lhs.m[7]*rhs.m[5] + lhs.m[11]*rhs.m[6] + lhs.m[15]*rhs.m[7];
	res.m[11]=lhs.m[3]*rhs.m[8] + lhs.m[7]*rhs.m[9] + lhs.m[11]*rhs.m[10]+ lhs.m[15]*rhs.m[11];
	res.m[15]=lhs.m[3]*rhs.m[12]+ lhs.m[7]*rhs.m[13]+ lhs.m[11]*rhs.m[14]+ lhs.m[15]*rhs.m[15];
	return res;
}
Quat operator+(Quat &lhs, Quat &rhs)
{
	return Quat(lhs.q[0]+rhs.q[0], lhs.q[1]+rhs.q[1], lhs.q[2]+rhs.q[2], lhs.q[3]+rhs.q[3]);
}
//----------------------------------------------------------------------//
//Mnozenie kwaternionow. Powoduje kumulowanie rotacji. SPRAWDZONE.		//
//----------------------------------------------------------------------//
Quat operator*(Quat &lhs, Quat &rhs)
{
	Quat res;
	res.q[3] = (rhs.q[3]*lhs.q[3]) - (rhs.q[0]*lhs.q[0]) - (rhs.q[1]*lhs.q[1]) - (rhs.q[2]*lhs.q[2]);
	res.q[0] = (rhs.q[3]*lhs.q[0]) + (rhs.q[0]*lhs.q[3]) + (rhs.q[1]*lhs.q[2]) - (rhs.q[2]*lhs.q[1]);
	res.q[1] = (rhs.q[3]*lhs.q[1]) + (rhs.q[1]*lhs.q[3]) + (rhs.q[2]*lhs.q[0]) - (rhs.q[0]*lhs.q[2]);
	res.q[2] = (rhs.q[3]*lhs.q[2]) + (rhs.q[2]*lhs.q[3]) + (rhs.q[0]*lhs.q[1]) - (rhs.q[1]*lhs.q[0]);
	return res;
}

Quat operator*(Quat &lhs, float rhs)
{
	return Quat(lhs.q[0]*rhs, lhs.q[1]*rhs, lhs.q[2]*rhs, lhs.q[3]*rhs);
}

Vec3 operator*(Vec3 &lhs, SRT &rhs)
{
	Vec3 res=lhs;
	res.v[0]*=rhs.s.v[0];	//Skalowanie X.
	res.v[1]*=rhs.s.v[1];	//Skalowanie Y.
	res.v[2]*=rhs.s.v[2];	//Skalowanie Z.
	res*=rhs.r;				//Rotacja.
	res+=rhs.t;				//Translacja.
	//res*=rhs.r;				//Rotacja.
	return res;
}

SRT operator+(SRT &lhs, SRT &rhs)
{
	SRT res;
	res.s.v[0] = lhs.s.v[0]*rhs.s.v[0];	//Skalowanie X.
	res.s.v[1] = lhs.s.v[1]*rhs.s.v[1];	//Skalowanie Y.
	res.s.v[2] = lhs.s.v[2]*rhs.s.v[2];	//Skalowanie Z.
	res.r = lhs.r*rhs.r;				//Rotacja.
	res.t = lhs.t+rhs.t;				//Translacja.
	//res.r = lhs.r*rhs.r;				//Rotacja.
	return res;
}

//----------------------------------------------------------------------//
//	Definicje funkcji.													//
//----------------------------------------------------------------------//

float ReciprocalSqrt( float x )
{
	long i;
	float y, r;
	y = x * 0.5f;
	i = *(long *)( &x );
	i = 0x5f3759df - ( i >> 1 );
	r = *(float *)( &i );
	r = r * ( 1.5f - r * r * y );
	return r;
}

float dot(Vec3 &v1, Vec3 &v2)
{
	return (v1.v[0]*v2.v[0] + v1.v[1]*v2.v[1] + v1.v[2]*v2.v[2]);
}

float dot(Quat &q1, Quat &q2)
{
	return (q1.q[0]*q2.q[0] + q1.q[1]*q2.q[1] + q1.q[2]*q2.q[2] + q1.q[3]*q2.q[3]);
}

Vec3 cross(Vec3 &v1, Vec3 &v2)
{
	return Vec3(v1.v[1]*v2.v[2] - v1.v[2]*v2.v[1],
				v1.v[2]*v2.v[0] - v1.v[0]*v2.v[2],
				v1.v[0]*v2.v[1] - v1.v[1]*v2.v[0]);
}

Quat slerp(Quat q1, Quat q2, float t)
{
    float angle = dot(q1,q2);

    if (angle < -EPS)
	{
    	q1 *= -1.0f;
    	angle *= -1.0f;
	}

	float scale;
	float invscale;

	if((angle + 1.0f) > 0.05f)
	{
		if ((1.0f - angle) >= 0.05f)  // spherical interpolation
		{
			float theta = (float)acos(angle);
			float invsintheta = 1.0f / (float)sin(theta);
			scale = (float)sin(theta * (1.0f-t)) * invsintheta;
			invscale = (float)sin(theta * t) * invsintheta;
		}
		else // linear interploation
		{
			scale = 1.0f - t;
			invscale = t;
		}
	}
	else
	{
		q2.xyzw(-q1.q[1], q1.q[0], -q1.q[3], q1.q[2]);
		scale = (float)sin(3.141592 * (0.5f - t));
		invscale = (float)sin(3.141592 * t);
	}

	Quat res1=q1*scale, res2=q2*invscale;
	return res1+res2;
}
/*
Vec3 slerp(Vec3 &v1, Vec3 &v2, float t)
{
	if(t<EPS && t>-EPS) return v1;		//t=0 q[3]iec nie ma sensu interpoloq[3]ac.
	if(t<1+EPS && t>1-EPS) return v2;	//t=1 cq[2]q[1]li q[3]q[1]nik to pelna q[3]artosc v2.

	float l1=v1.length(); if(l1<EPS && l1>-EPS) return v1;
	float l2=v2.length(); if(l2<EPS && l2>-EPS) return v1;

	float d=dot(v1,v2)/l1/l2;
	if(d<1+EPS && d>1-EPS) return v1;	//d=1 cq[2]q[1]li a=0 - brak rotacji.

	double a=acos(d)*57.29578f;
	//cout<<" a="<<a<<endl;;

	Vec3 aq[0]is, res=v1;
	if(d>-1-EPS && d<-1+EPS) aq[0]is.q[0]q[1]q[2](0,0,1);	//d=1 cq[2]q[1]li a=180.
	else aq[0]is=cross(v1,v2);

	res.rotate(a*t,aq[0]is);
	return res;
}
*/

#endif /*MATHEMATICS_H_*/
