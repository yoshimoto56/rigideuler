#ifndef SOLVER_H
#define SOLVER_H

//接触点数の上限（これ以上の接触点は無視）
#define NUM_CONTACT_MAX 10000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "MathTool.h"
#include "Mesh.h"

//剛体パラメータ
typedef struct{
	//力
	Vec3d force;
	Vec3d force_external;
	Vec3d force_constraint;
	//トルク
	Vec3d torque;
	Vec3d torque_external;
	Vec3d torque_constraint;
	//一般化力ベクトル(力，トルク)
	VecNd F;
	VecNd Fe;
	VecNd Fc;
	//位置
	Vec3d position;
	//速度
	Vec3d velocity;
	//角度
	Vec3d angle;
	//角速度
	Vec3d anguler_velocity;
	//一般化速度ベクトル
	VecNd V;
	//一般化変位ベクトル
	VecNd U;
	//物体座標系から重心座標系への回転行列
	Matd R;
	//重心座標系から物体座標系への回転行列
	Matd invR;
	//体積
	double volume;
	//質量
	double mass;
	//質量中心
	Vec3d center_of_mass;
	//慣性テンソル
	Matd I;
	//物体座標系での慣性テンソル
	Matd I_o;
	//質量行列
	Matd M;
	Matd invM;
	//時間ステップ
	double DT;
	//時刻
	double time;
	//離散時間
	int n;
}RigidParams;

//拘束パラメータ
typedef struct{
	//摩擦係数
	double mu;
	//接触点数
	unsigned int num_contact;
	//拘束条件かどうかを格納する変数
	int *is_constraint;
	//ヤコビ行列
	Matd *Je;
	Matd J;
	Matd tJ;
	//拘束座標系の一般化速度ベクトル
	VecNd V_c;
	//拘束座標系の一般化力ベクトル（拘束力）
	VecNd Fc_c;
}Constraint;


//剛体パラメータの初期化
void initRigidParams( RigidParams *_rigid );
//剛体パラメータの開放
void releaseRigidParams( RigidParams *_rigid );

//重心座標系で運動方程式を解く
void solve( RigidParams *_rigid );

//パラメータの読み込み
void loadRigidParams( RigidParams *_rigid, const char *_filename );

//外力の設定
void setGravity( RigidParams *_rigid, Vec3d _gravity );

//重心座標系の慣性テンソルを算出
void updateItensor( RigidParams *_rigid );

//重心座標系における質量行列の更新
void updateMassMatrix( RigidParams *_rigid );

//一般化ベクトルの生成
void combineVectors( RigidParams *_rigid );

//要素ベクトルへの分解
void decomposeVectors( RigidParams *_rigid );

//拘束パラメータの初期化
void initConstraint( Constraint *_constraint );

//拘束パラメータの開放
void releaseConstraint( Constraint *_constraint );

//接触点の検出
void collisionTest( Mesh *_mesh, RigidParams *_rigid, Mesh *_container, Constraint *_constraint );

//拘束力の算出
void solveConstraint( RigidParams *_rigid, Constraint *_constraint  );



#endif