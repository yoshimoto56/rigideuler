#include <GLFW/glfw3.h>
#include "MathTool.h"
#include "Mesh.h"
#include "GLTool.h"
#include "Solver.h"
//#include "RigidBody.h"

//�E�B���h�E�p�����[�^
#define WINDOW_SIZE_X 1440
#define WINDOW_SIZE_Y 810
//�}�E�X�̉�]�Q�C��
#define ANGLE_GAIN 0.2
//�}�E�X�̃Y�[���Q�C��
#define ZOOM_GAIN 0.01

//���̃p�����[�^
RigidParams rigid;
//�S���p�����[�^
Constraint constraint;
//�d�͉����x
Vec3d gravity;

//����
Lightf light;
//�}�e���A��
Materialf material1;
Materialf material2;
//���̃��b�V��
Mesh mesh;
//�e��
Mesh container;
//�E�B���h�E��
int width;
//�E�B���h�E����
int height;
//��t���[���O�̃}�E�X���W
double pre_x;
double pre_y;
//�J�����̉�]�p
Vec3d camera_rot_angle;
//�J�����̃Y�[����
double camera_zoom;
//���_�̉�]�Ɋւ���ϊ��s��
Matd R, invR;

//Display callback function
void render( void )
{
	//�J�����̐ݒ�
	setCamera( width, height );
 
	//�����̐ݒ�
	setLight( &light );

	//�ϊ��s��̐ݒ�
	glMultMatrixd( invR.X );
	glScaled( camera_zoom, camera_zoom, camera_zoom ); 

	glPushMatrix();
	glTranslated( 0, -35, 0 );
	renderGrid( 10 );
	glPopMatrix();

	//�����̐ݒ�
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 + light.id );
	//�}�e���A���̐ݒ�
	setMaterial( &material1 ); 
	//STL���b�V���̕`��
	glPushMatrix();
	//�v�Z�������̃p�����[�^���g���ĕ��̂���]�E���i�ړ������悤
	//[TODO]
	renderMesh( &mesh );
	glPopMatrix();

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	setMaterial( &material2 ); 
	renderMesh( &container );

	glDisable( GL_LIGHTING );
	glDisable( GL_LIGHT0 + light.id );

	glLineWidth(2);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	renderMesh( &container );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glLineWidth(1);

}

//Mouse motion callback function
void positionCallback(GLFWwindow *_window, double _x, double _y)
{
	//Set camera rotation
	Vec3d gravity_rot;
	Matd Rx, Ry;
	initMat( &Rx );
	initMat( &Ry );

	if(glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1)!=GLFW_RELEASE){    
		double input_x = _x - pre_x;
		double input_y = _y - pre_y;
		camera_rot_angle.x += ANGLE_GAIN * input_x;
		camera_rot_angle.y += ANGLE_GAIN * input_y;

		setRotationalMatrix( camera_rot_angle.x, ROT_AXIS_Y, &Rx );
		setRotationalMatrix( camera_rot_angle.y, ROT_AXIS_X, &Ry );
		multiMatandMat( &Rx, &Ry, &R );
		invMat( &R,&invR );

		//�d�͕�������]
		multiTransferMatandVec3( &invR, &gravity, &gravity_rot );
		setGravity( &rigid, gravity_rot );
	}
	pre_x = _x;
	pre_y = _y;
	releaseMat( &Rx );
	releaseMat( &Ry );
}

void scrollCallback(GLFWwindow *_window, double _x, double _y)
{
	camera_zoom += ZOOM_GAIN * _y;
}

//Keyboard callback function
void keyCallback(GLFWwindow *_window, int _key, int _code, int _action, int _modes )
{
	switch (_key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(_window, GL_TRUE);
			break;
		case GLFW_KEY_I:
			//������
			rigid.position.x = 0;  rigid.position.y = 50;  rigid.position.z = 0;
			rigid.velocity.x = 10;  rigid.velocity.y = 10;  rigid.velocity.z = 0;
			rigid.angle.x = 0;  rigid.angle.y = 0;  rigid.angle.z = 0;
			rigid.anguler_velocity.x = 1;  rigid.anguler_velocity.y = 0;  rigid.anguler_velocity.z = 1;
			gravity.x = 0;	gravity.y = -9.8;	gravity.z = 0;
			break;
		default:
			break;
	}
}

int main( int _argc, char *_argv[] )
{
	GLFWwindow *window;
	char filename[256];
	double current_time, last_time;

	initMat( &R );
	initMat( &invR );
	//�R�}���h���C������
	if( //[TODO]�t�@�C�������R�}���h���C���Ŏw�肵�悤
	}

	//���b�V���̏�����
	initMesh( &mesh );
	//���b�V���̓ǂݍ���
	printf("Loading mesh ...");
	if(	loadSTLMesh( &mesh, filename ) == -1 || loadSTLMesh( &container, "../data/container.stl" ) == -1 ){
		printf( "[FAIL]\n");
		return -1;
	}
	printf("[OK]\n");

	//�V�~�����[�V�����p�����[�^
	initRigidParams( &rigid );
	rigid.DT = 0.001;
	rigid.position.x = 0;  rigid.position.y = 50;  rigid.position.z = 0;
	rigid.velocity.x = 10;  rigid.velocity.y = 10;  rigid.velocity.z = 0;
	rigid.angle.x = 0;  rigid.angle.y = 0;  rigid.angle.z = 0;
	rigid.anguler_velocity.x = 1;  rigid.anguler_velocity.y = 0;  rigid.anguler_velocity.z = 1;
	gravity.x = 0;	gravity.y = -9.8;	gravity.z = 0;
	loadRigidParams( &rigid, "../data/param.ini" );
	setGravity( &rigid, gravity );

	//�S���p�����[�^
	initConstraint( &constraint );
	constraint.mu = 0.8;

	//�����p�����[�^�̐ݒ�
	light.id = 0;
	setVec4f( light.position, 0, 100, 100, 1.0 );
	setVec4f( light.ambient, 0.0, 0.0, 0.0, 1.0 );
	setVec4f( light.diffuse, 1.0, 1.0, 1.0, 1.0 );
	setVec4f( light.specular, 0.0, 0.0, 0.0, 1.0 );

	//���̗p�}�e���A���̐ݒ�
	setVec4f( material1.ambient, 0.0, 0.0, 0.0, 1.0 );
	setVec4f( material1.diffuse, 0.0, 1.0, 1.0, 1.0 );
	setVec4f( material1.specular, 0.0, 0.0, 0.0, 1.0 );

	//���p�}�e���A���̐ݒ�
	setVec4f( material2.ambient, 0.0, 0.0, 0.0, 0.1 );
	setVec4f( material2.diffuse, 0.5, 0.0, 0.5, 0.1 );
	setVec4f( material2.specular, 0.0, 0.0, 0.0, 0.1 );

	//�J�����̏����p�x�C�Y�[����ݒ�
	clearVec3( &camera_rot_angle );
	camera_zoom = 0.2;
	setMatDim( &R, 4, 4 );
	setMatDim( &invR, 4, 4 );
	identityMat( &R );
	identityMat( &invR );

	//GLFW�̏�����
	printf("Initializing GLFW ...");
	if (glfwInit() == GL_FALSE){
		printf("[FAIL]\n");
		return -1;
	}
	printf("[OK]\n");
	printf("Creating window ...");
	window=glfwCreateWindow( WINDOW_SIZE_X, WINDOW_SIZE_Y, "EX2", NULL, NULL );
	if (window == NULL){
		printf("[FAIL]\n");
		glfwTerminate();
		return -1;
	}
	printf("[OK]\n");

	//OpenGL�̏�����
	glfwMakeContextCurrent( window );
	glInit();

	//�R�[���o�b�N�֐��̓o�^
	glfwSetKeyCallback( window, keyCallback );
	glfwSetScrollCallback( window, scrollCallback );
	glfwSetCursorPosCallback( window, positionCallback );

	//�����_�����O���[�v
	last_time = glfwGetTime();
	while ( glfwWindowShouldClose( window ) == GL_FALSE ){
		current_time = glfwGetTime();
		//�t���[�����[�g��\������
		printf( "Frame Rate: %4.4f FPS\r", 1.0 /(current_time - last_time) );
		last_time = current_time;
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glfwGetFramebufferSize( window, &width, &height );

		//[TODO]
		//�ڐG���肵��
		//�S���͂��v�Z����
		//����������������

		render();

		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	//�I������
	glfwDestroyWindow( window );
	glfwTerminate();
  
	//���b�V���p�������̊J��
	releaseMesh( &mesh );
	releaseRigidParams( &rigid );
	releaseConstraint( &constraint );

	releaseMat( &R );
	releaseMat( &invR );

	return 0;
}