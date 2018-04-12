
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <cstdio>
#include <cassert>

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
namespace MyObject {
	GLuint myShaderCompile();
	void myInitCode();
	void myRenderCode(double currentTime);
	void myCleanupCode();
	GLuint myRenderProgram;
	GLuint myVAO;
	glm::vec4 points[];
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
	Box::setupCube();
	Cube::setupCube();
}

void myRenderCode(double currentTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//The matrix i use for all the objects of the scene
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	//Cub1
	glm::vec3 centerOfCube = glm::vec3(0.0f, 3.0f, 2.0f);
	modelMatrix = glm::translate(modelMatrix, centerOfCube);
	Cube::updateCube(modelMatrix);
	Cube::drawCube();

	//FIRST ROW
	//Left Cube
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0f, 1.0f, 4.0f));
	Cube::updateCube(modelMatrix);
	Cube::draw2Cube();

	//Center Cube
	modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0f, 0.0f, 0.0f));
	Cube::updateCube(modelMatrix);
	Cube::draw2Cube();

	//Right Cube
	modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0f, 0.0f, 0.0f));
	Cube::updateCube(modelMatrix);
	Cube::draw2Cube();

	//MIDDLE ROW
	//Left Cube
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0f, 1.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f));
	Cube::updateCube(modelMatrix);
	Cube::draw2Cube();

	//Center Cube
	modelMatrix = glm::translate(modelMatrix, glm::vec3(2.0f, 0.0f, 0.0f));
	Cube::updateCube(modelMatrix);
	Cube::draw2Cube();

	//Right Cube
	modelMatrix = glm::translate(modelMatrix, glm::vec3(2.0f, 0.0f, 0.0f));
	Cube::updateCube(modelMatrix);
	Cube::draw2Cube();

	//LAST ROW
	//Left Cube
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-4.0f, 1.0f, -4.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
	Cube::updateCube(modelMatrix);
	Cube::draw2Cube();

	//Center Cube
	modelMatrix = glm::translate(modelMatrix, glm::vec3(2.0f, 0.0f, 0.0f));
	Cube::updateCube(modelMatrix);
	Cube::draw2Cube();

	//Right Cube
	modelMatrix = glm::translate(modelMatrix, glm::vec3(2.0f, 0.0f, 0.0f));
	Cube::updateCube(modelMatrix);
	Cube::draw2Cube();

	//CAMERA
	//HORIZONTAL TRAVELLING
	if (status == 1)
	{
		static float x;
		x += sinf((float)currentTime)*0.1;
		RV::_modelView = glm::mat4(1.f);
		RV::_modelView = glm::translate(RV::_modelView, glm::vec3(2.0f-x, -2.0f, -11.0f));
		RV::_modelView = glm::rotate(RV::_modelView, glm::radians(20.f), glm::vec3(1.f, 0.f, 0.f));
	}
	

	//THE DOLLY ZOOM
	if (status == 3)
	{
		zoom = 0.0f + sinf((float)currentTime);
	}
	//OPEN/CLOSE FOV OR DOLLY ZOOM
	if (status == 2 || status == 3)
	{
		fov = 65.0f + sinf((float)currentTime) * 6.2;

		RV::_projection = glm::perspective(glm::radians(fov), 800.f / 600.f, 1.0f, 50.0f);

		glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 10.0f - zoom);
		RV::_modelView = glm::lookAt(cameraPos,centerOfCube, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	
	RV::_MVP = RV::_projection * RV::_modelView;

	Box::drawCube();
	ImGui::Render();
}
void myCleanupCode()
{
	Box::cleanupCube();
	Cube::cleanupCube();
}

namespace MyObject
{
	static const GLchar* vertex_shader_source[]=
	{
		""
	};

	static const GLchar* geom_shader_source[]=
	{
		""
	};
	static const GLchar* fragment_shader_source[] =
	{
		""
	};

	GLuint myShaderCompile()
	{
		GLuint vertex_shader;
		GLuint geom_shader;
		GLuint fragment_shader;
		GLuint program;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom_shader, 1, geom_shader_source, NULL);
		glCompileShader(geom_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
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

	void myInitCode()
	{
		myRenderProgram = myShaderCompile();
		glCreateVertexArrays(1, &myVAO);
		glBindVertexArray(myVAO);

		//Generar la semilla random
	}

	void myRenderCode(double currentTime)
	{
		glUseProgram(myRenderProgram);

		//glm::mat4 matrix;
		//glUniformMatrix4fv(glGetUniformLocation(myRenderProgram, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(matrix));
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	
	void myCleanupCode()
	{
		glDeleteVertexArrays(1, &myVAO);
		glDeleteProgram(myRenderProgram);
	}
}
