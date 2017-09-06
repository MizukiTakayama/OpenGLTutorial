#pragma once


/**
* @file Shader.h
*/
#define SHADER_H_INCLUDE
#include <GL/glew.h>

namespace Shader {
	GLuint CreateProgramFromFile(const char* vsFilename, const char* fsFilename);
}