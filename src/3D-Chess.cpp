#include "model.h"
#include <GLFW/glfw3.h>
#include "utility.h"

#define ASSERT(condition) if (!(condition)) __debugbreak()
#define GLCALL(call) ClearOpenGLErrors();\
    call;\
    ASSERT(LogOpenGLError(#call, __FILE__, __LINE__))

struct ChessPiece {
    const Model* pieceModel;
    glm::mat4 transformationMatrix;
};

constexpr float PIECE_SCALE_FACTOR = 0.16f;
constexpr int TOTAL_MAIN_PIECES = 16;
constexpr int PAWNS_PER_COLOR = 8;
constexpr int TOTAL_PIECES = TOTAL_MAIN_PIECES + 2 * PAWNS_PER_COLOR;
constexpr float SKYBOX_SCALE = 30.0f;

static void ClearOpenGLErrors() {
    while (glGetError() != GL_NO_ERROR);
}

static bool LogOpenGLError(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "OPENGL::ERROR_(" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

static glm::mat4 CreatePieceTransformationMatrix(const glm::vec3& position, float scale) {
    return glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(scale));
}

static GLFWwindow* InitializeWindowAndOpenGL() {
    GLFWwindow* window;
    GLFW_SETUP(window, width, height);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "ERROR::FAILED_LOADING_GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(-1);
    }

    return window;
}

static void SetupChessPieces(std::vector<ChessPiece>& pieces,
    const Model& chessBoard,
    const Model& blackKing,
    const Model& grayKing,
    const Model& blackQueen,
    const Model& grayQueen,
    const Model& blackBishop,
    const Model& grayBishop,
    const Model& blackKnight,
    const Model& grayKnight,
    const Model& blackRook,
    const Model& grayRook,
    const Model& blackPawn,
    const Model& grayPawn,
    float scale) {
    pieces.reserve(TOTAL_PIECES);

    pieces.emplace_back(ChessPiece{ &blackQueen,  CreatePieceTransformationMatrix(glm::vec3(-0.127f, 0.0f, 0.912f), scale) });
    pieces.emplace_back(ChessPiece{ &grayQueen,   CreatePieceTransformationMatrix(glm::vec3(-0.127f, 0.0f, -0.912f), scale) });
    pieces.emplace_back(ChessPiece{ &blackKing,   CreatePieceTransformationMatrix(glm::vec3(0.127f, 0.0f, 0.912f), scale) });
    pieces.emplace_back(ChessPiece{ &grayKing,    CreatePieceTransformationMatrix(glm::vec3(-0.127f, 0.0f, -0.912f), scale) });
    pieces.emplace_back(ChessPiece{ &blackBishop, CreatePieceTransformationMatrix(glm::vec3(0.383f, 0.0f, 0.898f), scale) });
    pieces.emplace_back(ChessPiece{ &blackBishop, CreatePieceTransformationMatrix(glm::vec3(-0.383f, 0.0f, 0.898f), scale) });
    pieces.emplace_back(ChessPiece{ &grayBishop,  CreatePieceTransformationMatrix(glm::vec3(0.383f, 0.0f, -0.898f), scale) });
    pieces.emplace_back(ChessPiece{ &grayBishop,  CreatePieceTransformationMatrix(glm::vec3(-0.383f, 0.0f, -0.898f), scale) });
    pieces.emplace_back(ChessPiece{ &blackKnight, CreatePieceTransformationMatrix(glm::vec3(0.645f, 0.0f, 0.907f), scale) });
    pieces.emplace_back(ChessPiece{ &blackKnight, CreatePieceTransformationMatrix(glm::vec3(-0.645f, 0.0f, 0.907f), scale) });
    pieces.emplace_back(ChessPiece{ &grayKnight,  CreatePieceTransformationMatrix(glm::vec3(0.645f, 0.0f, -0.907f), scale) });
    pieces.emplace_back(ChessPiece{ &grayKnight,  CreatePieceTransformationMatrix(glm::vec3(-0.645f, 0.0f, -0.907f), scale) });
    pieces.emplace_back(ChessPiece{ &blackRook,   CreatePieceTransformationMatrix(glm::vec3(0.902f, 0.0f, 0.907f), scale) });
    pieces.emplace_back(ChessPiece{ &blackRook,   CreatePieceTransformationMatrix(glm::vec3(-0.902f, 0.0f, 0.907f), scale) });
    pieces.emplace_back(ChessPiece{ &grayRook,    CreatePieceTransformationMatrix(glm::vec3(0.902f, 0.0f, -0.907f), scale) });
    pieces.emplace_back(ChessPiece{ &grayRook,    CreatePieceTransformationMatrix(glm::vec3(-0.902f, 0.0f, -0.907f), scale) });

    for (int i = 0; i < PAWNS_PER_COLOR; ++i) {
        float xPosition = -0.905f + i * 0.26f;
        pieces.emplace_back(ChessPiece{ &blackPawn, CreatePieceTransformationMatrix(glm::vec3(xPosition, 0.0f, 0.637f), scale) });
        pieces.emplace_back(ChessPiece{ &grayPawn, CreatePieceTransformationMatrix(glm::vec3(xPosition, 0.0f, -0.637f), scale) });
    }
}

static void SetupOpenGLResources(unsigned int& cullingVAO, unsigned int& cullingVBO, unsigned int& skyboxVAO, unsigned int& skyboxVBO, unsigned int& cubemapTexture) {
    configureGLState();
    setupCullingVAO(cullingVAO, cullingVBO);
    setupSkyboxVAO(skyboxVAO, skyboxVBO);
    cubemapTexture = loadCubemap(faces);
}

static void RenderChessBoard(MyShader& shader, const Model& chessBoard, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {
    shader.use();
    glm::mat4 boardModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.1f, 0.0f));
    shader.SetUniformMat4fv("model", 1, boardModelMatrix, GL_FALSE);
    shader.SetUniformMat4fv("projection", 1, projectionMatrix, GL_FALSE);
    shader.SetUniformMat4fv("view", 1, viewMatrix, GL_FALSE);
    shader.SetUniform3f("cameraPos", camera_pos);
    chessBoard.Draw(shader, FIRST);
}

static void RenderChessPieces(MyShader& shader, const std::vector<ChessPiece>& pieces, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {
    shader.use();
    shader.SetUniformMat4fv("projection", 1, projectionMatrix, GL_FALSE);
    shader.SetUniformMat4fv("view", 1, viewMatrix, GL_FALSE);
    shader.SetUniform3f("cameraPos", camera_pos);

    for (const auto& piece : pieces) {
        if (piece.pieceModel) {
            shader.SetUniformMat4fv("model", 1, piece.transformationMatrix, GL_FALSE);
            piece.pieceModel->Draw(shader, FIRST);
        }
    }
}

static void RenderSkybox(MyShader& skyboxShader, unsigned int skyboxVAO, unsigned int cubemapTexture, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {
    glDepthFunc(GL_LEQUAL);
    skyboxShader.use();

    glm::mat4 skyboxModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    skyboxModelMatrix = glm::scale(skyboxModelMatrix, glm::vec3(SKYBOX_SCALE));

    skyboxShader.SetUniformMat4fv("model", 1, skyboxModelMatrix, GL_FALSE);
    skyboxShader.SetUniformMat4fv("view", 1, viewMatrix, GL_FALSE);
    skyboxShader.SetUniformMat4fv("projection", 1, projectionMatrix, GL_FALSE);

    glBindVertexArray(skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
}

static void RunGameLoop(GLFWwindow* window, MyShader& modelShader, MyShader& skyboxShader, const Model& chessBoard, const std::vector<ChessPiece>& pieces, unsigned int skyboxVAO, unsigned int cubemapTexture, unsigned int cullingVAO, unsigned int cullingVBO, unsigned int skyboxVBO) {
    while (!glfwWindowShouldClose(window)) {
        // Clear buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Update timing
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        // Process user input
        ProcessInput(window);

        // Setup view and projection matrices
        glm::mat4 viewMatrix = glm::lookAt(camera_pos, camera_pos + camera_front, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(width) / height, 0.1f, 100.0f);

        // Render scene
        RenderChessBoard(modelShader, chessBoard, projectionMatrix, viewMatrix);
        RenderChessPieces(modelShader, pieces, projectionMatrix, viewMatrix);
        RenderSkybox(skyboxShader, skyboxVAO, cubemapTexture, projectionMatrix, viewMatrix);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup resources
    cleanup(window, cullingVAO, cullingVBO, skyboxVAO, skyboxVBO, cubemapTexture);
}

int main() {
    // Initialize GLFW and GLAD
    GLFWwindow* gameWindow = InitializeWindowAndOpenGL();

    // Initialize shaders
    MyShader modelShader(VS_MODEL, FS_MODEL);
    MyShader skyboxShader(VS_SKY, FS_SKY);

    // Load chess models (using the sequential loading logic from the provided code)
    Model chessBoard(CHESS_BOARD);
    Model blackKing(BLACK_KING);
    Model grayKing(GRAY_KING);
    Model blackQueen(BLACK_QUEEN);
    Model grayQueen(GRAY_QUEEN);
    Model blackBishop(BLACK_BISHOP);
    Model grayBishop(GRAY_BISHOP);
    Model blackKnight(BLACK_KNIGHT);
    Model grayKnight(GRAY_KNIGHT);
    Model blackRook(BLACK_ROOK);
    Model grayRook(GRAY_ROOK);
    Model blackPawn(BLACK_PAWN);
    Model grayPawn(GRAY_PAWN);

    // Setup OpenGL resources
    unsigned int cullingVAO, cullingVBO, skyboxVAO, skyboxVBO, cubemapTexture;
    SetupOpenGLResources(cullingVAO, cullingVBO, skyboxVAO, skyboxVBO, cubemapTexture);

    // Setup chess pieces
    std::vector<ChessPiece> chessPieces;
    SetupChessPieces(chessPieces, chessBoard, blackKing, grayKing, blackQueen, grayQueen,
        blackBishop, grayBishop, blackKnight, grayKnight, blackRook, grayRook,
        blackPawn, grayPawn, PIECE_SCALE_FACTOR);

    // Run the game loop
    RunGameLoop(gameWindow, modelShader, skyboxShader, chessBoard, chessPieces, skyboxVAO, cubemapTexture, cullingVAO, cullingVBO, skyboxVBO);

    std::cin.get();
    return 0;
}



