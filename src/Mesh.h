#ifndef MESH_H
#define MESH_H

#include "MathTool.h"
#include "GLTool.h"

//ノード構造体
typedef struct{
	//3次元座標
	Vec3d position;
}Node;

//面構造体
typedef struct{
	//三つのノードの3次元座標
	Vec3d position[3];
	//面法線
	Vec3d normal;
}Facet;

//メッシュ構造体
typedef struct{
	//ノード数
	unsigned int num_node;
	//面数
	unsigned int num_facet;
	//ノード
	Node *node;
	//面
	Facet *facet;
}Mesh;

//メッシュの初期化
void initMesh( Mesh *_mesh );
//メッシュのメモリ確保
void setMeshDim( Mesh *_mesh );
//メッシュのメモリ解放
void releaseMesh( Mesh *_mesh );
//STLメッシュの読み込み
int loadSTLMesh( Mesh *_mesh, const char *_filename );
//メッシュの描画
void renderMesh( Mesh *_mesh );

#endif
