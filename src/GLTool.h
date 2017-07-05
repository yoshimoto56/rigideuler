#ifndef GLTOOL_H
#define GLTOOL_H
#define GLFW_INCLUDE_GLU

#include "MathTool.h"
#include <GLFW/glfw3.h>

//�����̍\����
typedef struct{
	int id;
	float position[4];
	float ambient[4];
	float diffuse[4];
	float specular[4];
}Lightf;

//�}�e���A���̍\����
typedef struct
{
	float ambient[4];
	float diffuse[4];
	float specular[4];
}Materialf;

//OpenGL�̏�����
void glInit( void );
//�J�����̐ݒ�
void setCamera( int _width, int _height );
//�����̐ݒ�
void setLight( Lightf *_light );
//�}�e���A���̐ݒ�
void setMaterial( Materialf *_material );
//float�^�̎l�����z��ɒl���܂Ƃ߂đ������
void setVec4f( float *_vec, float _x, float _y, float _z, float _w);
//�i�q�̕`��
void renderGrid( double _scale );

#endif
