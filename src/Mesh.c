#include "Mesh.h"

void initMesh( Mesh *_mesh )
{
	_mesh->num_node = 0;
	_mesh->num_facet = 0;
	_mesh->node = NULL;
	_mesh->facet = NULL;
}

void setMeshDim( Mesh *_mesh )
{
	//[TODO]
	//calloc()を使ってnodeとfacetのメモリを必要数確保する処理を書きましょう
}

void releaseMesh( Mesh *_mesh )
{
	if( _mesh->num_node > 0 ){
		free( _mesh->node );
	}
	if( _mesh->num_facet > 0 ){
		free( _mesh->facet );
	}
	initMesh( _mesh );
}

int loadSTLMesh( Mesh *_mesh, const char *_filename )
{
	FILE *file;
	char buf[ 256 ];
	char dummy1[ 256 ];
	char dummy2[ 256 ];
	unsigned int i;
	unsigned int count;
  
	if( (file = fopen( _filename, "r" ) ) == NULL ){
		//読み込みに失敗
		return -1;
	}
	releaseMesh( _mesh );
	while( ! feof( file ) ){
		fgets( buf, 256, file );
		if( strstr( buf, "vertex" ) ){
			_mesh->num_node ++;
		}
		else if( strstr( buf, "normal" ) ){
			_mesh->num_facet ++;
		}
	}
	setMeshDim( _mesh );
	rewind( file );
	count = 0;
	while( ! feof( file ) ){
		fgets( buf, 256, file );
		if( strstr( buf, "normal" ) ){
			sscanf( buf, "%s %s %lf %lf %lf", dummy1 , dummy2
					, &_mesh->facet[ count ].normal.x
					, &_mesh->facet[ count ].normal.y
					, &_mesh->facet[ count ].normal.z );
		}
		else if( strstr( buf, "vertex" ) ){
			sscanf(buf, "%s %lf %lf %lf", dummy1
				, &_mesh->facet[ count ].position[ 0 ].x
				, &_mesh->facet[ count ].position[ 0 ].y
				, &_mesh->facet[ count ].position[ 0 ].z );
			fgets( buf, 256, file );
			sscanf(buf, "%s %lf %lf %lf", dummy1
				, &_mesh->facet[ count ].position[ 1 ].x
				, &_mesh->facet[ count ].position[ 1 ].y
				, &_mesh->facet[ count ].position[ 1 ].z );
			fgets( buf, 256, file );
			sscanf(buf, "%s %lf %lf %lf", dummy1
				, &_mesh->facet[ count ].position[ 2 ].x
				, &_mesh->facet[ count ].position[ 2 ].y
				, &_mesh->facet[ count ].position[ 2 ].z );
			for( i =0; i < 3; i ++ ){
				memcpy( _mesh->node[ 3 * count + i ].position.X
					, _mesh->facet[ count ].position[ i ].X, sizeof( double ) * 3 );
			}
			count ++;
		}
	}
	fclose( file );
	return 1;
}

void renderMesh( Mesh *_mesh )
{
	unsigned int i;
	glColor3d( 0, 0, 1 );
	//[TODO]
}
