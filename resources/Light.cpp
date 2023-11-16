#if 0
#include"ShaderClass.h" // includes glad.c, iostream, and fstream, string, and sstring
#include<GLFW/glfw3.h>
#include "model.h"

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



const char* VertexShaderSource = "C:/Pc/Shaders/LightVertexShaderSource.txt";
const char* FragmentShaderSource = "C:/Pc/Shaders/FragmentShaderSource.txt";
//"C:/Users/mouni/OneDrive/Desktop/LightFragmentShaderSource1.txt";
const char* LightSourceFragmentShaderSource = "C:/Pc/Shaders/LightSourceFragmentShaderSource.txt";

#define BlackFlag		"C:/Users/one tech/source/repos/OpenGLPractice/Resources/Textures/BlackFlag.jpg"
#define container		"C:/Users/one tech/source/repos/OpenGLPractice/Resources/Textures/container.jpg"
#define matrix			"C:/Users/one tech/source/repos/OpenGLPractice/Resources/Textures/matrix.jpg"
#define container2		"C:/Users/one tech/source/repos/OpenGLPractice/Resources/Textures/container2.png"
#define container_spec	"C:/Users/one tech/source/repos/OpenGLPractice/Resources/Textures/container2_specular.png"
#define mounir			"C:/Users/one tech/source/repos/OpenGLPractice/Resources/Textures/mounir.jpg"
#define grass			"C:/Users/one tech/source/repos/OpenGLPractice/Resources/Textures/grass.png"
#define trans_window	"C:/Users/one tech/source/repos/OpenGLPractice/Resources/Textures/window.png"

#define identity glm::mat4(1.0f)

static unsigned int width = 800;
static unsigned int height = 600;

void FrameBuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double Ypos);
void Short_texture(unsigned int texture, const char* Path);
std::pair<unsigned int, unsigned int> Valloc(unsigned int VAO, unsigned int VBO, const void* data, unsigned int stride);

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
glm::vec3 camera_pos = glm::vec3(0.0106683, 0.515807, 3.00831);

static float deltaTime = 0.0f;
static float lastTime = 0.0f;

glm::vec3 L_Source(0.0, 1.6309, 0.01327);
glm::vec3 LightColor[]
{
	glm::vec3(0.2f, 0.3f, 0.9f),
	glm::vec3(1.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f)
};
 //(1.0f + sin(glfwGetTime()) * 2.0f, sin(glfwGetTime() / 2.0f) * 1.0f, 1.0f)
glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
std::ostream& operator<<(std::ostream& stream, glm::vec3 vector)
{
	stream << vector.x << ", " << vector.y << ", " << vector.z << std::endl;
	return stream;
}
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

void FaceCulling()
{
	glEnable(GL_CULL_FACE); // works only on closed shapes 
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
}
void DeFaceCulling()
{
	glDisable(GL_CULL_FACE);
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
	MyShader LightSource(VertexShaderSource, LightSourceFragmentShaderSource);

	MyShader stencil(VertexShaderSource, FragmentShaderSource);
	MyShader single(VertexShaderSource, LightSourceFragmentShaderSource);
	// Z buffer for depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FaceCulling();
	DeFaceCulling();

	const float floor[] =
	{
		 5.0f, -0.51f,  5.0f, 0.0f, 0.0f, 1.0f,  2.0f, 0.0f,
		-5.0f, -0.51f,  5.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		-5.0f, -0.51f, -5.0f, 0.0f, 0.0f, 1.0f,  0.0f, 2.0f,
						 
		 5.0f, -0.51f,  5.0f, 0.0f, 0.0f, 1.0f,  2.0f, 0.0f,
		-5.0f, -0.51f, -5.0f, 0.0f, 0.0f, 1.0f,  0.0f, 2.0f,
		 5.0f, -0.51f, -5.0f, 0.0f, 0.0f, 1.0f,  2.0f, 2.0f
	};
	
	const float CULLING[] = {
		// back face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-0.5f, -0.5f, 0.5f,0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
		 0.5f,  0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
		-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top-left198 Chapter 25. Face culling
		-0.5f, -0.5f, 0.5f,0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
		-0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		// right face
		0.5f, 0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // top-left
		0.5f, -0.5f, -0.5f,-1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-right
		0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f,-1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-right
		0.5f, 0.5f, 0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // top-left
		0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom-left
		// bottom face
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, // top-left
		0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom-left
		0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // top-right
		// top face
		-0.5f, 0.5f, -0.5f,0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-left
		0.5f, 0.5f, 0.5f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-right
		0.5f, 0.5f, 0.5f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		-0.5f, 0.5f, -0.5f,0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-left
		-0.5f, 0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f // bottom-left
	};
	
	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CULLING), CULLING, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int f_VAO , f_VBO ;
	glGenVertexArrays(1, &f_VAO);
	glGenBuffers(1, &f_VBO);
	glBindVertexArray(f_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, f_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	stbi_set_flip_vertically_on_load(true);

	unsigned int texture0;
	glGenTextures(1, &texture0);
	Short_texture(texture0, container2);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	Short_texture(texture1, container_spec);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	Short_texture(texture2, matrix);
	unsigned int t_grass;
	glGenTextures(1, &t_grass);
	Short_texture(t_grass, trans_window);

	unsigned int texture3;
	glGenTextures(1, &texture3);
	Short_texture(texture3, container);

	//shader.use();
	//glUniform1i(glGetUniformLocation(shader.ID, "material.diffuse"), 2);
	//shader.SetUniform1i("material.specular", 0);
	//shader.SetUniform1i("material.emission", 1);


	glm::vec3 cubePositions[] = {
glm::vec3(0.0f, 0.0f, 0.0f),
glm::vec3(2.0f, 5.0f, -15.0f),
glm::vec3(-1.5f, 0.0f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3(2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f, 3.0f, -7.5f),
glm::vec3(1.3f, -2.0f, -2.5f),
glm::vec3(1.5f, 2.0f, -2.5f),
glm::vec3(1.5f, 0.2f, -1.5f),
glm::vec3(-1.3f, 1.0f, -1.5f)
	};

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f, 0.2f, 2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f, 2.0f, -12.0f),
	glm::vec3(0.0f, 0.0f, -3.0f)
	};

std::vector<glm::vec3> vegetation;
vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

std::map<float, glm::vec3> sorted;
for (int i = 0; i < vegetation.size(); i++)
{
	float distance = glm::length(camera_pos - vegetation[i]);
	sorted[distance] = vegetation[i];
}

	while (!glfwWindowShouldClose(window))
	{
		//std::cout << camera_pos << std::endl;
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		ProcessInput(window);

		deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		glm::mat4 model = glm::translate(identity, glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 view = glm::lookAt(camera_pos, (camera_pos + camera_front), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection = glm::perspective(glm::radians(fov), float(width / height), 0.1f, 100.0f);

		
		glStencilMask(0x00);
		stencil.use();
		glUniformMatrix4fv(glGetUniformLocation(stencil.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(stencil.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(stencil.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(f_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);// floor
		glBindVertexArray(0);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		
		for (int i = 0; i < vegetation.size(); i++) // cubes
		{
			model = glm::translate(identity, vegetation[i]);
			glUniformMatrix4fv(glGetUniformLocation(stencil.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glBindTexture(GL_TEXTURE_2D, texture3);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
		for (int i = 0; i < vegetation.size(); i++) //grass
		{
			model = glm::translate(identity, vegetation[i]);
			model = glm::scale(model, glm::vec3(1.0f));
			glUniformMatrix4fv(glGetUniformLocation(stencil.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glBindTexture(GL_TEXTURE_2D, t_grass);
			glBindVertexArray(VAO);
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) // windows
		{
			model = glm::translate(identity, it->second);
			model = glm::scale(model, glm::vec3(1.0f));
			glUniformMatrix4fv(glGetUniformLocation(stencil.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glBindTexture(GL_TEXTURE_2D, t_grass);
			glBindVertexArray(VAO);
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		single.use();

		glUniformMatrix4fv(glGetUniformLocation(single.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(single.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		single.SetUniform3f("LightColor", glm::vec3(0.7f));

		for (int i = 0; i < vegetation.size(); i++) // blending
		{
			model = glm::translate(identity, vegetation[i]);
			model = glm::scale(model, glm::vec3(1.1f));
			glUniformMatrix4fv(glGetUniformLocation(stencil.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);
		//shader.SetUniform3f("LightPos", glm::vec3(1.0f + sin(glfwGetTime()) * 2.0f, sin(glfwGetTime() / 2.0f) * 1.0f, 1.0f));
		
		//shader.SetUniform3f("material.ambient"  , glm::vec3(1.0f, 0.0f, 0.0f));
		//shader.SetUniform3f("material.diffuse"  , glm::vec3(0.0f, 0.50980392f, 0.50980392f));
		//shader.SetUniform3f("material.specular" , glm::vec3(0.50196078f, 0.50196078f, 0.50196078f));
		/*shader.SetUniform1f("material.shininess", 32.0f);

		shader.SetUniform3f("dirLight.ambient" , glm::vec3(0.2f));
		shader.SetUniform3f("dirLight.diffuse" , glm::vec3(0.5f));
		shader.SetUniform3f("dirLight.specular", glm::vec3(1.0f));
		///shader.SetUniform3f("light.position", camera_pos);
		shader.SetUniform3f("dirLight.direction", camera_front);

		shader.SetUniform1f("PointLight::constant", 1.0f);	///
		shader.SetUniform1f("PointLight::linear", 0.07f);	///
		shader.SetUniform1f("PointLight::quadratic", 0.017f);///

		shader.SetUniform3f("PointLight::ambient",  glm::vec3(0.2f));	///
		shader.SetUniform3f("PointLight::diffuse",  glm::vec3(0.5f));	///
		shader.SetUniform3f("PointLight::specular", glm::vec3(1.0f));///*/

		///shader.SetUniform3f("light.Fl_direction", camera_front);
		///shader.SetUniform1f("light.cutoff", glm::cos(glm::radians(12.5f)));
		///shader.SetUniform1f("light.outerCutoff", glm::cos(glm::radians(17.0f)));
		
		//shader.SetUniform3f("viewPos", camera_pos); only if we do the calculation in the world space

		/*shader.SetUniform1f("material.shininess", 32.0f);
		shader.SetUniform3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shader.SetUniform3f("dirLight.ambient", glm::vec3(0.1f));
		shader.SetUniform3f("dirLight.diffuse", glm::vec3(0.5f));
		shader.SetUniform3f("dirLight.specular", glm::vec3(1.0f));
		// point light 1
		shader.SetUniform3f("pointLights[0].position", pointLightPositions[0]);
		shader.SetUniform3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		shader.SetUniform3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		shader.SetUniform3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		shader.SetUniform1f("pointLights[0].constant", 1.0f);
		shader.SetUniform1f("pointLights[0].linear", 0.09f);
		shader.SetUniform1f("pointLights[0].quadratic", 0.032f);
		// point light 2
		shader.SetUniform3f("pointLights[1].position", pointLightPositions[1]);
		shader.SetUniform3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		shader.SetUniform3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		shader.SetUniform3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		shader.SetUniform1f("pointLights[1].constant", 1.0f);
		shader.SetUniform1f("pointLights[1].linear", 0.09f);
		shader.SetUniform1f("pointLights[1].quadratic", 0.032f);
		// point light 3
		shader.SetUniform3f("pointLights[2].position", pointLightPositions[2]);
		shader.SetUniform3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		shader.SetUniform3f("pointLights[2].diffuse", 1.0f, 1.0f, 0.0f);
		shader.SetUniform3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		shader.SetUniform1f("pointLights[2].constant", 1.0f);
		shader.SetUniform1f("pointLights[2].linear", 0.09f);
		shader.SetUniform1f("pointLights[2].quadratic", 0.032f);
		// point light 4
		shader.SetUniform3f("pointLights[3].position", pointLightPositions[3]);
		shader.SetUniform3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		shader.SetUniform3f("pointLights[3].diffuse", 0.0f, 0.8f, 0.0f);
		shader.SetUniform3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		shader.SetUniform1f("pointLights[3].constant", 1.0f);
		shader.SetUniform1f("pointLights[3].linear", 0.09f);
		shader.SetUniform1f("pointLights[3].quadratic", 0.032f);*/

		//shader.SetUniform1f("percentage", percentage);
		//
		//glBindTexture(GL_TEXTURE_2D, texture0);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);
		//glActiveTexture(GL_TEXTURE2);
		//
		//for (int i = 0; i < 10; i++)
		//{
		//	//shader.SetUniform3f("lightPos", pointLightPositions[i]);
		//	shader.SetUniform3f("Lcolor", LightColor[i]);
		//	model = glm::translate(identity, cubePositions[i]);
		//	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//	model = glm::scale(model, glm::vec3(1.5f));
		//	shader.SetUniformMat4fv("model", 1, model);
		//	glBindVertexArray(VAO);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//	glBindVertexArray(0);
		//}

		//LightSource.use();
		//
		//glUniformMatrix4fv(glGetUniformLocation(LightSource.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(glGetUniformLocation(LightSource.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		//
		//
		//for (int i = 0; i < 4; i++)
		//{
		//	model = identity;
		//	model = glm::translate(model, pointLightPositions[i]);//glm::vec3(1.0f + sin(glfwGetTime()) * 2.0f, sin(glfwGetTime() / 2.0f) * 1.0f, 1.0f));
		//	model = glm::scale(model, glm::vec3(0.2f));
		//
		//LightSource.SetUniformMat4fv("model", 1, model);
		//LightSource.SetUniform3f("LightColor", LightColor[i]);
		//
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);
		//}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shader.ID);
	glDeleteProgram(stencil.ID);
	glDeleteProgram(single.ID);
	glDeleteProgram(LightSource.ID);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glDeleteBuffers(1, &f_VBO);
	glDeleteVertexArrays(1, &f_VAO);

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
	if      (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_pos += camera_front * cameraSpeed;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_pos -= camera_front * cameraSpeed;
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) / 80.0f;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) / 80.0f;
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
	int width, height, nbrChannels;
	unsigned char* data = stbi_load(Path, &width, &height, &nbrChannels, 0);

	glBindTexture(GL_TEXTURE_2D, texture);
	if (nbrChannels == 4)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLenum format;
	if (nbrChannels == 1)
		format = GL_RED;
	else if (nbrChannels == 3)
		format = GL_RGB;
	else if (nbrChannels == 4)
		format = GL_RGBA;

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
std::pair<unsigned int, unsigned int> Valloc(unsigned int VAO, unsigned int VBO, const void* data, unsigned int stride)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return {VAO, VBO};
}
#endif