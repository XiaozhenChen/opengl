#include <GL/glew.h>  //
#include <GLFW/glfw3.h>
#include <iostream>
int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)   // #先有window 才能init
		std::cout << "Error glew_ok" << std::endl;
	
	float position[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	unsigned int buffer; //opengl state machine, 每个产生的东西都有对应的地址
	glGenBuffers(1, &buffer); //所以需要绑定
	glBindBuffer(GL_ARRAY_BUFFER, buffer);//绑定一个容器
	glBufferData(GL_ARRAY_BUFFER,6*sizeof(float),position,GL_STATIC_DRAW);                                  //声明这个容器的大小

	glEnableVertexAttrib
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*2,0);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) 
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		/*glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.0f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();

		*/
		
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}