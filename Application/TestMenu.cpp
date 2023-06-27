#include "TestMenu.h"

Test::TestMenu::TestMenu(TestBase*& _test) : current_test(_test)
{
	renderer.set_clear_color(0.5f, 0.5f, 0.5f, 1.f);
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
		renderer.set_clear_color(0.5f, 0.5f, 0.5f, 1.f);
	}
	renderer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
