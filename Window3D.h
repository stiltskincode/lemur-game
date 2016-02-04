class Window3D
{
public:
	void run();
protected:
	void display();
	void elements();
};


void Window3D::elements()
{
		glClearColor(200.0f/255.0f, 230.0f/255.0f, 1.0f, 0.0);
		glEnable(GL_DEPTH_TEST);
	    glShadeModel(GL_SMOOTH);

	    glViewport(0, 0, windowWidth,windowHeigth);

	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity(); gluPerspective(45,4.0f/3.0f,1,10000);

	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
}

void Window3D::run()
{
	elements();
	display();
}