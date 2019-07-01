#include <iostream>
#include <pangolin/pangolin.h>
#include "../Thirdparty/BB/bbutil.h"
#include <iostream>

#include <vector>


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
	

	// TODo : display inliers !


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

	pangolin::Var<bool> a_button("ui.A_Button", false, false);
	pangolin::Var<double> a_double("ui.A_Double", 3, 0, 5);
	pangolin::Var<int> an_int("ui.An_Int", 2, 0, 5);


	//pangolin::Var<bool> save_window("ui.Save_Window", false, false);
	int count = 0;
	while (!pangolin::ShouldQuit())
	{
		// Clear screen and activate view to render into
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (pangolin::Pushed(a_button))
		{
			std::cout << "You Pushed a button!" << std::endl;
			a_double = 3.3; //for test
		}

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
		for (vector<Vector3>::const_iterator it = vpts_inliers.begin(); it != vpts_inliers.end(); ++it)
		{
			//glVertex3d(it1->x, it1->y, it1->z - 0.01);
			glCircle3f(it->x, it->y, it->z - 0.01, 0.04);
			//glCircle3f(it->x, it->y, it->z - 0.01, 0.02);
		}

		// TODO 
		// Draw plane !


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