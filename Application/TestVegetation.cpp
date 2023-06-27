#include "TestVegetation.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>

using namespace OpenGLWrapper;

float planeVertices[] = {
	// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

bool leftMousePress = false;
bool rightMousePress = false;
bool firstPress = true;
glm::vec2 lastMousePos = { 0.f,0.f };

Test::TestVegetation::TestVegetation()
	: plane_texture(GL_TEXTURE_2D, "Resource\\Textures\\metal.png"),
	camera(60, 0.1f, 50, static_cast<float>(screen_width) / screen_height)
{
	plane_program.attach_shader(std::make_shared<Shader>(GL_VERTEX_SHADER, "Resource\\Shaders\\plane.vert"));
	plane_program.attach_shader(std::make_shared<Shader>(GL_FRAGMENT_SHADER, "Resource\\Shaders\\plane.frag"));
	plane_program.link();
	grass_program.attach_shader(std::make_shared<Shader>(GL_VERTEX_SHADER, "Resource\\Shaders\\grass.vert"));
	grass_program.attach_shader(std::make_shared<Shader>(GL_FRAGMENT_SHADER, "Resource\\Shaders\\grass.frag"));
	grass_program.attach_shader(std::make_shared<Shader>(GL_GEOMETRY_SHADER, "Resource\\Shaders\\grass.geom"));
	grass_program.attach_shader(std::make_shared<Shader>(GL_TESS_EVALUATION_SHADER, "Resource\\Shaders\\grass.tese"));
	grass_program.link();

	grass_vertices = create_grass_vertices();

	plane_texture.set_wrap_mode(GL_TEXTURE_WRAP_S, GL_REPEAT);
	plane_texture.set_wrap_mode(GL_TEXTURE_WRAP_T, GL_REPEAT);
	plane_texture.generate_mipmap();
	plane_texture.set_minification_filter(GL_LINEAR_MIPMAP_LINEAR);
	plane_texture.set_magification_filter(GL_LINEAR);

	plane_vertex_buffer.allocate_storage(sizeof(planeVertices), planeVertices, GL_DYNAMIC_STORAGE_BIT);

	plane_vertex_array.bind_vertex_buffer(0, plane_vertex_buffer, 0, 5 * sizeof(float));
	plane_vertex_array.bind_vertex_buffer(1, plane_vertex_buffer, 3 * sizeof(float), 5 * sizeof(float));

	plane_vertex_array.set_attrib(0, 3, GL_FLOAT, false, 0);
	plane_vertex_array.set_attrib(1, 2, GL_FLOAT, false, 0);
	plane_vertex_array.bind_attrib(0, 0);
	plane_vertex_array.bind_attrib(1, 1);
	plane_vertex_array.enable_attrib(0);
	plane_vertex_array.enable_attrib(1);

	grass_vertex_buffer.allocate_storage(grass_vertices.size() * sizeof(glm::vec3), &grass_vertices[0], GL_DYNAMIC_STORAGE_BIT);
	grass_vertex_array.bind_vertex_buffer(0, grass_vertex_buffer, 0, 3 * sizeof(float));
	grass_vertex_array.set_attrib(0, 3, GL_FLOAT, false, 0);
	grass_vertex_array.bind_attrib(0, 0);
	grass_vertex_array.enable_attrib(0);

	renderer.enable(GL_DEPTH_TEST);
	renderer.set_clear_color(0.2f, 0.3f, 0.3f, 1.0f);
}

Test::TestVegetation::~TestVegetation()
{
}

void Test::TestVegetation::OnUpdate(double deltaTime)
{
}

void Test::TestVegetation::OnRender()
{
	if (isWireFrame)
		renderer.set_polygon_mode(GL_FRONT_AND_BACK, GL_LINE);
	else
		renderer.set_polygon_mode(GL_FRONT_AND_BACK, GL_FILL);
	renderer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// render plane
	for (GLenum err; (err = glGetError()) != GL_NO_ERROR;)
	{
		std::cout << err << std::endl;
	}
	plane_texture.bind(0);
	plane_program.use();
	plane_program.set_uniform_value("modelMatrix", glm::mat4(1.f));
	plane_program.set_uniform_value("viewMatrix", camera.getViewMatrix());
	plane_program.set_uniform_value("projectionMatrix", camera.getProjectionMatrix());
	plane_program.set_uniform_value("texture0", 0);
	plane_vertex_array.bind();
	renderer.draw_arrays(GL_TRIANGLES, plane_vertex_array, plane_program);

	grass_program.use();
	grass_program.set_uniform_value("layer", layer);
	grass_program.set_uniform_value("topColor", topColor);
	grass_program.set_uniform_value("btnColor", btnColor);
	grass_program.set_uniform_value("width", grassWidth);
	grass_program.set_uniform_value("height", grassHeight);
	grass_program.set_uniform_value("modelMatrix", glm::mat4(1.f));
	grass_program.set_uniform_value("viewMatrix", camera.getViewMatrix());
	grass_program.set_uniform_value("projectionMatrix", camera.getProjectionMatrix());
	grass_program.set_uniform_value("windDirection", windDirection);
	grass_program.set_uniform_value("windIntensity", windIntensity);
	grass_program.set_uniform_value("time", static_cast<float>(glfwGetTime()));
	grass_program.set_patch_para(GL_PATCH_VERTICES, 3);

	float outerLev[] = { intensity, intensity, intensity };
	float innerLev[] = { intensity, intensity };
	grass_program.set_patch_para(GL_PATCH_DEFAULT_OUTER_LEVEL, outerLev);
	grass_program.set_patch_para(GL_PATCH_DEFAULT_INNER_LEVEL, innerLev);

	renderer.draw_arrays(GL_PATCHES, grass_vertex_array, grass_program);
}

void Test::TestVegetation::OnImGuiRender()
{
	ImGui::ColorEdit4("topColor", &topColor[0]);
	ImGui::ColorEdit4("btnColor", &btnColor[0]);
	ImGui::SliderInt("layer", &layer, 2, 10);
	ImGui::SliderFloat("intensity", &intensity, 1.f, 32.f);
	ImGui::SliderFloat("width", &grassWidth, 0.f, 0.05f);
	ImGui::SliderFloat("height", &grassHeight, 0.f, 0.5f);
	ImGui::SliderFloat("windDirection", &windDirection, 0.f, 360.f);
	ImGui::SliderFloat("windIntensity", &windIntensity, 0.f, 60.f);
	ImGui::Checkbox("wireFrame", &isWireFrame);
}

void Test::TestVegetation::OnMouseClick(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			firstPress = !leftMousePress && !rightMousePress;
			rightMousePress = true;
		}
		else if (action == GLFW_RELEASE)
		{
			rightMousePress = false;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			firstPress = !leftMousePress && !rightMousePress;
			leftMousePress = true;
		}
		else if (action == GLFW_RELEASE)
		{
			leftMousePress = false;
		}
	}
}

void Test::TestVegetation::OnMouseMove(double x, double y)
{
	if (leftMousePress || rightMousePress)
	{
		double currentX = x;
		double currentY = y;
		if (firstPress)
		{
			lastMousePos = { currentX, currentY };
			firstPress = false;
		}
		float xOffset = static_cast<float>(currentX - lastMousePos.x);
		float yOffset = static_cast<float>(currentY - lastMousePos.y);
		glm::vec2 mouseMotion = { xOffset / screen_width, yOffset / screen_height };
		lastMousePos = { currentX, currentY };
		if (leftMousePress)
		{
			camera.rotate(mouseMotion);
		}
		if (rightMousePress)
		{
			camera.move(mouseMotion);
		}
	}
}

void Test::TestVegetation::OnMouseScroll(double x, double y)
{
	camera.approach(static_cast<float>(y));
}

void Test::TestVegetation::OnKeyPress(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		camera.setCenter({0.f, 0.f, 0.f});
	}
}

std::vector<glm::vec3> Test::TestVegetation::create_grass_vertices()
{
	std::vector<glm::vec3> res;
	for (int x = -5; x < 5; x++)
	{
		for (int z = -5; z < 5; z++)
		{
			res.emplace_back(glm::vec3{ x, -0.5f, z });
			res.emplace_back(glm::vec3{ x, -0.5f, z + 1 });
			res.emplace_back(glm::vec3{ x + 1, -0.5f, z + 1 });
			res.emplace_back(glm::vec3{ x, -0.5f, z });
			res.emplace_back(glm::vec3{ x + 1, -0.5f, z + 1 });
			res.emplace_back(glm::vec3{ x + 1, -0.5f, z });
		}
	}
	return res;
}
