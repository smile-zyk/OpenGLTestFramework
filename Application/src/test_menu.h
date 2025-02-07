#pragma once

#include <vector>
#include <functional>

#include "test_base.h"
#include <string>

namespace Test
{
	class TestMenu : public TestBase
	{
	public:
		TestMenu(TestBase*& _test);
		~TestMenu() override;
		virtual void OnUpdate(double deltaTime) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			test_list.push_back(std::make_pair(name, []() { return new T(); }));
		}
	private:
		bool is_use = true;
		TestBase*& current_test;
		std::vector<std::pair<std::string, std::function<TestBase* ()>>> test_list;
	};
}

