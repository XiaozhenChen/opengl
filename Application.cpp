#include <GL/glew.h>  //
#include <GLFW/glfw3.h>
#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source) {

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char*  message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< "shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
 
	std::cout << "Success compile " <<
		(type == GL_VERTEX_SHADER ? "vertex" : "fragment")
		<< "shader!" << std::endl;

	return id;

}


static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program,vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}



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

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*2,0);

	std::string vertexShader =
		"#version 330 core\n"
		"layout(location = 0) in vec4 position;"
		"void main()\n"
		"{\n"
		" gl_Position = position;\n"
		"}\n";

	std::string fragmentShader =
		"#version 330 core\n"
		"layout(location = 0) out vec4 color;"
		"void main()\n"
		"{\n"
		" color = vec4(1.0,0.0,0.0,1.0);\n"
		"}\n";
		
	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);
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
		
		glDrawArrays(GL_TRIANGLES,0,3);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}