#include <windows.h>
#include <GL\glew.h>
#include <SDL2\SDL.h>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <cstdio>

#include "GL_framework.h"


/*extern void PhysicsInit();
extern void PhysicsUpdate(float dt);
extern void PhysicsCleanup();*/
extern void GUI();

extern void GLmousecb(MouseEvent ev);
extern void GLResize(int width, int height);
extern void GLinit(int width, int height);
extern void GLcleanup();
extern void GLrender(double currentTime);


extern void myRenderCode(double currentTime);
extern void myCleanupCode(void);
extern void myInitCode(void);
extern void GetStatus(int n);

//////
namespace {
	const int expected_fps = 30;
	const double expected_frametime = 1.0 / expected_fps;
	const uint32_t expected_frametime_ms = (uint32_t) (1e3 * expected_frametime);
	uint32_t prev_frametimestamp = 0;
	uint32_t curr_frametimestamp = 0;

	void waitforFrameEnd() {
		curr_frametimestamp = SDL_GetTicks();
		DWORD wait = 0;
		if ((curr_frametimestamp - prev_frametimestamp) < expected_frametime_ms) {
			wait = expected_frametime_ms - (curr_frametimestamp - prev_frametimestamp);
		}
		if(wait > 0) {
			Sleep(wait);
		}
		prev_frametimestamp = SDL_GetTicks();
	}
}

int main(int argc, char** argv) {
	//Init GLFW
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		SDL_Quit();
		return -1;
	}
	// Create window
	SDL_Window *mainwindow;
	SDL_GLContext maincontext;

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	mainwindow = SDL_CreateWindow("GL_framework", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (!mainwindow) { /* Die if creation failed */
			SDL_Log("Couldn't create SDL window: %s", SDL_GetError());
			SDL_Quit();
			return -1;
		}

	/* Create our opengl context and attach it to our window */
	maincontext = SDL_GL_CreateContext(mainwindow);

	// Init GLEW
	GLenum err = glewInit();
	if(GLEW_OK != err) {
		SDL_Log("Glew error: %s\n", glewGetErrorString(err));
	}
	SDL_Log("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	// Disable V-Sync
	SDL_GL_SetSwapInterval(0);

	int display_w, display_h;
	SDL_GL_GetDrawableSize(mainwindow, &display_w, &display_h);
	// Init scene
	GLinit(display_w, display_h);
	//PhysicsInit();

	//S'HA DE CREAR EN EL MY_RENDER_CODE
	//myInitCode();

	
	// Setup ImGui binding
	ImGui_ImplSdlGL3_Init(mainwindow);



	bool quit_app = false;
	while (!quit_app) {
		SDL_Event eve;
		while (SDL_PollEvent(&eve)) {
			ImGui_ImplSdlGL3_ProcessEvent(&eve);
			switch (eve.type) {
			case SDL_WINDOWEVENT:
				if (eve.window.event == SDL_WINDOWEVENT_RESIZED) {
					GLResize(eve.window.data1, eve.window.data2);
				}
				break;
			case SDL_KEYDOWN:
				if (eve.key.keysym.scancode == SDL_SCANCODE_1)
				{
					printf("Exercise 1: Travelling\n");
					GetStatus(1);
				}
				else if (eve.key.keysym.scancode == SDL_SCANCODE_2)
				{
					printf("Exercise 2: Open and Close Field Of View\n");
					GetStatus(2);
				}
				else if (eve.key.keysym.scancode == SDL_SCANCODE_3)
				{
					printf("Exercise 3: Dolly Efect\n");
					GetStatus(3);
				}
				else if (eve.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					quit_app = true;
				}
				break;
			case SDL_QUIT:
				quit_app = true;
				break;
			}
		}
		ImGui_ImplSdlGL3_NewFrame(mainwindow);

		ImGuiIO& io = ImGui::GetIO();
		
		
		GUI();
		//PhysicsUpdate((float)expected_frametime);
		if(!io.WantCaptureMouse) {
			MouseEvent ev = {io.MousePos.x, io.MousePos.y, 
				(io.MouseDown[0] ? MouseEvent::Button::Left : 
				(io.MouseDown[1] ? MouseEvent::Button::Right :
				(io.MouseDown[2] ? MouseEvent::Button::Middle :
				MouseEvent::Button::None)))};
			GLmousecb(ev);
		}


		double currentTime = (double)SDL_GetTicks() / 1000.0;
		GLrender(currentTime);
		
		//S'HA DE CREAR EN EL MY_RENDER_CODE
		//double currentTime = (double) SDL_GetTicks() / 1000.0;
		//myRenderCode(currentTime);
		


		SDL_GL_SwapWindow(mainwindow);
		waitforFrameEnd();
	}

	//S'HA DE CREAR EN EL MY_RENDER_CODE
	//myCleanupCode();

	GLcleanup();

	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(maincontext);
	SDL_DestroyWindow(mainwindow);
	SDL_Quit();
	return 0;
}