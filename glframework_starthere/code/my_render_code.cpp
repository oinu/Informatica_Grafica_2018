
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <cstdio>
#include <cassert>

#include "GL_framework.h"


namespace MyRender
{
	GLuint renderProgram;
	GLuint VAO;

	GLuint myShaderCompile();
	void myInitCode();
	void myRenderCode(double currentTime);
	void myCleanupCode();
}

namespace MyRender
{
	static const GLchar* vertex_shader_source[]=
	{
		"#version 330\n\
		void main(){\n\
		const vec4 verticles[3]=vec4[3](vec4(0.25,0.25,0.5,1.0),\n\
			vec4(-0.25,-0.25,0.5,1.0),\n\
			vec4(0.25,-0.25,0.5,1.0)));\n\
		gl_Postion=verticles[gl_VertexID];\n\
	}"
	};

	static const GLchar* fragment_shader_source[]=
	{
		"#version 330\n\
		out color;\n\
		void main(){\n\
		color=vec4(0.0,0.8,1.0,1.0);\n\
		}"
	};
	GLuint myShaderCompile()
	{
		GLuint program;

		return program=0;
	}

	void myInitCode()
	{
		renderProgram = myShaderCompile();
		glCreateVertexArrays(1,&VAO);
		glBindVertexArray(VAO);
	}

	void myRenderCode(double currentime)
	{
		
	}
	void myCleanupCode()
	{

	}
}