#include "Solver.h"

void initRigidParams( RigidParams *_rigid )
{
	clearVec3( &_rigid->force_constraint );
	clearVec3( &_rigid->force_external );
	clearVec3( &_rigid->force );
	clearVec3( &_rigid->torque_constraint );
	clearVec3( &_rigid->torque_external );
	clearVec3( &_rigid->torque );
	initVecN( &_rigid->Fc );
	setVecNDim( &_rigid->Fc, 6 );
	initVecN( &_rigid->Fe );
	setVecNDim( &_rigid->Fe, 6 );
	initVecN( &_rigid->F );
	setVecNDim( &_rigid->F, 6 );
	clearVec3( &_rigid->position );
	clearVec3( &_rigid->velocity );
	clearVec3( &_rigid->anguler_velocity );
	clearVec3( &_rigid->angle );
	initVecN( &_rigid->V );
	setVecNDim( &_rigid->V, 6 );
	initVecN( &_rigid->U );
	setVecNDim( &_rigid->U, 6 );
	initMat( &_rigid->R );
	setMatDim( &_rigid->R, 4, 4 );
	identityMat( &_rigid->R );
	initMat( &_rigid->invR );
	setMatDim( &_rigid->invR, 4, 4 );
	_rigid->volume = 0;
	_rigid->mass = 0;
	clearVec3( &_rigid->center_of_mass );
	initMat( &_rigid->I );
	setMatDim( &_rigid->I, 3, 3 );
	initMat( &_rigid->I_o );
	setMatDim( &_rigid->I_o, 3, 3 );
	initMat( &_rigid->M );
	setMatDim( &_rigid->M, 6, 6 );
	initMat( &_rigid->invM );
	setMatDim( &_rigid->invM, 6, 6 );
	_rigid->DT = 0;
	_rigid->time = 0;
	_rigid->n = 0;
}

void releaseRigidParams( RigidParams *_rigid )
{
	releaseVecN( &_rigid->Fc );
	releaseVecN( &_rigid->Fe );
	releaseVecN( &_rigid->F );
	releaseVecN( &_rigid->V );
	releaseVecN( &_rigid->U );
	releaseMat( &_rigid->I );
	releaseMat( &_rigid->I_o );
	releaseMat( &_rigid->R );
	releaseMat( &_rigid->invR );
	releaseMat( &_rigid->M );
	releaseMat( &_rigid->invM );
}

void solve( RigidParams *_rigid )
{
	VecNd dV;
	VecNd dU;
	Matd dR;
	Matd R;
	initVecN( &dV );
	setVecNDim ( &dV, 6 );
	initVecN( &dU );
	setVecNDim ( &dU, 6 );
	initMat( &dR );
	setMatDim( &dR, 4 ,4);
	initMat( &R );
	setMatDim( &R, 4 ,4);

	//並進と回転をまとめて一般化ベクトルを生成
	updateItensor( _rigid );
	updateMassMatrix( _rigid );
	combineVectors( _rigid );

	//オイラー法で剛体の運動方程式を解く
	//(V[n+1]=invM*F*DT+V[n], U[n+1]=V[n]*DT+U[n]を計算する)
	//[TODO]


	//一般化ベクトルを並進と回転に分解
	decomposeVectors( _rigid );

	//描画で利用するための回転行列の更新
	setRotationalMatrixFrom( _rigid->angle, &_rigid->R );
	trMat( &_rigid->R, &_rigid->invR );

	//時刻を進める
	_rigid->n ++;
	_rigid->time = _rigid->n * _rigid->DT;

	releaseVecN( &dV );
	releaseVecN( &dU );
	releaseMat( &dR );
	releaseMat( &R );
//	printVecN(&_mesh->V);
}

void combineVectors( RigidParams *_rigid )
{
	memcpy(&_rigid->U.X[ 0 ], _rigid->position.X, sizeof( double ) * 3 );
	memcpy(&_rigid->U.X[ 3 ], _rigid->angle.X, sizeof( double ) * 3 );
	memcpy(&_rigid->V.X[ 0 ], _rigid->velocity.X, sizeof( double ) * 3 );
	memcpy(&_rigid->V.X[ 3 ], _rigid->anguler_velocity.X, sizeof( double ) * 3 );
	sumVec3andVec3( &_rigid->force_constraint, &_rigid->force_external, &_rigid->force );
	sumVec3andVec3( &_rigid->torque_constraint, &_rigid->torque_external, &_rigid->torque );
	memcpy(&_rigid->Fc.X[ 0 ], _rigid->force_constraint.X, sizeof( double ) * 3 );
	memcpy(&_rigid->Fc.X[ 3 ], _rigid->torque_constraint.X, sizeof( double ) * 3 );
	memcpy(&_rigid->Fe.X[ 0 ], _rigid->force_external.X, sizeof( double ) * 3 );
	memcpy(&_rigid->Fe.X[ 3 ], _rigid->torque_external.X, sizeof( double ) * 3 );
	memcpy(&_rigid->F.X[ 0 ], _rigid->force.X, sizeof( double ) * 3 );
	memcpy(&_rigid->F.X[ 3 ], _rigid->torque.X, sizeof( double ) * 3 );
}

void decomposeVectors( RigidParams *_rigid )
{
	memcpy(_rigid->velocity.X, &_rigid->V.X[ 0 ], sizeof( double ) * 3 );
	memcpy(_rigid->anguler_velocity.X, &_rigid->V.X[ 3 ], sizeof( double ) * 3 );
	memcpy(_rigid->position.X, &_rigid->U.X[ 0 ], sizeof( double ) * 3 );
	memcpy(_rigid->angle.X, &_rigid->U.X[ 3 ], sizeof( double ) * 3 );
	memcpy(_rigid->force.X, &_rigid->F.X[ 0 ], sizeof( double ) * 3 );
	memcpy(_rigid->torque.X, &_rigid->F.X[ 3 ], sizeof( double ) * 3 );
}

void loadRigidParams( RigidParams *_rigid, const char *_filename )
{
	FILE *file;
	char buf[1024];
	if( (file = fopen( _filename, "r" ) ) != NULL){
		//体積
		fgets( buf, 1024, file );
		sscanf( buf, "volume:%lf\n", &_rigid->volume);
		//質量
		fgets( buf, 1024, file );
		sscanf( buf, "mass:%lf\n", &_rigid->mass);
		//質量中心
		fgets( buf, 1024, file );
		sscanf( buf, "com:%lf,%lf,%lf\n",
			&_rigid->center_of_mass.x, &_rigid->center_of_mass.y , &_rigid->center_of_mass.z);
		//慣性テンソル
		fgets( buf, 1024, file );
		sscanf( buf, "Itensor:%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
			&_rigid->I_o.X[ 0 ], &_rigid->I_o.X[ 1 ], &_rigid->I_o.X[ 2 ],
			&_rigid->I_o.X[ 3 ], &_rigid->I_o.X[ 4 ], &_rigid->I_o.X[ 5 ],
			&_rigid->I_o.X[ 6 ], &_rigid->I_o.X[ 7 ], &_rigid->I_o.X[ 8 ] );
		fclose( file );

		updateItensor( _rigid );
		updateMassMatrix( _rigid );
	}
}

void updateItensor( RigidParams *_rigid )
{
	Matd RI;
	initMat( &RI );
	setMatDim( &RI, 3, 3 );
	//物体座標系の慣性テンソル_rigid->I_oと回転行列を使って_rigid->Iを更新する
	//[TODO]
	releaseMat( &RI );
}

void updateMassMatrix( RigidParams *_rigid )
{
	unsigned int i, j;
	for( i = 0; i < 3; i ++ ){
		for( j = 0; j < 3; j ++ ){
			if( i == j){
				_rigid->M.X[ 6 * j + i ] = _rigid->mass;
			}else{
				_rigid->M.X[ 6 * j + i ] = 0;
			}
			_rigid->M.X[ 6 * (j + 3) + (i + 3) ] = _rigid->I.X[ 3 * j + i ];
			_rigid->M.X[ 6 * (j + 0) + (i + 3) ] = 0;
			_rigid->M.X[ 6 * (j + 3) + (i + 0) ] = 0;
		}
	}
	invMat( &_rigid->M, &_rigid->invM);
//	printMat( &_rigid->invM);
}


void setGravity(  RigidParams *_rigid, Vec3d _gravity )
{
	scalingVec3( _rigid->mass, &_gravity, &_rigid->force_external );
}


void initConstraint( Constraint *_constraint )
{
	unsigned int i;
	_constraint->is_constraint = ( int * )calloc( NUM_CONTACT_MAX * 6, sizeof(int) );
	_constraint->Je = ( Matd * )malloc( sizeof(Matd) * NUM_CONTACT_MAX );
	initMat(  &_constraint->J );
	initMat( &_constraint->tJ );
	initVecN( &_constraint->Fc_c );
	initVecN( &_constraint->V_c );
	for( i = 0; i < NUM_CONTACT_MAX; i++ ){
		initMat( &_constraint->Je[i] );
		setMatDim( &_constraint->Je[i], 6, 6 );
	}
}

void releaseConstraint( Constraint *_constraint )
{
	unsigned int i;
	for(i = 0; i < NUM_CONTACT_MAX; i++){
		releaseMat( &_constraint->Je[i] );
	}
	free( _constraint->is_constraint );
	free( _constraint->Je );
	releaseMat(  &_constraint->J );
	releaseMat( &_constraint->tJ );
	releaseVecN( &_constraint->Fc_c );
	releaseVecN( &_constraint->V_c );
}

void collisionTest( Mesh *_mesh, RigidParams *_rigid, Mesh *_container, Constraint *_constraint )
{
	unsigned int i, j, k, l;
	Vec3d ri_o, ri, ui, ui_c;
	Vec3d e[ 3 ], r_e[ 3 ];
	double dist;

	_constraint->num_contact = 0;

	for( i = 0; i < _mesh->num_node; i += ( int )( _mesh->num_node * 0.01 ) ){
		subVec3andVec3( &_mesh->node[ i ].position, &_rigid->center_of_mass, &ri_o );
		multiTransferMatandVec3( &_rigid->R, &ri_o, &ri );
		sumVec3andVec3( &ri, &_rigid->position, &ui );
		for( j = 0; j < _container->num_facet; j ++ ){
			subVec3andVec3( &ui, &_container->facet[ j ].position[ 0 ], &ui_c );
			dist = dotVec3andVec3( &ui_c, &_container->facet[ j ].normal );
			if( dist >= 0 ){//接触があった
				//(1) 拘束座標系の生成
				//e[ 0 ] = _container->facet[ j ].normal;
				scalingVec3(-1, &_container->facet[ j ].normal, &e[ 0 ]);
				getOrthogonalVec3( &e[ 0 ], &e[ 1 ] );
				crossVec3andVec3( &e[ 0 ], &e[ 1 ], &e[ 2 ] );
				crossVec3andVec3( &ri, &e[ 0 ], &r_e[ 0 ] );
				crossVec3andVec3( &ri, &e[ 1 ], &r_e[ 1 ] );
				crossVec3andVec3( &ri, &e[ 2 ], &r_e[ 2 ] );
				//(2) 要素ヤコビ行列の計算
				//eとr_eを使ってヤコビ行列_constraint->Je[ _constraint->num_contact ]をセットしよう
				//[TODO]
				if( NUM_CONTACT_MAX - 1 > _constraint->num_contact ){
					_constraint->num_contact ++;
				}
				break;
			}
		}
	}
}

void solveConstraint( RigidParams *_rigid, Constraint *_constraint  )
{
	unsigned int i, j, l, m;
	Matd A, A_f, invA_f;
	Matd JMdt;
	Matd D, invD, S, invDS;
	VecNd invDSF_c, DV_b;
	VecNd V_b;
	VecNd b, b_f, Fc_f;
	double norm;

	if( _constraint->num_contact > 0 ){
		initMat( &A );		initMat( &A_f );		initMat( &invA_f );		initMat( &JMdt );
		initMat( &D );		initMat( &invD );		initMat( &S );		initMat( &invDS );
		initVecN( &b );		initVecN( &b_f );
		initVecN( &Fc_f );		initVecN( &V_b );
		initVecN( &invDSF_c );		initVecN( &DV_b );
		setMatDim( &A, 6 * _constraint->num_contact, 6 * _constraint->num_contact );
		setMatDim( &A_f, 3 * _constraint->num_contact, 3 * _constraint->num_contact );
		setMatDim( &D, 6 * _constraint->num_contact, 6 * _constraint->num_contact );
		setMatDim( &invD, 6 * _constraint->num_contact, 6 * _constraint->num_contact );
		setMatDim( &S, 6 * _constraint->num_contact, 6 * _constraint->num_contact );
		setMatDim( &invDS, 6 * _constraint->num_contact, 6 * _constraint->num_contact );
		setMatDim( &_constraint->J, 6, 6 * _constraint->num_contact );
		setVecNDim( &_constraint->Fc_c, 6 * _constraint->num_contact );
		setVecNDim( &_constraint->V_c, 6 * _constraint->num_contact );
		setVecNDim( &b, 6 * _constraint->num_contact );
		setVecNDim( &b_f, 3 * _constraint->num_contact );
		setVecNDim( &Fc_f, 3 * _constraint->num_contact );

		//(3) 全体ヤコビ行列の生成
		for( i = 0; i < _constraint->num_contact; i ++ ){
			memcpy( &_constraint->J.X[ 6 * 6 * i ], _constraint->Je[ i ].X, sizeof( double ) * 6 * 6 );
		}
		trMat( &_constraint->J, &_constraint->tJ);

		//(4) 拘束方程式を構成する 
		//(V=AFc_c+b)を計算するためのA, b, _constraint->V_cを計算しよう
		//[TODO]

		//(5) 線形相補問題を解く(動きが拘束されて速度がゼロになる要素だけ取り出す)
		//(5)-1 拘束された要素のみを取り出す (0=A_fF_f+b_f)
		for(i = 0; i < _constraint->num_contact ; i++){
			for(l = 0; l < 3 ; l++){
				for(j = 0; j < _constraint->num_contact ; j++){
					for(m = 0; m < 3 ; m++){
						A_f.X[ 3 * _constraint->num_contact * (3 * j + m) + 3 * i + l ]
						= A.X[ 6 * _constraint->num_contact * (6 * j + m) + 6 * i + l ];
					}
				}
				b_f.X[ 3 * i + l ] = b.X[ 6 * i + l ];
				//接触問題では・ﾀ進が拘束され
				_constraint->is_constraint[ 6 * i + l + 0 ] = 1;
				//回転は拘束されない
				_constraint->is_constraint[ 6 * i + l + 3 ] = 0;
			}
		}
		//(5)-2 初期解をセットする
		invMat( &A_f, &invA_f);
		multiMatandVecN( &invA_f, &b_f, &Fc_f );
		//拘束条件を満たすかどうか確認する
		for( i = 0; i < _constraint->num_contact ; i++ ){
			if( Fc_f.X[ 3 * i + 0 ] < 0 ){
				Fc_f.X[ 3 * i + 0 ] = 0;
				_constraint->is_constraint[ 6 * i ] = 0;
			}
			norm = sqrt( Fc_f.X[ 3 * i + 1 ] * Fc_f.X[ 3 * i + 1 ] + Fc_f.X[ 3 * i + 2 ] * Fc_f.X[ 3 * i + 2 ] ); 
			if( norm > _constraint->mu * fabs( Fc_f.X[ 3 * i + 0 ] )){
				Fc_f.X[ 3 * i + 1 ] *= _constraint->mu * fabs( Fc_f.X[ 3 * i + 0 ] ) / norm;
				Fc_f.X[ 3 * i + 2 ] *= _constraint->mu * fabs( Fc_f.X[ 3 * i + 0 ] ) / norm;
				_constraint->is_constraint[ 6 * i + 1 ] = 0;
				_constraint->is_constraint[ 6 * i + 2 ] = 0;
			}
			memcpy( &_constraint->Fc_c.X[ 6 * i + 0 ], &Fc_f.X[ 3 * i + 0 ], sizeof( double ) * 3 );
			memset( &_constraint->Fc_c.X[ 6 * i + 3 ], 0, sizeof( double ) * 3 );
		}
		//(5)-3 ガウスザイデル法で解を更新する
		//A=D-Sに分解
		memcpy( S.X, A.X, sizeof( 6 * _constraint->num_contact * 6 * _constraint->num_contact ) );
		for( i = 0; i < 6 * _constraint->num_contact ; i++ ){
			D.X[ 6 * _constraint->num_contact * i + i ] = A.X[ 6 * _constraint->num_contact * i + i ];
			S.X[ 6 * _constraint->num_contact * i + i ] = 0;
		}
		scalingMat( -1, &S, &S );
		invMat( &D, &invD );
		multiMatandMat( &invD, &S, &invDS );
		for( j = 0; j < 6 * _constraint->num_contact ; j++ ){
			if( _constraint->is_constraint[ j ] == 1){
				_constraint->V_c.X[ j ] = 0;
			}
		}
		subVecNandVecN( &_constraint->V_c, &b, &V_b );
		//反復計算で解の精度を高める（試しに50回）
		for( i = 0; i < 50; i++ ){
			//V_cを求める
			multiMatandVecN( &invDS, &_constraint->Fc_c, &invDSF_c );
			multiMatandVecN( &invD, &V_b, &DV_b );
			subVecNandVecN( &invDSF_c, &DV_b, &_constraint->Fc_c );
			//条件確認
			for( j = 0; j < _constraint->num_contact ; j++ ){
				if( _constraint->Fc_c.X[ 6 * j + 0 ] < 0 ){
					_constraint->Fc_c.X[ 6 * j + 0 ] = 0;
					_constraint->is_constraint[ 6 * j ] = 0;
				}else{
					_constraint->is_constraint[ 6 * j ] = 1;
				}
				norm = sqrt( _constraint->Fc_c.X[ 6 * j + 1 ] * _constraint->Fc_c.X[ 6 * j+ 1 ] + _constraint->Fc_c.X[6 * j + 2 ] * _constraint->Fc_c.X[ 6 * j + 2 ] ); 
				if( norm > _constraint->mu * fabs( _constraint->Fc_c.X[ 6 * j + 0 ] )){
					_constraint->Fc_c.X[ 6 * j + 1 ] *= _constraint->mu * fabs( _constraint->Fc_c.X[ 6 * j + 0 ] ) / norm;
					_constraint->Fc_c.X[ 6 * j + 2 ] *= _constraint->mu * fabs( _constraint->Fc_c.X[ 6 *j + 0 ] ) / norm;
					_constraint->is_constraint[ 6 * j + 1 ] = 0;
					_constraint->is_constraint[ 6 * j + 2 ] = 0;
				}
				_constraint->Fc_c.X[ 6 * j + 3 ] = _constraint->Fc_c.X[ 6 * j + 4 ] = _constraint->Fc_c.X[ 6 * j + 5 ] =0;
			}
		}

		//(6) 求まった速度と力を拘束座標系から基準座標系へ変換し，更新する
		multiMatandVecN( &_constraint->tJ, &_constraint->Fc_c, &_rigid->Fc );
		for( j = 0; j < 6 * _constraint->num_contact ; j++ ){
			if( _constraint->is_constraint[ j ] == 1)_constraint->V_c.X[ j ] = 0;
		}
		multiMatandVecN( &_constraint->tJ, &_constraint->V_c, &_rigid->V );
		memcpy( _rigid->force_constraint.X, &_rigid->Fc.X[ 0 ], sizeof( double ) * 3 );
		memcpy( _rigid->torque_constraint.X, &_rigid->Fc.X[ 3 ], sizeof( double ) * 3 );
//		printVecN( &_rigid->V );

		releaseMat( &D );		releaseMat( &invD );		releaseMat( &S );		releaseMat( &invDS );
		releaseMat( &A );		releaseMat( &A_f );		releaseMat( &invA_f );		releaseMat( &JMdt );
		releaseVecN( &b );		releaseVecN( &b_f );		releaseVecN( &Fc_f );
		releaseVecN( &V_b );		releaseVecN( &DV_b );		releaseVecN( &invDSF_c );

	}else{
		memset( _rigid->force_constraint.X, 0, sizeof( double ) * 3 );
		memset( _rigid->torque_constraint.X, 0, sizeof( double ) * 3 );
	}
}
