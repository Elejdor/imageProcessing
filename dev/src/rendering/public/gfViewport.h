#pragma once

struct GLFWwindow;

class GfViewport
{
public:
	GfViewport();
	GLFWwindow* GetWindow() const { return m_window; }

protected:
	GLFWwindow* m_window;

private:
	void InitWindow();
};