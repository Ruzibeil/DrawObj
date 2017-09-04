#include "ObjModelLoader.h"
#include <stdlib.h>
#include <stdio.h>
#include <FreeImage/FreeImage.h>
#pragma comment(lib,"FreeImage.lib")
#define  INTERVAL 36
using namespace std;
typedef unsigned char  BYTE;
/* Application settings */
string title = "零件模型显示";
char s_imgname[50];
int windowWidth = 640,
windowHeight = 480;
float fieldOfVision = 80.0f,
cameraDistance = 5.0f;
float backgroundColor[3] = { 0, 0, 0 };                // !> 背景设为黑色
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

	glMatrixMode(GL_MODELVIEW);																				 // !>设置投影矩阵
	glLoadIdentity();																										 // !> 每一次投影之后可以复位回到屏幕原点
	
	glTranslatef(0.0f, 0.0f, -5 * cameraDistance);                                                            // !> 设置相机距离，使得图像投影在屏幕上大小适中
	// Adding light
	if (isLightingEnabled)
	{
		// Set lighting intensity and color
		glLightfv(GL_LIGHT0, GL_AMBIENT, colorDarkGray);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, colorLightGray);

		if (isSpecularLightEnabled)
		{
			glLightfv(GL_LIGHT0, GL_SPECULAR, colorWhite);

			// !> 设置材料的颜色
			glMaterialfv(GL_FRONT, GL_SPECULAR, colorWhite);
			glMaterialf(GL_FRONT, GL_SHININESS, shininess); // Shininess: 0 to 128
		}

		// Set the light position
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	}

	if (isWireframeModeEnabled)
	{
		// !> 打开mesh模式
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
	}
	else
	{
		// !> 关闭mesh模式
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);

	}
	//!> 使用一组push与pop 将一组旋转变换压栈再出栈
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
				// !> 使用FreeImage 保存图片，注意在程序运行前先建立自己相应的文件夹，这时候绘制的内容还存在后台缓冲区中。
				sprintf_s(s_imgname, "F:\\OpenGL_img1\\%d_%d_%d.bmp", rotateAngle_x, rotateAngle_y, rotateAngle_z); //!> 给每个image命名，根据旋转的角度
				BYTE* pixels = new BYTE[3 * windowWidth * windowHeight];//!> three channels
				glReadPixels(0, 0, windowWidth, windowHeight, FORMAT, GL_UNSIGNED_BYTE, pixels);   //!> 将绘制的内容读入到*pixel
				FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, windowWidth, windowHeight, 3 * windowWidth, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
				FreeImage_Save(FIF_BMP, image, s_imgname, 0);  //!> 存储为bmp位图
				FreeImage_Unload(image);
				delete[] pixels;
				// !>
				glFlush(); //!>强制将缓冲区的内容执行至屏幕显示
				glutSwapBuffers(); //!> 前台缓冲（屏幕显示）和后台缓冲交换（绘制） ，使得绘制可以显示， 
	
}
// !> 按键控制
/*  !> 该update实现每个方向同时旋转，每次旋转0.5，从而形成一个平缓的动画
void Update(int value)
{
	//rotateAngle += 0.5f;
	count_img++;
	sprintf_s(s_imgname, "F:\\OpenGL_img\\%d.bmp", count_img);
	if ((rotateAngle > 360) || (rotateAngle < -360))
	{
		rotateAngle = 0.0f;
	}
	glutPostRedisplay();                                    // !>标记当前窗口需要重新绘制
	glutTimerFunc(timeForUpdatingFrame, Update, 0);
}
*/

void Update(int value)  //!>int value 区分不同类别的更新,本程序只有一个update
{
	
	nscreenshots++;
	//cout << nscreenshots << endl;
	if (rotateAngle_z<324)
	{
		rotateAngle_z = rotateAngle_z + INTERVAL;// !> INTERVAL为间隔，本例取36
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
				if (rotateAngle_y ==0 &&rotateAngle_z ==0)  // !>终止条件
				{
					cout <<"得到"<< nscreenshots <<"个模型"<< endl;
					exit(EXIT_SUCCESS);
				}
				//cout << nscreenshots << endl;
				//exit(EXIT_SUCCESS);
			}
		}
	}
	glutPostRedisplay();  // !>更新显示内容需要
	glutTimerFunc(timeForUpdatingFrame, Update, 0); //!> 时间更新函数
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

		// !> 触发mesh模式
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

// !> 透视投影时需要从modelview改变为 GL_PROJECTION
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
	//glutIdleFunc(DrawScene); //!>不自动而是采取键盘更新
	glutReshapeFunc(HandleResize);
	glutKeyboardFunc(HandleKeypress);
	//glutPostRedisplay();
	glutTimerFunc(timeForUpdatingFrame, Update, 0);     // Add a timer
	glutMainLoop();
	return EXIT_SUCCESS;
}
