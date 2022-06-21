//   ___________________________________________________
//  |													|\
//  |	Khanon3D									    | |
//  |													| |
//  |	Clase:		 CREC								| |
//  |	Descripcion: Operaciones con matrices           | |
//  |___________________________________________________| |
//   \___________________________________________________\|
//

#ifndef CMATRIX_CLASS
#define CMATRIX_CLASS

// __________________________________________________
//
//   CLASE DE MATRICES
// __________________________________________________
//
class CMatrix
{
public:
	D3DXMATRIX Data;

	// Constructores
	CMatrix(bool Identity=false);

	// Funciones
	void  SetIdentity();
	void  BuildFromVector    (CVector *Vr);
	void  BuildFromQuaternion(CQuaternion *QRot);
	void  BuildPerspective	 (float XRatio=.665f,float YRatio=.5f,
							  float ZMin=1.0f,     float ZMax =100.0f);
	void  Inverse			 (float *Determinant=NULL);
	void  Mult				 (CMatrix & b);

	// Operadores
	CMatrix operator= (const CMatrix &b);
};
#endif