#include "test_menu.h"
#include <freetype-gl/freetype-gl.h>
#include <imgui.h>

Test::TestMenu::TestMenu(TestBase*& _test) : current_test(_test)
{
	gl_interface_.set_clear_color(0.5f, 0.5f, 0.5f, 1.f);
}

Test::TestMenu::~TestMenu()
{
}

void Test::TestMenu::OnUpdate(double deltaTime)
{
}

void Test::TestMenu::OnRender()
{
	if (is_use == false)
	{
		is_use = true;
		gl_interface_.set_clear_color(0.5f, 0.5f, 0.5f, 1.f);
	}
	gl_interface_.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Test::TestMenu::OnImGuiRender()
{
	for (auto& test : test_list)
	{
		if (ImGui::Button(test.first.c_str()))
		{
			is_use = false;
			current_test = test.second();
		}
	}
}
