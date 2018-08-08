//  main.cpp
//  Final Project Graphics
//
//  Created by Charlie Barber on 4/9/17.
//  Copyright © 2017 Charlie Barber. All rights reserved.
//
#ifdef __APPLE__

#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLFW/glfw3.h>
#include "cyTriMesh.h"
#include "cyGL.h"
#include "cyMatrix.h"

#else

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include "cyTriMesh.h"
#include "cyGL.h"
#include "cyMatrix.h"

#endif

#include "lodepng.hpp"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <iostream>
#include "ext.hpp"

using namespace std;
using namespace lodepng;
using namespace glm;

const int WIDTH = 1000;
const int HEIGHT = 800;
GLFWwindow* window;

//NORMAL TEAPOT VARIABLES
//**********
cyTriMesh teapotTriMesh;
vector<cyPoint3f> teapot_vert_array;
vector<cyPoint3f> teapot_normals_array;
vector<cyPoint3f> teapot_texture_coordinates_array;
GLuint teapotVAO;
GLuint teapotVBO;
GLuint teapotNBO;
GLuint teapotTBO;
//vec3 teapot_Position = vec3(0.0f, -3.0f, 10.0f);
vec3 teapot_Position = vec3(0.0f, -0.75f, 0.0f);
GLuint normal_map_shader_program;
mat4 teapotModelMatrix;
mat4 teapotRotationMatrix;
mat4 teapotScaleMatrix;
mat4 teapotTranslationMatrix;
GLuint teapotTexture;
GLuint teapotNormalMapTexture;
GLuint teapotDispMapTexture;
vector<unsigned char> brickImage;
vector<unsigned char> brickNormalMapImage;
vector<unsigned char> brickDisplMapImage;
//**********


//TANGENT VARIABLES
GLuint tangentVBO;
GLuint bitangentVBO;
vector<cyPoint3f> tangent_coordinates_array;
vector<cyPoint3f> bitangent_coordinates_array;


GLuint framebuffer;
GLuint texColorBuffer;
GLuint rbo;
GLuint renderedTexture;

//LIGHT AND CAMERA
//**********
vec3 cameraPos = vec3(0.0f, 0.0f, 3.5f);
vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

vec2 camera_rot = vec2(0.0,0.0);
GLfloat camera_radius = 3.0;
vec3 camera_center = vec3(0.0,0.0,0.0);
//**********
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;
GLfloat mouseYaw    = -90.0f;
GLfloat mousePitch  =  0.0f;
GLfloat lastX  =  WIDTH  / 2.0;
GLfloat lastY  =  HEIGHT / 2.0;
GLfloat fov =  45.0f;

//VIEW AND PROJECTION MATRIX VARIABLES
//**********
mat4 viewMatrix;
mat4 projectionMatrix;
//**********

//CUBE VARIABLES
cyTriMesh triMesh;
GLuint cubeVAO;
GLuint cubeVBO;
vector<cyPoint3f> cube_Vertex_Array;
vector<cyPoint3f> cube_normals_Array;
GLuint cubeNormalsBuffer;
mat4 cube_Scale;
mat4 cube_Translation;
mat4 cube_Rotation;
vec3 cube_Position = glm::vec3(0.0f,-7.0f,0.0f);
mat4 model;


//ROTATE VARIABLES
//**********
vec3 RotateX = vec3(1.0f, 0.0f, 0.0f);
vec3 RotateY = vec3(0.0f, 1.0f, 0.0f);
vec3 RotateZ = vec3(0.0f, 0.0f, 1.0f);
GLfloat startXDegrees = radians(-70.0f);
GLfloat startZDegrees = radians(-30.0f);
GLfloat posDegrees = radians(5.0f);
GLfloat negDegrees = radians(-5.0f);
GLboolean startZRotation = false;
GLboolean startNegZRotation = false;
GLboolean startYRotation = false;
GLboolean startNegYRotation = false;
GLboolean startXRotation = false;
GLboolean startNegXRotation = false;
//**********

//FUNCTIONS
//**********

void initWindow();
int generateVertexBufferAndArray();
void loadShadersAndDraw();
vector<cyPoint3f> makeArrayOfTriangleVerts(vector<cyPoint3f>, cyTriMesh);
vector<cyPoint3f> loadNormals(vector<cyPoint3f>, cyTriMesh);
vector<cyPoint3f> loadTexCoord(vector<cyPoint3f>, cyTriMesh);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void calcTanAndBiTan();
//**********




int main(int argc, const char * argv[])
{
    const char* file = argv[1];
    
    teapotTriMesh.LoadFromFileObj(file);
    
    teapot_vert_array = makeArrayOfTriangleVerts(teapot_vert_array, teapotTriMesh);
    
    teapot_normals_array = loadNormals(teapot_normals_array, teapotTriMesh);
    
    teapot_texture_coordinates_array = loadTexCoord(teapot_texture_coordinates_array, teapotTriMesh);
    
    glfwInit();
    
    initWindow();
    
    generateVertexBufferAndArray();
    
    loadShadersAndDraw();
    
    glfwTerminate();
    
    return 0;
    
}

void initWindow()

{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow( WIDTH, HEIGHT, "Final Project - Steep Parallax Map", NULL, NULL);
    
    
    glfwMakeContextCurrent(window);
    
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    
    //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glEnable(GL_DEPTH_TEST);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
}

int generateVertexBufferAndArray()
{
    glewExperimental = GL_TRUE;
    glewInit();
    
    //NORMAL TEAPOT VAO AND VBO
    glGenVertexArrays(1, &teapotVAO);
    glBindVertexArray(teapotVAO);
    
    glGenBuffers(1, &teapotVBO);
    glBindBuffer(GL_ARRAY_BUFFER, teapotVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cyPoint3f)*teapot_vert_array.size(), &teapot_vert_array.at(0), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glGenBuffers(1, &teapotNBO);
    glBindBuffer(GL_ARRAY_BUFFER, teapotNBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cyPoint3f)*teapot_normals_array.size(), &teapot_normals_array.at(0), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glGenBuffers(1, &teapotTBO);
    glBindBuffer(GL_ARRAY_BUFFER, teapotTBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cyPoint3f)*teapot_texture_coordinates_array.size(), &teapot_texture_coordinates_array.at(0), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    
    calcTanAndBiTan();
    
    //TANGENT VBO
    glGenBuffers(1, &tangentVBO);
    glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cyPoint3f)*tangent_coordinates_array.size(), &tangent_coordinates_array.at(0), GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    //BITANGENT VBO
    glGenBuffers(1, &bitangentVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cyPoint3f)*bitangent_coordinates_array.size(), &bitangent_coordinates_array.at(0), GL_STATIC_DRAW);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    return 0;
    
}

void loadShadersAndDraw()

{
    //TEAPOT CODE
    //*****************************
    //*                           *
    //*                           *
    //*****************************

    const char* normal_map_vertex_shader =
    "#version 410\n"
    "layout (location = 0) in vec3 position;"
    "layout (location = 1) in vec3 normal;"
    "layout (location = 2) in vec3 textureCoordinates;"
    "layout (location = 3) in vec3 tangent;"
    "layout (location = 4) in vec3 bitangent;"
    
    "uniform mat4 model, projection, view;"
    
    "out vec3 FragPos, FragTex, TangentLightPos, TangentCamPos, TangentFragPos, T, B, N;"
    
//    "vec3 lightPos = vec3(0.0f, 1.0f, 3.0f);"
    "vec3 lightPos = vec3(0.0f, 1.0f, 3.0f);"
    "vec3 camPos = vec3(0.0f, 0.0f, 3.5f);"
    
    
    
    "void main(){"
    
    "   gl_Position = projection * view * model * vec4(position, 1.0);"
    "   FragPos = vec3(model * vec4(position, 1.0));;"
    "   FragTex = textureCoordinates;"
    
    
    "   mat3 normalMatrix = transpose(inverse(mat3(model)));"
    "   T = normalize(normalMatrix * tangent);"
    "   B = normalize(normalMatrix * bitangent);"
    "   N = normalize(normalMatrix * normal);"
    
    "   mat3 TBN = transpose(mat3(T, B, N));"
    "   TangentLightPos = TBN * lightPos;"
    "   TangentCamPos = TBN * camPos;"
    "   TangentFragPos = TBN * FragPos;"
    
    
    "}";
    
    
    const char* normal_map_fragment_shader =
    "#version 410\n"
    "out vec4 FragColor;"
    
    "in vec3 FragPos, FragTex, TangentLightPos, TangentCamPos, TangentFragPos, T, B, N;"
    
    //    "uniform vec3 lightPos, camPos;"
    
    "uniform sampler2D teapotTexture, normalMap, depthMap;"
    "float heightScale = 0.1;"
    
    
    "void main()"
    "{"

    "   vec3 viewDir = normalize(TangentCamPos - TangentFragPos);"

    "   vec3 currentTexCoordinates = FragTex;"
    
    "   const float numLayers = 10;"
    "   float layerDepth = 1.0 / numLayers;"
    "   float currentDepth = 0.0;"
    
    "   vec3 pVector = viewDir * heightScale;"
    "   vec3 shiftCoordinates = pVector / numLayers;"
    
    "   float currentHeight = texture(depthMap, currentTexCoordinates.xy).r;"
    
    "   while(currentDepth < currentHeight)"
    "   {"
    
    "   currentTexCoordinates -= shiftCoordinates;"
    
    "   currentHeight = texture(depthMap, currentTexCoordinates.xy).r;"
    
    "   currentDepth += layerDepth;"
    
    "   }"


//    "   vec3 dTexture = texture(depthMap, currentTexCoordinates.xy).rgb;"

    // Obtain normal from normal map in range [0,1]
//        "  vec3 normal = texture(normalMap, vec2(FragTex.x, 1024 - FragTex.y)).rgb;"
//    "  vec3 normal = texture(normalMap, newOffsetFragTex.xy).rgb;"
    "  vec3 normal = texture(normalMap, currentTexCoordinates.xy).rgb;"

    // Transform normal vector to range [-1,1]
    "   normal = normalize(normal * 2.0 - 1.0);"
    
    
//    "   vec3 color = texture(teapotTexture, newOffsetFragTex.xy).rgb;"
    "   vec3 color = texture(teapotTexture, currentTexCoordinates.xy).rgb;"

//        "   vec3 color = vec3(0.0,0.0,1.0);"

    // Ambient
    "   vec3 ambient = 0.2 * color;"
    // Diffuse
    "   vec3 lightDir = normalize(TangentLightPos - TangentFragPos);"
    "   float diff = max(dot(lightDir, normal), 0.0);"
    "   vec3 diffuse = diff * color;"
    // Specular
    "   vec3 reflectDir = reflect(-lightDir, normal);"
    "   float spec = 0.0;"
    
    "   vec3 halfwayDir = normalize(lightDir + viewDir);"
    "   spec = pow(max(dot(normal, halfwayDir), 0.0), 256.0);"
    "   vec3 specular = vec3(1.5) * spec;"
    
    "   FragColor = vec4(ambient + diffuse + specular, 1.0f);"
//    "   FragColor = vec4(dTexture, 1.0f);"
//    "   FragColor = vec4(normal, 1.0f);"
    "}";
    
    
    GLuint normalMapVS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(normalMapVS, 1, &normal_map_vertex_shader, NULL);
    glCompileShader(normalMapVS);
    
    GLuint normalMapFS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(normalMapFS, 1, &normal_map_fragment_shader, NULL);
    glCompileShader(normalMapFS);
    
    normal_map_shader_program = glCreateProgram();
    glAttachShader(normal_map_shader_program, normalMapVS);
    glAttachShader(normal_map_shader_program, normalMapFS);
    glLinkProgram(normal_map_shader_program);
    
    glUseProgram(normal_map_shader_program);
    
    
    //TEAPOT CODE
    //*****************************
    //*                           *
    //*                           *
    //*****************************
    //Translate
    
    //Scale
    teapotScaleMatrix = scale(vec3(0.1f, 0.1f, 0.1f));
    
    //    teapotRotationMatrix = rotate(teapotRotationMatrix, startZDegrees, RotateZ);
    teapotRotationMatrix = rotate(teapotRotationMatrix, startXDegrees, RotateX);
    
    projectionMatrix = perspective(radians(60.0f), (GLfloat)WIDTH/(GLfloat)HEIGHT, 1.0f, 500.0f);
    GLuint projection_matrix_location = glGetUniformLocation(normal_map_shader_program, "projection");
    glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, &projectionMatrix[0][0]);
    
    
    //Teapot Texture
    //***************
    //Teapot Texture
    //***************
    unsigned int width, height;
    
//          lodepng::decode(brickImage, width, height, "/Users/CharlieBarber/Desktop/teapot/brick.png");
    lodepng::decode(brickImage, width, height, "/Users/CharlieBarber/Desktop/bricks.png");
//    lodepng::decode(brickImage, width, height, "/Users/CharlieBarber/Desktop/wood.png");

    
    glGenTextures(1, &teapotTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, teapotTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, brickImage.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    GLuint textureLocation = glGetUniformLocation(normal_map_shader_program, "teapotTexture");
    glUniform1i(textureLocation, 0);
    
    //***************
    
    //Teapot NormalMap Texture
    //***************
    //***************
    
    unsigned int normWidth, normHeight;
    
    lodepng::decode(brickNormalMapImage, normWidth, normHeight, "/Users/CharlieBarber/Desktop/bricks_normal.png");
//    lodepng::decode(brickNormalMapImage, normWidth, normHeight, "/Users/CharlieBarber/Desktop/brickwall_normal.png");
//    lodepng::decode(brickNormalMapImage, normWidth, normHeight, "/Users/CharlieBarber/Desktop/Hexagon.png");
//        lodepng::decode(brickNormalMapImage, normWidth, normHeight, "/Users/CharlieBarber/Desktop/toy_box_normal.png");


    
    glGenTextures(1, &teapotNormalMapTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, teapotNormalMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, normWidth, normHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, brickNormalMapImage.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    GLuint normalTextureLocation = glGetUniformLocation(normal_map_shader_program, "normalMap");
    glUniform1i(normalTextureLocation, 1);
    
    
    //DEPTH MAP TEXTURE
    //*******************
    //*******************

    unsigned int depthWidth, depthHeight;
    
    lodepng::decode(brickDisplMapImage, depthWidth, depthHeight, "/Users/CharlieBarber/Desktop/bricks_displacement.png");
//    lodepng::decode(brickDisplMapImage, depthWidth, depthHeight, "/Users/CharlieBarber/Desktop/toy_box_disp.png");

    
    glGenTextures(1, &teapotDispMapTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, teapotDispMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, normWidth, normHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, brickDisplMapImage.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    GLuint depthTextureLocation = glGetUniformLocation(normal_map_shader_program, "depthMap");
    glUniform1i(depthTextureLocation, 2);

    //***************
    //***************
    
    GLfloat cameraSpeed = 0.01f;
    
    
    while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 )
    {
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glfwPollEvents();
        
        glViewport(0,0,WIDTH, HEIGHT);
        
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(normal_map_shader_program);
        
        
        glBindVertexArray(teapotVAO);
        
        teapotTranslationMatrix =  translate(teapot_Position);
        
        if(startZRotation)
            teapotRotationMatrix = rotate(teapotRotationMatrix, posDegrees, RotateZ);
        if(startXRotation)
            teapotRotationMatrix = rotate(teapotRotationMatrix, posDegrees, RotateX);
        if(startYRotation)
            teapotRotationMatrix = rotate(teapotRotationMatrix, posDegrees, RotateY);
        if(startNegZRotation)
            teapotRotationMatrix = rotate(teapotRotationMatrix, negDegrees, RotateZ);
        if(startNegXRotation)
            teapotRotationMatrix = rotate(teapotRotationMatrix, negDegrees, RotateX);
        if(startNegYRotation)
            teapotRotationMatrix = rotate(teapotRotationMatrix, negDegrees, RotateY);
        
        
        //Model Matrix
        teapotModelMatrix = teapotTranslationMatrix * teapotRotationMatrix * teapotScaleMatrix;
        GLuint model_Uniform_Location = glGetUniformLocation(normal_map_shader_program, "model");
        glUniformMatrix4fv(model_Uniform_Location, 1, GL_FALSE, &teapotModelMatrix[0][0]);
        viewMatrix = lookAt(cameraPos, cameraPos + cameraTarget, cameraUp);
        
        
        GLuint view_matrix_location = glGetUniformLocation(normal_map_shader_program, "view");
        glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, &viewMatrix[0][0]);
        
        glDrawArrays(GL_TRIANGLES, 0, teapot_vert_array.size());
        
        glfwSwapBuffers(window);
        
    }
}

vector<cyPoint3f> makeArrayOfTriangleVerts(vector<cyPoint3f> vertArray, cyTriMesh triMesh)
{
    const cyTriMesh::TriFace *face = &triMesh.F(0);
    
    for(int i = 0; i < triMesh.NF(); i++)
    {
        const cyPoint3f point1Verts = triMesh.V(face->v[0]);
        const cyPoint3f point2Verts = triMesh.V(face->v[1]);
        const cyPoint3f point3Verts = triMesh.V(face->v[2]);
        vertArray.push_back(point1Verts);
        vertArray.push_back(point2Verts);
        vertArray.push_back(point3Verts);
        face++;
    }
    return vertArray;
}

vector<cyPoint3f> loadNormals(vector<cyPoint3f> normalsArray, cyTriMesh triMesh)
{
    const cyTriMesh::TriFace *face = &triMesh.FN(0);
    
    for(int i = 0; i < triMesh.NF(); i++)
    {
        const cyPoint3f point1Verts = triMesh.VN(face->v[0]);
        const cyPoint3f point2Verts = triMesh.VN(face->v[1]);
        const cyPoint3f point3Verts = triMesh.VN(face->v[2]);
        normalsArray.push_back(point1Verts);
        normalsArray.push_back(point2Verts);
        normalsArray.push_back(point3Verts);
        face++;
    }
    
    return normalsArray;
}

vector<cyPoint3f> loadTexCoord(vector<cyPoint3f> texCoordArray, cyTriMesh triMesh)
{
    const cyTriMesh::TriFace *face = &triMesh.FT(0);
    
    for(int i = 0; i < triMesh.NF(); i++)
    {
        const cyPoint3f point1Verts = triMesh.VT(face->v[0]);
        const cyPoint3f point2Verts = triMesh.VT(face->v[1]);
        const cyPoint3f point3Verts = triMesh.VT(face->v[2]);
        texCoordArray.push_back(point1Verts);
        texCoordArray.push_back(point2Verts);
        texCoordArray.push_back(point3Verts);
        face++;
    }
    
    return texCoordArray;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    GLfloat cameraSpeed = 0.005f;
    
    if(key == GLFW_KEY_W)
        cameraPos.z += 0.05;
    if(key == GLFW_KEY_S)
        cameraPos.z -= 0.05;
    if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        startZRotation = true;
    }
    if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        startNegZRotation = true;
    }
    if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        startNegXRotation = true;
    }
    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        startXRotation = true;
    }
    if(key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        startYRotation = true;
    }
    if(key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        startNegYRotation = true;
    }
    
    if(action != GLFW_PRESS)
    {
        startZRotation = false;
        startNegZRotation = false;
        startXRotation = false;
        startNegXRotation = false;
        startYRotation = false;
        startNegYRotation = false;
    }
    
}

bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
    lastX = xpos;
    lastY = ypos;
    
    GLfloat sensitivity = 0.05;	// Change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    mouseYaw += xoffset;
    mousePitch += yoffset;
    
    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (mousePitch > 89.0f)
        mousePitch = 89.0f;
    if (mousePitch < -89.0f)
        mousePitch = -89.0f;
    
    glm::vec3 front;
    front.x = cos(radians(mouseYaw)) * cos(radians(mousePitch));
    front.y = sin(radians(mousePitch));
    front.z = sin(radians(mouseYaw)) * cos(radians(mousePitch));
    cameraTarget = normalize(front);
}

void calcTanAndBiTan()
{
    const cyTriMesh::TriFace *faceVert = &teapotTriMesh.F(0);
    const cyTriMesh::TriFace *faceText = &teapotTriMesh.FT(0);
    
    for(int i = 0; i < teapotTriMesh.NF(); i++)
    {
        const cyPoint3f v0 = teapotTriMesh.V(faceVert->v[0]);
        const cyPoint3f v1 = teapotTriMesh.V(faceVert->v[1]);
        const cyPoint3f v2 = teapotTriMesh.V(faceVert->v[2]);
        
        
        cyPoint3f edge1 = v1 - v0;
        cyPoint3f edge2 = v2 - v0;
        
        const cyPoint3f vt0 = teapotTriMesh.VT(faceText->v[0]);
        const cyPoint3f vt1 = teapotTriMesh.VT(faceText->v[1]);
        const cyPoint3f vt2 = teapotTriMesh.VT(faceText->v[2]);
        
        cyPoint3f texEdge1 = vt1 - vt0;
        cyPoint3f texEdge2 = vt2 - vt0;
        
        float r = 1.0f / (texEdge1.x * texEdge2.y - texEdge1.y * texEdge2.x);
        cyPoint3f tangent = (edge1 * texEdge2.y - edge2 * texEdge1.y)*r;
        cyPoint3f bitangent = (edge2 * texEdge1.x - edge1 * texEdge2.x)*r;
        
        tangent.Normalize();
        bitangent.Normalize();
        
        tangent_coordinates_array.push_back(tangent);
        tangent_coordinates_array.push_back(tangent);
        tangent_coordinates_array.push_back(tangent);
        
        bitangent_coordinates_array.push_back(-bitangent);
        bitangent_coordinates_array.push_back(-bitangent);
        bitangent_coordinates_array.push_back(-bitangent);
        
        
        faceVert++;
        faceText++;
    }
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 45.0f)
        fov = 45.0f;
}









