#ifndef MESH_H
#define MESH_H

#include "MathTool.h"
#include "GLTool.h"

//�m�[�h�\����
typedef struct{
	//3�������W
	Vec3d position;
}Node;

//�ʍ\����
typedef struct{
	//�O�̃m�[�h��3�������W
	Vec3d position[3];
	//�ʖ@��
	Vec3d normal;
}Facet;

//���b�V���\����
typedef struct{
	//�m�[�h��
	unsigned int num_node;
	//�ʐ�
	unsigned int num_facet;
	//�m�[�h
	Node *node;
	//��
	Facet *facet;
}Mesh;

//���b�V���̏�����
void initMesh( Mesh *_mesh );
//���b�V���̃������m��
void setMeshDim( Mesh *_mesh );
//���b�V���̃��������
void releaseMesh( Mesh *_mesh );
//STL���b�V���̓ǂݍ���
int loadSTLMesh( Mesh *_mesh, const char *_filename );
//���b�V���̕`��
void renderMesh( Mesh *_mesh );

#endif
