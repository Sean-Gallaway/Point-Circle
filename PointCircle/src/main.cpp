#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

// init variables and function declarations.
void drawCircle (double x, double y, double r, int num_segments);
std::vector<double> drawPoints (double cr, double r, double* dist);
void drawRect(std::vector<double> points);
double width = 600;
double height = 400;
double h_width = width / 2;
double h_height = height / 2;


int main(void) {
	GLFWwindow* window;
	
	// initialize glfw
	if (!glfwInit())
		return -1;

	// create the window
	window = glfwCreateWindow(width, height, "Assignment", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// set the window
	glfwMakeContextCurrent(window);

	// set up variables
	double positions[8] = {
		 100.0/h_width,		0,
		 -100.0/h_width,	0,
		 0,				-100.0/h_height,
		 0,				100.0/h_height,
	};

	double distances[4] = {
		.1, .3, .7, .8
	};

	const double fps = 1.0 / 60.0;
	double lastUpdate = 0;
	double lastFrame = 0;

	// while the window is open.
	while (!glfwWindowShouldClose(window)) {
		double now = glfwGetTime();
		double deltaTime = now - lastUpdate;

		glfwPollEvents();

		// frame rate limited code section
		if ((now - lastFrame) >= fps) {
			// clear the screen
			glClear(GL_COLOR_BUFFER_BIT);

			// set the color
			glColor3f(1, 0, 0);

			// draw the unit circle
			drawCircle(0, 0, 100, 100);

			// draw a circle at each of the cardinal directions on the unit circle.
			for (int a = 0; a < 4; a++) {
				drawCircle(positions[2 * a], positions[2 * a + 1], 5, 20);
			}
			
			// draw the points that are set distances away from the cardinal points, then draw a rectangle using those new points.
			drawRect(drawPoints(100, 10, distances));

			// for funsies, update the distances every frame to see it moving.
			for (int a = 0; a < 4; a++) {
				distances[a] += .001;
				if (distances[a] > 1) {
					distances[a] = 0;
				}
			}

			glfwSwapBuffers(window);
			lastFrame = now;
		}

		lastUpdate = now;
	}

	glfwTerminate();
	return 0;
}

/*
* Draws a circle at a given x/y location, with a radius R and a resolution of num_segments using a Line Loop.
*/
void drawCircle (double x, double y, double r, int num_segments) {
	glBegin(GL_LINE_LOOP);
	for (int a = 0; a < num_segments; a++) {
		double theta = 2.0 * M_PI * a / double(num_segments);
		double x2 = r * cos(theta);
		double y2 = r * sin(theta);

		x2 /= h_width;
		y2 /= h_height;

		glVertex2f(x + x2, y + y2);
	}
	glEnd();
}

/*
* draws a given set of points as circles that are a certain distance away from a cardinal direction on the circle.
*/
std::vector<double> drawPoints (double cr, double r, double* dist) {
	glColor3f(0, 0, 1);
	double x, y, theta;
	double pos[4] = { 0, M_PI/2, M_PI, (3*M_PI)/2};
	std::vector<double> points;

	// for each point we are to draw, the distance from a cardinal point.
	for (int a = 0; a < 4; a++) {
		theta = 2 * (M_PI/4) * dist[a];
		x = (cos(pos[a] - theta))*(cr/h_width);
		y = (sin(pos[a] - theta))*(cr/h_height);
		points.insert(points.end(), x);
		points.insert(points.end(), y);
		drawCircle(x, y, r, 10);
	}

	return points;
}

/*
*Draws a rectangle to the current context with a given set of points. it is assumed that the points vector contains at least 8 doubles.
*/
void drawRect (std::vector<double> points) {
	glBegin(GL_QUADS);
	glColor3f(0, 1, 0);
	for (long a = 0; a < 4; a++) {
		glVertex2f(points.at(a*2), points.at(a * 2 + 1));
	}
	glEnd();
}