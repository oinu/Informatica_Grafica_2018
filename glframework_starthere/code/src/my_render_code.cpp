
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

void myInitCode()
{
	Box::setupCube();
	Cube::setupCube();
}
void myRenderCode(double currentTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	static float x, x2;

	//Cub1
	x += 0.1f;
	if (x >= 10.0f)x = 1.0f;
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x, 2.0f, 0.0f));
	Cube::updateCube(modelMatrix);
	Cube::drawCube();

	//Cub2
	modelMatrix = glm::mat4(1.0f);
	x2 -= 0.1f;
	if (x2 <= -10.0f)
	{
		x2 = 0.0f;
	}
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 2.0f, 0.0f));
	Cube::updateCube(modelMatrix);
	Cube::draw2Cube();

	//Camera
	/*RV::panv[0] += 0.1f;
	if (RV::panv[0] >= 10.0f)RV::panv[0] = 0.0f;
	RV::_modelView = glm::mat4(1.f);
	RV::_modelView = glm::translate(RV::_modelView, glm::vec3(RV::panv[0], RV::panv[1], RV::panv[2]));
	RV::_modelView = glm::rotate(RV::_modelView, RV::rota[1], glm::vec3(1.f, 0.f, 0.f));
	RV::_modelView = glm::rotate(RV::_modelView, RV::rota[0], glm::vec3(0.f, 1.f, 0.f));*/

	glm::vec3 c = glm::vec3(0.0f, 5.0f, 10.0f);
	RV::_modelView = glm::lookAt(c, glm::vec3(x, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	RV::_MVP = RV::_projection * RV::_modelView;

	Box::drawCube();

	ImGui::Render();
}
void myCleanupCode()
{
	Box::cleanupCube();
	Cube::cleanupCube();
}
