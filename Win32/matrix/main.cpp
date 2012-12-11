
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int u32;

typedef float Matrix22[2][2];
typedef float Matrix44[4][4];

inline void Matrix22MulMatrix22(Matrix22 c, const Matrix22& a, const Matrix22& b)
{
	float x1 = (a[0][0] + a[1][1]) * (b[0][0] + b[1][1]);
	float x2 = (a[1][0] + a[1][1]) * b[0][0];
	float x3 = a[0][0] * (b[0][1] - b[1][1]);
	float x4 = a[1][1] * (b[1][0] - b[0][0]);
	float x5 = (a[0][0] + a[0][1]) * b[1][1];
	float x6 = (a[1][0] - a[0][0]) * (b[0][0] + b[0][1]);
	float x7 = (a[0][1] - a[1][1]) * (b[1][0] + b[1][1]);

	c[0][0] = x1 + x4 -x5 + x7;
	c[0][1] = x3 + x5;
	c[1][0] = x2 + x4;
	c[1][1] = x1 + x3 - x2 + x6;

}

inline void Matrix44MulMatrix44(Matrix44 c, const Matrix44& a, const Matrix44& b)
{
	Matrix22 x[7];

	// (ma11 + ma22) * (mb11 + mb22)
	Matrix22 a0 = {a[0][0]+a[2][2], a[0][1]+a[2][3], a[1][0]+a[3][2], a[1][1]+a[3][3]};
	Matrix22 b0 = {b[0][0]+b[2][2], b[0][1]+b[2][3], b[1][0]+b[3][2], b[1][1]+b[3][3]};
	Matrix22MulMatrix22(x[0], a0, b0); 

	// (ma21 + ma22) * mb11 
	Matrix22 a1 = {a[2][0]+a[2][2], a[2][1]+a[2][3], a[3][0]+a[3][2], a[3][1]+a[3][3]};
	Matrix22 b1 = {b[0][0], b[0][1], b[1][0], b[1][1]};
	Matrix22MulMatrix22(x[1], a1, b1);  

	// ma11 * (mb12 - mb22) 
	Matrix22 a2 = {a[0][0], a[0][1], a[1][0], a[1][1]};
	Matrix22 b2 = {b[0][2]-b[2][2], b[0][3]-b[2][3], b[1][2]-b[3][2], b[1][3]-b[3][3]};
	Matrix22MulMatrix22(x[2], a2, b2);  


	// ma22 * (mb21 - mb11) 
	Matrix22 a3 = {a[2][2], a[2][3], a[3][2], a[3][3]};
	Matrix22 b3 = {b[2][0]-b[0][0], b[2][1]-b[0][1], b[3][0]-b[1][0], b[3][1]-b[1][1]};
	Matrix22MulMatrix22(x[3], a3, b3);   

	// (ma11 + ma12) * mb22 
	Matrix22 a4 = {a[0][0]+a[0][2], a[0][1]+a[0][3], a[1][0]+a[1][2], a[1][1]+a[1][3]};
	Matrix22 b4 = {b[2][2], b[2][3], b[3][2], b[3][3]};
	Matrix22MulMatrix22(x[4], a4, b4);  

	// (ma21 - ma11) * (mb11 + mb12) 
	Matrix22 a5 = {a[2][0]-a[0][0], a[2][1]-a[0][1], a[3][0]-a[1][0], a[3][1]-a[1][1]};
	Matrix22 b5 = {b[0][0]+b[0][2], b[0][1]+b[0][3], b[1][0]+b[1][2], b[1][1]+b[1][3]};
	Matrix22MulMatrix22(x[5], a5, b5);  

	// (ma12 - ma22) * (mb21 + mb22) 
	Matrix22 a6 = {a[0][2]-a[2][2], a[0][3]-a[2][3], a[1][2]-a[3][2], a[1][3]-a[3][3]};
	Matrix22 b6 = {b[2][0]+b[2][2], b[2][1]+b[2][3], b[3][0]+b[3][2], b[3][1]+b[3][3]};
	Matrix22MulMatrix22(x[6], a6, b6); 

	// ��һ�� 
	c[0][0] = x[0][0][0] + x[3][0][0] - x[4][0][0] + x[6][0][0]; 
	c[0][1] = x[0][0][1] + x[3][0][1] - x[4][0][1] + x[6][0][1]; 
	c[1][0] = x[0][1][0] + x[3][1][0] - x[4][1][0] + x[6][1][0]; 
	c[1][1] = x[0][1][1] + x[3][1][1] - x[4][1][1] + x[6][1][1]; 

	// �ڶ��� 
	c[0][2] = x[2][0][0] + x[4][0][0]; 
	c[0][3] = x[2][0][1] + x[4][0][1]; 
	c[1][2] = x[2][1][0] + x[4][1][0]; 
	c[1][3] = x[2][1][1] + x[4][1][1]; 

	// ������ 
	c[2][0] = x[1][0][0] + x[3][0][0]; 
	c[2][1] = x[1][0][1] + x[3][0][1]; 
	c[3][0] = x[1][1][0] + x[3][1][0]; 
	c[3][1] = x[1][1][1] + x[3][1][1]; 


	// ���Ŀ� 

	c[2][2] = x[0][0][0] - x[1][0][0] + x[2][0][0] + x[5][0][0]; 
	c[2][3] = x[0][0][1] - x[1][0][1] + x[2][0][1] + x[5][0][1]; 
	c[3][2] = x[0][1][0] - x[1][1][0] + x[2][1][0] + x[5][1][0]; 
	c[3][3] = x[0][1][1] - x[1][1][1] + x[2][1][1] + x[5][1][1]; 

}

template<class T>
class matrix4{
	T M[16];

	struct matrix2{
		T M[4];

		matrix2(){}

		matrix2(const T n1, const T n2, const T n3, const T n4){
			M[0] = n1;
			M[1] = n2;
			M[2] = n3;
			M[3] = n4;
		};

		const T& operator[](u32 index) const { return M[index]; }

		T& operator[](u32 index) { return M[index]; }

		inline void setByProduct(const matrix2& a, const matrix2& b)
		{
			T x0 = (a[0] + a[3]) * (b[0] + b[3]);
			T x1 = (a[1] + a[3]) * b[0];
			T x2 = a[0] * (b[2] - b[3]);
			T x3 = a[3] * (b[1] - b[0]);
			T x4 = (a[0] + a[2]) * b[3];
			T x5 = (a[1] - a[0]) * (b[0] + b[2]);
			T x6 = (a[2] - a[3]) * (b[1] + b[3]);

			M[0] = x0 + x3 -x4 + x6;
			M[1] = x1 + x3;
			M[2] = x2 + x4;
			M[3] = x0 + x2 - x1 + x5;
		}

	};
public:
	matrix4(){}

	matrix4(const T n11, const T n12, const T n13, const T n14,
		const T n21, const T n22, const T n23, const T n24,
		const T n31, const T n32, const T n33, const T n34,
		const T n41, const T n42, const T n43, const T n44){
			M[0] = n11;
			M[1] = n12;
			M[2] = n13;
			M[3] = n14;
			M[4] = n21;
			M[5] = n22;
			M[6] = n23;
			M[7] = n24;
			M[8] = n31;
			M[9] = n32;
			M[10] = n33;
			M[11] = n34;
			M[12] = n41;
			M[13] = n42;
			M[14] = n43;
			M[15] = n44;
	}

	inline void print() const{
		printf("%.3f,%.3f,%.3f,%.3f\r\n%.3f,%.3f,%.3f,%.3f\r\n%.3f,%.3f,%.3f,%.3f\r\n%.3f,%.3f,%.3f,%.3f\r\n",M[0],M[4],M[8],M[12],M[1],M[5],M[9],M[13],M[2],M[6],M[10],M[14],M[3],M[7],M[11],M[15]);
	}


	inline void setByProduct(const matrix4& a,const matrix4& b){
		const T *m1 = a.M;
		const T *m2 = b.M;

		M[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		M[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		M[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		M[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

		M[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		M[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		M[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		M[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

		M[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		M[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		M[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		M[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

		M[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		M[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		M[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		M[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
	}

	//1969��˹����ɭ���÷��β��Բ�����һЩ����������Ƴ�һ�־���˷���

	inline void setByFastProduct(const matrix4& a,const matrix4& b){
		matrix2 x[7];

		const T *m = a.M;
		const T *n = b.M;

		// (M0 + M3) * (N0 + N3)
		matrix2 m0(m[0]+m[10],m[1]+m[11],m[4]+m[14],m[5]+m[15]);
		matrix2 n0(n[0]+n[10],n[1]+n[11],n[4]+n[14],n[5]+n[15]);
		x[0].setByProduct(m0,n0);

		// (M1 + M3) * N0 
		matrix2 m1(m[2]+m[10],m[3]+m[11],m[6]+m[14],m[7]+m[15]);
		matrix2 n1(n[0],n[1],n[4],n[5]);
		x[1].setByProduct(m1,n1);

		// M0 * (N2 - N3) 
		matrix2 m2(m[0],m[1],m[4],m[5]);
		matrix2 n2(n[8]-n[10],n[9]-n[11],n[12]-n[14],n[13]-n[15]);
		x[2].setByProduct(m2,n2);

		// M3 * (N1 - N0)
		matrix2 m3(m[10],m[11],m[14],m[15]);
		matrix2 n3(n[2]-n[0],n[3]-n[1],n[6]-n[4],n[7]-n[5]);
		x[3].setByProduct(m3,n3);

		// (M0 + M2) * N3
		matrix2 m4(m[0]+m[8],m[1]+m[9],m[4]+m[12],m[5]+m[13]);
		matrix2 n4(n[10],n[11],n[14],n[15]);
		x[4].setByProduct(m4,n4);

		// (M1 - M0) * (N0 + N2) 
		matrix2 m5(m[2]-m[0],m[3]-m[1],m[6]-m[4],m[7]-m[5]);
		matrix2 n5(n[0]+n[8],n[1]+n[9],n[4]+n[12],n[5]+n[13]);
		x[5].setByProduct(m5,n5);

		// (M2 - M3) * (N1 + N3) 
		matrix2 m6(m[8]-m[10],m[9]-m[11],m[12]-m[14],m[13]-m[15]);
		matrix2 n6(n[2]+n[10],n[3]+n[11],n[6]+n[14],n[7]+n[15]);
		x[6].setByProduct(m6,n6);

		// X0 + X3 - X4 + X6
		M[0]=x[0][0]+x[3][0]-x[4][0]+x[6][0];
		M[1]=x[0][1]+x[3][1]-x[4][1]+x[6][1];
		M[4]=x[0][2]+x[3][2]-x[4][2]+x[6][2];
		M[5]=x[0][3]+x[3][3]-x[4][3]+x[6][3];

		// X1 + X3
		M[2]=x[1][0]+x[3][0];
		M[3]=x[1][1]+x[3][1];
		M[6]=x[1][2]+x[3][2];
		M[7]=x[1][3]+x[3][3];

		// X2 + X4
		M[8]=x[2][0]+x[4][0];
		M[9]=x[2][1]+x[4][1];
		M[12]=x[2][2]+x[4][2];
		M[13]=x[2][3]+x[4][3];

		// X0 + X2 - X1 + X5
		M[10]=x[0][0]+x[2][0]-x[1][0]+x[5][0];
		M[11]=x[0][1]+x[2][1]-x[1][1]+x[5][1];
		M[14]=x[0][2]+x[2][2]-x[1][2]+x[5][2];
		M[15]=x[0][3]+x[2][3]-x[1][3]+x[5][3];

	}
	/*
	inline void Matrix44MulMatrix44(const matrix4& a, const matrix4& b)
	{

		// ��һ�� 
		c[0][0] = x[0][0][0] + x[3][0][0] - x[4][0][0] + x[6][0][0]; 
		c[0][1] = x[0][0][1] + x[3][0][1] - x[4][0][1] + x[6][0][1]; 
		c[1][0] = x[0][1][0] + x[3][1][0] - x[4][1][0] + x[6][1][0]; 
		c[1][1] = x[0][1][1] + x[3][1][1] - x[4][1][1] + x[6][1][1]; 

		// �ڶ��� 
		c[0][2] = x[2][0][0] + x[4][0][0]; 
		c[0][3] = x[2][0][1] + x[4][0][1]; 
		c[1][2] = x[2][1][0] + x[4][1][0]; 
		c[1][3] = x[2][1][1] + x[4][1][1]; 

		// ������ 
		c[2][0] = x[1][0][0] + x[3][0][0]; 
		c[2][1] = x[1][0][1] + x[3][0][1]; 
		c[3][0] = x[1][1][0] + x[3][1][0]; 
		c[3][1] = x[1][1][1] + x[3][1][1]; 


		// ���Ŀ� 

		c[2][2] = x[0][0][0] - x[1][0][0] + x[2][0][0] + x[5][0][0]; 
		c[2][3] = x[0][0][1] - x[1][0][1] + x[2][0][1] + x[5][0][1]; 
		c[3][2] = x[0][1][0] - x[1][1][0] + x[2][1][0] + x[5][1][0]; 
		c[3][3] = x[0][1][1] - x[1][1][1] + x[2][1][1] + x[5][1][1]; 

	}
	*/
};
typedef matrix4<float> matrix4f;

int main(int argc,char ** argv)
{
	matrix4f m1(1,2,3,4,5,6,7,8,8,7,6,5,4,3,2,1);
	matrix4f m2(0,1,1,1,2,0,2,2,3,3,0,3,4,4,4,0);

	matrix4f rA;
	rA.setByProduct(m1,m2);
	rA.print();

	matrix4f rB;
	rB.setByFastProduct(m1,m2);
	rB.print();

	/*Matrix44 n1={1,2,3,4,5,6,7,8,8,7,6,5,4,3,2,1};
	Matrix44 n2={0,1,1,1,2,0,2,2,3,3,0,3,4,4,4,0};

	Matrix44 rB;
	Matrix44MulMatrix44(rB,n2,n1);
	for(int i=0;i<4;++i)
	{
		printf("%.2f,%.2f,%.2f,%.2f\r\n",rB[i][0],rB[i][1],rB[i][2],rB[i][3]);
	}*/
	system("pause");
	return 0;
}