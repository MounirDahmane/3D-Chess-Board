#if	0
// includes glad.c, iostream, and fstream, string, and sstring
#include "Headers/model.h"
#include <GLFW/glfw3.h>


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "OPENGL::ERROR_(" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

#define STB_IMAGE_IMPLEMENTATION
#include"stb-master\stb_image.h"
#include<vector>

const char* VertexShaderSource	 = "C:/Pc/Shaders/model/VertexShaderSource.txt";
const char* FragmentShaderSource = "C:/Pc/Shaders/model/FragmentShaderSource.txt";
const char* lightShaderSource	 = "C:/Pc/Shaders/model/lightmodel.txt";
const char* true2				 = "C:/Pc/Shaders/model/true.txt";

#define BlackFlag		"C:/Users/one tech/source/repos/OpenGLPractice/Resources/Textures/BlackFlag.jpg"
#define container		"C:/Users/one tech/source/repos/OpenGLPractice/Resources/Textures/container.jpg"
#define matrix          "C:/Users/one tech/source/repos/OpenGLPractice/Resources/Textures/matrix.jpg"
#define container2		"C:/Users/one tech/source/repos/OpenGLPractice/Resources/Textures/container2.png"
#define container_spec	"C:/Users/one tech/source/repos/OpenGLPractice/Resources/Textures/container2_specular.png"
#define mounir			"C:/Users/one tech/source/repos/OpenGLPractice/Resources/Textures/mounir.jpg"
#define identity glm::mat4(1.0f)
std::string backpack  = "resources/models/backpack/backpack.obj";
std::string dragon2   = "C:/Users/one tech/source/repos/OpenGLPractice/Resources/Models/dragon/orange/orange-dragon-lite.obj";

static unsigned int width = 800;
static unsigned int height = 600;

void FrameBuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double Ypos);
void Short_texture(unsigned int texture, const char* Path);
void Short_texture(unsigned int texture, const char* Path, bool png);

float last_x = width / 2;
float last_y = height / 2;
float pitch = 0.0f;
float yaw = -90.0f;
bool  firstMouse = true;

void ScrollCalback(GLFWwindow* window, double x_offset, double y_offset);
float fov = 45.0f;

static float percentage = 0.7f;

glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 0.0f);

static float deltaTime = 0.0f;
static float lastTime = 0.0f;

std::ostream& operator<<(std::ostream& stream, glm::vec3 vector)
{
	stream << vector.x << ", " << vector.y << ", " << vector.z << std::endl;
	return stream;
}


int main(void)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "ERROR::FAILED::LODADING_GLFW_WINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, FrameBuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, ScrollCalback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::FAILED_LOADING_GLAD" << std::endl;
	}

	MyShader shader(VertexShaderSource, FragmentShaderSource);
	MyShader single(VertexShaderSource, true2);
	MyShader light (VertexShaderSource, lightShaderSource);
	Model    Mymodel(backpack);
	//Model dragon(dragon2);

	// Z buffer for depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	const float cube[] =
	{

		// fornt
	   -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

		0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

		//back
	   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		//right
		0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

		//left
	   -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	   -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

	   //up

	  -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	   0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

	   0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	   0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

	   //down

	  -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	   0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,

	   0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int texture;
	glGenTextures(1, &texture);
	Short_texture(texture, container2);

	glm::vec3 cubePositions[] = {
glm::vec3(0.0f, 0.0f, 0.0f),
glm::vec3(2.0f, 5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3(2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f, 3.0f, -7.5f),
glm::vec3(1.3f, -2.0f, -2.5f),
glm::vec3(1.5f, 2.0f, -2.5f),
glm::vec3(1.5f, 0.2f, -1.5f),
glm::vec3(-1.3f, 1.0f, -1.5f)
	};


	while (!glfwWindowShouldClose(window))
	{
		//std::cout << camera_pos << std::endl;
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ProcessInput(window);

		deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		glm::mat4 model = glm::translate(identity, glm::vec3(0.0f, 0.0f, -8.0f));
		glm::mat4 view = glm::lookAt(camera_pos, (camera_pos + camera_front), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection = glm::perspective(glm::radians(fov), float(width / height), 0.1f, 100.0f);

		shader.use();

		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		shader.SetUniform3f("lightPos", glm::vec3(-0.0f, -0.0f, -5.0f));
		shader.SetUniform3f("LightColor", glm::vec3(0.6f));
		shader.SetUniform3f("ObjectColor", glm::vec3(1.0f, 0.2f, 0.6f));

		Mymodel.Draw(shader, FIRST);

		single.use();

		model = glm::translate(identity, glm::vec3(0.0f, 0.0f, -8.0f));
		model = glm::scale(model, glm::vec3(1.1f));

		glUniformMatrix4fv(glGetUniformLocation(single.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(single.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(single.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		
		Mymodel.Draw(single, SECOND);

		

		//glBindTexture(GL_TEXTURE_2D, texture);
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);



		//model = glm::translate(identity, glm::vec3(2.0f, 2.0f, -2.0f));
		//glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shader.ID);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;

}
void FrameBuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void ProcessInput(GLFWwindow* window)
{
	// exit
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// changing the percentage of mixing the textures
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		percentage += 0.001f;
		percentage = (percentage > 0.999f) ? percentage = 0.01f : percentage;
		std::cout << percentage << std::endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		percentage -= 0.001f;
		percentage = (percentage < 0.01f) ? percentage = 0.999f : percentage;
		std::cout << percentage << std::endl;
	}
	// camera movement
	float cameraSpeed = deltaTime * 10.0f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_pos += camera_front * cameraSpeed;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_pos -= camera_front * cameraSpeed;
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) / 40.0f;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) / 40.0f;
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		auto lambda = [=]() {camera_front = glm::vec3(0.0f, 0.0f, -1.0f); camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
		camera_pos = glm::vec3(0.0f, 0.0f, 3.0f); last_x = width / 2; last_y = height / 2;
		pitch = 0.0f; yaw = -90.0f; firstMouse = true; };
		lambda();
	}
}
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		last_x = xPos;
		last_y = yPos;
		firstMouse = false;
	}
	float x_offset = xPos - last_x;
	float y_offset = last_y - yPos;

	last_x = xPos;
	last_y = yPos;

	const float sensitivity = 0.1f;

	x_offset *= sensitivity;
	y_offset *= sensitivity;

	yaw += x_offset;
	pitch += y_offset;

	pitch = pitch >= 89.0f ? 89.0f : pitch;
	pitch = pitch <= -89.0f ? -89.0f : pitch;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	camera_front = glm::normalize(direction);
}
void ScrollCalback(GLFWwindow* window, double x_offset, double y_offset)
{
	fov -= (float)y_offset;

	fov = fov >= 45.0f ? 45.0f : fov;
	fov = fov <= 1.0f ? 1.0f : fov;
}
void Short_texture(unsigned int texture, const char* Path)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int width, height, nbrChannels;
	unsigned char* data = stbi_load(Path, &width, &height, &nbrChannels, 0);

	GLenum format;
	switch (nbrChannels)
	{
	case 1: format = GL_RED; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	default:
		std::cout << "FORMAT::ERROR_COULD_NOT_DEFINED_THE_FORMAT\n" << std::endl;
		break;
	}
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::FAILED::LOADING_TEXTURE" << std::endl;
	}
	stbi_image_free(data);
}
#endif