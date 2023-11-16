#if 0


#include "model.h"
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Game.h"

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

#define VertexShader "resources/Shaders/VertexShader.txt"
#define FragmentShader "resources/Shaders/FragmentShader.txt"
#define model_fs "resources/Shaders/model_fs.txt"
#define model_vs "resources/Shaders/model_vs.txt"
#define sky_vs "resources/Shaders/v_SkyBox.txt"
#define sky_fs "resources/Shaders/f_SkyBox.txt"

#define _chessBoard "resources/models/chessBoard/chessBoard.obj"
#define _bishop "resources/models/bishop/bishop.obj"
#define _king   "resources/models/king/king.obj"
#define _knight "resources/models/knight/knight.obj"
#define _pawn   "resources/models/pawn/pawn.obj"
#define _queen  "resources/models/queen/queen.obj"
#define _rock  "resources/models/rock/rock.obj"

std::ostream& operator<<(std::ostream& stream, glm::vec3 vector)
{
	stream << "x :" << vector.x << ", y :" << vector.y << ", z :" << vector.z << std::endl;
	return stream;
}

static unsigned int width = 800;
static unsigned int height = 600;

void FrameBuffer_size_callback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double Ypos);
void Short_texture(unsigned int texture, const char* Path);
unsigned int loadCubemap(vector<std::string> faces);

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
glm::vec3 camera_pos = glm::vec3(-0.0155904, 2.73837, 0.101453); // x :0.00364143, y :3.28952, z :2.28382 ,-0.0155904, 2.73837, 0.101453


static float deltaTime = 0.0f;
static float lastTime = 0.0f;

bool g = true;
float x = 0.0f;
float y = 0.0f;
float z = 0.0f;
float s = 1.0f;

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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, ScrollCalback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::FAILED_LOADING_GLAD" << std::endl;
	}

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();

	MyShader shader(VertexShader, FragmentShader);
	MyShader backpack(model_vs, model_fs);
	MyShader skybox(sky_vs, sky_fs);
	
	Model board(_chessBoard);
	//Model bishop(_bishop);
	//Model king(_king);
	//Model queen(_queen);
	//Model knight(_knight);
	Model rock(_rock);
	Model pawn(_pawn);
	

//	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	const float CULLING[] = {
		// back face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top-left198 Chapter 25. Face culling
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		// right face
		0.5f,   0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // top-left
		0.5f,  -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-right
		0.5f,   0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top-right
		0.5f,  -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-right
		0.5f,   0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // top-left
		0.5f,  -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom-left
		// bottom face
	   -0.5f,  -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // top-right
		0.5f,  -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f, // top-left
		0.5f,  -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom-left
		0.5f,  -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom-left
	   -0.5f,  -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, // bottom-right
	   -0.5f,  -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // top-right
		// top face
	   -0.5f,   0.5f, -0.5f,   0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-left
		0.5f,   0.5f,  0.5f,   0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		0.5f,   0.5f, -0.5f,   0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-right
		0.5f,   0.5f,  0.5f,   0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
	   -0.5f,   0.5f, -0.5f,   0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-left
	   -0.5f,   0.5f,  0.5f,   0.0f, -1.0f, 0.0f, 0.0f, 0.0f // bottom-left
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

	const float skyboxVertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f, 
		 1.0f,  1.0f, -1.0f, 
		 1.0f, -1.0f, -1.0f, 
		 1.0f,  1.0f, -1.0f, 
		-1.0f, -1.0f, -1.0f, 
		-1.0f,  1.0f, -1.0f, 
				// front face
		-1.0f, -1.0f,  1.0f, 
		 1.0f, -1.0f,  1.0f, 
		 1.0f,  1.0f,  1.0f, 
		 1.0f,  1.0f,  1.0f, 
		-1.0f,  1.0f,  1.0f, 
		-1.0f, -1.0f,  1.0f, 
				// left   
		-1.0f,  1.0f,  1.0f, 
		-1.0f,  1.0f, -1.0f, 
		-1.0f, -1.0f, -1.0f, 
		-1.0f, -1.0f, -1.0f, 
		-1.0f, -1.0f,  1.0f, 
		-1.0f,  1.0f,  1.0f, 
			// right
		 1.0f,   1.0f,  1.0f, 
		 1.0f,  -1.0f, -1.0f, 
		 1.0f,   1.0f, -1.0f, 
		 1.0f,  -1.0f, -1.0f, 
		 1.0f,   1.0f,  1.0f, 
		 1.0f,  -1.0f,  1.0f, 
			// bottom
	    -1.0f,  -1.0f, -1.0f, 
	   	 1.0f,  -1.0f, -1.0f, 
		 1.0f,  -1.0f,  1.0f, 
		 1.0f,  -1.0f,  1.0f, 
	    -1.0f,  -1.0f,  1.0f, 
	    -1.0f,  -1.0f, -1.0f, 
			// top 
	    -1.0f,   1.0f, -1.0f,  
	     1.0f,   1.0f,  1.0f,  
	     1.0f,   1.0f, -1.0f,  
	     1.0f,   1.0f,  1.0f,  
	    -1.0f,   1.0f, -1.0f,  
	    -1.0f,   1.0f,  1.0f,  
	};
	
	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	vector<std::string> faces
	{
		"resources/textures/skybox/right.jpg",
		"resources/textures/skybox/left.jpg",
		"resources/textures/skybox/top.jpg",
		"resources/textures/skybox/bottom.jpg",
		"resources/textures/skybox/front.jpg",
		"resources/textures/skybox/back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	double xpos, ypos;
	float z = 0.939f;
	float x = -0.617f;
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();
		ProcessInput(window);
	
		backpack.use();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 view = glm::lookAt(camera_pos, (camera_pos + camera_front), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection = glm::perspective(glm::radians(fov), float(width / height), 0.1f, 100.0f);

		backpack.SetUniformMat4fv("model", 1, model, GL_FALSE);
		backpack.SetUniformMat4fv("projection", 1, projection, GL_FALSE);
		backpack.SetUniformMat4fv("view", 1, view, GL_FALSE);

		board.Draw(backpack, FIRST);

		int counter = 0;
		
		z = 0.939f;
		x = -0.617f;
		for (int i = 0; i < 8; i++)
		{
			model = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.2f, z));
			model = glm::scale(model, glm::vec3(0.16f));

			backpack.SetUniformMat4fv("model", 1, model, GL_FALSE);

			rock.Draw(backpack, FIRST);
			x = x + (226.0f / 875.0f);
			z = 0.939f - (453.0f/1750.0f);
		}
		// the cubemap
		{
			glDepthFunc(GL_LEQUAL);
			skybox.use();

			model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(30.0f));
			view = glm::lookAt(camera_pos, (camera_pos + camera_front), glm::vec3(0.0f, 1.0f, 0.0f));
			projection = glm::perspective(glm::radians(fov), float(width / height), 0.1f, 100.0f);

			skybox.SetUniformMat4fv("model", 1, model, GL_FALSE);
			skybox.SetUniformMat4fv("view", 1, view, GL_FALSE);
			skybox.SetUniformMat4fv("projection", 1, projection, GL_FALSE);

			glBindVertexArray(skyboxVAO);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDepthFunc(GL_LESS);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("Settings");
			ImGui::Text("Pos Settings :");
			ImGui::SliderFloat("x", &x, -1.0f, 1.0f);
			ImGui::SliderFloat("pitch ", &pitch, 0.0f, 0.02f);
			ImGui::SliderFloat("yaw ", &yaw, 0.0f, 0.02f);
			ImGui::SliderFloat("y", &y, -0.25f, 0.25f);
			ImGui::SliderFloat("z", &z, -1.0f, 1.0f);
			ImGui::SliderFloat("scale", &s, 0.0f, 0.4f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		glfwGetCursorPos(window, &xpos, &ypos);
		//std::cout << xpos << ypos << std::endl;
		
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
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

	// camera movement
	float cameraSpeed = deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_pos += camera_front * deltaTime * 1.0f;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_pos -= camera_front * deltaTime * 1.0f;// camera_front* deltaTime * 1.0f;
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) / 200.0f;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) / 200.0f;

	else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		g == false ? g = true : g = true;
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		g == true ? g = false : g = false;

	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		auto lambda = [=]() {camera_front = glm::vec3(0.0f, 0.0f, -1.0f); camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
		camera_pos = glm::vec3(-0.0155904, 2.73837, 0.101453); last_x = width / 2; last_y = height / 2;
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
	
	pitch = -88.4, yaw = -90;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	if(g)
		camera_front = glm::normalize(direction);

	//std::cout << camera_pos << std::endl;
	//std::cout << "pitch :" << pitch << " ,yaw :" << yaw << std::endl;
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
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height,
			&nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap failed to load at path: " << faces[i]
				<< std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
		GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
		GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
		GL_CLAMP_TO_EDGE);
	return textureID;
}
#endif
