#include "GLTool.h"

void glInit( void )
{
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LINE_SMOOTH );
	glShadeModel( GL_FLAT );
	glDisable( GL_CULL_FACE );
	glCullFace( GL_FRONT );
	glFrontFace( GL_CW );
	glEnable( GL_BLEND );
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

void setCamera( int _width, int _height )
{
	glViewport( 0, 0, _width, _height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( -0.02, 0.02, -0.02*(double)_height / _width, 0.02*(double)_height / _width, 0.1, 1000 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( 0.0, 0.0, 150.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
}

void setLight( Lightf *_light )
{
	glLightfv( GL_LIGHT0+_light->id, GL_POSITION, _light->position );
	glLightfv( GL_LIGHT0+_light->id, GL_AMBIENT, _light->ambient );
	glLightfv( GL_LIGHT0+_light->id, GL_DIFFUSE, _light->diffuse );
	glLightfv( GL_LIGHT0+_light->id, GL_SPECULAR, _light->specular );
}

void setMaterial( Materialf *_material )
{
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, _material->ambient );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, _material->diffuse );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, _material->specular );
}

void setVec4f( float *_vec, float _x, float _y, float _z, float _w)
{
	_vec[ 0 ] = _x;	_vec[ 1 ] = _y;	_vec[ 2 ] = _z;	_vec[ 3 ] = _w;
}

void renderGrid( double _scale )
{
	int i;
	glColor3d( 0, 0, 0 );
	for( i = -10; i <= 10; i++ ){
		glBegin( GL_LINE_STRIP );
		glVertex3d( -10 * _scale, 0, i * _scale );
		glVertex3d( 10 * _scale, 0, i * _scale );
		glEnd();
		glBegin( GL_LINE_STRIP );
		glVertex3d( i * _scale, 0, -10 * _scale );
		glVertex3d( i * _scale, 0, 10 * _scale );
		glEnd();
	}
}