#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Input;

class Window
{
	friend class Input;
public:
	Window(int width, int height, const std::string& title, bool fullscreen = false, bool vsync = false);
	~Window();

	void refresh();
	void resize(int width, int height);

	bool isCloseRequested();
	bool isCreated();
	bool isFocused();

	int getWidth();
	int getHeight();
	std::string getTitle();

	glm::vec2 getCenter();
	float getAspectRatio();

	Input* getInput();

	void moveToScreenCenter();
protected:
private:
	GLFWwindow* m_glfwWindow;
	Input* m_input;

	std::string m_title;

	GLFWwindow* getGLFWwindow();
};

