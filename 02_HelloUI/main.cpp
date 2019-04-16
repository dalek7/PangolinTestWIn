#include <iostream>
#include <pangolin/pangolin.h>

using namespace std;

struct CustomType
{
	CustomType()
		: x(0), y(0.0f) {}

	CustomType(int x, float y, std::string z)
		: x(x), y(y), z(z) {}

	int x;
	float y;
	std::string z;
};

std::ostream& operator<< (std::ostream& os, const CustomType& o) {
	os << o.x << " " << o.y << " " << o.z;
	return os;
}

std::istream& operator >> (std::istream& is, CustomType& o) {
	is >> o.x;
	is >> o.y;
	is >> o.z;
	return is;
}

int main(int /*argc*/, char** /*argv*/)
{
	pangolin::CreateWindowAndBind("20190416@Chuncheon", 640, 480);
	glEnable(GL_DEPTH_TEST);

	// Define Projection and initial ModelView matrix
	pangolin::OpenGlRenderState s_cam(
		pangolin::ProjectionMatrix(640, 480, 420, 420, 320, 240, 0.1, 1000),
		pangolin::ModelViewLookAt(-0, 0.5, -3, 0, 0, 0, pangolin::AxisY)
	);

	// Create Interactive View in window
	pangolin::Handler3D handler(s_cam);
	
	const int UI_WIDTH = 180;

	// Add named OpenGL viewport to window and provide 3D Handler
	pangolin::View& d_cam = pangolin::CreateDisplay()
		.SetBounds(0.0, 1.0, pangolin::Attach::Pix(UI_WIDTH), 1.0, -640.0f / 480.0f)
		.SetHandler(new pangolin::Handler3D(s_cam));

	// Add named Panel and bind to variables beginning 'ui'
	// A Panel is just a View with a default layout and input handling
	pangolin::CreatePanel("ui")
		.SetBounds(0.0, 1.0, 0.0, pangolin::Attach::Pix(UI_WIDTH));

	pangolin::Var<bool> a_button("ui.A_Button", false, false);
	pangolin::Var<double> a_double("ui.A_Double", 3, 0, 5);
	pangolin::Var<int> an_int("ui.An_Int", 2, 0, 5);

	pangolin::Var<CustomType> any_type("ui.Some_Type", CustomType(0, 1.2f, "Hello"));

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
		
		a_double = 3.0+ sin( (double)count / 1000.0);

		
		d_cam.Activate(s_cam);

		// Render OpenGL Cube
		pangolin::glDrawColouredCube();

		// Swap frames and Process Events
		pangolin::FinishFrame();
		count++;
		if (count == 100000)
			count = 0;
	}

	return 0;
}