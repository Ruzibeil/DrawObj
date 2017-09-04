#include "ObjModelLoader.h"
#include <stdlib.h>
#include <stdio.h>
#include <FreeImage/FreeImage.h>
#pragma comment(lib,"FreeImage.lib")
#define  INTERVAL 36
using namespace std;
typedef unsigned char  BYTE;
/* Application settings */
string title = "���ģ����ʾ";
char s_imgname[50];
int windowWidth = 640,
windowHeight = 480;
float fieldOfVision = 80.0f,
cameraDistance = 5.0f;
float backgroundColor[3] = { 0, 0, 0 };                // !> ������Ϊ��ɫ
int timeForUpdatingFrame = 25;
const int KEY_ESCAPE = 27;
bool isWireframeModeEnabled = false;                      // Default: false

/* Lighting Settings */
// Set material properties for lighting
bool isLightingEnabled = true,
isSpecularLightEnabled = false;

float shininess = 50.0f;                      // Shininess: 0 to 128;

GLfloat colorWhite[] = { 1.00, 1.00, 1.00, 1.0 };  // Color: White
GLfloat colorDarkGray[] = { 0.10, 0.10, 0.10, 1.0 };  // Color: Dark gray
GLfloat colorLightGray[] = { 0.75, 0.75, 0.75, 1.0 };  // Color: Light gray

GLfloat lightPosition[] = { 0.0, 1.0, 1.0, 0.0 };
// save i mg
static GLubyte *pixels = NULL;
static const GLenum FORMAT = GL_RGB;
static const GLuint FORMAT_NBYTES = 4;
int nscreenshots = 0;


string filename = "L_shape3.obj";
ObjModelLoader *monkeyModel = NULL;
// ------------------------------------------------

int rotateAngle_x = 0, rotateAngle_y = 0, rotateAngle_z = 0;


void DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);																				 // !>����ͶӰ����
	glLoadIdentity();																										 // !> ÿһ��ͶӰ֮����Ը�λ�ص���Ļԭ��
	
	glTranslatef(0.0f, 0.0f, -5 * cameraDistance);                                                            // !> ����������룬ʹ��ͼ��ͶӰ����Ļ�ϴ�С����
	// Adding light
	if (isLightingEnabled)
	{
		// Set lighting intensity and color
		glLightfv(GL_LIGHT0, GL_AMBIENT, colorDarkGray);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, colorLightGray);

		if (isSpecularLightEnabled)
		{
			glLightfv(GL_LIGHT0, GL_SPECULAR, colorWhite);

			// !> ���ò��ϵ���ɫ
			glMaterialfv(GL_FRONT, GL_SPECULAR, colorWhite);
			glMaterialf(GL_FRONT, GL_SHININESS, shininess); // Shininess: 0 to 128
		}

		// Set the light position
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	}

	if (isWireframeModeEnabled)
	{
		// !> ��meshģʽ
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
	}
	else
	{
		// !> �ر�meshģʽ
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);

	}
	//!> ʹ��һ��push��pop ��һ����ת�任ѹջ�ٳ�ջ
				glPushMatrix();

				// Drawing a cube
				glColor3f(0.60, 0.60, 0.60);                            // Color: Gray
				glRotatef(rotateAngle_x, true, false, false);
				glRotatef(rotateAngle_y, false, true, false);
				glRotatef(rotateAngle_z, false, false, true);
				glScaled(0.4, 0.4, 0.4);
				glTranslatef(-2195.0f, -755.0f, -712.0f);
				//cout << "wait" << endl;
				// ------------------------------------------------
				monkeyModel->Draw();
				// ------------------------------------------------
				glPopMatrix();
				// !> ʹ��FreeImage ����ͼƬ��ע���ڳ�������ǰ�Ƚ����Լ���Ӧ���ļ��У���ʱ����Ƶ����ݻ����ں�̨�������С�
				sprintf_s(s_imgname, "F:\\OpenGL_img1\\%d_%d_%d.bmp", rotateAngle_x, rotateAngle_y, rotateAngle_z); //!> ��ÿ��image������������ת�ĽǶ�
				BYTE* pixels = new BYTE[3 * windowWidth * windowHeight];//!> three channels
				glReadPixels(0, 0, windowWidth, windowHeight, FORMAT, GL_UNSIGNED_BYTE, pixels);   //!> �����Ƶ����ݶ��뵽*pixel
				FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, windowWidth, windowHeight, 3 * windowWidth, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
				FreeImage_Save(FIF_BMP, image, s_imgname, 0);  //!> �洢Ϊbmpλͼ
				FreeImage_Unload(image);
				delete[] pixels;
				// !>
				glFlush(); //!>ǿ�ƽ�������������ִ������Ļ��ʾ
				glutSwapBuffers(); //!> ǰ̨���壨��Ļ��ʾ���ͺ�̨���彻�������ƣ� ��ʹ�û��ƿ�����ʾ�� 
	
}
// !> ��������
/*  !> ��updateʵ��ÿ������ͬʱ��ת��ÿ����ת0.5���Ӷ��γ�һ��ƽ���Ķ���
void Update(int value)
{
	//rotateAngle += 0.5f;
	count_img++;
	sprintf_s(s_imgname, "F:\\OpenGL_img\\%d.bmp", count_img);
	if ((rotateAngle > 360) || (rotateAngle < -360))
	{
		rotateAngle = 0.0f;
	}
	glutPostRedisplay();                                    // !>��ǵ�ǰ������Ҫ���»���
	glutTimerFunc(timeForUpdatingFrame, Update, 0);
}
*/

void Update(int value)  //!>int value ���ֲ�ͬ���ĸ���,������ֻ��һ��update
{
	
	nscreenshots++;
	//cout << nscreenshots << endl;
	if (rotateAngle_z<324)
	{
		rotateAngle_z = rotateAngle_z + INTERVAL;// !> INTERVALΪ���������ȡ36
	}
	else
	{
		rotateAngle_z = 0;
		if (rotateAngle_y<324)
		{
			rotateAngle_y = rotateAngle_y + INTERVAL;
		} 
		else
		{
			rotateAngle_y = 0;
			if (rotateAngle_x <324)
			{
				rotateAngle_x = rotateAngle_x + INTERVAL;
			}
			
			else
			{
				rotateAngle_x = 0;
				if (rotateAngle_y ==0 &&rotateAngle_z ==0)  // !>��ֹ����
				{
					cout <<"�õ�"<< nscreenshots <<"��ģ��"<< endl;
					exit(EXIT_SUCCESS);
				}
				//cout << nscreenshots << endl;
				//exit(EXIT_SUCCESS);
			}
		}
	}
	glutPostRedisplay();  // !>������ʾ������Ҫ
	glutTimerFunc(timeForUpdatingFrame, Update, 0); //!> ʱ����º���
}

// Called when a key is pressed
void HandleKeypress(unsigned char key, int x, int y) 
{
	switch (key)
	{
		// Escape key
	case KEY_ESCAPE:
		exit(EXIT_SUCCESS);
		break;

		// !> ����meshģʽ
	case 'w':
		isWireframeModeEnabled = !isWireframeModeEnabled;
		isLightingEnabled = !isLightingEnabled;
		break;
// 	case 'x':
// 		rotateAngle_x = rotateAngle_x + INTERVAL;
// 		if (rotateAngle_x >= 360)  rotateAngle_x = 0;
// 		break;
// 	case  'y':
// 		rotateAngle_y = rotateAngle_y + INTERVAL;
// 		if (rotateAngle_y >= 360)  rotateAngle_y = 0;
// 		break;
// 	case 'z':
// 		rotateAngle_z = rotateAngle_z + INTERVAL;
// 		if (rotateAngle_z >=360)  rotateAngle_z = 0;
// 		break;
	}
}

// !> ͸��ͶӰʱ��Ҫ��modelview�ı�Ϊ GL_PROJECTION
void HandleResize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfVision, (double)width / (double)height, 1.0, 300.0);

}


int main(int argc, char* argv[])
{
	// Initialize GLUT
// 	cin >> rotateAngle_x;
// 	cin >> rotateAngle_y;
// 	cin >> rotateAngle_z;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);

	//Create the window
	glutCreateWindow(title.c_str());

	// Initialize
	glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
	glClearDepth(1.0f);                                 // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);                            // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);                             // Set the type of depth-test
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);                            // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

	// Lighting set up
	if (isLightingEnabled)
	{
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	// ------------------------------------------------
	monkeyModel = new ObjModelLoader(filename);
	// ------------------------------------------------

	// Set handler functions
	//count_img++;
	
	glutDisplayFunc(DrawScene);
	//glutIdleFunc(DrawScene); //!>���Զ����ǲ�ȡ���̸���
	glutReshapeFunc(HandleResize);
	glutKeyboardFunc(HandleKeypress);
	//glutPostRedisplay();
	glutTimerFunc(timeForUpdatingFrame, Update, 0);     // Add a timer
	glutMainLoop();
	return EXIT_SUCCESS;
}
