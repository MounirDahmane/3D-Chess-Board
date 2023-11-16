#if 0
#include"ShaderClass.h" // includes glad.c, iostream, and fstream, string, and sstring
#include<GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

const char* VertexShaderSource = "C:/Users/mouni/OneDrive/Desktop/VertexShaderSource.txt";
const char* FragmentShaderSource = "C:/Users/mouni/OneDrive/Desktop/FragmentShaderSource.txt";

#define BlackFlag "C:/Users/mouni/source/repos/_OpenGL/resourses/Textures/BlackFlag.jpg"
#define container "C:/Users/mouni/source/repos/_OpenGL/resourses/Textures/container.jpg"
#define mounir "C:/Users/mouni/source/repos/_OpenGL\resourses/mounir.jpg"
#define identity glm::mat4(1.0f)

static unsigned int width = 800;
static unsigned int height = 600;

void FrameBuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double Ypos);
void Short_texture(unsigned int texture, const char* Path);

float last_x = width / 2;
float last_y = height / 2;
float pitch = 0.0f;
float yaw = -90.0f;
bool firstMouse = true;

void ScrollCalback(GLFWwindow* window, double x_offset, double y_offset);
float fov = 45.0f;

static float percentage = 0.7f;

glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);

static float deltaTime = 0.0f;
static float lastTime = 0.0f;

// clone loockat matrix 
#if 0 
glm::mat4 myLoockat(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	glm::vec3 dir_vec = glm::normalize(eye - center);
	glm::vec3 R_axes = glm::normalize(glm::cross(up, dir_vec));
	glm::vec3 Up_axes = glm::cross(dir_vec, R_axes);

	// can access matrices by indexes of 2d array :

	glm::mat4 rotate = glm::mat4(R_axes.x, R_axes.y, R_axes.z, 0.0f,
		Up_axes.x, Up_axes.y, Up_axes.z, 0.0f,
		dir_vec.x, dir_vec.y, dir_vec.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 translate = glm::mat4(1.0f, 0.0f, 0.0f, -eye.x,
		0.0f, 1.0f, 0.0f, -eye.y,
		0.0f, 0.0f, 1.0f, -eye.z,
		0.0f, 0.0f, 0.0f, 1.0f);

	return  translate * rotate;
}
#endif

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

	const float vertices[] =
	{

	   -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	   -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

	};

	// Z buffer for depth testing
	glEnable(GL_DEPTH_TEST);
	const float cube[] =
	{

		// fornt
	   -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
	   -0.5f,  0.5f, 0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,  1.0f, 0.0f,

		0.5f,  0.5f, 0.5f,  1.0f, 1.0f,
	   -0.5f,  0.5f, 0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,  1.0f, 0.0f,

		//back
	   -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		//right
		0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,

		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f,

		//left
	   -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	   -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

	   //up

	  -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

	   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,

	   //down

	  -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	   0.5f, -0.5f,  0.5f, 1.0f, 1.0f,

	   0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		0, 3, 2,
	};

	unsigned int VBO, VAO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	stbi_set_flip_vertically_on_load(true);

	unsigned int texture0;
	glGenTextures(1, &texture0);
	Short_texture(texture0, BlackFlag);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	Short_texture(texture1, container);

	shader.use();
	glUniform1i(glGetUniformLocation(shader.ID, "txtr_smp0"), 0);
	shader.SetUniform1i("txtr_smp1", 1);

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

	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 camera_direction = glm::normalize(camera_pos - origin);
	glm::vec3 camera_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camera_direction));
	glm::vec3 camera_up = glm::cross(camera_direction, camera_right);

	float R = (float)cos(glfwGetTime());
	float G = (float)cos(glfwGetTime() * 10);
	float B = (float)cos(glfwGetTime() * 20);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(R, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ProcessInput(window);

		shader.use();

		float time = glfwGetTime();
		float green = (tan(time * 10) / 2.0f) + 0.5f;
		float red = (sin(time * 10) / 2.0f) + 0.5f;
		float blue = (cos(time * 10) / 2.0f) + 0.5f;

		deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		shader.SetUniform3f("aColor", red, green, blue);
		shader.SetUniform1f("offset", 0.5f);
		shader.SetUniform1f("percentage", percentage);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glm::mat4 model = glm::rotate(identity, glm::radians(-40.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 view = glm::lookAt(camera_pos, (camera_pos + camera_front), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection = glm::perspective(glm::radians(fov), float(width / height), 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		for (int i = 0; i < 10; i++)
		{
			model = glm::translate(identity, cubePositions[i]);
			model = glm::rotate(model, time, glm::vec3(cos(i), 0, 1.0f));
			shader.SetUniformMat4fv("model", 1, model);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shader.ID);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
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
	float cameraSpeed = deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_pos += camera_front * deltaTime * 10.0f;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_pos -= camera_front * deltaTime * 10.0f;
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) / 20.0f;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) / 20.0f;

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

	int width, height, nbrChanels;
	unsigned char* data = stbi_load(Path, &width, &height, &nbrChanels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::FAILED::LOADING_TEXTURE" << std::endl;
	}
	stbi_image_free(data);
}
#endif