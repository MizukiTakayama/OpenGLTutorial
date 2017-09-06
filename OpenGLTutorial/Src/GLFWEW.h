//�����#pragma once�ł͂Ȃ�
//�ڐA���̍����C���N���[�h�o�[�h���g��
#ifndef GLFWEW_INCLUDED
#define GLFWEW_INCLUDED
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>



namespace GLFWEW {


	/**
	* @file GLFWEW.h
	*/
	class Window
	{
	public:
		static Window& Instance();
		bool Init(int w, int h, const char* title);
		bool ShouldClose() const;
		void SwapBuffers() const;

	private:
		Window();
		~Window();
		Window(const Window&) = delete;
		Window& operator = (const Window&) = delete;

		bool isGLFWInitialized;
		bool isInitialized;
		GLFWwindow* window;
	};

	/**
	* GLFW����̃G���[�񍐂���������
	*
	* @param error�@�G���[�ԍ�
	* @param desc �G���[�̓��e
	*/
	void ErrorCallback(int error, const char* desc);

}//namespace GLFWEW


#endif // !GLFWEW_INCLUDED