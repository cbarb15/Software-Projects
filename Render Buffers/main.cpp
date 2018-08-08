//
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
vec3 teapot_Position = vec3(0.0f, -3.0f, 10.0f);
GLuint normal_map_shader_program;
mat4 teapotModelMatrix;
mat4 teapotRotationMatrix;
mat4 teapotScaleMatrix;
mat4 teapotTranslationMatrix;
GLuint teapotTexture;
GLuint teapotNormalMapTexture;
vector<unsigned char> brickImage;
//**********

vec3 cameraFront = vec3(0.0, 0.0f, 0.0f);
GLuint framebuffer;
GLuint texColorBuffer;
GLuint rbo;
GLuint renderedTexture;

//LIGHT AND CAMERA
//**********
vec3 cameraPos = vec3(0.0f, 0.0f, -6.0f);
vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

vec3 light_Position = vec3(0.0f, 15.0f, 0.0f);
//**********


//VIEW AND PROJECTION MATRIX VARIABLES
//**********
mat4 viewMatrix;
mat4 projectionMatrix;
//**********

//CUBE VARIABLES
cyTriMesh triMesh;
GLuint cubeVertexArrayObject;
GLuint cubeVertexBufferObject;
GLuint cubeTBO;
vector<cyPoint3f> cube_Vertex_Array;
vector<cyPoint3f> cube_Texture_Coordinates_Array;
mat4 cube_Scale;
mat4 cube_Translation;
mat4 cube_Rotation;
vec3 cube_Position = glm::vec3(0.0f,0.0f,0.0f);

GLfloat cube_Vert[] =
{
    -1.0f,-1.0f,0.0f,
    1.0f,-1.0f,0.0f,
    1.0f, 1.0f,0.0f,
    
    1.0f, 1.0f,0.0f,
    -1.0f, 1.0f,0.0f,
    -1.0f,-1.0f,0.0f
    
   
};


GLfloat cube_Tex[] =
{
    0.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,
    1.0f,1.0f,0.0f,
    
    1.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f
};

//ROTATE VARIABLES
//**********
vec3 RotateX = vec3(1.0f, 0.0f, 0.0f);
vec3 RotateY = vec3(0.0f, 1.0f, 0.0f);
vec3 RotateZ = vec3(0.0f, 0.0f, 1.0f);
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
void move_Cam();
//**********

bool keys[1024];
// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;

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
    
    window = glfwCreateWindow( WIDTH, HEIGHT, "HW4 - Normal, Render to Texture", NULL, NULL);
    
    
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, key_callback);
    
    glEnable(GL_DEPTH_TEST);
    
    glClearColor(0.4f, 0.4f, 0.4f, 0.0f);

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
    
    //CUBE MAP VAO AND VBO
    //*****************************
    //*                           *
    //*                           *
    //*****************************
    vector<unsigned char> cube_Image;
    unsigned int cubeW, cubeH;
    
    
    glGenVertexArrays(1, &cubeVertexArrayObject);
    glBindVertexArray(cubeVertexArrayObject);
    
    
    glGenBuffers(1, &cubeVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*18, cube_Vert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glGenBuffers(1, &cubeTBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeTBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*18, cube_Tex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    
    
    //FRAME BUFFER
    //******************
    //******************
    
    GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
    
    // The texture we're going to render to
    glGenTextures(1, &renderedTexture);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    
    // Give an empty image to OpenGL ( the last "0" means "empty" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, WIDTH, HEIGHT, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
    
    // Poor filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // The depth buffer
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    
    
    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
    
    
    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    
    //******************
    //******************
    
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
    
    "out vec3 FragPosWorldSpace, NormalWorldSpace, TexCoords;"
    
    "uniform mat4 projection, view, model;"
    
    "void main(){"

    "   gl_Position = projection * view * model * vec4(position, 1.0);"
    "   FragPosWorldSpace = vec3(model * vec4(position, 1.0));"
    "   mat3 normalMatrix = transpose(inverse(mat3(model)));"
    "   NormalWorldSpace = normalMatrix * normal;"
    "   TexCoords = textureCoordinates;"
    
    "}";
    
    
    const char* normal_map_fragment_shader =
    "#version 410\n"
    "out vec4 FragColor;"
    "in vec3 FragPosWorldSpace, NormalWorldSpace, TexCoords;"
   
    "uniform sampler2D textureMap;"
    
    "uniform vec3 lightPos, camPos;"
    
    "uniform mat4 model;"
    
    "void main()"
    "{"
    
    "   vec3 normal = normalize(NormalWorldSpace);"

    // Get diffuse color
    "   vec3 color = texture(textureMap, TexCoords.xy).rgb;"
    // Ambient
    "   vec3 ambient = 0.3 * color;"
    // Diffuse
    "   vec3 lightDir = normalize(lightPos - FragPosWorldSpace);"
    "   float diff = max(dot(lightDir, normal), 0.0);"
    "   vec3 diffuse = diff * color;"
    // Specular
    "   vec3 viewDir = normalize(camPos - FragPosWorldSpace);"
    "   vec3 reflectDir = reflect(-lightDir, normal);"
    "  vec3 halfwayDir = normalize(lightDir + viewDir);"
    "  float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);"
    "  vec3 specular = vec3(0.3) * spec;"
    
    "vec4 texel = texture(textureMap, TexCoords.xy);"
  "  FragColor = vec4(ambient + diffuse + specular, 1.0f) * texel;"
//    "  FragColor = vec4(ambient + diffuse + specular, 1.0f);"
    
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
    teapotTranslationMatrix =  translate(teapot_Position);
    
    //Scale
    teapotScaleMatrix = scale(vec3(0.5f, 0.5f, 0.5f));
    
    teapotRotationMatrix = rotate(teapotRotationMatrix, radians(-100.0f), RotateX);
    teapotRotationMatrix = rotate(teapotRotationMatrix, radians(170.0f), RotateZ);
    
    
    //Model Matrix
    teapotModelMatrix = teapotTranslationMatrix * teapotRotationMatrix * teapotScaleMatrix;
    GLuint model_Uniform_Location = glGetUniformLocation(normal_map_shader_program, "model");
    glUniformMatrix4fv(model_Uniform_Location, 1, GL_FALSE, &teapotModelMatrix[0][0]);
    
    viewMatrix = lookAt(cameraPos, cameraTarget, cameraUp);
    GLuint view_matrix_location = glGetUniformLocation(normal_map_shader_program, "view");
    glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, &viewMatrix[0][0]);
    
    projectionMatrix = perspective(radians(60.0f), (GLfloat)WIDTH/(GLfloat)HEIGHT, 1.0f, 500.0f);
    GLuint projection_matrix_location = glGetUniformLocation(normal_map_shader_program, "projection");
    glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, &projectionMatrix[0][0]);
    
    GLuint camPos_Uniform_Location = glGetUniformLocation(normal_map_shader_program, "camPos");
    glUniformMatrix4fv(camPos_Uniform_Location, 1, GL_FALSE, &cameraPos[0]);
    
    GLuint lightPos_Uniform_Location = glGetUniformLocation(normal_map_shader_program, "lightPos");
    glUniformMatrix4fv(lightPos_Uniform_Location, 1, GL_FALSE, &light_Position[0]);
    
    
    //Teapot Texture
    //***************
    unsigned int width, height;
    
    lodepng::decode(brickImage, width, height, "/Users/CharlieBarber/Desktop/bricks.png");
    
    
    glGenTextures(1, &teapotTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, teapotTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, brickImage.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    GLuint textureLocation = glGetUniformLocation(normal_map_shader_program, "textureMap");
    glUniform1i(textureLocation, 0);
    
    //***************
    
    
    //********************
    //********************

    
    //CUBE CODE
    //*****************************
    //*                           *
    //*                           *
    //*****************************
    const char* cube_vertex_shader =
    
    "#version 410\n"
    "layout (location = 0) in vec3 vertex_position;"
    "layout (location = 1) in vec3 textureCoordinates;"
    "uniform mat4 projection, model, view;"
    "out vec3 texCoord;"
    
    "void main() {"
    
    "   gl_Position = projection * view * model * vec4(vertex_position, 1.0);"
    "   texCoord = textureCoordinates;"
    
    "}";
    
    const char* cube_fragment_shader =
    "#version 410\n"
    "out vec4 frag_color;"
    "in vec3 texCoord;"
    
    "uniform sampler2D teapotTexture;"
    
    "void main() {"
    
    "vec4 texel = texture(teapotTexture, texCoord.xy);"
    "frag_color = texel;"
//    " frag_color = vec4(0.0, 0.0, 1.0, 1.0);"
    
    "}";
    
    
    GLuint cubeVS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(cubeVS, 1, &cube_vertex_shader, NULL);
    glCompileShader(cubeVS);
    
    
    
    GLuint cubeFS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(cubeFS, 1, &cube_fragment_shader, NULL);
    glCompileShader(cubeFS);
    
    
    
    GLuint cube_Shader_Program = glCreateProgram();
    glAttachShader(cube_Shader_Program, cubeVS);
    glAttachShader(cube_Shader_Program, cubeFS);
    glLinkProgram(cube_Shader_Program);
    
    
    glUseProgram(cube_Shader_Program);
    
    
    cube_Translation = glm::translate(cube_Position);
    cube_Scale = glm::scale(glm::vec3(2.5f,2.0f,2.0f));
    cube_Rotation = glm::rotate(cube_Rotation, glm::radians(55.0f), RotateX);
    cube_Rotation = glm::rotate(cube_Rotation, glm::radians(25.0f), RotateZ);
    glm::mat4 model = cube_Translation * cube_Rotation * cube_Scale;
    GLuint model_Location = glGetUniformLocation(cube_Shader_Program, "model");
    glUniformMatrix4fv(model_Location, 1, GL_FALSE, &model[0][0]);
    
    
    GLuint cube_projection_matrix_location = glGetUniformLocation(cube_Shader_Program, "projection");
    glUniformMatrix4fv(cube_projection_matrix_location, 1, GL_FALSE, &projectionMatrix[0][0]);

    while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 )
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glfwPollEvents();
        move_Cam();
        
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0,0,WIDTH, HEIGHT);
        
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glUseProgram(normal_map_shader_program);
        glBindTexture(GL_TEXTURE_2D, teapotTexture);
        glBindVertexArray(teapotVAO);
        glDrawArrays(GL_TRIANGLES, 0, teapot_vert_array.size());
        
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0,0,WIDTH,HEIGHT);
        
        //Clear the screen
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(cube_Shader_Program);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        
        GLuint quadTexLocation = glGetUniformLocation(cube_Shader_Program, "teapotTexture");
        glUniform1i(quadTexLocation, 0);
        
        
        GLuint cube_view_matrix_location = glGetUniformLocation(cube_Shader_Program, "view");
        glUniformMatrix4fv(cube_view_matrix_location, 1, GL_FALSE, &viewMatrix[0][0]);
        
        glBindVertexArray(cubeVertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        
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


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void move_Cam()
{
    // Camera controls
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if (keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
