//   ___________________________________________________
//  |													|\
//  |	Level 51 f.c								    | |
//  |													| |
//  |	Clase:		 CREC								| |
//  |	Descripcion: Operaciones con matrices           | |
//  |___________________________________________________| |
//   \___________________________________________________\|
//
#include "Global.h"

void  ludcmp(D3DMATRIX & a, int *indx, float *d);
void  lubksb(D3DMATRIX & a, int *indx, float *b);
float mget  (D3DMATRIX & m, int x,int y);
void  mset  (D3DMATRIX & m, int x,int y,float val);

// __________________________________________________
//

CMatrix::CMatrix(bool Identity)
{
	if (Identity) SetIdentity();
}

// __________________________________________________
//

void CMatrix::Mult(CMatrix & b)
{
	D3DMATRIX ret;
	memset(&ret,0,sizeof(D3DMATRIX));
	
	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++) {
			for (int k=0; k<4; k++) 
			{
				float v=mget(ret,i,j) + mget(Data,k, j) * mget(b.Data,i, k);
				mset(ret,i, j,v);//mget(Data,k, j) * mget(b,i, k);
			}
		}
	}
	memcpy(&Data,&ret,sizeof(D3DMATRIX));
} 

// __________________________________________________
//
void CMatrix::SetIdentity(void)
{
	memset(&Data,0,sizeof(Data));
	Data._11=Data._22=Data._33=Data._44=1.0f;
}
// __________________________________________________
//
void CMatrix::BuildFromVector(CVector *Vr)
{
	/*
	CQuaternion Q(Vr);
	BuildFromQuaternion(&Q);
	*/
	D3DXMatrixRotationYawPitchRoll(&Data,Vr->Data.y,Vr->Data.x,Vr->Data.z);
}
// __________________________________________________
//
void CMatrix::BuildPerspective(float XRatio,float YRatio, 
							   float ZMin,  float ZMax)
{
	/*
	float fFOV		= 80.0f/180.0f;
	float fAspect	= YRatio/XRatio;
    if(fabs(ZMax-ZMin)<0.01f)   return;
    if(fabs(sin(fFOV/2))<0.01f) return;

    FLOAT w = fAspect * (FLOAT)( cos(fFOV/2)/sin(fFOV/2) );
    FLOAT h =   1.0f  * (FLOAT)( cos(fFOV/2)/sin(fFOV/2) );
    FLOAT Q = ZMax / ( ZMax - ZMin );

    ZeroMemory( &Data, sizeof(D3DMATRIX) );
    Data._11 = w;
    Data._22 = h;
    Data._33 = Q;
    Data._34 = 1.0f;
    Data._43 = -Q*ZMin;
	*/
	D3DXMatrixPerspectiveFovLH(&Data,D3DX_PI/4,1.33f,ZMin,ZMax);
}
// __________________________________________________
//
void CMatrix::BuildFromQuaternion(CQuaternion *QRot)
{
    FLOAT xx = QRot->Data.x*QRot->Data.x,
		  xy = QRot->Data.x*QRot->Data.y,
		  xz = QRot->Data.x*QRot->Data.z,
		  xw = QRot->Data.x*QRot->Data.w;
    FLOAT yy = QRot->Data.y*QRot->Data.y,
		  yz = QRot->Data.y*QRot->Data.z,
		  yw = QRot->Data.y*QRot->Data.w;
    FLOAT zz = QRot->Data.z*QRot->Data.z,
		  zw = QRot->Data.z*QRot->Data.w;
    FLOAT ww = QRot->Data.w*QRot->Data.w;

    Data._11 = xx-yy-zz+ww;
    Data._12 = 2.0f*(xy-zw);
    Data._13 = 2.0f*(xz+yw);

    Data._21 = 2.0f*(xy+zw);
    Data._22 = -xx+yy-zz+ww;
    Data._23 = 2.0f*(yz-xw);

    Data._31 = 2.0f*(xz-yw);
    Data._32 = 2.0f*(yz+xw);
    Data._33 = -xx-yy+zz+ww;

    Data._14 = Data._41 = 0.0f;
    Data._24 = Data._42 = 0.0f;
    Data._34 = Data._43 = 0.0f;
    Data._44 = xx+yy+zz+ww;

	//D3DXMatrixRotationQuaternion(&Data,&QRot->Data);
}
// __________________________________________________
//
void CMatrix::Inverse(float *Determinant)
{
	D3DMATRIX	n;
	memcpy(&n,&Data,sizeof(D3DMATRIX));
	int			i, j, indx[4];
	float		d, col[4];
	ludcmp(n, indx, &d);
	for (j=0; j<4; j++) 
	{
		for (i=0; i<4; i++) 
		{
			col[i] = 0.0f;
		}
		col[j] = 1.0f;
		lubksb(n, indx, col);
		for (i=0; i<4; i++) 
		{
			Data(i, j) = col[i];
		}
	}
	
	//if (Determinant==NULL) D3DXMatrixInverse(&Data,NULL,&Data);
	//else				   D3DXMatrixInverse(&Data,Determinant,&Data);
}
// __________________________________________________
//
void lubksb(D3DMATRIX & a, int *indx, float *b)
{
	int		i, j, ii=-1, ip;
	float	sum;

	for (i=0; i<4; i++) 
	{
		ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
		if (ii>=0) 
		{
			for (j=ii; j<=i-1; j++) 
			{
				sum -= mget(a,i, j) * b[j];
			}
		} 
		else if (sum != 0.0) 
		{
			ii = i;
		}
		b[i] = sum;
	}
	for (i=3; i>=0; i--) 
	{
		sum = b[i];
		for (j=i+1; j<4; j++) 
		{
			sum -= mget(a,i, j) * b[j];
		}
		b[i] = sum/mget(a,i, i);
	}
}
// __________________________________________________
//
void ludcmp(D3DMATRIX & a, int *indx, float *d)
{
	float	vv[4];              
	float	big, dum, sum, tmp;
	int		i, imax, j, k;

	*d = 1.0f;
	for (i=0; i<4; i++) 
	{
		big = 0.0f;
		for (j=0; j<4; j++) 
		{
			if ((tmp = (float) fabs(mget(a,i, j))) > big) big = tmp;
		}
		vv[i] = 1.0f/big;
	}
	for (j=0; j<4; j++) 
	{
		for (i=0; i<j; i++) 
		{
			sum = mget(a,i, j);
			for (k=0; k<i; k++) 
			{
				sum -= mget(a,i, k) * mget(a,k, j);
			}
			mset(a,i, j,sum);
		}
		big = 0.0f;
		for (i=j; i<4; i++) 
		{
			sum = mget(a,i, j);
			for (k=0; k<j; k++) 
			{
				sum -= mget(a,i, k)*mget(a,k, j);
			}
			mset(a,i, j,sum);
			if ((dum = vv[i] * (float)fabs(sum)) >= big) 
			{
				big = dum;
				imax = i;
			}
		}
		if (j != imax) 
		{
			for (k=0; k<4; k++) 
			{
				dum = mget(a,imax, k);
				mset(a,imax, k,mget(a,j, k));
				mset(a,j, k,dum);
			}
			*d = -(*d);
			vv[imax] = vv[j];
		}
		indx[j] = imax;
		if (mget(a,j, j) == 0.0f) 
		{
			mset(a,j, j,1.0e-20f);      /* can be 0.0 also... */
		}
		if (j != 3) 
		{
			dum = 1.0f/mget(a,j, j);
			for (i=j+1; i<4; i++) 
			{
				float t=mget(a,i,j);
				mset(a,i,j,t*dum);
			}
		}
	}
}


__inline void mset(D3DMATRIX &m,int x,int y,float val)
{
	x++;y++;
	switch(x)
	{
	case 1: switch(y)
			{
				case 1: m._11 = val;break;
				case 2: m._12 = val;break;
				case 3: m._13 = val;break;
				case 4: m._14 = val;break;
			} break;
	case 2: switch(y)
			{
				case 1: m._21 = val;break;
				case 2: m._22 = val;break;
				case 3: m._23 = val;break;
				case 4: m._24 = val;break;
			} break;
	case 3: switch(y)
			{
				case 1: m._31 = val;break;
				case 2: m._32 = val;break;
				case 3: m._33 = val;break;
				case 4: m._34 = val;break;
			} break;
	case 4: switch(y)
			{
				case 1: m._41 = val;break;
				case 2: m._42 = val;break;
				case 3: m._43 = val;break;
				case 4: m._44 = val;break;
			} break;
	}
}

__inline float mget(D3DMATRIX &m,int x,int y)
{
	x++;y++;
	switch(x)
	{
	case 1: switch(y)
			{
				case 1: return m._11;break;
				case 2: return m._12;break;
				case 3: return m._13;break;
				case 4: return m._14;break;
			} break;
	case 2: switch(y)
			{
				case 1: return m._21;break;
				case 2: return m._22;break;
				case 3: return m._23;break;
				case 4: return m._24;break;
			} break;
	case 3: switch(y)
			{
				case 1: return m._31;break;
				case 2: return m._32;break;
				case 3: return m._33;break;
				case 4: return m._34;break;
			} break;
	case 4: switch(y)
			{
				case 1: return m._41;break;
				case 2: return m._42;break;
				case 3: return m._43;break;
				case 4: return m._44;break;
			} break;
	}
	return 0.0f;
}

// ------------------------
// Operadores
// ------------------------

CMatrix CMatrix::operator =(const CMatrix &b)
{
	Data._11 = b.Data._11;
	Data._12 = b.Data._12;
	Data._13 = b.Data._13;
	Data._14 = b.Data._14;

	Data._21 = b.Data._21;
	Data._22 = b.Data._22;
	Data._23 = b.Data._23;
	Data._24 = b.Data._24;

	Data._31 = b.Data._31;
	Data._32 = b.Data._32;
	Data._33 = b.Data._33;
	Data._34 = b.Data._34;

	Data._41 = b.Data._41;
	Data._42 = b.Data._42;
	Data._43 = b.Data._43;
	Data._44 = b.Data._44;

	return *this;
}