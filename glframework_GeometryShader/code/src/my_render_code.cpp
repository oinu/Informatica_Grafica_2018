
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <cstdio>
#include <time.h> 
#include <cassert>
#include <iostream>

#include "GL_framework.h"

namespace ImGui {
	void Render();
}
namespace Box {
	void setupCube();
	void cleanupCube();
	void drawCube();
}
namespace Cube {
	void setupCube();
	void cleanupCube();
	void updateCube(const glm::mat4& transform);
	void drawCube();
	void draw2Cube();
}
namespace RenderVars {
	extern const float FOV;
	extern const float zNear;
	extern const float zFar;

	extern glm::mat4 _projection;
	extern glm::mat4 _modelView;
	extern glm::mat4 _MVP;
	extern glm::mat4 _inv_modelview;
	extern glm::vec4 _cameraPoint;

	extern float panv[3];
	extern float rota[2];
}
namespace RV = RenderVars;
namespace MyCube {
	GLuint myShaderCompile();
	void myInitCode();
	void myRenderCode(double currentTime);
	void myCleanupCode();
	GLuint myRenderProgram;
	GLuint myVAO;
	glm::vec4 points[4];
}

static int status = 1;
static float fov, zoom;

// To change the efect
// 1. Travelling
// 2. Open/Close FOV
// 3. Dolly Effect
void GetStatus(int n)
{
	if (n < 1)n = 1;
	else if (n > 3)n = 3;

	status = n;
	
	//Restart de valors of the variables
	fov = 65.0f;
	zoom = 0.0f;
}
void myInitCode()
{
	MyCube::myInitCode();
}

void myRenderCode(double currentTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui::Render();
	MyCube::myRenderCode(currentTime);
}
void myCleanupCode()
{
	MyCube::myCleanupCode();
}

namespace MyCube
{//1. define the shader source code
	static const GLchar* vertex_shade_source[] =
	{
		"#version 330														\n\
																			\n\
		void main() {\n\
		 gl_Position =vec4(0.0);						\n\
		}"
	};

	static const GLchar* geom_shader_source[] =
	{
		"#version 330												\n\
				layout(triangles) in;									\n\
				uniform mat4 rotationMatrix;								\n\
				uniform vec4 cube0;\n\
				uniform vec4 cube1;\n\
				uniform vec4 cube2;\n\
				uniform vec4 cube3;\n\
				layout(triangle_strip, max_vertices = 96) out;			\n\
																		\n\
				void Cube(vec4 position)												\n\
				{														\n\
					vec4 cara1[4] = vec4[4]							\n\
						(vec4(0.5, -0.5, 0.5, 1.0),			\n\
						vec4(0.5, 0.5, 0.5, 1.0),			\n\
						vec4(-0.5,-0.5, 0.5, 1.0),			\n\
						vec4(-0.5, 0.5, 0.5, 1.0));			\n\
															\n\
															\n\
					vec4 cara2[4] = vec4[4]							\n\
						(vec4(-0.5, -0.5, 0.5, 1.0),			\n\
						vec4(-0.5, 0.5, 0.5, 1.0),			\n\
						vec4(-0.5,-0.5, -0.5, 1.0),			\n\
						vec4(-0.5, 0.5, -0.5, 1.0));			\n\
															\n\
															\n\
					vec4 cara3[4] = vec4[4]							\n\
						(vec4(-0.5, -0.5, -0.5, 1.0),			\n\
						vec4(-0.5, 0.5, -0.5, 1.0),			\n\
						vec4(0.5,-0.5, -0.5, 1.0),			\n\
						vec4(0.5, 0.5, -0.5, 1.0));			\n\
															\n\
															\n\
					vec4 cara4[4] = vec4[4]							\n\
						(vec4(0.5, -0.5, -0.5, 1.0),			\n\
						vec4(0.5, 0.5, -0.5, 1.0),			\n\
						vec4(0.5,-0.5, 0.5, 1.0),			\n\
						vec4(0.5, 0.5, 0.5, 1.0));			\n\
															\n\
															\n\
					vec4 cara5[4] = vec4[4]							\n\
						(vec4(0.5, 0.5, 0.5, 1.0),			\n\
						vec4(0.5, 0.5, -0.5, 1.0),			\n\
						vec4(-0.5,0.5, 0.5, 1.0),			\n\
						vec4(-0.5, 0.5, -0.5, 1.0));			\n\
															\n\
															\n\
					vec4 cara6[4] = vec4[4]							\n\
						(vec4(0.5, -0.5, -0.5, 1.0),			\n\
						vec4(0.5, -0.5, 0.5, 1.0),			\n\
						vec4(-0.5,-0.5, -0.5, 1.0),			\n\
						vec4(-0.5, -0.5, 0.5, 1.0));			\n\
															\n\
															\n\
								                              \n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = cara1[j] * rotationMatrix + position;		\n\
						gl_PrimitiveID=0;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = cara2[j] * rotationMatrix+ position;		\n\
						gl_PrimitiveID=1;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = cara3[j] * rotationMatrix+ position;		\n\
						gl_PrimitiveID=2;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = cara4[j] * rotationMatrix+ position;		\n\
						gl_PrimitiveID=3;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = cara5[j] * rotationMatrix+ position;		\n\
						gl_PrimitiveID=4;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = cara6[j] * rotationMatrix+ position;		\n\
						gl_PrimitiveID=5;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
																		\n\
			}\n\
																		\n\
																		\n\
			void main(){												\n\
			Cube(cube0);												\n\
			Cube(cube1);												\n\
			Cube(cube2);												\n\
			Cube(cube3);												\n\
			}															\n\
			"
	};

	static const GLchar* fragment_shade_source[] =
	{
		"#version 330																	\n\
			out vec4 color;\n\
			void main() {																	\n\
				const vec4 colors[8] = vec4[8](vec4(0, 1, 0, 1.0),							\n\
										       vec4(0.25, 0.25, 0.5, 1.0),					\n\
										       vec4(1, 0.25, 0.5, 1.0),						\n\
										       vec4(0.25, 0, 0, 1.0),						\n\
										       vec4(1, 0, 0, 1.0),							\n\
										       vec4(0.5, 0, 0.5, 1.0),						\n\
											   vec4(0, 0, 1, 1.0),							\n\
											   vec4(0, 0.25, 0, 1.0));						\n\
				color = colors[gl_PrimitiveID];												\n\
			}"
	};

	//2. compile and link the shaders
	GLuint myShaderCompile()
	{
		GLuint vertex_shader;
		GLuint geom_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shade_source, NULL);
		glCompileShader(vertex_shader);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shade_source, NULL);
		glCompileShader(fragment_shader);


		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, geom_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(geom_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	//3. init function
	void myInitCode()
	{
		myRenderProgram = myShaderCompile();
		glCreateVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);
		srand(time(NULL));
		for (int i = 0; i < 4; i++)
		{
			float x = rand()%10;
			int n = rand();
			if (n % 2 == 0)x = -x;
			x /= 10;

			float y = rand() % 10;
			n = rand();
			if (n % 2 == 0)y = -y;
			y /= 10;

			float z = rand() % 10;
			n = rand();
			if (n % 2 == 0)z = -z;
			z /= 10;

			points[i] = glm::vec4(x,y,z,1.0f);
		}
			
	}

	//4. render function
	static float rot;
	glm::mat4 myMVP;
	void myRenderCode(double currentTime)
	{
		glm::mat4 matrix;
		rot += 0.01;
		if (rot >= 360.0f)
		{
			rot = 0.0f;
		}

		matrix = glm::rotate(matrix, rot, glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, RV::panv[1]+4.5f, glm::vec3(1.0f, 0.0f, 0.0f));
		matrix = glm::scale(matrix, glm::vec3(0.1));
		glUseProgram(myRenderProgram);

		glUniform4f(glGetUniformLocation(myRenderProgram, "cube0"), points[0].x, points[0].y, points[0].z, points[0].w);
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(matrix));
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUniform4f(glGetUniformLocation(myRenderProgram, "cube1"), points[1].x, points[1].y, points[1].z, points[1].w);
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(matrix));
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUniform4f(glGetUniformLocation(myRenderProgram, "cube2"), points[2].x, points[2].y, points[2].z, points[2].w);
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(matrix));
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUniform4f(glGetUniformLocation(myRenderProgram, "cube3"), points[3].x, points[3].y, points[3].z, points[3].w);
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(matrix));
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	//5. cleanup function
	void myCleanupCode()
	{
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}
}
