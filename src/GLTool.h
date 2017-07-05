#ifndef GLTOOL_H
#define GLTOOL_H
#define GLFW_INCLUDE_GLU

#include "MathTool.h"
#include <GLFW/glfw3.h>

//光源の構造体
typedef struct{
	int id;
	float position[4];
	float ambient[4];
	float diffuse[4];
	float specular[4];
}Lightf;

//マテリアルの構造体
typedef struct
{
	float ambient[4];
	float diffuse[4];
	float specular[4];
}Materialf;

//OpenGLの初期化
void glInit( void );
//カメラの設定
void setCamera( int _width, int _height );
//光源の設定
void setLight( Lightf *_light );
//マテリアルの設定
void setMaterial( Materialf *_material );
//float型の四次元配列に値をまとめて代入する
void setVec4f( float *_vec, float _x, float _y, float _z, float _w);
//格子の描画
void renderGrid( double _scale );

#endif
