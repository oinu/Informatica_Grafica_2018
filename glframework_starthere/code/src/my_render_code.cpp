
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
