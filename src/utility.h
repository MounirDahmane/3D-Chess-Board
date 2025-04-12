#pragma once

// PATH_TO_WD is a personal defined macro

#define VERTEX_SHADER	PATH_TO_WD "/resources/Shaders/VertexShader.txt"
#define FRAGMENT_SHADER	PATH_TO_WD "/resources/Shaders/FragmentShader.txt"
#define FS_MODEL	PATH_TO_WD "/resources/Shaders/model_fs.txt"
#define VS_MODEL	PATH_TO_WD "/resources/Shaders/model_vs.txt"
#define VS_SKY		PATH_TO_WD "/resources/Shaders/v_SkyBox.txt"
#define FS_SKY		PATH_TO_WD "/resources/Shaders/f_SkyBox.txt"
								   
#define CHESS_BOARD	PATH_TO_WD "/resources/models/chessBoard/chessBoard.obj"
#define BLACK_BISHOP	PATH_TO_WD "/resources/models/bishop/bishop.obj"
#define GRAY_BISHOP	PATH_TO_WD "/resources/models/bishop_gray/bishop.obj"
#define BLACK_KING	PATH_TO_WD "/resources/models/king/king.obj"
#define GRAY_KING	PATH_TO_WD "/resources/models/king_gray/king.obj"
#define BLACK_KNIGHT	PATH_TO_WD "/resources/models/knight/knight.obj"
#define GRAY_KNIGHT	PATH_TO_WD "/resources/models/knight_gray/knight.obj"
#define BLACK_PAWN	PATH_TO_WD "/resources/models/pawn/pawn.obj"
#define GRAY_PAWN	PATH_TO_WD "/resources/models/pawn_gray/pawn.obj"
#define BLACK_QUEEN	PATH_TO_WD "/resources/models/queen/queen.obj"
#define GRAY_QUEEN	PATH_TO_WD "/resources/models/queen_gray/queen.obj"
#define BLACK_ROOK	PATH_TO_WD "/resources/models/rook/rook.obj"
#define GRAY_ROOK	PATH_TO_WD "/resources/models/rook_gray/rook.obj"

unsigned int width = 1280, height = 720;
float deltaTime = 0.0f;
float lastTime = 0.0f;

float last_x = width / 2;
float last_y = height / 2;

float percentage = 0.7f;

const float CULLING[] = {
	 // back face
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, // bottom-left
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f, // top-right
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f, // top-right
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, // bottom-left
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f, // top-left
	 // front face
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, // bottom-left
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, // bottom-right
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, // top-right
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, // top-right
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, // top-left
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, // bottom-left
	 // left face
	-0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // top-right
	-0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // top-left
	-0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // bottom-left
	-0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // bottom-left
	-0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // bottom-right
	-0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,	// top-right
	 // right face
         0.5f,   0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // top-left
	 0.5f,  -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // bottom-right
	 0.5f,   0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // top-right
	 0.5f,  -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // bottom-right
	 0.5f,   0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // top-left
	 0.5f,  -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // bottom-left
	 // bottom face
        -0.5f,  -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, // top-right
 	 0.5f,  -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f, // top-left
	 0.5f,  -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f, // bottom-left
	 0.5f,  -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f, // bottom-left
        -0.5f,  -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, // bottom-right
        -0.5f,  -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, // top-right
   	 // top face
        -0.5f,   0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f, // top-left
  	 0.5f,   0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f, // bottom-right
   	 0.5f,   0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f, // top-right
   	 0.5f,   0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f, // bottom-right
        -0.5f,   0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f, // top-left
        -0.5f,   0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f  // bottom-left
};
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
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 // bottom
	 -1.0f, -1.0f, -1.0f,
	  1.0f, -1.0f, -1.0f,
	  1.0f, -1.0f,  1.0f,
	  1.0f, -1.0f,  1.0f,
	 -1.0f, -1.0f,  1.0f,
	 -1.0f, -1.0f, -1.0f,
	 // top 
	 -1.0f,  1.0f, -1.0f,
	  1.0f,  1.0f,  1.0f,
	  1.0f,  1.0f, -1.0f,
	  1.0f,  1.0f,  1.0f,
	 -1.0f,  1.0f, -1.0f,
	 -1.0f,  1.0f,  1.0f,
};
const vector<std::string> faces
{
	PATH_TO_WD "/resources/textures/skybox/right.jpg",
	PATH_TO_WD "/resources/textures/skybox/left.jpg",
	PATH_TO_WD "/resources/textures/skybox/top.jpg",
	PATH_TO_WD "/resources/textures/skybox/bottom.jpg",
	PATH_TO_WD "/resources/textures/skybox/front.jpg",
	PATH_TO_WD "/resources/textures/skybox/back.jpg"
};

float pitch = 0.0f;
float yaw = -90.0f;
bool  firstMouse = true;
bool  first_peek = true;

float fov = 45.0f;
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camera_pos = glm::vec3(-0.00313685, 1.83403, 3.19938);

bool g = true;
float x = 0.0f;
float y = 0.0f;
float z = 0.0f;
float s = 1.0f;

void FrameBuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	glViewport(0, 0, width, height);
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

	if (first_peek)
	{
		pitch = -25.0f, yaw = -91.0f;
		first_peek = false;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	if (g)
		camera_front = glm::normalize(direction);

}

void ScrollCalback(GLFWwindow* window, double x_offset, double y_offset)
{
	fov -= (float)y_offset;

	fov = fov >= 45.0f ? 45.0f : fov;
	fov = fov <= 1.0f ? 1.0f : fov;
}

int GLFW_SETUP(GLFWwindow*& window, unsigned int& width, unsigned int& height) {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(width, height, "ChessScapeRender", NULL, NULL);

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
}

void ProcessInput(GLFWwindow* window)
{
	// exit
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// camera movement
	float cameraSpeed = deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_pos += camera_front * deltaTime * 10.0f;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_pos -= camera_front * deltaTime * 10.0f;// camera_front* deltaTime * 1.0f;
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) / 20.0f;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) / 20.0f;

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
void configureGLState() {
	// Enable depth testing and set the depth function
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Enable blending and set the blending function
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable stencil testing, set the stencil function and operations
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}
void setupCullingVAO(unsigned int& VAO, unsigned int& VBO) {

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

}
void setupSkyboxVAO(unsigned int& VAO, unsigned int& VBO) {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
}
void cleanup(GLFWwindow* window, unsigned int VAO, unsigned int VBO, unsigned int skyboxVAO, unsigned int skyboxVBO, unsigned int cubemapTexture) {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	glDeleteTextures(1, &cubemapTexture);
	glfwDestroyWindow(window);
	glfwTerminate();
}
