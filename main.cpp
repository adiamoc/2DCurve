#ifdef WIN32
#include <windows.h>
#endif

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

//other includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "curve.cpp"

using namespace std;

// ---------- Data Section ------------
/****set in main()****/
//the number of pixels in the grid
int grid_width;
int grid_height;

//the size of pixels sets the inital window height and width
//don't make the pixels too large or the screen size will be larger than
//your display size
float pixel_size;

/*Window information*/
int win_height;
int win_width;


int curveSel = -1;
int curveNum = 0;
int cptRatio = 5;

vector<Curve> curves;
Curve curr;
bool startCurve = false;
bool doPoint = false;
bool addPoint = false;
bool delPoint = false;
bool doSel = false;
int indexI = -1;
int indexJ = -1;
int selKnot = -1;
Point selPt;

int window_1, window_2, window_3;
float R = 1, G = 0, B = 0;
float selR = 0, selG = 0, selB = 0;
char* filename;

// ------------- Functions --------------

void init();
void idle();
void display();
void draw_pix(int x, int y, int ratio = 1, float r = 0.0, float g = 0.0, float b = 0.0);
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void SpecialInput(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();

void panel_reshape(int width, int height) {
	/*set up projection matrix to define the view port*/
	//update the ne window width and height

	//creates a rendering area across the window
	glutReshapeWindow( 350, 300);
	glViewport(0, 0, 350, 300);
	// up an orthogonal projection matrix so that
	// the pixel space is mapped to the grid space
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 350, 0, 300, -1, 1);

	//clear the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//set pixel size based on width, if the aspect ratio
	//changes this hack won't work as well

	//set pixel size relative to the grid cell size
	glPointSize(1);
	//check for opengl errors
	check();
}

void panel_mouse(int button, int state, int x, int y) {
	float yy = 300 - y;
	float xx = x;

	if (state == GLUT_DOWN)
	{
		if (x > 300 && x < 350)
		{
			if (yy < 50) {
				R = 1;
				G = yy / 50;
				B = 0;
			}
			else if (yy < 100) {
				R = 1 - (yy - 50) / 50;
				G = 1;
				B = 0;
			}
			else if (yy < 150) {
				R = 0;
				G = 1;
				B = (yy - 100) / 50;
			}
			else if (yy < 200) {
				R = 0;
				G = 1 - (yy - 150) / 50;
				B = 1;
			}
			else if (yy < 250) {
				R = (yy - 200) / 50;
				G = 0;
				B = 1;
			}
			else if (yy < 300) {
				R = 1;
				G = 0;
				B = 1 - (yy - 250) / 50;
			}
		}

		if (xx > 0 && xx < 300) {
			selR = (xx / 300) * ((float)yy / 300 * R) + (1 - xx / 300) * ((float)yy / 300);
			selG = (xx / 300) * ((float)yy / 300 * G) + (1 - xx / 300) * ((float)yy / 300);
			selB = (xx / 300) * ((float)yy / 300 * B) + (1 - xx / 300) * ((float)yy / 300);
			if (selR > 1) selR = 1;
			if (selR < 0) selR = 0;
			if (selG > 1) selG = 1;
			if (selG < 0) selG = 0;
			if (selB > 1) selB = 1;
			if (selB < 0) selB = 0;
		}
	}

	if (indexI != -1)
	{
		curves[indexI].r = selR;
		curves[indexI].g = selG;
		curves[indexI].b = selB;
	}
	if (curveNum  == curr.id || curveNum - 1 == curr.id)
	{
		curr.r = selR;
		curr.g = selG;
		curr.b = selB;
	}
	glutSetWindow(window_1);
	glutPostRedisplay();
	glutSetWindow(window_2);


}

void panel_display() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	float r = 0, g = 0, b = 0;

	for (float i = 0; i <  300; ++i)
	{
		glLineWidth(1);
		glBegin(GL_LINES);

		glColor3f(0, 0, 0);
		if (i < 50) {
			r = 1;
			g = i / 50;
			b = 0;
			// glColor3f(1, i/50, 0);
		}
		else if (i < 100) {
			r = 1 - (i - 50) / 50;
			g = 1;
			b = 0;
			// glColor3f(1-(i-50)/50, 1, 0);
		}
		else if (i < 150) {
			r = 0;
			g = 1;
			b = (i - 100) / 50;
			// glColor3f(0, 1, (i-100)/50);
		}
		else if (i < 200) {
			r = 0;
			g = 1 - (i - 150) / 50;
			b = 1;
			// glColor3f(0, 1-(i-150)/50, 1);
		}
		else if (i < 250) {
			r = (i - 200) / 50;
			g = 0;
			b = 1;
			// glColor3f((i-200)/50, 0, 1);
		}
		else if (i < 300) {
			r = 1;
			g = 0;
			b = 1 - (i - 250) / 50;
			// glColor3f(1, 0, 1 - (i-250)/50);
		}

		glColor3f(r, g, b);

		glVertex3f(300, i, 0);  
		glVertex3f(350, i, 0);
		glEnd();
	}

	for (int i = 0; i < 300; ++i)
	{
		glLineWidth(1);
		glBegin(GL_LINES);

		glColor3f((float)i / 300, (float)i / 300, (float)i / 300);
		glVertex3f(0, i, 0);

		glColor3f((float)i / 300 * R, (float)i / 300 * G, (float)i / 300 * B);
		glVertex3f(300, i, 0);
		glEnd();
	}


	glFlush();
	//blits the current opengl framebuffer on the screen
	glutSwapBuffers();
	//checks for open
	check();
}


void knot_reshape(int width, int height) {
	/*set up projection matrix to define the view port*/
	//update the ne window width and height

	//creates a rendering area across the window
	glutReshapeWindow( win_width, 100);
	glViewport(0, 0, win_width, 100);
	// up an orthogonal projection matrix so that
	// the pixel space is mapped to the grid space
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, win_width, 0, 100, -1, 1);

	//clear the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
	//set pixel size based on width, if the aspect ratio
	//changes this hack won't work as well

	//set pixel size relative to the grid cell size
	glPointSize(1);
	//check for opengl errors
	check();
}

void knot_motion(int x, int y) {
	x *= (grid_width * pixel_size) / win_width;
	x = x ;

	if (curveNum != 0 && selKnot != -1 && indexI != -1)
	{
		int last = curves[indexI].knots.size() - 1;
		float pos = win_width / (curves[indexI].knots.size() + 1);
		if (selKnot != 0 && selKnot != last)
		{
			if ((x - pos) / pos > curves[indexI].knots[selKnot - 1]
			        && (x - pos) / pos < curves[indexI].knots[selKnot + 1])
			{
				curves[indexI].knots[selKnot] = (x - pos) / pos;
			}
		} else {
			if (selKnot == 0)
			{
				if (curves[indexI].knots[0] < curves[indexI].knots[1])
					curves[indexI].knots[0] = (x - pos) / pos;
			}
			else {
				if (curves[indexI].knots[last] > curves[indexI].knots[last - 1])
					curves[indexI].knots[last] = (x - pos) / pos;
			}
		}
	}

	if (curveNum == curr.id)
	{
		int last = curr.knots.size() - 1;
		float pos = win_width / (curr.knots.size() + 1);
		if (selKnot != 0 && selKnot != last)
		{
			if ((x - pos) / pos > curr.knots[selKnot - 1]
			        && (x - pos) / pos < curr.knots[selKnot + 1])
			{
				curr.knots[selKnot] = (x - pos) / pos;
			}
		} else {
			if (selKnot == 0)
			{
				if (curr.knots[0] < curr.knots[1])
					curr.knots[0] = (x - pos) / pos;
			}
			else {
				if (curr.knots[last] > curr.knots[last - 1])
					curr.knots[last] = (x - pos) / pos;
			}
		}
	}

	glutSetWindow(window_1);
	glutPostRedisplay();
	glutSetWindow(window_3);
	glutPostRedisplay();
	return;
}

void knot_mouse(int button, int state, int x, int y) {
	x *= (grid_width * pixel_size) / win_width;
	x = x ;

	if (state == GLUT_DOWN)
	{
		if (indexI != -1)
		{
			float pos = win_width / (curves[indexI].knots.size() + 1);
			for (int j = 0; j < curves[indexI].knots.size(); ++j)
			{
				if (fabs(x - (curves[indexI].knots[j] * pos + pos)) < pixel_size * cptRatio)
				{
					selKnot = j;
					break;
				}
			}
		}
		if ((curveNum == curr.id))
		{
			float pos = win_width / (curr.knots.size() + 1);
			for (int j = 0; j < curr.knots.size(); ++j)
			{
				if (fabs(x - (curr.knots[j] * pos + pos)) < pixel_size * cptRatio)
				{
					selKnot = j;
					break;
				}
			}
		}
	}

	if (state == GLUT_UP)
	{
		selKnot = -1;
	}

	glutPostRedisplay();
}

void knot_display() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	if (indexI != -1 && doPoint)
	{
		if (curves[indexI].type == 1 && curves[indexI].pts.size())
		{
			float pos = win_width / (curves[indexI].knots.size() + 1);
			for (int i = 0; i < curves[indexI].knots.size(); ++i)
			{
				draw_pix(curves[indexI].knots[i] * pos + pos, 50, 5, 
					curves[indexI].r, curves[indexI].g, curves[indexI].b);
			}
		}
	}

	if (curr.id == curveNum )
	{
		if (curr.type == 1 && curr.pts.size())
		{
			float pos = win_width / (curr.knots.size() + 1);
			for (int i = 0; i < curr.knots.size(); ++i)
			{
				draw_pix(curr.knots[i] * pos + pos, 50, 5, curr.r, curr.g, curr.b);
			}
		}
	}

	glFlush();
	//blits the current opengl framebuffer on the screen
	glutSwapBuffers();
	//checks for open
	check();
}


void readFile(int argc, char** argv) {
	if (argc != 2)
		return;
	ifstream f(argv[1]);

	filename = (char*)malloc(strlen(argv[1])+1);
	strcpy(filename, argv[1]);
	
	if (f)
	{

		Point p;
		float t;
		f >> curveNum;
		for (int i = 0; i < curveNum; ++i)
		{
			Curve c;
			f >> c.ptNum;
			f >> c.id;
			f >> c.type;
			f >> c.res;
			f >> c.k;
			f >> c.r;
			f >> c.g;
			f >> c.b;
			f >> c.stroke;
			for (int j = 0; j < c.ptNum; ++j){
				f >> p.x;
				f >> p.y;
				c.pts.push_back(p);
			}
			if (c.type == 1){
				for (int j = 0; j < c.k + c.ptNum; ++j){
					f >> t;
					c.knots.push_back(t);
				}
			}

			curves.push_back(c);
		}
		f.close();
	} else {
		ofstream n(filename);
		n.close();
	}
}

int main(int argc, char **argv)
{
	pixel_size = 1;
	grid_width = 800;
	grid_height = 500;

	win_width = pixel_size * grid_width;
	win_height = pixel_size * grid_height;

	readFile(argc, argv);

	// setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(win_width, 100);
	glutInitWindowPosition(350, win_height + 70);
	window_3 = glutCreateWindow("Knot");
	init();
	// callback
	glutDisplayFunc(knot_display); //rendering calls here
	glutReshapeFunc(knot_reshape); //update GL on window size change
	glutMouseFunc(knot_mouse);     //mouse button events
	glutMotionFunc(knot_motion);



	glutInitWindowSize(350, 300);
	glutInitWindowPosition(0, 0);
	window_2 = glutCreateWindow("Color");
	init();
	// callback
	glutDisplayFunc(panel_display); //rendering calls here
	glutReshapeFunc(panel_reshape); //update GL on window size change
	glutMouseFunc(panel_mouse);     //mouse button events

	glutInitWindowSize(win_width, win_height);
	glutInitWindowPosition(350, 0);
	window_1 = glutCreateWindow("Project 4");
	init();
	// callback
	glutDisplayFunc(display); //rendering calls here
	glutReshapeFunc(reshape); //update GL on window size change
	glutMouseFunc(mouse);     //mouse button events
	glutMotionFunc(motion);   //mouse movement events
	glutKeyboardFunc(key);    //Keyboard events
	glutSpecialFunc(SpecialInput); // special
	glutIdleFunc(idle);       //Function called while program is sitting "idle"

	glutMainLoop();

	return 0;
}

void init()
{
	//set clear color (Default background to white)
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//checks for OpenGL errors
	check();
}

//called repeatedly when glut isn't doing anything else
void idle()
{
	//redraw the scene over and over again
	glutPostRedisplay();

}

void draw_line(float x1, float y1, float x2, float y2, int ratio = 1, float r = 0.0, float g = 0.0, float b = 0.0) {
	glLineWidth(pixel_size * ratio);
	glBegin(GL_LINES);
	glColor3f(r, g, b);
	glVertex3f(x1 + .5, y1 + .5, 0);
	glVertex3f(x2 + .5, y2 + .5, 0);
	glEnd();
}

void lerp(Point& d, Point a, Point b, float t) {
	d.x = a.x * (1 - t) + b.x * t;
	d.y = a.y * (1 - t) + b.y * t;

}

Point bezierPoint(Curve c, float t)
{
	int n = c.pts.size();

	Point d[n][n];
	for (int i = 0; i < n; i++)
	{
		d[0][i] = c.pts[i];
	}

	for (int j = 1; j < n; j++)
	{
		for (int i = 0; i < n - j; i ++)
			lerp(d[j][i], d[j - 1][i], d[j - 1][i + 1], t);
	}
	return d[n - 1][0];
}

void Bezier(Curve c) {
	if (c.ptNum < 2)
		return;

	float step = (float)1 / c.res;
	for (float t = 0; t < 1 - step / 2; t += step)
	{
		Point p1 = bezierPoint(c, t);
		Point p2 = bezierPoint(c, t + step);
		draw_line(p1.x, p1.y, p2.x, p2.y, c.stroke, c.r, c.g, c.b);
	}
}

Point bsplinePoint(Curve c, float t)
{

	int n = c.ptNum;
	int k = c.k;
	int I = -1;

	Point d[n][n];
	for (int i = 0; i < n; i++)
		d[0][i] = c.pts[i];

	for (int i = 0; i < k + n; i++)
	{
		if (t < c.knots[i + 1] && t >= c.knots[i])
		{
			I = i;
			break;
		}
	}

	for (int j = 1; j <= k - 1; j++)
	{
		for (int i = (I - (k - 1)); i <= I - j; i++)
		{
			float a = (float)(c.knots[i + k] - t) / (c.knots[i + k] - c.knots[i + j]);
			float b = (float)(t - c.knots[i + j]) / (c.knots[i + k] - c.knots[i + j]);
			d[j][i].x = d[j - 1][i].x * a + d[j - 1][i + 1].x * b;
			d[j][i].y = d[j - 1][i].y * a + d[j - 1][i + 1].y * b;

		}
	}

	return d[k - 1][I - (k - 1)];

}

void setupKnots(Curve& c) {
	c.knots.clear();

	if (!c.pts.size())
		return;

	for (float i = 0; i < c.k + c.pts.size(); i += 1.0)
	{
		c.knots.push_back(i);
	}
}

void BSpline(Curve& c)
{
	if (c.knots.empty()) {
		setupKnots(c);
	}

	float step = 1.0 / c.res;
	int k = c.k;
	int n = c.ptNum ;

	if (k == 1)
	{
		for (int i = 0; i < c.ptNum; ++i)
		{
			draw_pix(c.pts[i].x, c.pts[i].y);
		}
		return;
	}

	for (float t = c.knots[k - 1]; t <= c.knots[n] - step / 2; t += step)
	{
		Point p1 = bsplinePoint( c, t);
		Point p2 = bsplinePoint( c, t + step);
		draw_line(p1.x, p1.y, p2.x, p2.y, c.stroke, c.r, c.g, c.b);
	}

}

void drawCurve(Curve& c) {
	if (c.pts.empty())
		return;

	for (int i = 0; i < c.ptNum; ++i)
	{
		float r = 0, g = 0, b = 0;
		if (c.type == 0) {
			r = .8;
		}
		if (c.type == 1) {
			g = .5;
			b = .8;
		}
		if (addPoint) {
			r = .8;
			b = 1;
		}
		if (delPoint)
		{
			r = .5;
			g = .8;
			b = 0;
		}
		if (doPoint || (c.id == curveNum ))
		{
			draw_pix(c.pts[i].x, c.pts[i].y, cptRatio, r, g, b);
			if (i != c.ptNum - 1)
			{
				draw_line(c.pts[i].x, c.pts[i].y, c.pts[i + 1].x, c.pts[i + 1].y, 1, r, g, b);
			}
		}
	}
	if (c.type == 0)
		Bezier(c);
	if (c.type == 1)
		BSpline(c);
}

//this is where we render the screen
void display()
{
	//clears the screen
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//clears the opengl Modelview transformation matrix
	glLoadIdentity();
	drawCurve(curr);
	for (int i = 0; i < curveNum; ++i)
	{
		drawCurve(curves[i]);
	}
	glutSetWindow(window_3);
	glutPostRedisplay();
	glutSetWindow(window_1);

	//blits the current opengl framebuffer on the screen
	glutSwapBuffers();
	//checks for opengl errors
	check();
}

void draw_pix(int x, int y, int ratio, float r, float g, float b) {
	glPointSize(pixel_size * ratio);
	glBegin(GL_POINTS);
	glColor3f(r, g, b);
	glVertex3f(x + .5, y + .5, 0);
	glEnd();
}


/*Gets called when display size changes, including initial craetion of the display*/
void reshape(int width, int height)
{
	/*set up projection matrix to define the view port*/
	//update the ne window width and height
	win_width = width;
	win_height = height;

	//creates a rendering area across the window
	glViewport(0, 0, width, height);
	// up an orthogonal projection matrix so that
	// the pixel space is mapped to the grid space
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, grid_width, 0, grid_height, -1, 1);

	//clear the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutSetWindow(window_3);
	knot_reshape( width, height);
	glutSetWindow(window_1);

	//set pixel size based on width, if the aspect ratio
	//changes this hack won't work as well
	pixel_size = width / (float)grid_width;

	//set pixel size relative to the grid cell size
	glPointSize(pixel_size);
	//check for opengl errors
	check();
}

void saveFile() {

	ofstream f(filename);
	if (f)
	{
		f << curveNum << endl << endl;
		for (int i = 0; i < curveNum; ++i)
		{
			
			f << curves[i].ptNum << endl;
			f << curves[i].id << endl;
			f << curves[i].type << endl;
			f << curves[i].res << endl;
			f << curves[i].k << endl;
			f << curves[i].r << endl;
			f << curves[i].g << endl;
			f << curves[i].b << endl;
			f << curves[i].stroke  << endl;
			for (int j = 0; j < curves[i].ptNum; ++j){
				f << curves[i].pts[j].x << endl;
				f << curves[i].pts[j].y << endl;
			}
			if (curves[i].type == 1){
				for (int j = 0; j < curves[i].k + curves[i].ptNum; ++j){
					f << curves[i].knots[j] << endl;
				}
			}
		}
		free(filename);
	} else {
		ofstream n("curves.txt");
		f << curveNum << endl << endl;
		for (int i = 0; i < curveNum; ++i)
		{
			n << curves[i].ptNum << endl;
			n << curves[i].id << endl;
			n << curves[i].type << endl;
			n << curves[i].res << endl;
			n << curves[i].k << endl;
			n << curves[i].r << endl;
			n << curves[i].g << endl;
			n << curves[i].b << endl;
			n << curves[i].stroke  << endl;
			for (int j = 0; j < curves[i].ptNum; ++j){
				n << curves[i].pts[j].x << endl;
				n << curves[i].pts[j].y << endl;
			}
			if (curves[i].type == 1){
				for (int j = 0; j < curves[i].k + curves[i].ptNum; ++j){
					n << curves[i].knots[j] << endl;
				}
			}
		}
		n.close();
	}
	f.close();

}

//gets called when a mouse button is pressed
void mouse(int button, int state, int x, int y)
{
	x *= (grid_width * pixel_size) / win_width;
	y = win_height - y;
	y *= (grid_height * pixel_size) / (win_height);
	Point pt;
	pt.x = x / pixel_size;
	pt.y = y / pixel_size;

	if (state == GLUT_DOWN)
	{
		indexI = -1;
		indexJ = -1;
		if (startCurve)
		{
			indexI = -1;
			indexJ = -1;
			curr.id = curveNum;
			curr.ptNum++;
			curr.pts.push_back(pt);
			if (curr.type == 1) {
				setupKnots(curr);
			}
		} else {
			if (curr.type != -1)
			{
				indexI = -1;
				indexJ = -1;
				curveNum++;
				curves.push_back(curr);
				curr.type = -1;
				curr.ptNum = 0;
				curr.res = 10;
				curr.pts.clear();
			}
			if (doPoint || (curr.id == curveNum - 1) )
			{
				for (int i = 0; i < curveNum; ++i)
				{
					Point p;
					for (int j = 0; j < curves[i].ptNum; ++j)
					{
						p = curves[i].pts[j];
						if (fabs(pt.x - p.x) < pixel_size * cptRatio
						        && fabs(pt.y - p.y) < pixel_size * cptRatio)
						{
							selPt = p;
							indexI = i;
							indexJ = j;
							break;
						}
					}
					if (selPt == p)
						break;
				}
			}
		}
		if (addPoint && indexI != -1)
		{
			curves[indexI].ptNum++;
			curves[indexI].pts.insert(curves[indexI].pts.begin() +  indexJ , pt);
			setupKnots(curves[indexI]);
			glutPostRedisplay();
		}

		if (delPoint && indexI != -1)
		{
			curves[indexI].ptNum--;
			curves[indexI].pts.erase(curves[indexI].pts.begin() +  indexJ );
			setupKnots(curves[indexI]);
			glutPostRedisplay();
		}
	}

	glutPostRedisplay();
}

//gets called when the curser moves accross the scene
void motion(int x, int y)
{
	x *= (grid_width * pixel_size) / win_width;
	y = win_height - y;
	y *= (grid_height * pixel_size) / (win_height);

	Point pt;
	pt.x = x / pixel_size;
	pt.y = y / pixel_size;

	if (doPoint && curveNum != 0 && indexI != -1 && indexJ != -1)
	{
		curves[indexI].pts[indexJ] = pt;
		glutPostRedisplay();
		return;
	}

	//redraw the scene after mouse movement
	glutPostRedisplay();
}

void SpecialInput(int key, int x, int y) {
	if (curr.id == curveNum)
	{
		if (key == GLUT_KEY_UP)
			curr.res++;

		if (key == GLUT_KEY_DOWN && curr.res != 1)
			curr.res--;

		if (curr.type == 1)
			setupKnots(curr);

	}

	if (indexI != -1)
	{
		if (key == GLUT_KEY_UP)
			curves[indexI].res++;

		if (key == GLUT_KEY_DOWN && curves[indexI].res != 1)
			curves[indexI].res--;

		if (curves[indexI].type == 1)
			setupKnots(curves[indexI]);
	}


	if (curr.id == curveNum)
	{
		if (key == GLUT_KEY_RIGHT && curr.k != curr.knots.size())
			curr.k++;

		if (key == GLUT_KEY_LEFT && curr.k != 1)
			curr.k--;

		if (curr.type == 1)
			setupKnots(curr);
	}

	if (indexI != -1)
	{
		if (key == GLUT_KEY_RIGHT && curves[indexI].k != curves[indexI].knots.size())
			curves[indexI].k++;

		if (key == GLUT_KEY_LEFT && curves[indexI].k != 1 )
			curves[indexI].k--;
		if (curves[indexI].type == 1)
			setupKnots(curves[indexI]);
	}

	glutPostRedisplay();
}

//gets called when a key is pressed on the keyboard
void key(unsigned char ch, int x, int y)
{
	if (ch == ']')
	{
		curr.stroke++;
		if (indexI != -1)
			curves[indexI].stroke++;
	}
	if (ch == '[')
	{
		if (curr.stroke != 1)
			curr.stroke--;
		if (indexI != -1 && curves[indexI].stroke != 1)
			curves[indexI].stroke--;

	}

	if (ch == 's')
	{
		saveFile();
	}

	if (ch == 'a')
	{
		doPoint ^= true;
	}

	if (ch == 'b')
	{
		curr.type = 0;
		startCurve ^= true;
	}

	if (ch == 'p')
	{
		curr.type = 1;
		startCurve ^= true;
	}

	if (ch == 'i' && (doPoint || curveNum  == curr.id
	                  || curveNum - 1 == curr.id))
	{
		delPoint = false;
		addPoint ^= true;
	}

	if (ch == 'd' && (doPoint || curveNum  == curr.id
	                  || curveNum - 1 == curr.id))
	{
		addPoint = false;
		delPoint ^= true;
	}

	if (startCurve)
	{
		delPoint = false;
		addPoint = false;
	}

	glutPostRedisplay();
}


//checks for any opengl errors in the previous calls and
//outputs if they are present
void check()
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("GLERROR: There was an error %s\n", gluErrorString(err) );
		exit(1);
	}
}