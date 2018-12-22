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

#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "src\vendor\imgui\imgui.h"
#include "src\vendor\imgui\imgui_impl_glfw_gl3.h"

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
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
	
	std::cout << glGetString(GL_VERSION) << std::endl;
	
		float positions[] = {
			100.0f,100.0f,0.0f,0.0f,
			200.0f,100.0f,1.0f,0.0f,
			200.0f, 200.0f,1.0f,1.0f,
			100.0f,200.0f,0.0f,1.0f

		};
	



	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));

	//unsigned int vao;
	//GLCall(glGenVertexArrays(1, &vao));
	//GLCall(glBindVertexArray(vao));

	VertexArray va;

	VertexBuffer vb(positions, 4 * 4 * sizeof(float));//data size
	
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);
	
	IndexBuffer ib(indices,6);//data count

	glm::mat4 proj = glm::ortho(0.0f,960.0f,0.0f,540.0f,-1.0f,1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f) , glm::vec3(-100, 0, 0));
	
	Shader shader("Basic.shader");


	shader.Bind();
	shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

	

	Texture texture("res/texture/cxz.png");
	texture.Bind();

	shader.SetUniform1i("u_Texture", 0);

	va.UnBind();
	vb.UnBind();
	ib.UnBind();
		shader.Unbind();
	
	
		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		glm::vec3 translation(200, 200, 0);

		//bool show_demo_window = true;
		//bool show_another_window = false;
		//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	float r = 0.0f;
	float increment = 0.05f;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) 
	{
		renderer.Clear();

		ImGui_ImplGlfwGL3_NewFrame();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);

		glm::mat4 mvp = proj * view * model;



		shader.Bind();
		shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
		
		shader.SetUniformMat4f("u_MVP", mvp);
		
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

		{
		//	static float f = 0.0f;
		//	static int counter = 0;
		//	ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
			ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


	/* Swap front and back buffers */

		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

//	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}