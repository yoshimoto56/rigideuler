#ifndef SOLVER_H
#define SOLVER_H

//�ڐG�_���̏���i����ȏ�̐ڐG�_�͖����j
#define NUM_CONTACT_MAX 10000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "MathTool.h"
#include "Mesh.h"

//���̃p�����[�^
typedef struct{
	//��
	Vec3d force;
	Vec3d force_external;
	Vec3d force_constraint;
	//�g���N
	Vec3d torque;
	Vec3d torque_external;
	Vec3d torque_constraint;
	//��ʉ��̓x�N�g��(�́C�g���N)
	VecNd F;
	VecNd Fe;
	VecNd Fc;
	//�ʒu
	Vec3d position;
	//���x
	Vec3d velocity;
	//�p�x
	Vec3d angle;
	//�p���x
	Vec3d anguler_velocity;
	//��ʉ����x�x�N�g��
	VecNd V;
	//��ʉ��ψʃx�N�g��
	VecNd U;
	//���̍��W�n����d�S���W�n�ւ̉�]�s��
	Matd R;
	//�d�S���W�n���畨�̍��W�n�ւ̉�]�s��
	Matd invR;
	//�̐�
	double volume;
	//����
	double mass;
	//���ʒ��S
	Vec3d center_of_mass;
	//�����e���\��
	Matd I;
	//���̍��W�n�ł̊����e���\��
	Matd I_o;
	//���ʍs��
	Matd M;
	Matd invM;
	//���ԃX�e�b�v
	double DT;
	//����
	double time;
	//���U����
	int n;
}RigidParams;

//�S���p�����[�^
typedef struct{
	//���C�W��
	double mu;
	//�ڐG�_��
	unsigned int num_contact;
	//�S���������ǂ������i�[����ϐ�
	int *is_constraint;
	//���R�r�s��
	Matd *Je;
	Matd J;
	Matd tJ;
	//�S�����W�n�̈�ʉ����x�x�N�g��
	VecNd V_c;
	//�S�����W�n�̈�ʉ��̓x�N�g���i�S���́j
	VecNd Fc_c;
}Constraint;


//���̃p�����[�^�̏�����
void initRigidParams( RigidParams *_rigid );
//���̃p�����[�^�̊J��
void releaseRigidParams( RigidParams *_rigid );

//�d�S���W�n�ŉ^��������������
void solve( RigidParams *_rigid );

//�p�����[�^�̓ǂݍ���
void loadRigidParams( RigidParams *_rigid, const char *_filename );

//�O�͂̐ݒ�
void setGravity( RigidParams *_rigid, Vec3d _gravity );

//�d�S���W�n�̊����e���\�����Z�o
void updateItensor( RigidParams *_rigid );

//�d�S���W�n�ɂ����鎿�ʍs��̍X�V
void updateMassMatrix( RigidParams *_rigid );

//��ʉ��x�N�g���̐���
void combineVectors( RigidParams *_rigid );

//�v�f�x�N�g���ւ̕���
void decomposeVectors( RigidParams *_rigid );

//�S���p�����[�^�̏�����
void initConstraint( Constraint *_constraint );

//�S���p�����[�^�̊J��
void releaseConstraint( Constraint *_constraint );

//�ڐG�_�̌��o
void collisionTest( Mesh *_mesh, RigidParams *_rigid, Mesh *_container, Constraint *_constraint );

//�S���͂̎Z�o
void solveConstraint( RigidParams *_rigid, Constraint *_constraint  );



#endif