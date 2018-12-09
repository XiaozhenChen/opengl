#include <GL/glew.h>  //
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>6
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
     x;\
     ASSERT(GLLogCall(#x,__FILE__,__LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[Opengl Error] (" << error <<")"<< function<<
			" "<<file<<":"<<line<< std::endl;
		return false;
	}
	return true;
}
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;

};
static  ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};


	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	if (stream.is_open())
		std::cout << "File ./" << filepath << " is open.\n";
	else
		std::cout << "Error opening " << filepath << ".\n";


	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) 
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
		
			else if (line.find("fragment") != std::string::npos)
				//set mode to fragment
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}

		
	}
	
	return { ss[0].str(),ss[1].str() };
}

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


	unsigned int buffer; //opengl state machine, 每个产生的东西都有对应的地址
	glGenBuffers(1, &buffer); //所以需要绑定
	glBindBuffer(GL_ARRAY_BUFFER, buffer);//绑定一个容器
	glBufferData(GL_ARRAY_BUFFER,4*2*sizeof(float),position,GL_STATIC_DRAW);                                  //声明这个容器的大小

	//why must unsigned?
	unsigned int ibo; //opengl state machine, 每个产生的东西都有对应的地址
	glGenBuffers(1, &ibo); //所以需要绑定
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);//绑定一个容器
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6  * sizeof(unsigned int), indices, GL_STATIC_DRAW);                                  //声明这个容器的大小


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*2,0);

	ShaderProgramSource source = ParseShader("Basic.shader");  // notice the file path
	//文件夹后面才有 ‘/’ 这个是错误的  /res/shaders/Basic.shader 读不到
	std::cout << "vertex" << std::endl;
	//std::cout << source.VertexSource<< std::endl;
	std::cout << "fragment" << std::endl;
	//std::cout << source.FragmentSource << std::endl;

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);
	GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));
	
	GLCall(glBindVertexArray(0));
	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


	float r = 0.0f;
	float increment = 0.05f;
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


		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
		GLCall(glBindVertexArray(vao));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
		


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		
	//	glDrawArrays(GL_TRIANGLES,0,6);// change the points number to braw
		GLCall(glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,nullptr));
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
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}