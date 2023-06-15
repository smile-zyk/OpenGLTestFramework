#include "TestMenu.h"

Test::TestMenu::~TestMenu()
{
}

void Test::TestMenu::OnUpdate(float deltaTime)
{

}

void Test::TestMenu::OnRender()
{

}

void Test::TestMenu::OnImGuiRender()
{
	for (auto& test : test_list)
	{
		if (ImGui::Button(test.first.c_str()))
		{
			current_test = test.second();
		}
	}
}
