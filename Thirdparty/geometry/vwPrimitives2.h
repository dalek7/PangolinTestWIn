#pragma once

#include "gl/gl.h"
#include "gl/glu.h"
//#include "../3d/dhVector.h"
//#include "cameraSimple.h"
 

#ifndef PI
#define PI              3.14159265358979323   /* 180 deg */
#endif

#ifndef DEG
#define DEG(a)          (180.*a/PI )
#endif 

#ifndef RAD
#define RAD(a)          (PI*a/180.)
#endif 

namespace DD {

	// http://cs.lmu.edu/~ray/notes/openglexamples/
	inline void TestTriangle()
	{
		glBegin(GL_POLYGON);
			glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
			glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
			glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
		glEnd();

	}


	inline void glCross3f(GLfloat x, GLfloat y,GLfloat z, GLfloat d)
	{
		glPushMatrix();

		glBegin(GL_LINE_STRIP);
			glVertex3f(x - d/2 , y, z);
			glVertex3f(x + d/2 , y, z);
		glEnd();

		glBegin(GL_LINE_STRIP);
			glVertex3f(x , y - d/2, z);
			glVertex3f(x , y + d/2, z);
		glEnd();


		glPopMatrix();
	}


	inline void glCircle3f(GLfloat x, GLfloat y,GLfloat z, GLfloat r)
	{
		float step = 360.0 / (2.0*PI*r) / 16;
		glPushMatrix();
		glTranslatef(x, y, z);
		glBegin(GL_LINE_STRIP);
		//glVertex2f(0, 0);
		for(float i=0; i<360; i+=step)
		{
			glVertex3f((cos(RAD(i))*r), (sin(RAD(i))*r), 0);
		}
		glEnd();
		glPopMatrix();
	}



	inline void glCircle(GLfloat x, GLfloat y, GLfloat r)
	{
		float step = 360.0 / (2.0*PI*r) / 10;
		glPushMatrix();
		glTranslatef(x, y, 0);
		glBegin(GL_LINE_STRIP);
		//glVertex2f(0, 0);
		for(float i=0; i<360; i+=step)
		{
			glVertex2f((cos(RAD(i))*r), (sin(RAD(i))*r));
		}
		glEnd();
		glPopMatrix();
	}

	//http://ozark.hendrix.edu/~burch/cs/490/sched/feb8/
	//drawSphere(1.0, 10, 10);
	inline void drawSphere(double r, int lats, int longs) 
	{

		int i, j;
		for(i = 0; i <= lats; i++) {
			   double lat0 = PI * (-0.5 + (double) (i - 1) / lats);
			   double z0  = sin(lat0);
			   double zr0 =  cos(lat0);
    
			   double lat1 = PI * (-0.5 + (double) i / lats);
			   double z1 = sin(lat1);
			   double zr1 = cos(lat1);
   
			   glBegin(GL_QUAD_STRIP);
			   for(j = 0; j <= longs; j++) {
				   double lng = 2 * PI * (double) (j - 1) / longs;
				   double x = cos(lng);
				   double y = sin(lng);
  
				   glNormal3f(r* x * zr0, r* y * zr0, r* z0);
				   glVertex3f(r* x * zr0, r* y * zr0, r* z0);
				   glNormal3f(r* x * zr1, r* y * zr1, r* z1);
				   glVertex3f(r* x * zr1, r* y * zr1, r* z1);
			   }
			   glEnd();
		  }
	}

	inline void DrawGrid(float size=10, float step=1, bool xz=true)
	{
		// disable lighting
		glDisable(GL_LIGHTING);

		glBegin(GL_LINES);

		glColor3f(0.3f, 0.3f, 0.3f);
		for(float i=step; i <= size; i+= step)
		{
        

			if(xz)
			{
				glVertex3f(-size, 0,  i);   // lines parallel to X-axis
				glVertex3f( size, 0,  i);
				glVertex3f(-size, 0, -i);   // lines parallel to X-axis
				glVertex3f( size, 0, -i);

				glVertex3f( i, 0, -size);   // lines parallel to Z-axis
				glVertex3f( i, 0,  size);
				glVertex3f(-i, 0, -size);   // lines parallel to Z-axis
				glVertex3f(-i, 0,  size);
			}
			else
			{
				glVertex3f(-size,  i, 0);   // lines parallel to X-axis
				glVertex3f( size,  i, 0);
				glVertex3f(-size, -i, 0);   // lines parallel to X-axis
				glVertex3f( size, -i, 0);


				glVertex3f( i,  -size,0);   // lines parallel to Z-axis
				glVertex3f( i,   size,0);
				glVertex3f(-i,  -size,0);   // lines parallel to Z-axis
				glVertex3f(-i,   size,0);


			}
		}

		// x-axis
		glColor3f(0.5f, 0, 0);
		glVertex3f(-size, 0, 0);
		glVertex3f( size, 0, 0);

		if(xz)
		{
			// z-axis
			glColor3f(0,0,0.5f);
			glVertex3f(0, 0, -size);
			glVertex3f(0, 0,  size);

		}
		else
		{
			// z-axis
			glColor3f(0,0.5f,0);
			glVertex3f(0, -size,0);
			glVertex3f(0,  size,0);
		}


		glEnd();

		// enable lighting back
		glColor3f(1,1,1);
		glEnable(GL_LIGHTING);
	}


	inline void DrawCRect(float length)
	{
		glDisable(GL_LIGHTING);

		glBegin(GL_LINES);
			glVertex3f(-length/2.0,	length/2.0,0);
			glVertex3f(length/2.0,	length/2.0,0);
		glEnd();


		glBegin(GL_LINES);
			glVertex3f(length/2.0,length/2.0,0);
			glVertex3f(length/2.0,-length/2.0,0);
		glEnd();


		glBegin(GL_LINES);
			glVertex3f(-length/2.0,length/2.0,0);
			glVertex3f(-length/2.0,-length/2.0,0);
		glEnd();

		glBegin(GL_LINES);
			glVertex3f(-length/2.0,-length/2.0,0);
			glVertex3f(length/2.0,-length/2.0,0);
		glEnd();

		glEnable(GL_LIGHTING);

	}
	/*
	#include <gl\glut.h>

	//http://www.lighthouse3d.com/opengl/picking/index.php3?color1
	inline void drawSnowMan() {


		glColor3f(1.0f, 1.0f, 1.0f);

	// Draw Body	
		glTranslatef(0.0f ,0.75f, 0.0f);
		glutSolidSphere(0.75f,20,20);


	// Draw Head
		glTranslatef(0.0f, 1.0f, 0.0f);
		glutSolidSphere(0.25f,20,20);

	// Draw Eyes
		glPushMatrix();
		glColor3f(0.0f,0.0f,0.0f);
		glTranslatef(0.05f, 0.10f, 0.18f);
		glutSolidSphere(0.05f,10,10);
		glTranslatef(-0.1f, 0.0f, 0.0f);
		glutSolidSphere(0.05f,10,10);
		glPopMatrix();

	// Draw Nose
		glColor3f(1.0f, 0.5f , 0.5f);
		glRotatef(0.0f,1.0f, 0.0f, 0.0f);
		glutSolidCone(0.08f,0.5f,10,2);
	}
	*/

	inline void DrawRoom()										// Draw The Room (Box)
	{
		glBegin(GL_QUADS);									// Begin Drawing Quads
			// Floor
			glNormal3f(0.0f, 1.0f, 0.0f);					// Normal Pointing Up
			glVertex3f(-10.0f,-10.0f,-20.0f);				// Back Left
			glVertex3f(-10.0f,-10.0f, 20.0f);				// Front Left
			glVertex3f( 10.0f,-10.0f, 20.0f);				// Front Right
			glVertex3f( 10.0f,-10.0f,-20.0f);				// Back Right
			// Ceiling
			glNormal3f(0.0f,-1.0f, 0.0f);					// Normal Point Down
			glVertex3f(-10.0f, 10.0f, 20.0f);				// Front Left
			glVertex3f(-10.0f, 10.0f,-20.0f);				// Back Left
			glVertex3f( 10.0f, 10.0f,-20.0f);				// Back Right
			glVertex3f( 10.0f, 10.0f, 20.0f);				// Front Right
			// Front Wall
			glNormal3f(0.0f, 0.0f, 1.0f);					// Normal Pointing Away From Viewer
			glVertex3f(-10.0f, 10.0f,-20.0f);				// Top Left
			glVertex3f(-10.0f,-10.0f,-20.0f);				// Bottom Left
			glVertex3f( 10.0f,-10.0f,-20.0f);				// Bottom Right
			glVertex3f( 10.0f, 10.0f,-20.0f);				// Top Right
			// Back Wall
			glNormal3f(0.0f, 0.0f,-1.0f);					// Normal Pointing Towards Viewer
			glVertex3f( 10.0f, 10.0f, 20.0f);				// Top Right
			glVertex3f( 10.0f,-10.0f, 20.0f);				// Bottom Right
			glVertex3f(-10.0f,-10.0f, 20.0f);				// Bottom Left
			glVertex3f(-10.0f, 10.0f, 20.0f);				// Top Left
			// Left Wall
			glNormal3f(1.0f, 0.0f, 0.0f);					// Normal Pointing Right
			glVertex3f(-10.0f, 10.0f, 20.0f);				// Top Front
			glVertex3f(-10.0f,-10.0f, 20.0f);				// Bottom Front
			glVertex3f(-10.0f,-10.0f,-20.0f);				// Bottom Back
			glVertex3f(-10.0f, 10.0f,-20.0f);				// Top Back
			// Right Wall
			glNormal3f(-1.0f, 0.0f, 0.0f);					// Normal Pointing Left
			glVertex3f( 10.0f, 10.0f,-20.0f);				// Top Back
			glVertex3f( 10.0f,-10.0f,-20.0f);				// Bottom Back
			glVertex3f( 10.0f,-10.0f, 20.0f);				// Bottom Front
			glVertex3f( 10.0f, 10.0f, 20.0f);				// Top Front
		glEnd();											// Done Drawing Quads
	}

}

