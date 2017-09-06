//sample


/**
* @file Main.cpp
*/
#include "GLFWEW.h"
#include "glm\gtc\matrix_transform.hpp"
#include <vector>
#include "Texture.h"
#include "Shader.h"
#include "OffscreenBuffer.h"


///3D�x�N�^�[�^
struct Vector3
{
	float x, y, z;
};

/// RGBA�J���[�^
struct Color
{
	float r, g, b, a;
};

/// ���_�f�[�^�^
struct Vertex
{
	glm::vec3 position; ///���W
	glm::vec4 color; ///�F
	glm::vec2 texCoord; ///�e�N�X�`�����W
};

/// ���_�f�[�^
const Vertex vertices[] = {
	{ { -0.5f, -0.3f, 0.5f },{ 0.0f, 0.0f, 1.0f, 1.0f },{ 0.0f,0.0f } },
	{ {  0.3f, -0.3f, 0.5f },{ 0.0f, 1.0f, 0.0f, 1.0f },{ 1.0f,0.0f } },
	{ {  0.3f,  0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f, 1.0f },{ 1.0f,1.0f } },
	{ { -0.5f,  0.5f, 0.5f },{ 1.0f, 0.0f, 0.0f, 1.0f },{ 0.0f,1.0f } },

	{ { -0.3f,  0.3f, 0.1f},{ 0.0f, 0.0f, 1.0f, 1.0f},{ 0.0f,1.0f } },
	{ { -0.3f, -0.5f, 0.1f},{ 0.0f, 1.0f, 1.0f, 1.0f},{ 0.0f,0.0f } },
	{ {  0.5f, -0.5f, 0.1f},{ 0.0f, 0.0f, 1.0f, 1.0f},{ 1.0f,0.0f } },
	{ {  0.5f, -0.5f, 0.1f},{ 1.0f, 1.0f, 0.0f, 1.0f},{ 1.0f,0.0f } },
	{ {  0.5f,  0.3f, 0.1f},{ 1.0f, 1.0f, 0.0f, 1.0f},{ 1.0f,1.0f } },
	{ { -0.3f,  0.3f, 0.1f},{ 1.0f, 0.0f, 0.0f, 1.0f},{ 0.0f,1.0f } },

	{ { -1.0f, -1.0f,0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f},{ 1.0f,0.0f } },
	{ {  1.0f, -1.0f,0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f},{ 0.0f,0.0f } },
	{ {  1.0f,  1.0f,0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f},{ 0.0f,1.0f } },
	{ { -1.0f,  1.0f,0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f},{ 1.0f,1.0f } },

};

/// �C���f�b�N�X�f�[�^
const GLuint indices[] = {
	0,1,2,2,3,0,
	4,5,6,7,8,9,
	10,11,12,12,13,10,
};

///���_�V�F�[�_�̃p�����[�^�^
struct VertexData {
	glm::mat4 matMVP;
	glm::vec4 lightPosition;
	glm::vec4 lightColor;
	glm::vec4 ambientColor;
};

/**
* �����`��f�[�^
*/
struct RenderingPart {
	GLsizei size; ///< �`�悷��C���f�b�N�X��
	GLvoid* offset; ///< �`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g
};

/**
* RenderingPart���쐬����
*
* @param size �`�悷��C���f�b�N�X��
* @param offset �`��J�n�C���f�b�N�X�̃I�t�Z�b�g(�C���f�b�N�X�P��)
*
* @return �쐬���������`��I�u�W�F�N�g
*/
constexpr RenderingPart MakeRenderingPart(GLsizei size, GLsizei offset) {
	return{ size, reinterpret_cast<GLvoid*>(offset * sizeof(GLuint)) };
}

/**
* �����`��f�[�^���X�g
*/
static const RenderingPart renderingParts[] = {
	MakeRenderingPart(12, 0),
	MakeRenderingPart(6,12),
};

/*
/// ���_�V�F�[�_
static const char* vsCode =
"#version 400\n"
"layout (location = 0) in vec3 vPosition;"
"layout (location = 1) in vec4 vColor;"
"layout(location = 2) in vec2 vTexCoord;"
"layout (location = 0) out vec4 outColor;"
"layout(location = 1) out vec2 outTexCoord;"
"uniform mat4x4 matMVP;"
"void main() {"
" outColor = vColor;"
" outTexCoord = vTexCoord;"
" gl_Position = matMVP * vec4(vPosition, 1.0);"
"}";
*/
/*
/// �t���O�����g�V�F�[�_
static const char* fsCode =
"#version 400\n"
"layout(location = 0) in vec4 inColor;"
"layout(location = 1) in vec2 inTexCoord;"
"uniform sampler2D colorSampler;"
"out vec4 fragColor;"
"void main() {"
"  fragColor = inColor * texture(colorSampler, inTexCoord);"
"}";
*/
/**
* Vertex Buffer Object���쐬����
*
* @param size ���_�f�[�^�̃T�C�Y
* @param data ���_�f�[�^�ւ̃|�C���^
*
* @return �쐬����VBO
*/
GLuint CreateVBO(GLsizeiptr size, const GLvoid* data)
{
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}

/**
* Index Buffer Object ���쐬����
*
* @param size �C���f�b�N�X�f�[�^�T�C�Y
* @param data �C���f�b�N�X�f�[�^�ւ̃|�C���^
*
* @return �쐬����IBO
*/
GLuint CreateIBO(GLsizeiptr size, const GLvoid* data) {
	GLuint ibo = 0;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return ibo;
}

/**
* ���_�A�g���r���[�g�̃C���f�b�N�X
*
* @param index ���_�A�g���r���[�g�̃C���f�b�N�X
* @param cls ���_�f�[�^�^��
* @param mbr ���_�A�g���r���[�g�ɐݒ肷��cls�̃����o�ϐ���
*/
#define SetVertexAttribPointer(index, cls, mbr) SetVertexAttribPointerl(\
	index, \
	sizeof(cls::mbr) / sizeof(float), \
	sizeof(cls), \
	reinterpret_cast<GLvoid*>(offsetof(cls,mbr)))

void SetVertexAttribPointerl(
	GLuint index, GLint size, GLsizei stride, const GLvoid* pointer)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
}

/**
* Vertex Array Object���쐬����
* 
* @param vbo VAO�Ɋ֘A�t������VBO
*
* @return �쐬����VAO
*/
GLuint CreateVAO(GLuint vbo)
{
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	SetVertexAttribPointer(0, Vertex, position);
	SetVertexAttribPointer(1, Vertex, color);
	SetVertexAttribPointer(2, Vertex, texCoord);
	glBindVertexArray(0);
	return vao;
	
}

/**
* Vertex Array Object ���쐬����
*
* @param vbo VAO�Ɋ֘A�t������VBO
* @param ibo VAO�Ɋ֘A�t������IBO�@�@
*
* @return �쐬����VAO
*/
GLuint CreateVAO(GLuint vbo, GLuint ibo)
{
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	SetVertexAttribPointer(0, Vertex, position);
	SetVertexAttribPointer(1, Vertex, color);
	SetVertexAttribPointer(2, Vertex, texCoord);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);
	return vao;
}

/**
* Uniform Block Object ���쐬����
*
* @param size Uniform Block�̃T�C�Y
* @param data Uniform Block�ɓ]������f�[�^�ւ̃|�C���^
*
* @return �쐬����UBO
*/
GLuint CreateUBO(GLsizeiptr size, const GLvoid* data = nullptr) {
	GLuint ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return ubo;
}


/*
/**
* �V�F�[�_�R�[�h���R���p�C������
* 
* @param type �V�F�[�_�̎��
* @param string�@�V�F�[�_�R�[�h�ւ̃|�C���^
*
* @return �쐬�����V�F�[�_�I�u�W�F�N�g
*
GLuint Compileshader(GLenum type, const GLchar* string)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &string, nullptr);
	glCompileShader(shader);
	GLint compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen) {
			std::vector<char> buf;
			buf.resize(infoLen);
			if (static_cast<int>(buf.size()) >= infoLen) {
				glGetShaderInfoLog(shader, infoLen, NULL, buf.data());
				std::cerr << "ERROR: �V�F�[�_�̃R���p�C���̎��s\n" << buf.data() << std::endl;
			}
		}
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}


/**
* �v���O�����I�u�W�F�N�g���쐬����
*
* @param vsCode ���_�V�F�[�_�R�[�h�ւ̃|�C���^
* @param fsCode �t���O�����g�V�F�[�_�R�[�h�ւ̃|�C���^
*
* @return �쐬�����v���O�����I�u�W�F�N�g
*
GLuint CreateShaderProgram(const GLchar* vsCode, const GLchar* fsCode)
{
	GLuint vs = Compileshader(GL_VERTEX_SHADER, vsCode);
	GLuint fs = Compileshader(GL_FRAGMENT_SHADER, fsCode);
	if (!vs || !fs) {
		return 0;
	}
	GLuint program = glCreateProgram();
	glAttachShader(program, fs);
	glDeleteShader(fs);
	glAttachShader(program, vs);
	glDeleteShader(vs);
	glLinkProgram(program);
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen) {
			std::vector<char> buf;
			buf.resize(infoLen);
			if (static_cast<int>(buf.size()) >= infoLen) {
				glGetProgramInfoLog(program, infoLen, NULL, buf.data());
				std::cerr << "ERROR: �V�F�[�_�̃����N�Ɏ��s\n" << buf.data() << std::endl;
			}
		}
		glDeleteProgram(program);
		return 0;
	}
	return program;
}
*/
int main()
{
	glfwSetErrorCallback(GLFWEW::ErrorCallback);

	if (glfwInit() != GL_TRUE) {
		return 1;
	}

	/*
		GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Tutorial", nullptr, nullptr);
		if (!window) {
			glfwTerminate();
			return 1;
		}

		glfwMakeContextCurrent(window);
	*/

	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (!window.Init(800, 600, "OpenGL Tutorial")) {
		return 1;
	}



	if (glewInit() != GLEW_OK) {
		std::cerr << "ERROR: GLEW�̏������Ɏ��s���܂���" << std::endl;
		glfwTerminate();
		return 1;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	std::cout << "Renderer:" << renderer << std::endl;
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Version:" << version << std::endl;
	const GLubyte* vendor = glGetString(GL_VENDOR);
	std::cout << "Vendor:" << vendor << std::endl;


	const GLuint vbo = CreateVBO(sizeof(vertices), vertices);
	const GLuint ibo = CreateIBO(sizeof(indices), indices);
	const GLuint vao = CreateVAO(vbo, ibo);
	const GLuint ubo = CreateUBO(sizeof(VertexData));
	const GLuint shaderProgram =
		Shader::CreateProgramFromFile("Src/Tutorial.vert", "Src/Tutorial.frag");
	if (!vbo || !ibo || !vao || !ubo || !shaderProgram) {
		return 1;
	}
	const GLuint uboIndex = glGetUniformBlockIndex(shaderProgram, "VertexData");
	if (uboIndex == GL_INVALID_INDEX) {
		return 1;
	}
	glUniformBlockBinding(shaderProgram, uboIndex, 0);

	/*
	const GLuint shaderProgram =
		Shader::CreateProgramFromFile("Src/Tutorial.vert", "Src/Tutorial.frag");
	if (!vbo || !ibo || !vao || !shaderProgram) {
		return 1;
	}
	*/
	

	//�ۑ�̕�
	float color_r = 0.3; //�J���[�ύX�Ɏg���ϐ�
	float color_g = 0.1; //�J���[�ύX�Ɏg���ϐ�
	float color_b = 0.3; //�J���[�ύX�Ɏg���ϐ�

	//�e�N�X�`���f�[�^
	static const uint32_t textureData[] = {
		0xffffffff, 0xffcccccc, 0xffffffff, 0xffcccccc, 0xffffffff,
		0xff888888, 0xffffffff, 0xff888888, 0xffffffff, 0xff888888,
		0xffffffff, 0xff444444, 0xffffffff, 0xff444444, 0xffffffff,
		0xff000000, 0xffffffff, 0xff000000, 0xffffffff, 0xff000000,
		0xffffffff, 0xff000000, 0xffffffff, 0xff000000, 0xffffffff,
	};

	//TexturePtr tex = Texture::Create(5, 5, GL_RGBA8, GL_RGBA, textureData);
	TexturePtr tex = Texture::LoadFromFile("Images/fukei_02.bmp");
	if (!tex) {
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	const OffscreenBufferPtr offscreen = OffscreenBuffer::Create(800, 600);

	//���C�����[�v
	while (!window.ShouldClose()) {
		glBindFramebuffer(GL_FRAMEBUFFER, offscreen->GetFramebuffer());

		glClearColor(color_r, color_g, color_b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		static float degree = 0.0f;
		degree += 0.1f;
		if (degree >= 360.0f) { degree -= 360.0f; }
		const glm::vec3 viewPos = glm::rotate(
			glm::mat4(), glm::radians(degree), glm::vec3(0, 1, 0)) * glm::vec4(2, 3, 3, 1);
		glUseProgram(shaderProgram);




		const glm::mat4x4 matProj =
			glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		const glm::mat4x4 matView =
			glm::lookAt(viewPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		VertexData vertexData;
		vertexData.matMVP = matProj * matView;
		vertexData.lightPosition = glm::vec4(1, 1, 1, 1);
		vertexData.lightColor = glm::vec4(2, 2, 2, 1);
		vertexData.ambientColor = glm::vec4(0.05f, 0.1f, 0.2f, 1);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(VertexData), &vertexData);




		/*
		const GLint matMVPLoc = glGetUniformLocation(shaderProgram, "matMVP");

		if (matMVPLoc >= 0) {
			const glm::mat4x4 matProj =
				glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
			const glm::mat4x4 matView =
				glm::lookAt(viewPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			const glm::mat4x4 matMVP = matProj * matView;
			glUniformMatrix4fv(matMVPLoc, 1, GL_FALSE, &matMVP[0][0]);
		}
		*/


		const GLint colorSamplerLoc = glGetUniformLocation(shaderProgram, "colorSampler");
		if (colorSamplerLoc >= 0) {
			glUniform1i(colorSamplerLoc, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex->Id());
		}
		



		glBindVertexArray(vao);
		/*
		glDrawElements(
			GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]),
			GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));
		*/
		glDrawElements(GL_TRIANGLES,renderingParts[0].size, GL_UNSIGNED_INT,renderingParts[0].offset);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.5f, 0.3f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (colorSamplerLoc >= 0) {
			glBindTexture(GL_TEXTURE_2D, offscreen->GetTexture());
		}
		/*
		glDrawElements(
			GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]),
			GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));
		*/
		vertexData = {};
		vertexData.ambientColor = glm::vec4(1, 1, 1, 1);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(VertexData), &vertexData);

//		if (matMVPLoc >= 0) {
//			glUniformMatrix4fv(matMVPLoc, 1, GL_FALSE, &glm::mat4()[0][0]);
//		}
		glDrawElements(
			GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);


		window.SwapBuffers();

		if (color_b < 1.0f) {
			color_g += 0.0001f;
			color_b += 0.0001f;
		}
		
	}

	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();

	return 0;
}

