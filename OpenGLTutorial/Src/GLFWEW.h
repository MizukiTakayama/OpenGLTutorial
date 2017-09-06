//今回は#pragma onceではなく
//移植性の高いインクルードバードを使う
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
	* GLFWからのエラー報告を処理する
	*
	* @param error　エラー番号
	* @param desc エラーの内容
	*/
	void ErrorCallback(int error, const char* desc);

}//namespace GLFWEW


#endif // !GLFWEW_INCLUDED