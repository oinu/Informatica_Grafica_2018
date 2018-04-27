
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
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
	void myRenderCode(double currentTime,glm::vec4 point);
	void myCleanupCode();
	GLuint myRenderProgram;
	GLuint myVAO;
}
namespace MyCube3 {
	GLuint myShaderCompile();
	void myInitCode();
	void myRenderCode(double currentTime,glm::mat4 modelMatrix, glm::vec4 point);
	void myCleanupCode();
	GLuint myRenderProgram;
	GLuint myVAO;
}

static int status = 1;
bool show_test_window = false;
static bool perspective = false;
static float fov, zoom;
static float rot;
static int numRandom = 10;
glm::vec4 points[4];


void GetStatus(int n)
{
	if (n < 1)n = 1;
	else if (n > 3)n = 3;

	status = n;
}

void NewPositionPoints()
{
	srand(time(NULL));
	for (int i = 0; i < 4; i++)
	{
		float x = rand() % numRandom;
		int n = rand();
		if (n % 2 == 0)x = -x;
		x /= 10;

		float y = rand() % numRandom;
		n = rand();
		if (n % 2 == 0)y = -y;
		y /= 10;

		float z = rand() % numRandom;
		n = rand();
		if (n % 2 == 0)z = -z;
		z /= 10;

		points[i] = glm::vec4(x, y, z, 1.0f);
	}
}

void GUI() {
	bool show = true;
	ImGui::Begin("Welcome!", &show, 0);

	// Do your GUI code here....
	{
		const char* listbox_items[] = { "Exercise 1", "Exercise 2 (NONE)", "Exercise 3", "Exercise 4 (NONE)", "Exercise 5 (NONE)", "Exercise 6 (NONE)", "Exercise 7 (NONE)" };
		static int listbox_item_current = -1, listbox_item_current2 = -1;
		ImGui::ListBox("Click on\nany exercise!\n\n(single select)", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 7);
		ImGui::PushItemWidth(-1);
		ImGui::PopItemWidth();
		if (ImGui::Button("Randomize", ImVec2(100, 25)))
		{
			NewPositionPoints();
		}

		// Choose exercise in GUI
		switch (listbox_item_current) {
		case 0:
			status = 1;
			/*if (perspective)
			{
				//numRandom = 10;
				//NewPositionPoints();
				perspective = false;
			}*/
			break;
		case 1:
			break;
		case 2:
			status = 3;
			/*if (ImGui::Button("Orthonormal", ImVec2(100, 25)))
			{
				if (perspective)
				{
					//numRandom = 10;
					//NewPositionPoints();
					perspective = false;
				}
			}
			if (ImGui::Button("Perspective", ImVec2(100, 25)))
			{
				if (!perspective)
				{
					//numRandom = 10;
					//NewPositionPoints();
					perspective = true;
				}
			}*/
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		default:
			break;
		}
	}
	// .........................

	ImGui::End();

	// Example code -- ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

void myInitCode()
{
	NewPositionPoints();
	MyCube::myInitCode();
	MyCube3::myInitCode();
}

void myRenderCode(double currentTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ImGui::Render();

	switch (status)
	{
	case 1:
		for (int i = 0; i < 4; i++)
		{
			MyCube::myRenderCode(currentTime, points[i]);
		}
			
		break;
	case 3:
		rot += 0.01;
		if (rot >= 360.0f)
		{
			rot = 0.0f;
		}
		glm::mat4 matrix;
		matrix = glm::rotate(matrix, rot, glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::scale(matrix, glm::vec3(2.f));
		MyCube3::myRenderCode(currentTime, matrix, points[0]);

		matrix = glm::mat4(1.0f);
		matrix = glm::rotate(matrix, -rot, glm::vec3(0.0f, 1.0f, 1.0f));
		matrix = glm::scale(matrix, glm::vec3(2.f));
		MyCube3::myRenderCode(currentTime, matrix, points[1]);

		matrix = glm::mat4(1.0f);
		matrix = glm::rotate(matrix, rot, glm::vec3(1.0f, 0.0f, 0.0f));
		matrix = glm::scale(matrix, glm::vec3(2.f));
		MyCube3::myRenderCode(currentTime, matrix, points[2]);

		matrix = glm::mat4(1.0f);
		matrix = glm::rotate(matrix, -rot, glm::vec3(1.0f, 1.0f, 0.0f));
		matrix = glm::scale(matrix, glm::vec3(2.f));
		MyCube3::myRenderCode(currentTime, matrix, points[3]);

		break;
	}
}
void myCleanupCode()
{
	MyCube::myCleanupCode();
	MyCube3::myCleanupCode();
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
				uniform mat4 viewMatrix;								\n\
				uniform vec4 cube0;\n\
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
						gl_Position = viewMatrix * rotationMatrix* cara1[j] + position;		\n\
						gl_PrimitiveID=0;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = viewMatrix * rotationMatrix *cara2[j]+ position;		\n\
						gl_PrimitiveID=1;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = viewMatrix * rotationMatrix * cara3[j] + position;		\n\
						gl_PrimitiveID=2;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = viewMatrix * rotationMatrix * cara4[j]+ position;		\n\
						gl_PrimitiveID=3;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = viewMatrix * rotationMatrix*cara5[j]+ position;		\n\
						gl_PrimitiveID=4;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = viewMatrix * rotationMatrix*cara6[j] + position;		\n\
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
			}															\n\
			"
	};

	static const GLchar* fragment_shade_source[] =
	{
		"#version 330																	\n\
			out vec4 color;\n\
			void main() {																	\n\
				const vec4 colors[8] = vec4[8](vec4(0, 1, 0, 1.0),							\n\
										       vec4(0, 0, 1, 1.0),					\n\
										       vec4(1, 1, 0.0, 1.0),						\n\
										       vec4(0.5, 0, 0.5, 1.0),						\n\
										       vec4(1, 0, 0, 1.0),							\n\
										       vec4(0.5, 0.5, 0.5, 1.0),						\n\
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
	}

	//4. render function
	static float rot;
	void myRenderCode(double currentTime,glm::vec4 point)
	{
		glm::mat4 matrix;
		rot += 0.01;
		if (rot >= 360.0f)
		{
			rot = 0.0f;
		}
		matrix = glm::rotate(matrix, rot, glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::scale(matrix, glm::vec3(0.25f));
		RV::_MVP = glm::mat4(1.f);

		glUseProgram(myRenderProgram);

		glUniform4f(glGetUniformLocation(myRenderProgram, "cube0"), point.x, point.y, point.z, point.w);

		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(matrix));
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	//5. cleanup function
	void myCleanupCode()
	{
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}
}

namespace MyCube3
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
				uniform mat4 viewMatrix;								\n\
				uniform vec4 cube0;\n\
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
						gl_Position = viewMatrix * rotationMatrix* cara1[j] + position;		\n\
						gl_PrimitiveID=0;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = viewMatrix * rotationMatrix *cara2[j]+ position;		\n\
						gl_PrimitiveID=1;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = viewMatrix * rotationMatrix * cara3[j] + position;		\n\
						gl_PrimitiveID=2;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = viewMatrix * rotationMatrix * cara4[j]+ position;		\n\
						gl_PrimitiveID=3;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = viewMatrix * rotationMatrix*cara5[j]+ position;		\n\
						gl_PrimitiveID=4;								\n\
						EmitVertex();									\n\
						}												\n\
						EndPrimitive();									\n\
																		\n\
																		\n\
						for (int j = 0; j < 4;j++)				\n\
						{										\n\
						gl_Position = viewMatrix * rotationMatrix*cara6[j] + position;		\n\
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
			}															\n\
			"
	};

	static const GLchar* fragment_shade_source[] =
	{
		"#version 330																	\n\
			out vec4 color;\n\
			void main() {																	\n\
				const vec4 colors[8] = vec4[8](vec4(0, 1, 0, 1.0),							\n\
										       vec4(0, 0, 1, 1.0),					\n\
										       vec4(1, 1, 0.0, 1.0),						\n\
										       vec4(0.5, 0, 0.5, 1.0),						\n\
										       vec4(1, 0, 0, 1.0),							\n\
										       vec4(0.5, 0.5, 0.5, 1.0),						\n\
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
	}

	//4. render function
	void myRenderCode(double currentTime, glm::mat4 modelMatrix,glm::vec4 point)
	{
		if(perspective)RV::_projection = glm::perspective(glm::radians(65.0f), 800.f / 600.f, 1.0f, 100.0f);
		else RV::_projection = glm::ortho(-(float)800.f / 50.f, (float)800.f / 50.f, -(float)600.f / 50.f, (float)600.f / 50.f, 1.0f, 100.f);
		glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 10.0f);
		RV::_modelView = glm::lookAt(cameraPos, glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f));


		RV::_MVP = RV::_projection * RV::_modelView;
		

		glUseProgram(myRenderProgram);
		glUniform4f(glGetUniformLocation(myRenderProgram, "cube0"), point.x, point.y, point.z, point.w);
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(RV::_MVP));
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	//5. cleanup function
	void myCleanupCode()
	{
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}
}