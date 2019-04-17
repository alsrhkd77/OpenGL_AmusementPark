#include<glut.h>
#include<GLU.H>
#include<GL.H>
#include<iostream>
#include<Windows.h>
#include<mmsystem.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

using namespace std;

#pragma comment(lib,"winmm.lib")
#define M_Run "..\\BGM\\parkmusic.wav"
#define GL_PI 3.1415f
#define GL_Root 1.414f
GLfloat fAspect;
GLfloat WidthFactor;
GLfloat HeightFactor;
float xRot = 15.0;
float yRot = 225.0;
float zRot = 0.0;
float ClickX = 0.0;
float ClickY = 0.0;
float MouseRotX = 0.0;
float MouseRotY = 0.0;
float ViewAngle = 0.0;
float ViewUpAngle = 0.0;
float ViewMoveX = 0.0;
float ViewMoveZ = -16.0;
float HeroMotion;
float HeroMotionAdd = 10.0;
float HeroRot = 0.0;
float HeroMoveX = 17.0;
float HeroMoveZ = -17.0;
float CloudPosition[10][3] = { 0 };
float CloudETC[10][2] = { 0 };
float FireWorkPosition[5][3] = { 0 };	//x+z+scale
float FireWorkColor[5][4] = { 0 };
float WheelRot;
float VikingRot;
float VikingAngle = 3.0;
float DropTowerMove = -27.0;
float DropToewrUpdown = 0;	// 0 = �ö󰡱�, 1 = ��������
int WorldTime = 0;
int SkyColor[3] = { 165, 205, 255 };
int mousecheck = 0;
int View = 0;	//����  0=3��Ī, 1= ���ΰ� 1��Ī
int RolPosition[3] = { 0,485,483 };
int RolSpeed = 1;
int HeroMoving = 0;
int HeroMoveCheck = 0;
int DropTowerDelay = 0;
int OnRide = 0;	// 0 = ��Ž, 1 = ����ŷ, 2 = ������, 3 = ���̵��, 4 = �ѷ��ڽ���  - ������ȯ�� ������ �ᵵ�ɵ�, ���� �ٽ� ���ƿö� ��ġ �ʱ�ȭ �ϰ� ride 0���� �����
int FireworkTail = 0;
int FireworkProgress = 0;	//�Ҳɳ��� �����Ȳ
int FireworkTime = 0;

void MakeCloud(float x = 0.0, float y = 0.0, float z = 0.0, float scale = 1.0) {
	glPushMatrix();
	glTranslatef(x, 20.0 + y, z);
	glScalef(scale, scale, scale);
	glutSolidSphere(2.2, 25, 25);
	glTranslatef(3.25, 0.0, 0.0);
	glutSolidSphere(2.6, 25, 25);
	glTranslatef(3.25, 0.0, 0.0);
	glutSolidSphere(2.2, 25, 25);
	glPopMatrix();
}

void Cloud() {
	for (int i = 0; i < 10; i++) {
		glPushMatrix();
		glRotatef(CloudETC[i][0], 0.0, 1.0, 0.0);
		MakeCloud(CloudPosition[i][0], CloudPosition[i][1], CloudPosition[i][2], CloudETC[i][1]);
		glPopMatrix();
	}
}

void ResetView() {
	ViewMoveX = 0.0;
	ViewMoveZ = -16.0;
	ViewUpAngle = 0.0;
	ViewAngle = 0.0;
}

void ResetHero() {
	HeroRot = 0.0;
	HeroMoveX = 17.0;
	HeroMoveZ = -17.0;
	OnRide = 0;
	xRot = 15.0;
	yRot = 225.0;
}

void ViewMoveRange() {
	//������ ������ ��ġ �ʱ�ȭ
	if (ViewMoveX > 34.5) {
		ResetView();
	}
	if (ViewMoveX < -34.5) {
		ResetView();
	}
	if (ViewMoveZ > 34.5) {
		ResetView();
	}
	if (ViewMoveZ < -34.5) {
		ResetView();
	}
}

int HeroMoveRange(int Value) {	//�̵������� �������� Ȯ�� / 1=�̵����� 2=�̵��Ұ�
	int result;

	result = 1;

	if (Value == 1) {	//+z����
		if (HeroMoveZ == -28.0&&HeroMoveX >= 29.5&&HeroMoveX <= 32.0) {		//�ѷ��ڽ��� ��ǥ��
			result = 0;
		}
		if (HeroMoveZ == 9.0 && HeroMoveX <= 34.0&&HeroMoveX >= 21.0) {		//�ѷ��ڽ��� ȸ������
			result = 0;
		}
		if (HeroMoveZ == 10.0 && HeroMoveX <= 17.5&&HeroMoveX >= 13.5) {	//����ŷ ��ǥ��
			result = 0;
		}
		if (HeroMoveZ == 12.0 && HeroMoveX <= 19.5&&HeroMoveX >= 11.0) {	//����ŷ
			result = 0;
		}
		if (HeroMoveZ == -3.5&&HeroMoveX <= 9.5&&HeroMoveX >= -9.5) {		//������
			result = 0;
		}
		if (HeroMoveZ == -6.0&&HeroMoveX <= 2.0&&HeroMoveX >= -2.0) {		//����ġ ��ǥ��
			result = 0;
		}
		if (HeroMoveZ == -23.5&&HeroMoveX <= -11.5&&HeroMoveX >= 14.5) {	//���̵�� ��ǥ��
			result = 0;
		}
		if (HeroMoveZ == -28.0 && HeroMoveX <= -14.5&&HeroMoveX >= -28.0) {		//���̵��
			result = 0;
		}
		if (HeroMoveZ>= 34.0) {		//�� ��
			result = 0;
		}
	}
	else if (Value == 2) {	//-z����
		if (HeroMoveZ == 26.0 && HeroMoveX <= 34.0 && HeroMoveX >= 21.0) {	//�ѷ��ڽ��� ȸ������
			result = 0;
		}
		if (HeroMoveZ == 18.0&&HeroMoveX <= 19.5 && HeroMoveX >= 11.0) {	//����ŷ
			result = 0;
		}
		if (HeroMoveZ == 1.5&&HeroMoveX <= 9.5&&HeroMoveX >= -9.5) {	//������
			result = 0;
		}
		if (HeroMoveZ == -8.0&&HeroMoveX <= 34.0&&HeroMoveX >= 32.0) {	//�ѷ��ڽ��� ���ۺκ�
			result = 0;
		}
		if (HeroMoveZ == -24.0&&HeroMoveX <= 32.0 && HeroMoveX >= 29.5) {	//�ѷ��ڽ��� ��ǥ��
			result = 0;
		}
		if (HeroMoveZ == -30.5&&HeroMoveX <= 30.0&&HeroMoveX >= -6.0) {	//�ѷ��ڽ��� ������ ���ۺκ�
			result = 0;
		}
		if (HeroMoveZ == -14.0&&HeroMoveX <= -14.5&&HeroMoveX >= -28.0) {	//���̵��
			result = 0;
		}
		if (HeroMoveZ == -19 && HeroMoveX <= -11.5&&HeroMoveX >= -14.5) {	//���̵�� ��ǥ��
			result = 0;
		}
		if (HeroMoveZ <= -34.0) {	//�� ��
			result = 0;
		}
	}
	else if (Value == 3) {	//+x����
		if (HeroMoveX == -28.0&&HeroMoveZ <= -14.0&&HeroMoveZ >= -28.0) {	//���̵��
			result = 0;
		}
		if (HeroMoveX == -9.5&&HeroMoveZ <= 1.5&&HeroMoveZ >= -3.5) {	//������
			result = 0;
		}
		if (HeroMoveX == -2.0&&HeroMoveZ <= -3.5&&HeroMoveZ >= -5.0) {	//������ ��ǥ��
			result = 0;
		}
		if (HeroMoveX == 11.0&&HeroMoveZ <= 18.0&&HeroMoveZ >= 12.0) {	//����ŷ
			result = 0;
		}
		if (HeroMoveX == 13.5&&HeroMoveZ <= 12.0&&HeroMoveZ >= 10.0) {	//����ŷ ��ǥ��
			result = 0;
		}
		if (HeroMoveX == 21.0&&HeroMoveZ <= 26.0&&HeroMoveZ >= 9) {	//�ѷ��ڽ��� ȸ���κ�
			result = 0;
		}
		if (HeroMoveX == -6.0&&HeroMoveZ <= -30.5&&HeroMoveZ >= -34.0) {	//�ѷ��ڽ��� ������ �κ�
			result = 0;
		}
		if (HeroMoveX == 30.0&&HeroMoveZ <= -28.0&&HeroMoveZ >= -30.5) {	//�ѷ��ڽ��� ��ǥ�� ��1
			result = 0;
		}
		if (HeroMoveX == 32.0&&HeroMoveZ <= -8.0&&HeroMoveZ >= -24.0) {	//�ѷ��ڽ��� ��ǥ�� ��2
			result = 0;
		}
		if (HeroMoveX == 29.5&&HeroMoveZ <= -24.0&&HeroMoveZ >= -28.0) {	//�ѷ��ڽ��� ��ǥ��
			result = 0;
		}
		if (HeroMoveX >= 34.0) {	//�� ��
			result = 0;
		}
	}
	else {	//-x����
		if (HeroMoveX == 19.5&&HeroMoveZ <= 18.0&&HeroMoveZ >= 12.0) {	//����ŷ
			result = 0;
		}
		if (HeroMoveX == 17.5&&HeroMoveZ <= 12.0&&HeroMoveZ >= 10.0) {	//����ŷ ��ǥ��
			result = 0;
		}
		if (HeroMoveX == 9.5&&HeroMoveZ <= 1.5&&HeroMoveZ >= -3.5) {	//������
			result = 0;
		}
		if (HeroMoveX == 2.0&&HeroMoveZ <= -3.5&&HeroMoveZ >= -6.0) {	//������ ��ǥ��
			result = 0;
		}
		if (HeroMoveX == -11.5&&HeroMoveZ <= -19.0&&HeroMoveZ >= -23.5) {	//���̵�� ��ǥ��
			result = 0;
		}
		if (HeroMoveX == -14.5&&HeroMoveZ <= -14.0&&HeroMoveZ >= -28.0) {	//���̵��
			result = 0;
		}
		if (HeroMoveX <= -34.0) {
			result = 0;
		}
	}

	return result;
}

void ChangeRide() {
	if (HeroMoveX <= 17.5&&HeroMoveX >= 13.5&&HeroMoveZ <= 10.0&&HeroMoveZ >= 9.0 && HeroRot == 180.0) {	//����ŷ �¿��
		OnRide = 1;
	}
	if (HeroMoveX <= 2.0&&HeroMoveX >= -2.0&&HeroMoveZ <= -5.0&&HeroMoveZ >= -6.0 && HeroRot == 180.0) {	//������ �¿��
		OnRide = 2;
	}
	if (HeroMoveX <= -10.5&&HeroMoveX >= -11.5&&HeroMoveZ <= -19.0&&HeroMoveZ >= -23.5 && HeroRot == 90.0) {	//���̵�� �¿��
		OnRide = 3;
	}
	if (HeroMoveX <= 30.5&&HeroMoveX >= 29.5&&HeroMoveZ <= -24.0&&HeroMoveZ >= -28 && HeroRot == -90.0) {	//�ѷ��ڽ��� �¿��
		OnRide = 4;
	}
}

void CalculateViewXA() {	//1��Ī�����̵� = x������++
	float ViewX;
	float ViewZ;

	ViewX = sin(GL_PI*((ViewAngle + 90.0) / 180.0));
	ViewZ = cos(GL_PI*((ViewAngle + 90.0) / 180.0));

	ViewMoveX = ViewMoveX - ViewX;
	ViewMoveZ = ViewMoveZ + ViewZ;
}

void CalculateViewZA() {	//1��Ī�����̵� = z������++
	float ViewX;
	float ViewZ;

	ViewX = sin(GL_PI*(ViewAngle / 180.0));
	ViewZ = cos(GL_PI*(ViewAngle / 180.0));

	ViewMoveX = ViewMoveX - ViewX;
	ViewMoveZ = ViewMoveZ + ViewZ;
}

void CalculateViewXM() {	//1��Ī�����̵� = x������--
	float ViewX;
	float ViewZ;

	ViewX = sin(GL_PI*((ViewAngle + 90.0) / 180.0));
	ViewZ = cos(GL_PI*((ViewAngle + 90.0) / 180.0));

	ViewMoveX = ViewMoveX + ViewX;
	ViewMoveZ = ViewMoveZ - ViewZ;
}

void CalculateViewZM() {	//1��Ī�����̵� = z������--
	float ViewX;
	float ViewZ;

	ViewX = sin(GL_PI*(ViewAngle / 180.0));
	ViewZ = cos(GL_PI*(ViewAngle / 180.0));

	ViewMoveX = ViewMoveX + ViewX;
	ViewMoveZ = ViewMoveZ - ViewZ;
}

void FireWork(int Tail) {
	glPushMatrix();
	glPushMatrix();	//����
	glScalef(0.7, 0.7, 0.7);
	glutWireIcosahedron();
	glPopMatrix();

	glPointSize(1.5);

	for (int i = 1; i <= Tail; i++) {	//����
		glTranslatef(0.0, -((float)i/60.0), 0.0);
		glBegin(GL_POINTS);
		glVertex3f(0.0, 0.0, 0.0);
		glEnd();
	}

	glPopMatrix();
}

void MakeFireWork() {	//¦������
	glPushMatrix();
	if (FireworkProgress <= 28) {	//���
		glTranslatef(0.0, -28.0 + (float)FireworkProgress, 0.0);
		glPushMatrix();
		FireWork(FireworkTail);
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glTranslatef(0.0, -28.0 + (float)FireworkProgress, 0.0);
		FireWork(FireworkTail);
		glPopMatrix();
		for (float j = 60.0; j <= 120.0; j = j + 60.0) {	//��Ʈ����
			for (float i = 0.0; i <= 360.0; i = i + 60.0) {
				glPushMatrix();
				glRotatef(i, 0.0, 1.0, 0.0);
				glRotatef(j, 1.0, 0.0, 0.0);
				glScalef(0.5, 0.5, 0.5);
				glTranslatef(0.0, -28.0 + (float)FireworkProgress, 0.0);
				FireWork(FireworkTail);
				glPopMatrix();
			}
		}
	}
	glPopMatrix();
}

void PlayFireWork() {
	for (int i = 0; i < 5; i++) {
		glPushMatrix();
		glTranslatef(FireWorkPosition[i][0], 0.0, FireWorkPosition[i][1]);
		glColor3f((float)FireWorkColor[i][0] / 255.0, (float)FireWorkColor[i][1] / 255.0, (float)FireWorkColor[i][2] / 255.0);
		glScalef(FireWorkPosition[i][2], FireWorkPosition[i][2], FireWorkPosition[i][2]);
		MakeFireWork();
		glPopMatrix();
	}
}

void Hero() {	//���ΰ� �׸���

	glPushMatrix();
	if (OnRide == 0) {
		glTranslatef(HeroMoveX, -26.6, HeroMoveZ);
		glRotatef(HeroRot, 0.0, 1.0, 0.0);
	}

	glPushMatrix();	//�Ӹ�
		glTranslatef(0.0, 1.1, 0.0);
		glColor3f((float)255 / 255.0, (float)222 / 255.0, (float)191 / 255.0);
		glutSolidCube(1.0);
		glPushMatrix();	//���� ��
			glTranslatef(0.2, 0.1, -0.4);
			glColor3f(0.0, 0.0, 0.0);
			glutSolidCube(0.25);
			glTranslatef(0.0, 0.0, -0.1);
			glColor3f(1.0, 1.0, 1.0);
			glutSolidCube(0.15);
		glPopMatrix();
		glPushMatrix();	//������ ��
			glTranslatef(-0.2, 0.1, -0.4);
			glColor3f(0.0, 0.0, 0.0);
			glutSolidCube(0.25);
			glTranslatef(0.0, 0.0, -0.1);
			glColor3f(1.0, 1.0, 1.0);
			glutSolidCube(0.15);
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();//����
		glScalef(1.4, 2.0, 1.0);
		glColor3f((float)255 / 255.0, (float)180 / 255.0, (float)190 / 255.0);
		glutSolidCube(0.6);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(0.6);
	glPopMatrix();

	glPushMatrix();	//����
		glTranslatef(0.6, 0.36, 0.0);
		if (OnRide == 0) {
			glRotatef(HeroMotion, 1.0, 0.0, 0.0);
		}
		if ((OnRide == 3 && DropToewrUpdown == 1) || (OnRide == 4 && RolSpeed > 1)) {
			glRotatef(180.0, 1.0, 0.0, 0.0);
		}
		glTranslatef(0.0, -0.18, 0.0);
		glScalef(1.0, 2.1, 1.0);
		glColor3f((float)255 / 255.0, (float)180 / 255.0, (float)190 / 255.0);
		glutSolidCube(0.4);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(0.4);
	glPopMatrix();

	glPushMatrix();	//������
		glTranslatef(-0.6, 0.36, 0.0);
		if (OnRide == 0) {
			glRotatef(-HeroMotion, 1.0, 0.0, 0.0);
		}
		if ((OnRide == 3 && DropToewrUpdown == 1) || (OnRide == 4 && RolSpeed > 1)) {
			glRotatef(180.0, 1.0, 0.0, 0.0);
		}
		glTranslatef(0.0, -0.18, 0.0);
		glScalef(1.0, 2.1, 1.0);
		glColor3f((float)255 / 255.0, (float)180 / 255.0, (float)190 / 255.0);
		glutSolidCube(0.4);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(0.4);
	glPopMatrix();

	if (OnRide == 0 || OnRide == 3) {		//���̱ⱸ ž������ �ƴҰ��
		glPushMatrix();	//���ʴٸ�
			if (OnRide == 0) {
				glRotatef(-(HeroMotion / 2.0), 1.0, 0.0, 0.0);
				}
			if (OnRide == 3) {
				glRotatef(45.0, 1.0, 0.0, 0.0);
			}
			glTranslatef(0.2, -1.0, 0.0);
			glScalef(1.0, 2.1, 1.0);
			glColor3f((float)63 / 255.0, (float)77 / 255.0, (float)181 / 255.0);
			glutSolidCube(0.4);
			glColor3f(0.0, 0.0, 0.0);
			glutWireCube(0.4);
		glPopMatrix();
		glPushMatrix();	//�����ʴٸ�
			if (OnRide == 0) {
				glRotatef(HeroMotion / 2.0, 1.0, 0.0, 0.0);
			}
			if (OnRide == 3) {
				glRotatef(45.0, 1.0, 0.0, 0.0);
			}
			glTranslatef(-0.2, -1.0, 0.0);
			glScalef(1.0, 2.1, 1.0);
			glColor3f((float)63 / 255.0, (float)77 / 255.0, (float)181 / 255.0);
			glutSolidCube(0.4);
			glColor3f(0.0, 0.0, 0.0);
			glutWireCube(0.4);
		glPopMatrix();
	}
		
	glPopMatrix();
}

void Wheel() {		//������
	int color[3] = { 200,100,100 };
	GLUquadricObj *pQuad;
	pQuad = gluNewQuadric();

	gluQuadricDrawStyle(pQuad, GLU_FILL);	//������ ���
	glColor3f((float)127 / 255.0, (float)127 / 255.0, (float)127 / 255.0);
	glPushMatrix();
	glTranslatef(0.0, -13.2, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.6, 0.0);
	gluCylinder(pQuad, 0.4, 0.4, 15.0, 100, 100);
	glPopMatrix();

	gluQuadricDrawStyle(pQuad, GLU_LINE);	//������ ����
	glColor3f((float)50 / 255.0, (float)35 / 255.0, (float)20 / 255.0);	//�������
	glPushMatrix();
	glTranslatef(0.0, -13.3, -2.5);
	glRotatef(WheelRot, 0.0, 0.0, 1.0);
	glLineWidth(3.0f);	//������ ����(������)
	gluCylinder(pQuad, 6.0, 6.0, 3.0, 10, 1);
	gluDisk(pQuad, 0.0, 12.0, 10, 2);
	glTranslatef(0.0, 0.0, 3.0);
	gluDisk(pQuad, 0.0, 12.0, 10, 2);
	glLineWidth(1.0f);	//������ ���󺹱�
	for (float angle = 0.0f; angle < (2.0f*GL_PI); angle = angle + (2.0f*GL_PI / 10.0f)) {	//������ �˸���
		float WheelX = 12.0f*sin(angle);
		float WheelY = 12.0f*cos(angle);
		glPushMatrix();
		glTranslatef(WheelX, WheelY, -1.5);
		glRotatef(-WheelRot, 0.0, 0.0, 1.0);
		glColor3f((float)color[0] / 255.0, (float)color[1] / 255.0, (float)color[2] / 255.0);	//�˸��� ����-���򿩷���
		glutSolidCube(3.0);
		glColor3f((float)0 / 255.0, (float)0 / 255.0, (float)0 / 255.0);	//�׵θ�
		glutWireCube(3.0);
		glPopMatrix();
		color[0] = color[0] - 10;
		color[1] = color[1] + 10;
		color[2] = color[2] + 10;
	}
	glPopMatrix();

	gluQuadricDrawStyle(pQuad, GLU_FILL);	//������ ���
	glColor3f((float)127 / 255.0, (float)127 / 255.0, (float)127 / 255.0);
	glPushMatrix();
	glTranslatef(0.0, -13.2, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -2.4, 0.0);
	gluCylinder(pQuad, 0.4, 0.4, 15.0, 100, 100);
	glPopMatrix();
}

void Viking() {	//����ŷ
	GLUquadricObj *pQuad;
	pQuad = gluNewQuadric();

	glPushMatrix();
	glTranslatef(15.0, -23.0, 15.0);	//����ŷ ��ġ���� - x��,z�� �ٲ� ��

	glPushMatrix();//���
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 1.5, -0.3);
	glColor3f(0.5, 0.5, 0.5);
	gluCylinder(pQuad, 0.3, 0.3, 5.5, 100, 100);
	glPopMatrix();

	glPushMatrix();	//����+��
	glRotatef(VikingRot, 0.0, 0.0, 1.0);	//�����̼� ���θ��� ������
	glTranslatef(0.0, -3.0, 0.0);

	if (OnRide == 1) {
		glPushMatrix();	//���ΰ� �¿��
		glTranslatef(0.0, 0.7, 0.0);
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		Hero();
		glPopMatrix();
	}
	
	glPushMatrix();	//����
	glColor3f((float)235 / 255.0, (float)50 / 255.0, (float)50 / 255.0);	//�������-�����ʿ�
	glBegin(GL_POLYGON);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-2.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-2.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, -1.0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(-1.0, 1.0, 1.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glVertex3f(1.0, 1.0, 1.0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(-1.0, 1.0, -1.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glVertex3f(1.0, 1.0, -1.0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(2.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(1.0, -1.0, 0.0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(1.0, -1.0, 0.0);
	glVertex3f(1.0, 1.0, -1.0);
	glVertex3f(2.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, 1.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();	//��ü�� �� ����
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(-0.1, 0.0, -3.1);
	glRotatef(45.0, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.5, 0.5);
	gluCylinder(pQuad, 0.2, 0.2, 2.8, 100, 100);
	glPopMatrix();

	glPushMatrix();	//��ü�� �� ����
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.1, 0.0, -3.1);
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.5, 0.5);
	gluCylinder(pQuad, 0.2, 0.2, 2.8, 100, 100);
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();//���
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -1.5, -0.3);
	glColor3f(0.5, 0.5, 0.5);
	gluCylinder(pQuad, 0.3, 0.3, 5.5, 100, 100);
	glPopMatrix();

	//��ջ��� ��
	glTranslatef(0.0, 0.0, -1.7);
	gluCylinder(pQuad, 0.3, 0.3, 3.5, 100, 100);

	glPopMatrix();
}

void RolBoxHead() {
	glPushMatrix();
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 0.7, 0.0);

		glPushMatrix();
			glTranslatef(-0.8, 0.0, 0.0);
			glScalef(0.5, 0.5, 0.5);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidIcosahedron();
			glColor3f(0.0, 0.0, 0.0);
			glutWireIcosahedron();
		glPopMatrix();

		glPushMatrix();
			glScalef(1.5, 1.0, 1.0);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidCube(1.0);
			glColor3f(0.0, 0.0, 0.0);
			glutWireCube(1.0);
		glPopMatrix();

		//ĳ���� �¿��
		if (OnRide == 4) {
			glPushMatrix();
			glTranslatef(0.0, 0.7, 0.0);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			Hero();
			glPopMatrix();
		}
	glPopMatrix();
}

void RolBoxTail() {
	glPushMatrix();
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.7, 0.0);
	glPushMatrix();
	glScalef(1.5, 1.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidCube(1);
	glColor3f(0.0, 0.0, 0.0);
	glutWireCube(1);
	glPopMatrix();
	glPopMatrix();
}

void RolRail() {
	int position;
	GLUquadricObj *pQuad;
	pQuad = gluNewQuadric();
	glPushMatrix();
	glTranslatef(33.0, -28.0, -27.0);
	for (position = 0; position < 487; position++) {
		glTranslatef(0.0, 0.0, -1.0);
		glColor3f((float)140 / 255.0, (float)105 / 255.0, (float)60 / 255.0);	//�������
		if (position >= 0 && position<9) {	//Ŀ��
			glRotatef(10.0, 0.0, 1.0, 0.0);
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.2, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.2, 50, 50);
			glPopMatrix();
		}

		if (position == 37) {	//������ ���� ��
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 29.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 29.0, 50, 50);
			glPopMatrix();
			glRotatef(15.0, 1.0, 0.0, 0.0);
		}
		if (position >= 38 && position<42) {	//������ 1 60��(+60��)
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 41) {
				glRotatef(15.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 60) {	//���
			glPushMatrix();
			glRotatef(30, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 19.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 19.0, 50, 50);
			glPopMatrix();
		}
		if (position == 77) {	//������
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 37, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 37, 50, 50);
			glPopMatrix();
			glRotatef(-15.0, 1.0, 0.0, 0.0);
		}
		if (position >= 78 && position<82) {	//������ 1 60��(+60��)
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 81) {
				glRotatef(-15.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 82) {	//������ �� Ŀ�� ��
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.8, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.8, 50, 50);
			glPopMatrix();

			glPushMatrix();	//���
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 35.2, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 35.2, 50, 50);
			glPopMatrix();
		}
		if (position >= 83 && position<92) {	//������ �� Ŀ��
			glRotatef(10.0, 0.0, 1.0, 0.0);
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.2, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.2, 50, 50);
			glPopMatrix();
			if (position == 91) {
				glRotatef(-15.0, 1.0, 0.0, 0.0);
			}
		}
		if (position >= 92 && position<96) {	//������ 1 -60��(-60��)
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 95) {
				glRotatef(-15.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 99) {	//���
			glPushMatrix();
			glRotatef(150, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 29.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 29.4, 50, 50);
			glPopMatrix();
		}
		if (position == 102) {
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 8.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 8.0, 50, 50);
			glPopMatrix();
			glRotatef(10.0, 1.0, 0.0, 0.0);
		}
		if (position >= 103 && position<112) {	//30��(+90��)
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 111) {
				glRotatef(10.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 114) {
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 3.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 3.0, 50, 50);
			glPopMatrix();
			glRotatef(-10.0, 1.0, 0.0, 0.0);
		}
		if (position >= 115 && position<121) {	//-30��(-60��)
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 120) {
				glRotatef(-10.0, 1.0, 0.0, 0.0);
			}
			else {
				glRotatef(15.0, 0.0, 0.0, 1.0);
			}
		}
		if (position == 116) {	//���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 27.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 27.4, 50, 50);
			glPopMatrix();
		}
		if (position >= 121 && position<145) {	// �������౸��
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 144) {
				glRotatef(15.0, 0.0, 0.0, 1.0);
			}
			else {
				glRotatef(10.0, 1.0, 0.0, 0.0);
			}
		}
		if (position >= 145 && position<154) {	//60��(+90��)
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 153) {
				glRotatef(10.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 146) {	//���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 13.8, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 13.8, 50, 50);
			glPopMatrix();
		}
		if (position == 154) {
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.0, 50, 50);
			glPopMatrix();
			glRotatef(-15.0, 1.0, 0.0, 0.0);
		}
		if (position >= 155 && position<159) {	//�������ڳ� �� 0��(-60��)
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 158) {
				glRotatef(-15.0, 1.0, 0.0, 0.0);
			}
		}
		if (position >= 159 && position<168) {	//ù ������ �� Ŀ��
			glRotatef(10.0, 0.0, 1.0, 0.0);
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.2, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.2, 50, 50);
			glPopMatrix();
			if (position == 167) {
				glRotatef(-15.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 162) {	//Ŀ�걸�� �� ���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 19.5, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 19.5, 50, 50);
			glPopMatrix();
		}
		if (position >= 168 && position<171) {	//������2 -45��(-45��)
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 164) {
				glRotatef(-15.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 169) {	//���
			glPushMatrix();
			glRotatef(135, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 18.7, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 18.7, 50, 50);
			glPopMatrix();
		}
		if (position == 183) {	//������ ����
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 13.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 13.0, 50, 50);
			glPopMatrix();
			glRotatef(10.0, 1.0, 0.0, 0.0);
		}
		if (position >= 184 && position<190) {	//�������� �� ������ ��
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 189) {
				glRotatef(10.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 190) {	//���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 4.5, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 4.5, 50, 50);
			glPopMatrix();
		}
		if (position == 195) {	//�������� ��
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 6.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 6.0, 50, 50);
			glPopMatrix();
			glRotatef(-2.0, 0.0, 1.0, 0.0);
			glRotatef(10.0, 1.0, 0.0, 0.0);
		}
		if (position >= 196 && position<231) {	//��������
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 230) {
				glRotatef(-2.0, 0.0, 1.0, 0.0);
				glRotatef(10.0, 1.0, 0.0, 0.0);
			}
			else {
				glRotatef(3.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 234) {	//���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 4.5, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 4.5, 50, 50);
			glPopMatrix();
		}
		if (position == 241) {	//���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 4.5, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 4.5, 50, 50);
			glPopMatrix();
		}
		if (position == 242) {	//�������� �� ����
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 12.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 12.0, 50, 50);
			glPopMatrix();
			glRotatef(10.0, 1.0, 0.0, 0.0);
		}
		if (position >= 243 && position<248) {	//�������� �� ������ 60��(+60��)
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 247) {
				glRotatef(10.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 258) {	//���
			glPushMatrix();
			glRotatef(40, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 15.5, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 15.5, 50, 50);
			glPopMatrix();
		}
		if (position == 267) {
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 20.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 20.0, 50, 50);
			glPopMatrix();
			glRotatef(-10.0, 1.0, 0.0, 0.0);
		}
		if (position >= 268 && position<273) {
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 272) {
				glRotatef(-10.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 274) {
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 2.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 2.0, 50, 50);
			glPopMatrix();
		}
		if (position == 279) {	//���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 23.8, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 23.8, 50, 50);
			glPopMatrix();
		}
		if (position >= 275 && position<284) {	//������ Ŀ��
			glRotatef(10.0, 0.0, 1.0, 0.0);
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.2, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.2, 50, 50);
			glPopMatrix();
			if (position == 283) {
				glRotatef(-15.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 284) {	//���
			glPushMatrix();
			glRotatef(105, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 23.6, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 23.6, 50, 50);
			glPopMatrix();
		}
		if (position == 291) {
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 8.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 8.0, 50, 50);
			glPopMatrix();
			glRotatef(15.0, 1.0, 0.0, 0.0);
		}
		if (position >= 292 && position<436) {	//����̵�
			glTranslatef(0.0, -0.15, 0.0);
			glRotatef(10.0, 0.0, 1.0, 0.0);
			glPushMatrix();
			glRotatef(15.0, 0.0, 0.0, 1.0);
			glRotatef(-16.5, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.2, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.2, 50, 50);
			glPopMatrix();
		}
		if (position == 405) {	//���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 4.6, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 4.6, 50, 50);
			glPopMatrix();
		}
		if (position == 410) {	//���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 3.9, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 3.9, 50, 50);
			glPopMatrix();
		}
		if (position == 415) {	//���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 3.1, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 3.1, 50, 50);
			glPopMatrix();
		}
		if (position == 420) {	//���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 2.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 2.4, 50, 50);
			glPopMatrix();
		}
		if (position == 425) {	//���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.6, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.6, 50, 50);
			glPopMatrix();
		}
		if (position == 430) {	//���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 0.9, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 0.9, 50, 50);
			glPopMatrix();
		}
		if (position == 432) {	//���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 0.7, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 0.7, 50, 50);
			glPopMatrix();
		}
		if (position == 439) {	//����̵� �� ������ ��
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 4.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 4.0, 50, 50);
			glPopMatrix();
			glRotatef(10.0, 1.0, 0.0, 0.0);
		}
		if (position >= 440 && position<445) {	//����̵� �� ������ 60��(+60��)
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 444) {
				glRotatef(10.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 449) {
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 5.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 5.0, 50, 50);
			glPopMatrix();
			glRotatef(-10.0, 1.0, 0.0, 0.0);
		}
		if (position == 454) {	//���
			glPushMatrix();
			glRotatef(90, 1.0, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 8.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 8.0, 50, 50);
			glPopMatrix();
		}
		if (position >= 450 && position<457) {	//������ ������ 20��(-80��)
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position != 456) {
				glRotatef(-10.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 477) {
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 22.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 22.0, 50, 50);
			glPopMatrix();
			glRotatef(10.0, 1.0, 0.0, 0.0);
		}
		if (position >= 478 && position<480) {	//������ �� ���� -20��
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 1.4, 50, 50);
			glPopMatrix();
			if (position == 479) {
				glRotatef(2.0, 0.0, 1.0, 0.0);
			}
			else {
				glRotatef(10.0, 1.0, 0.0, 0.0);
			}
		}
		if (position == 486) {
			glPushMatrix();
			gluCylinder(pQuad, 0.1, 0.1, 7.0, 50, 50);
			glTranslatef(0.5, 0.0, 0.0);
			gluCylinder(pQuad, 0.1, 0.1, 7.0, 50, 50);
			glPopMatrix();
		}
		//����+��� ��
		if (position == RolPosition[0]) {
			RolBoxHead();
		}
		if (position == RolPosition[1]) {
			RolBoxTail();
		}
		if (position == RolPosition[2]) {
			RolBoxTail();
		}

	}
	glPushMatrix();		//������ ����
	glTranslatef(-55.5, 11.0, 58.3);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glRotatef(-60.0, 1.0, 0.0, 0.0);
	glScalef(1.0, 1.0, 6.0);
	glColor3f((float)171 / 255.0, (float)121 / 255.0, (float)98 / 255.0);	//�������
	glutSolidTorus(1.0, 3.0, 25, 25);
	glColor3f(0.0, 0.0, 0.0);
	glutWireTorus(1.0, 3.0, 25, 25);
	glPopMatrix();

	glPopMatrix();
}

void RolSpeedControl() {	//�ѷ��ڽ��� �ӵ� ���� - 1,2,3�ܰ�
	if (RolPosition[0] >= 92 && RolPosition[0] < 103) {
		RolSpeed = 3;
	}
	else if (RolPosition[0] >= 103 && RolPosition[0] < 118) {
		RolSpeed = 2;
	}
	else if (RolPosition[0] >= 118 && RolPosition[0] < 145) {
		RolSpeed = 3;
	}
	else if (RolPosition[0] >= 145 && RolPosition[0] < 168) {
		RolSpeed = 2;
	}
	else if (RolPosition[0] >= 168 && RolPosition[0] < 184) {
		RolSpeed = 3;
	}
	else if (RolPosition[0] >= 184 && RolPosition[0] < 250) {
		RolSpeed = 2;
	}
	else if (RolPosition[0] >= 285 && RolPosition[0] < 440) {
		RolSpeed = 2;
	}
	else if (RolPosition[0] >= 450 && RolPosition[0] < 478) {
		RolSpeed = 2;
	}
	else {
		RolSpeed = 1;
	}
}

void DropTower() {
	GLUquadricObj *pQuad;
	pQuad = gluNewQuadric();

	glPushMatrix();
	glTranslatef(-21.0, 0.0, -21.0);

	glPushMatrix();	//���� ���
	glRotatef(90, 1.0, 0.0, 0.0);
	glColor3f((float)127 / 255.0, (float)127 / 255.0, (float)127 / 255.0);
	glColor3f((float)200 / 255.0, (float)25 / 255.0, (float)25 / 255.0);
	gluCylinder(pQuad, 1.5, 1.5, 30, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, DropTowerMove, 0.0);
	glPushMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	glColor3f((float)127 / 255.0, (float)127 / 255.0, (float)127 / 255.0);
	glColor3f((float)10 / 255.0, (float)10 / 255.0, (float)10 / 255.0);
	glutSolidTorus(1.0, 2.8, 15, 15);
	gluDisk(pQuad, 1.7, 6.0, 8, 1);
	glPopMatrix();
	if (OnRide == 3) {	//���ΰ� �¿��
		glPushMatrix();
		glTranslatef(4.6, 0.6, 0.0);
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		Hero();
		glPopMatrix();
	}
	glPopMatrix();

	glPopMatrix();
}

void MakeTicketBox() {
	glPushMatrix();
	
	glTranslatef(0.0, -26.6, 0.0);

	glPushMatrix();	//��ǥ�� ����
	glScalef(1.0, 1.5, 1.0);
	glColor3f((float)127 / 255.0, (float)127 / 255.0, (float)127 / 255.0);	//��ǥ�� ��
	glutSolidCube(2.0);
	glTranslatef(0.0, -0.5, -0.55);
	glColor3f(0.0, 0.0, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 1.5, 0.0);
	glRotatef(45.0, 0.0, 1.0, 0.0);
	glScalef(1.5, 2.0, 1.5);
	glColor3f((float)200 / 255.0, (float)55 / 255.0, (float)55 / 255.0);	//��ǥ�� ���� ��
	glutSolidOctahedron();
	glPopMatrix();

	glPopMatrix();
}

void TicketOffice() {
	glPushMatrix();	//����ŷ ��ǥ��
	glTranslatef(15.5, 0.0, 12.0);
	MakeTicketBox();
	glPopMatrix();

	glPushMatrix();	//������ ��ǥ��
	glTranslatef(0.0, 0.0, -4.0);
	MakeTicketBox();
	glPopMatrix();

	glPushMatrix();	//���̵�� ��ǥ��
	glTranslatef(-13.5, 0.0, -21.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	MakeTicketBox();
	glPopMatrix();

	glPushMatrix();	//�ѷ��ڽ��� ��ǥ��
	glTranslatef(31.5, 0.0, -26.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	MakeTicketBox();
	glPopMatrix();
}

void MakeGround() {
	glPushMatrix();
	glTranslatef(0.0, -30.0, 0.0);
	glScalef(1.0, 0.05, 1.0);	// 70 * 0.05 = 3.5
	glColor3f((float)94 / 255.0, (float)161 / 255.0, (float)82 / 255.0);	//�ڽ�
	glutSolidCube(70);
	glColor3f((float)0 / 255.0, (float)0 / 255.0, (float)0 / 255.0);	//�׵θ�
	glutWireCube(70);
	glPopMatrix();
}

void ParkDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLUquadricObj *pQuad;
	pQuad = gluNewQuadric();

	////////////////////////////////////////�ǵ帮�� ����/////////////////////////////////////////////////////////
	glMatrixMode(GL_PROJECTION);	//������ȭ�� ��Ȱ�ϰ� �ϱ����� �������
	glLoadIdentity();
	if (View == 1) {	// 1 = 1��Ī / 0 = 3��Ī
		gluPerspective(60.0f, fAspect, 1.0, 100.0);
	}
	else {
		glOrtho(0.0*WidthFactor, 100.0*WidthFactor, 0.0*HeightFactor, 100.0*HeightFactor, -300.0, 300.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	glBegin(GL_POLYGON);	//�㳷 ���
	glColor3f((float)SkyColor[0] / 255.0, (float)SkyColor[1] / 255.0, (float)SkyColor[2] / 255.0);	//����(�ϴû�)
	glVertex3f(150.0, 150.0, -80.0);
	glVertex3f(-150.0, 150.0, -80.0);
	glColor3f(1.0, 1.0, 1.0);	//�Ʒ����� ���
	glVertex3f(-150.0, -150.0, -80.0);
	glVertex3f(150.0, -150.0, -80.0);
	glEnd();

	glPushMatrix();
	if (View == 1) {	//1��Ī
		gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
		glRotatef(ViewUpAngle, 1.0, 0.0, 0.0);
		glRotatef(ViewAngle, 0.0, 1.0, 0.0);
		glTranslatef(0.0, 25.0, 0.0);
		glTranslatef(ViewMoveX, 0.0, ViewMoveZ);
		ViewMoveRange();
	}
	else {
		glTranslatef(50.0, 50.0, 0.0);
		if (xRot + MouseRotY < 0.0) {	//���������ϱ�
			glRotatef(0.0, 1.0, 0.0, 0.0);
		}
		else if (xRot + MouseRotY > 90.0) {
			glRotatef(90.0, 1.0, 0.0, 0.0);
		}
		else {
			glRotatef(xRot + MouseRotY, 1.0, 0.0, 0.0);
		}
		glRotatef(yRot-MouseRotX, 0.0, 1.0, 0.0);
	}

	Cloud();	//������

	MakeGround();	//�ٴ�

	Viking();		//����ŷ

	Wheel();		//������

	if (FireworkTime == 1) {
		PlayFireWork();	//�Ҳɳ���
	}

	TicketOffice();	//��ǥ��'��'

	DropTower();	//���̵��

	RolRail();		//�ѷ��ڽ���

	if (OnRide == 0&&View==0) {	//���̱ⱸ ��������+3��Ī �����϶�
		Hero();	//���ΰ�
	}

	glPopMatrix();

	gluDeleteQuadric(pQuad);

	glutSwapBuffers();
}

void RotWheel(int value) {
	WheelRot = WheelRot + 0.5;
	if (WheelRot == 360) {
		WheelRot = 0;
	}
	glutTimerFunc(40, RotWheel, 1);
	glutPostRedisplay();
}

void RotViking(int value) {
	if (VikingRot > 36.0 || VikingRot < -36.0) {
		VikingAngle = -VikingAngle;
	}
	VikingRot = VikingRot + VikingAngle;
	glutTimerFunc(25, RotViking, 1);
	glutPostRedisplay();
}

void MoveRol(int value) {
	RolSpeedControl();
	for (int i = 0; i < 3; i++) {
		RolPosition[i] = RolPosition[i] + RolSpeed;
		if (RolPosition[i] > 486) {
			RolPosition[i] = 0;
		}
	}
	glutTimerFunc(40, MoveRol, 1);
	glutPostRedisplay();
}

void MoveDropToewr(int value) {
	if (DropTowerMove >= -0.6) {
		if (DropTowerDelay >= 3) {
			DropToewrUpdown = 1;
			DropTowerDelay = 0;
		}
		else {
			DropTowerDelay++;
		}
	}
	if (DropTowerMove <= -27.0) {
		if (DropTowerDelay >= 3) {
			DropToewrUpdown = 0;
			DropTowerDelay = 0;
		}
		else {
			DropTowerDelay++;
		}
	}

	if (DropToewrUpdown == 0 && DropTowerDelay == 0) {
		DropTowerMove = DropTowerMove + 0.1;
	}
	if (DropToewrUpdown == 1 && DropTowerDelay == 0) {
		DropTowerMove = DropTowerMove - 1.0;
	}

	glutTimerFunc(40, MoveDropToewr, 1);
	glutPostRedisplay();
}

void RotArmLeg(int value) {
	if (HeroMoving == 1) {	//���ΰ� �ȴٸ� ȸ��
		HeroMotion = HeroMotion + HeroMotionAdd;
		if (HeroMotion > 90 || HeroMotion < -90) {
			HeroMotionAdd = -HeroMotionAdd;
		}
	}

	HeroMoveCheck++;
	if (HeroMoveCheck == 3) {	//�ȿ����̸� �ȴٸ� �Ʒ���
		HeroMoving = 0;
		HeroMotion = 0.0;
	}

	glutTimerFunc(40, RotArmLeg, 1);
	glutPostRedisplay();
}

void FireworkPlay(int value) {
	if (FireworkTime == 1) {
		FireworkProgress++;
		FireworkTail = FireworkTail + 2;
		if (FireworkTail == 50) {
			FireworkTail = 0;
		}
		if (FireworkProgress == 50) {
			FireworkProgress = 0;
			FireworkTime = 0;
		}
	}
	glutTimerFunc(10, FireworkPlay, 1);
	glutPostRedisplay();
}

void WorldTimer(int value) {	//�㳷 ���ư��� ����� Ÿ�̸�
	WorldTime++;
	if (WorldTime == 480) {
		WorldTime = 0;
	}
	if (WorldTime >= 0 && WorldTime < 120) {
		if (SkyColor[0] >= 35) {
			SkyColor[0]--;
		}
		if (SkyColor[1] >= 50) {
			SkyColor[1]--;
		}
		if (SkyColor[2] >= 100) {
			SkyColor[2]--;
		}
	}
	else if (WorldTime >= 120 && WorldTime < 240) {
		for (int i = 0; i < 3; i++) {
			if (SkyColor[i] > 0) {
				SkyColor[i]--;
			}
		}
		if (WorldTime >= 180 && FireworkTime == 0) {	//���̵Ǹ� �ڵ����� �Ҳɳ���
			FireworkTime = 1;
			for (int i = 0; i < 5; i++) {
				FireWorkPosition[i][0] = (float)((rand() % 62) - 31);
				FireWorkPosition[i][1] = (float)((rand() % 62) - 31);
				FireWorkPosition[i][2] = (float)(((rand() % 5) + 5.0) / 10.0);
				FireWorkColor[i][0] = (float)(rand() % 256);
				FireWorkColor[i][1] = (float)(rand() % 256);
				FireWorkColor[i][2] = (float)(rand() % 256);
			}
		}
	}
	else if (WorldTime >= 240 && WorldTime < 360) {
		if (SkyColor[0] <= 35) {
			SkyColor[0]++;
		}
		if (SkyColor[1] <= 50) {
			SkyColor[1]++;
		}
		if (SkyColor[2] <= 100) {
			SkyColor[2]++;
		}
		if (WorldTime <= 300 && FireworkTime == 0) {	//���̵Ǹ� �ڵ����� �Ҳɳ���
			FireworkTime = 1;
			for (int i = 0; i < 5; i++) {
				FireWorkPosition[i][0] = (float)((rand() % 62) - 31);
				FireWorkPosition[i][1] = (float)((rand() % 62) - 31);
				FireWorkPosition[i][2] = (float)(((rand() % 5) + 5.0) / 10.0);
				FireWorkColor[i][0] = (float)(rand() % 256);
				FireWorkColor[i][1] = (float)(rand() % 256);
				FireWorkColor[i][2] = (float)(rand() % 256);
			}
		}
	}
	else if (WorldTime >= 360 && WorldTime < 480) {
		if (SkyColor[0] <= 165) {
			SkyColor[0]++;
		}
		if (SkyColor[1] <= 205) {
			SkyColor[1]++;
		}
		if (SkyColor[2] <= 255) {
			SkyColor[2]++;
		}
	}
	else {
		SkyColor[0] = 165;
		SkyColor[1] = 204;
		SkyColor[2] = 255;
	}

	for (int c = 0; c < 10; c++) {
		CloudETC[c][0] = CloudETC[c][0] + 0.2;
		if (CloudETC[c][0] >= 360.0) {
			CloudETC[c][0] = CloudETC[c][0] - 360.0;
		}
	}

	glutTimerFunc(10, WorldTimer, 1);
	glutPostRedisplay();
}

void ChangeSize(int NewWidth, int NewHeight) {
	fAspect = (GLfloat)NewWidth / (GLfloat)NewHeight;
	glViewport(0, 0, NewWidth, NewHeight);
	WidthFactor = (GLfloat)NewWidth / (GLfloat)800;
	HeightFactor = (GLfloat)NewHeight / (GLfloat)800;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (View == 1) {
		gluPerspective(60.0f, fAspect, 1.0, 100.0);
	}
	else {
		glOrtho(0.0*WidthFactor, 100.0*WidthFactor, 0.0*HeightFactor, 100.0*HeightFactor, -300.0, 300.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SpecialInsert(int Key, int x, int y) {	//1��Ī�϶� ����
	if (Key == GLUT_KEY_LEFT) {
		ViewAngle = ViewAngle - 6.0;
	}
	if (Key == GLUT_KEY_RIGHT) {
		ViewAngle = ViewAngle + 6.0;
	}

	if (Key == GLUT_KEY_UP && ViewUpAngle >= -90.0) {	//�ø������°� �ִ��ּҰ��� ����
		ViewUpAngle = ViewUpAngle - 6.0;
	}
	if (Key == GLUT_KEY_DOWN && ViewUpAngle <= 90.0) {
		ViewUpAngle = ViewUpAngle + 6.0;
	}
	glutPostRedisplay();
}

void KeyboardInsert(unsigned char key, int x, int y) {	//ĳ���� �̵�
	switch (key)
	{
	case 'a':
		HeroRot = 180.0;
		if (HeroMoveRange(1) == 1) {
			HeroMoving = 1;
			HeroMoveCheck = 0;
			HeroMoveZ = HeroMoveZ + 0.5;
		}
		CalculateViewXM();
		break;
	case 'd':
		HeroRot = 0.0;
		if (HeroMoveRange(2) == 1) {
			HeroMoving = 1;
			HeroMoveCheck = 0;
			HeroMoveZ = HeroMoveZ - 0.5;
		}
		CalculateViewXA();
		break;
	case 'w':
		HeroRot = 90.0;
		if (HeroMoveRange(4) == 1) {
			HeroMoving = 1;
			HeroMoveCheck = 0;
			HeroMoveX = HeroMoveX - 0.5;
		}
		CalculateViewZA();
		break;
	case 's':
		HeroRot = -90.0;
		if (HeroMoveRange(3) == 1) {
			HeroMoving = 1;
			HeroMoveCheck = 0;
			HeroMoveX = HeroMoveX + 0.5;
		}
		CalculateViewZM();
		break;
	case 'p':
		if (OnRide == 0 && View == 0) {	//���̱ⱸ ž��
			ChangeRide();
		}
		else {
			OnRide = 0;
		}
		break;
	case 'f':	//�Ҳɳ��� ��������
		FireworkTime = 1;
		for (int i = 0; i < 5; i++) {
			FireWorkPosition[i][0] = (float)((rand() % 62) - 31);
			FireWorkPosition[i][1] = (float)((rand() % 62) - 31);
			FireWorkPosition[i][2] = (float)(((rand() % 7) + 5.0) / 10.0);
			FireWorkColor[i][0] = (float)(rand() % 256);
			FireWorkColor[i][1] = (float)(rand() % 256);
			FireWorkColor[i][2] = (float)(rand() % 256);
		}
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) {
	if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN && View != 1) {
		ClickX = (float)X;
		ClickY = (float)Y;
		mousecheck = 1;
	}
	else {	//Ŭ���� ���� �̵��� ��ŭ ����
		xRot = xRot + MouseRotY;
		yRot = yRot - MouseRotX;
		MouseRotX = 0.0;
		MouseRotY = 0.0;
		if (xRot >= 360) {
			xRot = xRot - 360;
		}
		if (yRot >= 360) {
			yRot = yRot - 360;
		}
		mousecheck = 0;
	}
	glutPostRedisplay();
}

void MyMouseMove(GLint X, GLint Y) {
	if (mousecheck == 1) {
		MouseRotX = (ClickX - X)/2.0;
		MouseRotY = (Y-ClickY) / 2.0;
		
	}
	glutPostRedisplay();
}

void MyMainMenu(int in) {
	glutPostRedisplay();
}

void TimeSubMenu(int in) {
	if (in == 1) {	//��
		WorldTime = 0;
		SkyColor[0] = 165;
		SkyColor[1] = 205;
		SkyColor[2] = 255;
	}
	if (in == 2) {	//��
		WorldTime = 240;
		SkyColor[0] = 35;
		SkyColor[1] = 50;
		SkyColor[2] = 100;
	}

	glutPostRedisplay();
}

void ViewSubMenu(int in) {
	if (in == 1){	//1��Ī
		View = 1;
		ResetHero();
		ResetView();
	}
	if (in == 2) {	//3��Ī
		View = 0;
		ResetHero();
		ResetView();
	}
	glutPostRedisplay();
}

void MenuFunc() {
	glutCreateMenu(TimeSubMenu);
	glutAddMenuEntry("Day", 1);
	glutAddMenuEntry("Night", 2);

	glutCreateMenu(ViewSubMenu);
	glutAddMenuEntry("Hero(1��Ī)", 1);
	glutAddMenuEntry("God(3��Ī)", 2);

	glutCreateMenu(MyMainMenu);
	glutAddSubMenu("�ð�", 1);
	glutAddSubMenu("����", 2);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void SetDisplay() {
	//����
	cout << endl;
	cout << "  **************20153308 �۹α� ���̰���**************" << endl;
	cout << "  ****************************************************" << endl;
	cout << "  **                                                **" << endl;
	cout << "  **  �޴� - ���콺 ������ Ŭ��                     **" << endl;
	cout << "  **    (�㳷 ����, ���� ���� ����)                 **" << endl;
	cout << "  **                                                **" << endl;
	cout << "  **  �Ҳɳ��� - f                                  **" << endl;
	cout << "  **                                                **" << endl;
	cout << "  **  3��Ī ����                                    **" << endl;
	cout << "  **    �̵� - w,a,s,d                              **" << endl;
	cout << "  **    ȸ�� - �巡��                               **" << endl;
	cout << "  **    ���̱ⱸ ž�� - p(���̱ⱸ �� ��ǥ�ҿ���)   **" << endl;
	cout << "  **                                                **" << endl;
	cout << "  **  1��Ī ����                                    **" << endl;
	cout << "  **    �̵� - w,a,s,d                              **" << endl;
	cout << "  **    �������� - ����Ű                           **" << endl;
	cout << "  **                                                **" << endl;
	cout << "  ****************************************************" << endl;
	cout << "  ****************************************************" << endl;

	glClearColor((float)255 / 255.0, (float)255 / 255.0, (float)255 / 255.0, 0.0f);	//���ʱ�ȭ
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	for (int i = 0; i < 10; i++) {	//���� ��ġ�� ũ�� ����
		CloudPosition[i][0] = (float)((rand() % 80) - 40);
		CloudPosition[i][1] = (float)(rand() % 15);
		CloudPosition[i][2] = (float)((rand() % 80) - 40);
		CloudETC[i][0] = (float)(rand() % 360);
		CloudETC[i][1] = (float)(((rand() % 3) + 7.0) / 10.0);
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("�α�����");

	srand(time(NULL));

	glutDisplayFunc(ParkDisplay);
	glutReshapeFunc(ChangeSize);

	glutSpecialFunc(SpecialInsert);
	glutKeyboardFunc(KeyboardInsert);
	glutMouseFunc(MyMouseClick);
	glutMotionFunc(MyMouseMove);

	glutTimerFunc(40, RotWheel, 1);
	glutTimerFunc(25, RotViking, 1);
	glutTimerFunc(40, MoveRol, 1);
	glutTimerFunc(40, MoveDropToewr, 1);
	glutTimerFunc(40, RotArmLeg, 1);
	glutTimerFunc(40, FireworkPlay, 1);
	glutTimerFunc(10, WorldTimer, 1);
	MenuFunc();

	SetDisplay();

	PlaySound(TEXT(M_Run), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	glutMainLoop();
	return 0;
}