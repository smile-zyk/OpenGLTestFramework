#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define IMGUI_ENABLE_FREETYPE
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "test_base.h"
#include "test_menu.h"
#include "test_2d.h"
#include "test_vegetation.h"

using namespace glinterface;

//extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

glm::vec2 scrollOffset{0.f, 0.f};

int screen_width = 0;
int screen_height = 0;

Test::TestBase* current_test = nullptr;

void OnWindowResize(GLFWwindow* window, int width, int height);
void OnMouseClick(GLFWwindow* window, int button, int action, int mods);
void OnMouseMove(GLFWwindow* window, double x, double y);
void OnMouseScroll(GLFWwindow* window, double x, double y);
void OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods);

void initImgui(GLFWwindow* window)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    io.Fonts->AddFontFromFileTTF("..\\..\\Application\\Resource\\Fonts\\segoeui.ttf", 30.0f);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/GLInterface backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

int main()
{
    // glfw init and set opengl version and profile
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // create glfw window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenglLab", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // set window context to current thread main context
    glfwMakeContextCurrent(window);

    // load opengl function address by glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	std::cout << glGetString(GL_VERSION) << std::endl;

	glfwGetWindowSize(window, &screen_width, &screen_height);

	{
		IMGUI_CHECKVERSION();
		// init imgui
		initImgui(window);

		Test::TestMenu* test_menu = new Test::TestMenu(current_test);
		current_test = test_menu;

		test_menu->RegisterTest<Test::TestVegetation>("Vegetation Demo");
		test_menu->RegisterTest<Test::Test2D>("2d Rendering Demo");

		glfwSetFramebufferSizeCallback(window, OnWindowResize);
		glfwSetScrollCallback(window, OnMouseScroll);
		glfwSetCursorPosCallback(window, OnMouseMove);
		glfwSetMouseButtonCallback(window, OnMouseClick);
		glfwSetKeyCallback(window, OnKeyPress);
		double last_frame_time = glfwGetTime();
		while (!glfwWindowShouldClose(window))
		{
			double new_frame_time = glfwGetTime();
			double frame_duration_time = new_frame_time - last_frame_time;
			last_frame_time = new_frame_time;
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			if (current_test)
			{
				current_test->OnUpdate(frame_duration_time);
				current_test->OnRender();
				ImGui::Begin("Test");
				ImGui::LabelText("", "fps: %lf", 1. / frame_duration_time);
				if (current_test != test_menu && ImGui::Button("Back to menu"))
				{
					delete current_test;
					current_test = test_menu;
				}
				current_test->OnImGuiRender();
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			// double buffer
			glfwSwapBuffers(window);

			// check events
			glfwPollEvents();
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void OnWindowResize(GLFWwindow* window, int width, int height)
{
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, width, height);
	if(current_test != nullptr)
	{
		current_test->OnWindowResize(width, height);
	}
}

void OnMouseClick(GLFWwindow* window, int button, int action, int mods)
{
	if (current_test != nullptr)
	{
		current_test->OnMouseClick(button, action, mods);
	}
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

void OnMouseMove(GLFWwindow* window, double x, double y)
{
	if (current_test != nullptr)
	{
		current_test->OnMouseMove(x, y);
	}
	ImGui_ImplGlfw_CursorPosCallback(window, x, y);
}

void OnMouseScroll(GLFWwindow* window, double x, double y)
{
	if (current_test != nullptr)
	{
		current_test->OnMouseScroll(x, y);
	}
	ImGui_ImplGlfw_ScrollCallback(window, x, y);
}

void OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (current_test != nullptr)
	{
		current_test->OnKeyPress(key, scancode, action, mods);
	}
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}
