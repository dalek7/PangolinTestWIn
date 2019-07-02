#include <iostream>
#include <pangolin/pangolin.h>
#include "../Thirdparty/BB/bbutil.h"
#include <iostream>

#include <vector>
#include <opencv2/opencv.hpp>


#ifndef PI
#define PI              3.14159265358979323   /* 180 deg */
#endif

#ifndef DEG
#define DEG(a)          (180.*a/PI )
#endif 

#ifndef RAD
#define RAD(a)          (PI*a/180.)
#endif 


using namespace std;
#include "geometry/mPlane.h"

void glCircle3f(GLfloat x, GLfloat y, GLfloat z, GLfloat r);
void DrawGrid(float size = 10, float step = 1, bool xz = true, bool lighton = true);
void DrawSphere(double r, int lats, int longs);

void glCross3f(GLfloat x, GLfloat y, GLfloat z, GLfloat d);

int LoadFile(string fname,  vector<Vector3> &vPts, char deliminiter=',');
int main()
{
	string fnload = "data/pts_20190629_120246_715000.txt";
	std::vector<Vector3> vPts;
	int ret = LoadFile(fnload, vPts, '\t');
	if (ret)
	{
		cout << "Loaded...." << fnload << "\t..." <<  vPts.size() << endl;
	}

	//ransac_plane_fitting(std::vector<Vector3> vpts, mPlane &model, double distance_threshold);
	mPlane model1;
	double threshold = 0.05;
	std::vector<Vector3> vpts_inliers;
	std::vector<unsigned int> vidx_inliers;
	double ret1 = mPlane::ransac_plane_fitting(vPts, vidx_inliers, vpts_inliers, model1, threshold);
	
	model1.Desc();
	char buf[255];
	sprintf_s(buf, "%.2f %c", 100.0f* vpts_inliers.size() / (float) vPts.size(), 37);
	cout << vPts.size() << " --> " << vpts_inliers.size() << " ( " <<  string(buf) <<" )" << endl;
	cout << vidx_inliers.size() << endl;
	

	Vector3 centerPts = mPlane::CalcCentroid(vpts_inliers);
	cout << "center : " << centerPts << endl;

	// Eigen
	cv::Vec3d eigv0, eigv1, eigv2;
	cv::Vec3d eigval;
	{
		// TODo : display inliers !
		cv::Mat_<double> cldm(vpts_inliers.size(), 3);
		for (unsigned int i = 0; i < vpts_inliers.size(); i++) {
			cldm.row(i)(0) = vpts_inliers[i].x;
			cldm.row(i)(1) = vpts_inliers[i].y;
			cldm.row(i)(2) = vpts_inliers[i].z;
		}

		cv::Mat_<double> mean;
		cv::PCA pca(cldm, mean, CV_PCA_DATA_AS_ROW);

		eigv0 = pca.eigenvectors.row(0);
		eigv1 = pca.eigenvectors.row(1);
		eigv2 = pca.eigenvectors.row(2);
		
		
		eigval[0] = pca.eigenvalues.at<double>(0);
		eigval[1] = pca.eigenvalues.at<double>(1);
		eigval[2] = pca.eigenvalues.at<double>(2);

		cout << "Eigen vectors" << endl;
		cout << eigv0 << endl;
		cout << eigv1 << endl;
		cout << eigv2 << endl;

		cout << "Eigen values" << endl;
		cout << eigval << endl;

	}


	cout << "--------------------------" << endl;

	int w = 640;// 640;
	int h = 480;// 480;
	pangolin::CreateWindowAndBind("Hello Points", w, h);
	glEnable(GL_DEPTH_TEST);
	// Define Projection and initial ModelView matrix
	pangolin::OpenGlRenderState s_cam(
		pangolin::ProjectionMatrix(w, h, 420, 420, w/2, h/2, 0.1, 1000),
		pangolin::ModelViewLookAt(-0, 0.5, -3, 0, 0, 0, pangolin::AxisY)
	);

	// Create Interactive View in window
	pangolin::Handler3D handler(s_cam);

	const int UI_WIDTH = 180;

	// Add named OpenGL viewport to window and provide 3D Handler
	pangolin::View& d_cam = pangolin::CreateDisplay()
		.SetBounds(0.0, 1.0, pangolin::Attach::Pix(UI_WIDTH), 1.0, - w/ (float) h)
		.SetHandler(new pangolin::Handler3D(s_cam));

	// Add named Panel and bind to variables beginning 'ui'
	// A Panel is just a View with a default layout and input handling
	pangolin::CreatePanel("ui")
		.SetBounds(0.0, 1.0, 0.0, pangolin::Attach::Pix(UI_WIDTH));

	pangolin::Var<bool> bDrawAxis("ui.Draw Plane", false, false);
	pangolin::Var<bool> bDrawInliers("ui.Draw Inliers", false, false);
	pangolin::Var<double> a_double("ui.A_Double", 3, 0, 5);
	pangolin::Var<int> an_int("ui.An_Int", 2, 0, 5);

	
	//pangolin::Var<bool> save_window("ui.Save_Window", false, false);
	int count = 0;
	while (!pangolin::ShouldQuit())
	{
		// Clear screen and activate view to render into
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//a_double = 3.0 + sin((double)count / 1000.0);

		d_cam.Activate(s_cam);

		// Render OpenGL Cube
		//pangolin::glDrawColouredCube();

		// Draw points defined by vPts
		glPointSize(3);
		glBegin(GL_POINTS);
		{
			for (vector<Vector3>::const_iterator it = vPts.begin(); it != vPts.end(); ++it)
			{
				glColor3ub(255, 255, 255);
				if (1)
				{
					glVertex3d(it->x, it->y, it->z - 0.01);
				}
				else
				{
					glCircle3f(it->x, it->y, it->z, 0.02);
				}
			}
		}
		glEnd();

		
		glColor3ub(255, 0, 0);
		if(bDrawInliers)
		for (vector<Vector3>::const_iterator it = vpts_inliers.begin(); it != vpts_inliers.end(); ++it)
		{
			//glVertex3d(it1->x, it1->y, it1->z - 0.01);
			glCircle3f(it->x, it->y, it->z - 0.01, 0.04);
			//glCircle3f(it->x, it->y, it->z - 0.01, 0.02);
		}
		//glCross3f(centerPts.x, centerPts.y, centerPts.z, 1);

		/*
		glColor3ub(255, 255, 255);
		glPushMatrix();
			glTranslatef(centerPts.x, centerPts.y, centerPts.z);
			DrawSphere(0.1, 16, 16);
		glPopMatrix();
		*/

		if (bDrawAxis)
		{
			glLineWidth(2);

			glColor3f(1, 0, 0);
			glBegin(GL_LINE_STRIP);
			glVertex3f(centerPts.x, centerPts.y, centerPts.z);
			glVertex3f(centerPts.x - eigv0[0], centerPts.y - eigv0[1], centerPts.z - eigv0[2]);
			glEnd();

			glColor3f(0, 1, 0);
			glBegin(GL_LINE_STRIP);
			glVertex3f(centerPts.x, centerPts.y, centerPts.z);
			glVertex3f(centerPts.x + eigv1[0], centerPts.y + eigv1[1], centerPts.z + eigv1[2]);
			glEnd();

			glColor3f(0, 0, 1);
			glBegin(GL_LINE_STRIP);
			glVertex3f(centerPts.x, centerPts.y, centerPts.z);
			glVertex3f(centerPts.x + eigv2[0], centerPts.y + eigv2[1], centerPts.z + eigv2[2]);
			glEnd();

			glColor3f(0.5, 0.5, 0.5);

			glLineWidth(1);

			// Plane drawing
			int pw = 5;
			int ph = ceil(pw * eigval[1]); //eig[1]//eigval[0]


			for (int i = -pw; i<pw + 1; i++)
			{
				float ii = (float)i;
				glBegin(GL_LINE_STRIP);

				glVertex3f(centerPts.x - ph*eigv1[0] + ii*eigv0[0], centerPts.y - ph*eigv1[1] + ii*eigv0[1], centerPts.z - ph*eigv1[2] + ii*eigv0[2]);
				glVertex3f(centerPts.x + ph*eigv1[0] + ii*eigv0[0], centerPts.y + ph*eigv1[1] + ii*eigv0[1], centerPts.z + ph*eigv1[2] + ii*eigv0[2]);

				glEnd();

			}
			for (int i = -ph; i<ph + 1; i++)
			{
				float ii = (float)i;
				glBegin(GL_LINE_STRIP);

				glVertex3f(centerPts.x - pw*eigv0[0] + ii*eigv1[0], centerPts.y - (pw)*eigv0[1] + ii*eigv1[1], centerPts.z - pw*eigv0[2] + ii*eigv1[2]);
				glVertex3f(centerPts.x + pw*eigv0[0] + ii*eigv1[0], centerPts.y + (pw)*eigv0[1] + ii*eigv1[1], centerPts.z + pw*eigv0[2] + ii*eigv1[2]);

				glEnd();

			}

			glColor3f(1, 1, 1);

		}


		// Swap frames and Process Events
		pangolin::FinishFrame();
		count++;
		if (count == 100000)
			count = 0;
	}

}

//http://www.cplusplus.com/forum/general/150001/
int LoadFile(std::string fname, vector<Vector3> &vPts, char deliminiter)
{
	//std::cout << "Current path is " << GetWorkingDir() << "\n";
	std::ifstream infile(fname);
	if (!infile) {
		std::cerr << "Could not open file." << std::endl;
		return 0;
	}
	std::string line;
	while (std::getline(infile, line)) {  // Read file line by line.

		//cout << line << endl;
		std::string field;
		std::vector<std::string> separated_fields;
		std::istringstream iss_line(line);
		Vector3 pts;
		int i = 0;
		while (std::getline(iss_line, field, deliminiter)) { // Split line on the ':' character
			//pts.x = atof(field.c_str());
			pts[i++] = atof(field.c_str());
			separated_fields.push_back(field);    // Vector containing each field, i.e. name, address...
		}

		vPts.push_back(pts);
		//cout << pts[0] << "\t" << pts[1] << "\t" << pts[2] << endl;
	}


	return 1;
}

void glCircle3f(GLfloat x, GLfloat y, GLfloat z, GLfloat r)
{
	float step = 360.0 / (2.0*PI*r) / 16;
	glPushMatrix();
	glTranslatef(x, y, z);
	glBegin(GL_LINE_STRIP);
	//glVertex2f(0, 0);
	for (float i = 0; i<360; i += step)
	{
		glVertex3f((cos(RAD(i))*r), (sin(RAD(i))*r), 0);
	}
	glEnd();
	glPopMatrix();
}


void glCross3f(GLfloat x, GLfloat y, GLfloat z, GLfloat d)
{
	glPushMatrix();

	glBegin(GL_LINE_STRIP);
	glVertex3f(x - d / 2, y, z);
	glVertex3f(x + d / 2, y, z);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(x, y - d / 2, z);
	glVertex3f(x, y + d / 2, z);
	glEnd();


	glPopMatrix();
}

//http://ozark.hendrix.edu/~burch/cs/490/sched/feb8/
//drawSphere(1.0, 10, 10);
void drawSphere(double r, int lats, int longs)
{

	int i, j;
	for (i = 0; i <= lats; i++) {
		double lat0 = PI * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = PI * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= longs; j++) {
			double lng = 2 * PI * (double)(j - 1) / longs;
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



void DrawGrid(float size, float step, bool xz, bool lighton)
{
	// disable lighting
	if(lighton)
		glDisable(GL_LIGHTING);

	glBegin(GL_LINES);

	glColor3f(0.3f, 0.3f, 0.3f);
	for (float i = step; i <= size; i += step)
	{


		if (xz)
		{
			glVertex3f(-size, 0, i);   // lines parallel to X-axis
			glVertex3f(size, 0, i);
			glVertex3f(-size, 0, -i);   // lines parallel to X-axis
			glVertex3f(size, 0, -i);

			glVertex3f(i, 0, -size);   // lines parallel to Z-axis
			glVertex3f(i, 0, size);
			glVertex3f(-i, 0, -size);   // lines parallel to Z-axis
			glVertex3f(-i, 0, size);
		}
		else
		{
			glVertex3f(-size, i, 0);   // lines parallel to X-axis
			glVertex3f(size, i, 0);
			glVertex3f(-size, -i, 0);   // lines parallel to X-axis
			glVertex3f(size, -i, 0);


			glVertex3f(i, -size, 0);   // lines parallel to Z-axis
			glVertex3f(i, size, 0);
			glVertex3f(-i, -size, 0);   // lines parallel to Z-axis
			glVertex3f(-i, size, 0);


		}
	}

	// x-axis
	glColor3f(0.5f, 0, 0);
	glVertex3f(-size, 0, 0);
	glVertex3f(size, 0, 0);

	if (xz)
	{
		// z-axis
		glColor3f(0, 0, 0.5f);
		glVertex3f(0, 0, -size);
		glVertex3f(0, 0, size);

	}
	else
	{
		// z-axis
		glColor3f(0, 0.5f, 0);
		glVertex3f(0, -size, 0);
		glVertex3f(0, size, 0);
	}


	glEnd();

	// enable lighting back
	glColor3f(1, 1, 1);

	if (lighton)
		glEnable(GL_LIGHTING);
}

void DrawSphere(double r, int lats, int longs)
{

	int i, j;
	for (i = 0; i <= lats; i++) {
		double lat0 = PI * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = PI * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= longs; j++) {
			double lng = 2 * PI * (double)(j - 1) / longs;
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