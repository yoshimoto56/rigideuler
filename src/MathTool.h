#ifndef MATHTOOL_H //�C���N���[�h�K�[�h
#define MATHTOOL_H

#ifndef M_PI
#define M_PI 3.141592653589793238
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//��]���̕���
enum{
	ROT_AXIS_Z,
	ROT_AXIS_X,
	ROT_AXIS_Y
};

/*�x�N�g���̍\����*/
typedef union
{
	//x,y,z��X[0],X[1],X[2]�����ꂼ��Ή�
	struct{ double x, y, z; };
	double X[ 3 ];
}Vec3d;

typedef struct
{
	//�x�N�g���̎���
	unsigned int dim;
	//�x�N�g���̗v�f
	double *X;
}VecNd;

/*�s��̍\����*/
typedef struct
{
	//��
	unsigned int ncol;
	//�s��
	unsigned int nrow;
	//�v�f��
	unsigned int size;
	//�s��̗v�f
	double *X;
}Matd;

/*�x�N�g���v�Z�Ɋւ��֐�*/
//�x�N�g���̏������F�x�N�g�����g�p����O�ɍŏ��ɌĂяo��
void initVecN( VecNd *_A );

//�x�N�g���̃[���N���A
void clearVec3( Vec3d *_A );
void clearVecN( VecNd *_A );

//�����̐ݒ�F�w�肵�������̃x�N�g���v�f���������m�ۂ���
void setVecNDim( VecNd *_A, unsigned int _dim );

//�x�N�g���̉���F�m�ۂ����v�f�̃��������J������
void releaseVecN( VecNd *_A );

//�x�N�g���̑����Z
void sumVec3andVec3( Vec3d *_A, Vec3d *_B, Vec3d *_dst );
void sumVecNandVecN( VecNd *_A, VecNd *_B, VecNd *_dst );

//�x�N�g���̈����Z
void subVec3andVec3( Vec3d *_A, Vec3d *_B, Vec3d *_dst );
void subVecNandVecN( VecNd *_A, VecNd *_B, VecNd *_dst );

//�x�N�g���̃X�P�[�����O
void scalingVec3( double _s, Vec3d *_A, Vec3d *_dst );
void scalingVecN( double _s, VecNd *_A, VecNd *_dst );

//�x�N�g���̐��K��
void normalizeVec3( Vec3d *_A,  Vec3d *_dst );
void normalizeVecN( VecNd *_A,  VecNd *_dst );

//�x�N�g���̓���
double dotVec3andVec3(Vec3d *_A, Vec3d *_B);
double dotVecNandVecN(VecNd *_A, VecNd *_B);

//�x�N�g���̊O��
void crossVec3andVec3(Vec3d *_A, Vec3d *_B, Vec3d *_dst);

//�x�N�g���̐�Βl
double absVec3( Vec3d *_A );
double absVecN( VecNd *_A );

//�����x�N�g���̎Z�o
void getOrthogonalVec3( Vec3d *_A,  Vec3d *_dst );

//�x�N�g���̃R���\�[���ւ̏o��
void printVec3(Vec3d *_A);
void printVecN(VecNd *_A);

/*�s��v�Z�Ɋւ��֐�*/
//�s��̏������F�s����g�p����O�ɍŏ��ɌĂяo��
void initMat( Matd *_A );

//�s��̃[���N���A
void clearMat( Matd *_A );

//�P�ʍs����Z�b�g
void identityMat( Matd *_A );

//�����̐ݒ�F�w�肵���񐔁E�s���̍s��̗v�f���������m�ۂ���
void setMatDim( Matd *_A, unsigned int _ncol, unsigned int _nrow );

//�s��̉���F�s��̗v�f�̃��������J������
void releaseMat( Matd *_A );

//�s��̑����Z
void sumMatandMat( Matd *_A, Matd *_B, Matd *_dst );

//�s��̈����Z
void subMatandMat( Matd *_A, Matd *_B, Matd *_dst );

//�s��̊|���Z
void multiMatandMat( Matd *_A, Matd *_B, Matd *_dst );
void multiMatandVec3( Matd *_A, Vec3d *_B, VecNd *_dst );
void multiMat33andVec3( Matd *_A, Vec3d *_B, Vec3d *_dst );
void multiMat33andMat33( Matd *_A, Matd *_B, Matd *_dst );
void multiTransferMatandVec3( Matd *_A, Vec3d *_B, Vec3d *_dst );
void multiMatandVecN( Matd *_A, VecNd *_B, VecNd *_dst );

//�s��̃X�P�[�����O
void scalingMat( double _s, Matd *_A, Matd *_dst );

//�s��
double detMat( Matd *_A);

//�]�u�s��
void trMat( Matd *_A, Matd *_dst);

//�t�s��
void invMat( Matd *_A, Matd *_dst);
double lu( int n, double *a, int *ip );
double invMatbyLU( double *a_inv, double *a, int n );

//��]�s��̐���
void setRotationalMatrix( double _angle, int _axis,  Matd *_dst );
void setRotationalMatrixFrom( Vec3d _axis, Matd *_dst);

//�s��̃R���\�[���ւ̏o��
void printMat(Matd *_A);


#endif