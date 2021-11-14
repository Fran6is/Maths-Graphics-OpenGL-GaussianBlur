#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader.h>
#include <Texture.h>
#include <Blur.h>

#include <CMakeProjectMacros.h>

#define MOUSE_THUMB_BTN_2 4

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

//SD: Standard deviation
template<uint32_t GRID_DIMENSION_XY>
void UpdateShaderKernel(std::array< std::array<float, GRID_DIMENSION_XY>, GRID_DIMENSION_XY>& Out_Grid, float SD, const Shader& Program);


// Window settings
unsigned int SCR_WIDTH  = 800;
unsigned int SCR_HEIGHT = 600;

std::ostream& operator<<(std::ostream& cout, const glm::vec2& vector);
std::ostream& operator<<(std::ostream& cout, const glm::vec3& vector);
std::ostream& operator<<(std::ostream& cout, const glm::vec4& vector);
std::ostream& operator<<(std::ostream& cout, const glm::mat3& Matrix);
std::ostream& operator<<(std::ostream& cout, const glm::mat4& Matrix);

glm::vec2 PlaneVertexData[]
{
    //Positions             //Texture co-ordinates
    glm::vec2(-.49f, +.80f), glm::vec2(0.f, 1.f),
    glm::vec2(-.49f, -.80f), glm::vec2(0.f, 0.f),
    glm::vec2(+.49f, -.80f), glm::vec2(1.f, 0.f),
    glm::vec2(+.49f, +.80f), glm::vec2(1.f, 1.f)
};

unsigned int PlaneDrawIndices[]
{
    0, 1, 2,
    2, 3, 0
};

//To modify Kernel at Runtime
bool  bShouldBlur           = true;
float BlurStandardDeviation = 1.f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Gaussian Blur", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }   


    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneVertexData), PlaneVertexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PlaneDrawIndices), PlaneDrawIndices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2  * sizeof(PlaneVertexData[0]), (void*)0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(PlaneVertexData[0]), (void*)(sizeof(PlaneVertexData[0])));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    float DeltaTime = 0;
    float LastTime  = 0;

    Shader ShaderProgram(
        PROJECT_DIR"/ShaderPrograms/Quad.vert", 
        PROJECT_DIR"/ShaderPrograms/Quad.frag"
    );

    FTextureParameters TextureSetting;


    unsigned char* ReadImageDataPtr = nullptr;
    Texture Image( 
        FImageLoader::LoadImage(PROJECT_DIR"/Textures/ColorGrid.png", &ReadImageDataPtr), 
        FTextureParameters{}, 
        true
    );
    FImageLoader::FreeImage(ReadImageDataPtr);

    //Toggle blur
    glfwSetKeyCallback(window, [](GLFWwindow* WINDOW, int Key, int scancode, int keystate, int mods)
    {
        if(Key == GLFW_KEY_SPACE && keystate == GLFW_PRESS)
        {
            bShouldBlur = !bShouldBlur;

            printf("\nBlur filter is %s\n", 
                (bShouldBlur ? "ON  " : "OFF ")
            );
        }
    });

    //Modify standard deviation
    glfwSetScrollCallback(window, [](GLFWwindow* Window, double XOffset, double YOffset)
    {
        if(!bShouldBlur) return;
    
        const double STEP = 0.05;
        const double MIN  = 0.00001;
        const double MAX  = 5.0;
        
        BlurStandardDeviation = static_cast<float>( glm::clamp( BlurStandardDeviation + YOffset * STEP, MIN, MAX ) );

        printf("\nStandard Deviation = %.2f %s\n", BlurStandardDeviation, (bShouldBlur ? "" : "\t Blur filter is OFF") );

    });
    
    //Kernel
    const size_t GridSize = 7;
    std::array< std::array<float, GridSize>, GridSize > BlurKernel;
    BlurStandardDeviation = 0.84089642f;
    

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.01f, 0.05f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        Image.Bind();
        ShaderProgram.Use();
        ShaderProgram.SetVector2("UWindowDimension", glm::vec2(SCR_WIDTH, SCR_HEIGHT) );
        ShaderProgram.SetBool("UShouldBlur",   bShouldBlur );

        if(bShouldBlur)
        {
            UpdateShaderKernel(BlurKernel, BlurStandardDeviation, ShaderProgram);   
        }
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 2);

        float CurrentTime = (float)glfwGetTime();
        DeltaTime = CurrentTime - LastTime;
        LastTime  = CurrentTime;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

//SD: Standard deviation
template<uint32_t GRID_DIMENSION_XY>
void UpdateShaderKernel(std::array< std::array<float, GRID_DIMENSION_XY>, GRID_DIMENSION_XY>& Out_Grid, float SD, const Shader& Program)
{
    GenerateWeights(Out_Grid, SD);

    Program.Use();
    for(size_t Row = 0; Row < GRID_DIMENSION_XY; Row++)
    {
        for(size_t Column = 0; Column < GRID_DIMENSION_XY; Column++)
        {
            char Buffer[20]{};

            sprintf(Buffer, "UKernel[%i][%i]", Row, Column);
            Program.SetFloat(Buffer, Out_Grid[Row][Column] );
        }
    }

    Program.SetInt("UGridSize", Out_Grid.size() );
    
    
}



void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH  = width;
    SCR_HEIGHT = height;
}

std::ostream& operator<<(std::ostream& cout, const glm::vec2& vector)
{
    printf("< %3.2f, %3.2f >", vector.x, vector.y);
    return cout;
}

std::ostream& operator<<(std::ostream& cout, const glm::vec3& vector)
{
    printf("< %3.2f, %3.2f, %3.2f >", vector.x, vector.y, vector.z);
    return cout;
}

std::ostream& operator<<(std::ostream& cout, const glm::vec4& vector)
{
    printf("< %3.2f, %3.2f, %3.2f, %3.2f >", vector.x, vector.y, vector.z, vector.w);

    return cout;
}

std::ostream& operator<<(std::ostream& cout, const glm::mat3& Matrix)
{
    std::cout << "========================================MATRIX==========================================\n";
    printf("   X|   Y|    Z|  \n");
    printf("%3.2f, %3.2f, %3.2f\n", Matrix[0][0], Matrix[1][0], Matrix[2][0] );
    printf("%3.2f, %3.2f, %3.2f\n", Matrix[0][1], Matrix[1][1], Matrix[2][1] );
    printf("%3.2f, %3.2f, %3.2f\n", Matrix[0][2], Matrix[1][2], Matrix[2][2] );
    cout << "-------------------------------\n";
    cout << "X = " << Matrix[0] << "\n";
    cout << "Y = " << Matrix[1] << "\n";
    cout << "Z = " << Matrix[2] << "\n";
    std::cout << "=======================================================================================\n";

    return cout;
}

std::ostream& operator<<(std::ostream& cout, const glm::mat4& Matrix)
{
    std::cout << "========================================MATRIX==========================================\n";
    printf("   X|   Y|    Z|     W|  \n");
    printf("%3.2f, %3.2f, %3.2f, %3.2f\n", Matrix[0][0], Matrix[1][0], Matrix[2][0], Matrix[3][0] );
    printf("%3.2f, %3.2f, %3.2f, %3.2f\n", Matrix[0][1], Matrix[1][1], Matrix[2][1], Matrix[3][1] );
    printf("%3.2f, %3.2f, %3.2f, %3.2f\n", Matrix[0][2], Matrix[1][2], Matrix[2][2], Matrix[3][2] );
    printf("%3.2f, %3.2f, %3.2f, %3.2f\n", Matrix[0][3], Matrix[1][3], Matrix[2][3], Matrix[3][3] );
    cout << "-------------------------------\n";
    cout << "X = " << Matrix[0] << "\n";
    cout << "Y = " << Matrix[1] << "\n";
    cout << "Z = " << Matrix[2] << "\n";
    cout << "W = " << Matrix[3] << "\n";
    std::cout << "=======================================================================================\n";


    return cout;
}