#include <GL/glew.h>  
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"




int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE );

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK)   // #先有window 才能init
		std::cout << "Error glew_ok" << std::endl;
	
	float position[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f,
	
	};

	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	VertexArray va;

	VertexBuffer vb(position, 4 * 2 * sizeof(float));//data size
	
	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);
	
	IndexBuffer ib(indices,6);//data count

	Shader shader("Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

	va.UnBind();

	vb.UnBind();
	ib.UnBind();
		shader.Unbind();
	
	
		Renderer renderer;

	float r = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) 
	{
		renderer.Clear();
		shader.Bind();
		shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
		
		
		renderer.Draw(va, ib, shader);

	//	glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		
	//	glDrawArrays(GL_TRIANGLES,0,6);// change the points number to braw
	//	GLCall(glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,nullptr));
		if (r > 1.0f)
			increment = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;
		r += increment;

	/* Swap front and back buffers */

		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
//	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}