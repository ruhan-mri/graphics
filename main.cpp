//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionalLight.h"
#include "spotLight.h"
#include "cube.h"
#include "stb_image.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
//void bed(Shader& lightingShader, glm::mat4 alTogether, Cube& cube);
void floor(Shader& lightingShaderWithTexture, glm::mat4 moveMatrix, Cube& cube);
void box(Shader& lightingShaderWithTexture, Cube& cube, glm::mat4 model);
void borderx(Shader& lightingShaderWithTexture, Cube& cube, glm::mat4 model);
void bordery(Shader& lightingShaderWithTexture, Cube& cube, glm::mat4 model);
void table(Shader& lightingShaderWithTexture, Cube& cube, glm::mat4 model);
void chair(Shader& lightingShaderWithTexture, Cube& cube, glm::mat4 model);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// camera
Camera camera(glm::vec3(0.0f, 2.0f, 2.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// lights
// positions of the point lights
//glm::vec3 pointLightPositions[] = {
//    glm::vec3(1.50f,  1.50f,  0.0f),
//    glm::vec3(1.5f,  -1.5f,  0.0f),
//    glm::vec3(-1.5f,  1.5f,  0.0f),
//    glm::vec3(-1.5f,  -1.5f,  0.0f)
//};
//PointLight pointlight1(
//
//    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
//    0.05f, 0.05f, 0.05f,     // ambient
//    0.8f, 0.8f, 0.8f,     // diffuse
//    1.0f, 1.0f, 1.0f,        // specular
//    1.0f,   //k_c
//    0.09f,  //k_l
//    0.032f, //k_q
//    1       // light number
//);
//PointLight pointlight2(
//
//    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
//    0.05f, 0.05f, 0.05f,     // ambient
//    0.8f, 0.8f, 0.8f,     // diffuse
//    1.0f, 1.0f, 1.0f,        // specular
//    1.0f,   //k_c
//    0.09f,  //k_l
//    0.032f, //k_q
//    2       // light number
//);
//PointLight pointlight3(
//
//    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
//    0.05f, 0.05f, 0.05f,     // ambient
//    0.8f, 0.8f, 0.8f,     // diffuse
//    1.0f, 1.0f, 1.0f,        // specular
//    1.0f,   //k_c
//    0.09f,  //k_l
//    0.032f, //k_q
//    3       // light number
//);
//PointLight pointlight4(
//
//    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
//    0.05f, 0.05f, 0.05f,     // ambient
//    0.8f, 0.8f, 0.8f,     // diffuse
//    1.0f, 1.0f, 1.0f,        // specular
//    1.0f,   //k_c
//    0.09f,  //k_l
//    0.032f, //k_q
//    4       // light number
//);

// light initialization

glm::vec3 lightPositions[] = {
        glm::vec3(8.0f, 4.0f, 8.0f),
        glm::vec3(-8.0f, 4.0f, 8.0f),
        glm::vec3(-8.0f, 4.0f, -8.0f),
        glm::vec3(8.0f, 4.0f, -8.0f),
        glm::vec3(0.0f, 4.0f, 0.0f),
        glm::vec3(-2.3f, 2.5f, 4.8f)
};

glm::vec3 lightDirections[] = {
    glm::vec3(0.4f, -0.2f, -1.0f)
};


PointLight pointLight1(

    lightPositions[0].x, lightPositions[0].y, lightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);

PointLight pointLight2(

    lightPositions[1].x, lightPositions[1].y, lightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);

PointLight pointLight3(

    lightPositions[2].x, lightPositions[2].y, lightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);

PointLight pointLight4(

    lightPositions[3].x, lightPositions[3].y, lightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);

PointLight pointLight5(

    lightPositions[4].x, lightPositions[4].y, lightPositions[4].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    5       // light number
);

DirectionalLight directionalLight(

    lightPositions[4].x, lightPositions[4].y, lightPositions[4].z,  // direction
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1           // light number
);

SpotLight spotLight(

    lightPositions[5].x, lightPositions[5].y, lightPositions[5].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    lightDirections[5].x, 0.0f, lightDirections[5].z,  // direction
    8.5f,   // cutoff
    15.5f,   // outer cutoff
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);

// light settings
bool dirLightOn = true;
//bool pointLightOn = true;
//bool spotLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;

bool lightingOn = false;
float ambientOn = 1.0;
float diffuseOn = 1.0;
float specularOn = 1.0;
bool dark = false;

float directionalLightOn = 0.0;
float pointLightOn = 1.0;
float spotLightOn = 0.0;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    //axis line 

    float axisVertices[] = {
        // X-axis
        0.0f, 0.0f, 0.0f,   // Start point
        10.0f, 0.0f, 0.0f,   // End point

        // Y-axis
        0.0f, 0.0f, 0.0f,   // Start point
        0.0f, 10.0f, 0.0f,   // End point

        // Z-axis
        0.0f, 0.0f, 0.0f,   // Start point
        0.0f, 0.0f, 10.0f    // End point
    };


    //axis line VBO,VAO
    unsigned int axisVAO, axisVBO;
    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);

    // Bind the axis VAO and VBO
    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    string diffuseMapPath = "pic_grass.jpg";
    string specularMapPath = "container2_specular.png";


    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 92.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "pic_brickwall_1.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube1 = Cube(diffMap, specMap, 92.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "pic_brickwall_2.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube2 = Cube(diffMap, specMap, 92.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "pic_road.jpg";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube3 = Cube(diffMap, specMap, 92.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath = "pic_grass_2.png";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube4 = Cube(diffMap, specMap, 92.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    //Sphere sphere = Sphere(); pic_grass_2.png

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float x[] = { 8.5f, 8.5f, 8.5f ,
        4.0f, 0.0f,-4.0f,  -8.5f, 
        -8.5f, 8.5f };
    float z[] = { 0.0f, 4.5f, 8.5f,
        8.5f, 8.5f, 8.5f, 8.5f,
        -8.5f, -8.5f };
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        lightingShaderWithTexture.use();

        // light shader
        pointLight1.setUpPointLight(lightingShaderWithTexture);
        pointLight2.setUpPointLight(lightingShaderWithTexture);
        pointLight3.setUpPointLight(lightingShaderWithTexture);
        pointLight4.setUpPointLight(lightingShaderWithTexture);
        pointLight5.setUpPointLight(lightingShaderWithTexture);
        directionalLight.setUpLight(lightingShaderWithTexture);
        spotLight.setUpLight(lightingShaderWithTexture);



        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        floor(lightingShaderWithTexture, translateMatrix, cube);

        /*translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        box(lightingShaderWithTexture, cube1, translateMatrix);*/

        //bounder

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 10.0f));
        borderx(lightingShaderWithTexture, cube2, translateMatrix);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, -10.0f));
        borderx(lightingShaderWithTexture, cube2, translateMatrix);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 0.0f, 0.0f));
        bordery(lightingShaderWithTexture, cube2, translateMatrix);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-10.0f, 0.0f, 0.0f));
        bordery(lightingShaderWithTexture, cube2, translateMatrix);

        //table
        
        for (int tab = 0; tab < 9; tab++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(x[tab], 0.0f, z[tab]));
            table(lightingShaderWithTexture, cube1, translateMatrix);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(x[tab], 0.0f, z[tab]));
            chair(lightingShaderWithTexture, cube4, translateMatrix);
        }
        



        

            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPositions[0]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            cube.drawCube(lightingShaderWithTexture, model);

            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPositions[1]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            cube.drawCube(lightingShaderWithTexture, model);

            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPositions[2]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            cube.drawCube(lightingShaderWithTexture, model);

            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPositions[3]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            cube.drawCube(lightingShaderWithTexture, model);

            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPositions[4]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            cube.drawCube(lightingShaderWithTexture, model);

            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPositions[5]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            cube.drawCube(lightingShaderWithTexture, model);



        //Setting up Directional Light
        directionalLight.setUpLight(lightingShaderWithTexture);
        if (!directionalLightOn)
            directionalLight.turnOff();
        if (!ambientOn)
            directionalLight.turnAmbientOff();
        if (!diffuseOn)
            directionalLight.turnDiffuseOff();
        if (!specularOn)
            directionalLight.turnSpecularOff();

        //Setting up Point Light
        {
            pointLight1.setUpPointLight(lightingShaderWithTexture);
            if (!pointLightOn)
                pointLight1.turnOff();
            if (!ambientOn)
                pointLight1.turnAmbientOff();
            if (!diffuseOn)
                pointLight1.turnDiffuseOff();
            if (!specularOn)
                pointLight1.turnSpecularOff();

            pointLight2.setUpPointLight(lightingShaderWithTexture);
            if (!pointLightOn)
                pointLight2.turnOff();
            if (!ambientOn)
                pointLight2.turnAmbientOff();
            if (!diffuseOn)
                pointLight2.turnDiffuseOff();
            if (!specularOn)
                pointLight2.turnSpecularOff();

            pointLight3.setUpPointLight(lightingShaderWithTexture);
            if (!pointLightOn)
                pointLight3.turnOff();
            if (!ambientOn)
                pointLight3.turnAmbientOff();
            if (!diffuseOn)
                pointLight3.turnDiffuseOff();
            if (!specularOn)
                pointLight3.turnSpecularOff();

            pointLight4.setUpPointLight(lightingShaderWithTexture);
            if (!pointLightOn)
                pointLight4.turnOff();
            if (!ambientOn)
                pointLight4.turnAmbientOff();
            if (!diffuseOn)
                pointLight4.turnDiffuseOff();
            if (!specularOn)
                pointLight4.turnSpecularOff();

            pointLight5.setUpPointLight(lightingShaderWithTexture);
            if (!pointLightOn)
                pointLight5.turnOff();
            if (!ambientOn)
                pointLight5.turnAmbientOff();
            if (!diffuseOn)
                pointLight5.turnDiffuseOff();
            if (!specularOn)
                pointLight5.turnSpecularOff();
        }
        

        //Setting up Spot Light
        spotLight.setUpLight(lightingShaderWithTexture);
        if (!spotLightOn)
            spotLight.turnOff();
        if (!ambientOn)
            spotLight.turnAmbientOff();
        if (!diffuseOn)
            spotLight.turnDiffuseOff();
        if (!specularOn)
            spotLight.turnSpecularOff();

        //Setting up Camera and Others
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setBool("lightingOn", lightingOn);
        lightingShaderWithTexture.setFloat("ambientOn", ambientOn);
        lightingShaderWithTexture.setFloat("diffuseOn", diffuseOn);
        lightingShaderWithTexture.setFloat("specularOn", specularOn);
        lightingShaderWithTexture.setFloat("pointLightOn", pointLightOn);
        lightingShaderWithTexture.setFloat("directionalLightOn", directionalLightOn);
        lightingShaderWithTexture.setFloat("spotLightOn", spotLightOn);
        lightingShaderWithTexture.setBool("dark", dark);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {             // Forward
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {             // Backword
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {             // Left
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {             // Right
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {             // Up
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {             // Down
        camera.ProcessKeyboard(DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {             // Pitch Positive
        camera.ProcessYPR(0.0f, 0.1f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {             // Pitch Negative
        camera.ProcessYPR(0.0f, -0.1f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {             // Yaw Positive
        camera.ProcessYPR(0.1f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {             // Yaw Negative
        camera.ProcessYPR(-0.1f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {             // Roll Positive
        camera.ProcessYPR(0.0f, 0.0f, 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {             // Roll Negative
        camera.ProcessYPR(0.0f, 0.0f, -0.1f);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {             // Rotate camera around a look at point Right
        camera.RotateAroundLookAt(0.1f);
    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {             // Rotate camera around a look at point Left
        camera.RotateAroundLookAt(-0.1f);
    }

    /*if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        fan_on = !fan_on;
    }*/
    
    

    //***************Lighting***************

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)                   //Lighting On-Off
{
        lightingOn ^= true;
        cout << lightingOn << endl;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)                   //Ambient On
    {
        ambientOn = 1.0;
        pointLight1.turnAmbientOn();
        pointLight2.turnAmbientOn();
        pointLight3.turnAmbientOn();
        pointLight4.turnAmbientOn();
        pointLight5.turnAmbientOn();
        spotLight.turnAmbientOn();
        directionalLight.turnAmbientOn();
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)                   //Ambient Off
    {
        ambientOn = 0.0;
        pointLight1.turnAmbientOff();
        pointLight2.turnAmbientOff();
        pointLight3.turnAmbientOff();
        pointLight4.turnAmbientOff();
        pointLight5.turnAmbientOff();
        spotLight.turnAmbientOff();
        directionalLight.turnAmbientOff();
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)                   //Diffuse On
    {
        diffuseOn = 1.0;
        pointLight1.turnDiffuseOn();
        pointLight2.turnDiffuseOn();
        pointLight3.turnDiffuseOn();
        pointLight4.turnDiffuseOn();
        pointLight5.turnDiffuseOn();
        spotLight.turnDiffuseOn();
        directionalLight.turnDiffuseOn();

    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)                   //Diffuse Off
    {
        diffuseOn = 0.0;
        pointLight1.turnDiffuseOff();
        pointLight2.turnDiffuseOff();
        pointLight3.turnDiffuseOff();
        pointLight4.turnDiffuseOff();
        pointLight5.turnDiffuseOff();
        spotLight.turnDiffuseOff();
        directionalLight.turnDiffuseOff();
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)                   //Specular On
    {
        specularOn = 1.0;
        pointLight1.turnSpecularOn();
        pointLight2.turnSpecularOn();
        pointLight3.turnSpecularOn();
        pointLight4.turnSpecularOn();
        pointLight5.turnSpecularOn();
        spotLight.turnSpecularOn();
        directionalLight.turnSpecularOn();
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)                   //Specular Off
    {
        specularOn = 0.0;
        pointLight1.turnSpecularOff();
        pointLight2.turnSpecularOff();
        pointLight3.turnSpecularOff();
        pointLight4.turnSpecularOff();
        pointLight5.turnSpecularOff();
        spotLight.turnSpecularOff();
        directionalLight.turnSpecularOff();
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)                   //Point Light On
    {
        pointLightOn = 1.0;
        pointLight1.turnOn();
        pointLight2.turnOn();
        pointLight3.turnOn();
        pointLight4.turnOn();
        pointLight5.turnOn();
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)                   //Point Light Off
    {
        pointLightOn = 0.0;
        pointLight1.turnOff();
        pointLight2.turnOff();
        pointLight3.turnOff();
        pointLight4.turnOff();
        pointLight5.turnOff();
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)                   //Directional Light On
    {
        directionalLightOn = 1.0;
        directionalLight.turnOn();
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)                   //Directional Light Off
    {
        directionalLightOn = 0.0;
        directionalLight.turnOff();
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)                   //Spot Light On
    {
        spotLightOn = 1.0;
        spotLight.turnOn();
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)                   //Spot Light Off
    {
        spotLightOn = 0.0;
        spotLight.turnOff();
    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)                   //Dark On-Off
    {
        dark ^= true;
        cout << dark << endl;
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointLight1.turnOff();
            pointLight2.turnOff();
            pointLight3.turnOff();
            pointLight4.turnOff();
            pointLight5.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointLight1.turnOn();
            pointLight2.turnOn();
            pointLight3.turnOn();
            pointLight4.turnOn();
            pointLight5.turnOn();
            pointLightOn = !pointLightOn;
        }
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void floor(Shader& lightingShaderWithTexture, glm::mat4 moveMatrix, Cube& cube)
{
    
    glm::mat4 translateMatrix, scaleMatrix, model;
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(20.0f, 0.01f, 20.0f));
    model = moveMatrix * translateMatrix * scaleMatrix;
    cube.drawCubeWithTexture(lightingShaderWithTexture, model);
}

void box(Shader& lightingShaderWithTexture, Cube& cube, glm::mat4 model)
{
    glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(2.0f, 1.0f, 2.0f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
}

void borderx(Shader& lightingShaderWithTexture, Cube& cube, glm::mat4 model)
{
    glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(20.0f, 2.0f, 0.01f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
}

void bordery(Shader& lightingShaderWithTexture, Cube& cube, glm::mat4 model)
{
    glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.01f, 2.0f, 20.0f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
}

void table(Shader& lightingShaderWithTexture, Cube& cube, glm::mat4 model)
{
    glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
    modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(1.0f, 0.01f, 1.0f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

    // leg
    modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(.45f, 0.25f, 0.45f)); // 1, 0, .5
    modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.05f, 0.5f, 0.05f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

    modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(-0.45f, 0.25f, 0.45f)); // 1, 0, .5
    modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.05f, 0.5f, 0.05f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

    modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(-.45f, 0.25f, -0.45f)); // 1, 0, .5
    modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.05f, 0.5f, 0.05f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

    modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(.45f, 0.25f, -0.45f)); // 1, 0, .5
    modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.05f, 0.5f, 0.05f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
}


void chair(Shader& lightingShaderWithTexture, Cube& cube, glm::mat4 model)
{
    glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.75f, 0.125f, 0.0f));
    modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.35f, 0.45f, 0.35f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

    modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(-0.75f, 0.125f, 0.0f));
    modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.35f, 0.45f, 0.35f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

    modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.125f, 0.75f));
    modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.35f, 0.45f, 0.35f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

    modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.125f, -0.75f));
    modelMatrixForContainer = glm::scale(modelMatrixForContainer, glm::vec3(0.35f, 0.45f, 0.35f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
}