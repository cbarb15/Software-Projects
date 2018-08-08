//

//  main.cpp

//  HW6

//

//  Created by Charlie Barber on 3/15/17.

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
#include "glm/ext.hpp"

using namespace std;
using namespace lodepng;

#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0


const GLfloat WIDTH = 1000;
const GLfloat HEIGHT = 800;
cyTriMesh triMesh;
cyTriMesh teapotTriMesh;
cyGLSLShader vertexShaderObj;
cyGLSLShader fragmentShaderObj;
cyGLSLProgram program;
GLuint cubeVertexArrayObject;
GLuint cubeVertexBufferObject;
GLuint normalsBufferObject;
GLuint cube_Shader_Program;
GLFWwindow* window;
GLuint teapot_view_matrix_location;
GLuint teapot_projection_matrix_location;
vector<cyPoint3f> cube_Vertex_Array;
GLuint view_matrix_location;
GLuint projection_matrix_location;
const char* fileName;
glm::mat4 viewMatrix;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -10.0f);
//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 projectionMatrix;
//glm::vec3 cube_Position = glm::vec3(0.0f,0.0f,10.0f);
glm::vec3 cube_Position = glm::vec3(0.0f,0.0f,0.0f);
glm::mat4 cube_Rotation;
glm::mat4 cube_Scale;
glm::mat4 cube_Translation;
glm::mat4 teapot_Transformation_Matrix;
GLuint teapot_Model_Matrix_Location;
glm::mat4 teapot_View_Matrix;
glm::mat4 teapot_Projection_Matrix;
vector<cyPoint3f> teapot_vert_array;
vector<cyPoint3f> teapot_normals_Array;
vector<cyPoint3f> teapot_texture_coordinates_Array;
vector<unsigned char> brick_texture;
GLuint teapot_texture;
GLuint teapot_textureId;
GLuint teapotVAO;
GLuint teapotVBO;
GLuint teapotNormalsBuffer;
GLuint teapotTextureBufferObject;
//glm::vec3 teapot_Position = glm::vec3(0.0f, -5.0f, 0.0f);
glm::vec3 teapot_Position = glm::vec3(0.0f, -3.5f, 5.0f);
glm::mat4 teapotModelMatrix;
glm::mat4 teapotRotationMatrix;
glm::vec3 teapotRotateX = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 teapotRotateY = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 teapotRotateZ = glm::vec3(0.0f, 0.0f, 1.0f);
glm::mat4 teapotScaleMatrix;
glm::mat4 teapotTranslationMatrix;
glm::vec3 light_Position = glm::vec3(0.0f, 0.0f, -10.0f);

void initWindow();
int generateVertexBufferAndArray();
void loadShadersAndDraw();
vector<cyPoint3f> makeArrayOfTriangleVerts(vector<cyPoint3f>, cyTriMesh);
GLuint createCubeMap();
vector<cyPoint3f> loadNormals();
vector<cyPoint3f> loadTexCoord();

int main(int argc, const char * argv[])

{
    const char* file = argv[1];
    
    teapotTriMesh.LoadFromFileObj(file);
    
    teapot_vert_array = makeArrayOfTriangleVerts(teapot_vert_array, teapotTriMesh);
    
    teapot_normals_Array = loadNormals();
    
    teapot_texture_coordinates_Array = loadTexCoord();
    
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
    
    window = glfwCreateWindow( WIDTH, HEIGHT, "HW6 - Environment Mapping", NULL, NULL);
    
    
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

GLuint cubeMapTextureId;

int generateVertexBufferAndArray()
{
    glewExperimental = GL_TRUE;
    glewInit();
    
    glGenVertexArrays(1, &teapotVAO);
    glBindVertexArray(teapotVAO);
    
    glGenBuffers(1, &teapotVBO);
    glBindBuffer(GL_ARRAY_BUFFER, teapotVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cyPoint3f)*teapot_vert_array.size(), &teapot_vert_array[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    
    glGenBuffers(1, &teapotNormalsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, teapotNormalsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cyPoint3f)*teapot_normals_Array.size(), &teapot_normals_Array[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    
    glGenBuffers(1, &teapotTextureBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, teapotTextureBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cyPoint3f)*teapot_texture_coordinates_Array.size(), &teapot_texture_coordinates_Array[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    
    //CUBE MAP VAO AND VBO
    //*****************************
    //*                           *
    //*                           *
    //*****************************
    vector<unsigned char> cube_Image;
    unsigned int cubeW, cubeH;
    
    bool error = triMesh.LoadFromFileObj("/Users/CharlieBarber/Desktop/cube.obj");
    
    if(!error)
        cout << "error";
    
    
    cube_Vertex_Array = makeArrayOfTriangleVerts(cube_Vertex_Array, triMesh);
    
    
    glGenVertexArrays(1, &cubeVertexArrayObject);
    glBindVertexArray(cubeVertexArrayObject);
    
    
    glGenBuffers(1, &cubeVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cyPoint3f)*cube_Vertex_Array.size(), &cube_Vertex_Array[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    cubeMapTextureId = createCubeMap();

    
    return 0;
    
}

GLuint cube_map_location;

void loadShadersAndDraw()

{
    //TEAPOT CODE
    //*****************************
    //*                           *
    //*                           *
    //*****************************
     
    //Create the teapot vertex and fragement shader and create and link the program
    //******************************
    //******************************
    
    const char* teapot_vertex_shader =
    "#version 410\n"
    "layout (location = 0) in vec3 teapot_vertex_position;"
    "layout (location = 1) in vec3 teapot_normal_position;"
    "uniform mat4 model, view, projection;"
    "out vec3 posVectorInCamCoord, normVectorInCamCoord, Normal, Position;"
    
    "void main() {"
   
    "   Normal = mat3(transpose(inverse(model))) * teapot_normal_position;"
    "   Position = vec3(model * vec4(teapot_vertex_position, 1.0f));"
    "   posVectorInCamCoord = vec3(view * model * vec4(teapot_vertex_position, 1.0));"
    "   normVectorInCamCoord = vec3(view * model * vec4(teapot_normal_position, 0.0f));"
    "   gl_Position = projection * view * model * vec4(teapot_vertex_position, 1.0);"
    
    "}";
    
    
    const char* teapot_fragment_shader =
    "#version 410\n"
    "in vec3 Normal, Position, posVectorInCamCoord, normVectorInCamCoord;"
    "out vec4 frag_color;"
    
    "uniform vec3 cameraPos;"
    "uniform samplerCube skybox;"
    "uniform mat4 view;"

    
    "void main() {"
    
    "   vec3 objectColor = vec3(0.0, 0.0, 0.8);"
    "   vec3 light_pos = vec3(0.0, 0.0, -12.0);"
    "   vec3 camera_pos = vec3(0.0, 2.0, 8.0);"
    "   vec3 lightColor = vec3(1.0, 1.0, 1.0);"
    "   vec3 lightPosInCamCoord = vec3(view * vec4(light_pos, 1.0));"
    "   vec3 lightVector = normalize(lightPosInCamCoord - posVectorInCamCoord);"
    "   vec3 normVector = normalize(normVectorInCamCoord);"
    
    
    // Ambient
    "float ambientStrength = 0.2f;"
    "vec3 ambient = ambientStrength * lightColor;"
    //Diffuse
    " float diff = max(dot(normVector, lightVector), 0.0);"
    " vec3 diffuse = diff * lightColor;"
    
    //Specular
    "vec3 viewVector = normalize(camera_pos - posVectorInCamCoord);"
    "vec3 halfwayVector = normalize(lightVector + viewVector);"
    "float spec = pow(max(dot(normVector, halfwayVector), 0.0), 8.0);"
    "vec3 specular =  vec3(0.3) * spec * lightColor;"
    
    
    "vec3 I = normalize(Position - cameraPos);"
    "vec3 R = reflect(I, normalize(Normal));"
    "vec4 texel = texture(skybox, R);"
    
    //"frag_color = vec4((ambient + diffuse + specular) * objectColor, 1.0);"

    "frag_color = vec4((ambient + diffuse + specular), 1.0) * texel;"
    
    "}";
    
    
    GLuint teapotVS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(teapotVS, 1, &teapot_vertex_shader, NULL);
    glCompileShader(teapotVS);
    
    GLuint teapotFS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(teapotFS, 1, &teapot_fragment_shader, NULL);
    glCompileShader(teapotFS);
    
    GLuint teapot_shader_program = glCreateProgram();
    glAttachShader(teapot_shader_program, teapotVS);
    glAttachShader(teapot_shader_program, teapotFS);
    glLinkProgram(teapot_shader_program);
    
    glUseProgram(teapot_shader_program);
    
   //MVP Matrix
    //********************
    //********************
    
    //Translate
    teapotTranslationMatrix = glm::translate(teapot_Position);
   
    //Scale
    teapotScaleMatrix = glm::scale(glm::vec3(0.3f, 0.3f, 0.3f));
    
    teapotRotationMatrix = glm::rotate(teapotRotationMatrix, glm::radians(-100.0f), teapotRotateX);
    teapotRotationMatrix = glm::rotate(teapotRotationMatrix, glm::radians(170.0f), teapotRotateZ);

    
    //Model Matrix
    teapotModelMatrix = teapotTranslationMatrix * teapotRotationMatrix * teapotScaleMatrix;
    GLuint model_Uniform_Location = glGetUniformLocation(teapot_shader_program, "model");
    glUniformMatrix4fv(model_Uniform_Location, 1, GL_FALSE, &teapotModelMatrix[0][0]);
    
    teapot_View_Matrix = glm::lookAt(cameraPos, glm::vec3(0,0,10), glm::vec3(0,1,0));
    GLuint view_matrix_location = glGetUniformLocation(teapot_shader_program, "view");
    glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, &teapot_View_Matrix[0][0]);

    teapot_Projection_Matrix = glm::perspective(glm::radians(60.0f), (GLfloat)WIDTH/(GLfloat)HEIGHT, 1.0f, 500.0f);
    teapot_projection_matrix_location = glGetUniformLocation(teapot_shader_program, "projection");
    glUniformMatrix4fv(teapot_projection_matrix_location, 1, GL_FALSE, &teapot_Projection_Matrix[0][0]);
    
    GLuint camPos_Uniform_Location = glGetUniformLocation(teapot_shader_program, "cameraPos");
    glUniformMatrix4fv(camPos_Uniform_Location, 1, GL_FALSE, &cameraPos[0]);

    
    teapotTranslationMatrix = glm::translate(teapot_Position);
    
    //Scale
    teapotScaleMatrix = glm::scale(glm::vec3(0.3f, 0.3f, 0.3f));
    
    teapotRotationMatrix = glm::rotate(teapotRotationMatrix, glm::radians(-100.0f), teapotRotateX);
    teapotRotationMatrix = glm::rotate(teapotRotationMatrix, glm::radians(170.0f), teapotRotateZ);
    
    teapot_Projection_Matrix = glm::perspective(glm::radians(60.0f), (GLfloat)WIDTH/(GLfloat)HEIGHT, 1.0f, 500.0f);
    teapot_projection_matrix_location = glGetUniformLocation(teapot_shader_program, "projection");
    glUniformMatrix4fv(teapot_projection_matrix_location, 1, GL_FALSE, &teapot_Projection_Matrix[0][0]);
    
    //********************
    //********************
    
    
    //CUBE MAP CODE
    //*****************************
    //*                           *
    //*                           *
    //*****************************
    const char* cube_vertex_shader =
    
    "#version 410\n"
    "layout (location = 0) in vec3 vertex_position;"
    "uniform mat4 projection, model, view;"
    "out vec3 textureCoordinates;"
    "void main() {"
    
    "   gl_Position = projection * view * model * vec4(vertex_position, 1.0);"
    "   textureCoordinates = vertex_position;"
    
    "}";
    
    const char* cube_fragment_shader =
    "#version 410\n"
    "out vec4 frag_color;"
    "in vec3 textureCoordinates;"
    "uniform samplerCube cube_texture;"
    
    "void main() {"
    
    " frag_color = texture(cube_texture, textureCoordinates);"
    
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
    cube_Scale = glm::scale(glm::vec3(1.0f,1.0f,1.0f));
    glm::mat4 model = cube_Translation * cube_Scale;
    GLuint model_Location = glGetUniformLocation(cube_Shader_Program, "model");
    glUniformMatrix4fv(model_Location, 1, GL_FALSE, &model[0][0]);
    

    viewMatrix = glm::lookAt(cameraPos, glm::vec3(0,0,10), glm::vec3(0,1,0));
    view_matrix_location = glGetUniformLocation(cube_Shader_Program, "view");
    glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, &viewMatrix[0][0]);
    
    
    projectionMatrix = glm::perspective(glm::radians(60.0f), (GLfloat)WIDTH/(GLfloat)HEIGHT, 1.0f, 500.0f);
    projection_matrix_location = glGetUniformLocation(cube_Shader_Program, "projection");
    glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, &projectionMatrix[0][0]);
    
    
    //send in cube map
    cube_map_location = glGetUniformLocation(cube_Shader_Program, "cube_texture");
    //0 is the texture unit

    
    while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 )
    {
        glfwPollEvents();
    
        
        //Draw cube map
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(cube_Shader_Program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureId);
        glUniform1i(cube_map_location, 0);
        glBindVertexArray(cubeVertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, cube_Vertex_Array.size());
        
        
        
        //Draw teapot
        glUseProgram(teapot_shader_program);
        glBindVertexArray(teapotVAO);
        glDrawArrays(GL_TRIANGLES, 0, teapot_vert_array.size());
        
        
        glfwSwapBuffers(window);
        
    }
}

vector<cyPoint3f> makeArrayOfTriangleVerts(vector<cyPoint3f> vert_array, cyTriMesh triMesh)
{
    
    const cyTriMesh::TriFace *face = &triMesh.F(0);
    
    for(int i = 0; i < triMesh.NF(); i++)
    {
        const cyPoint3f point1Verts = triMesh.V(face->v[0]);
        const cyPoint3f point2Verts = triMesh.V(face->v[1]);
        const cyPoint3f point3Verts = triMesh.V(face->v[2]);
        
        vert_array.push_back(point1Verts);
        vert_array.push_back(point2Verts);
        vert_array.push_back(point3Verts);
        face++;
        
    }
    
    return vert_array;
}


vector<cyPoint3f> loadNormals()
{
    
    const cyTriMesh::TriFace *face = &teapotTriMesh.FN(0);
    
    for(int i = 0; i < teapotTriMesh.NF(); i++)
    {
        const cyPoint3f point1Verts = teapotTriMesh.VN(face->v[0]);
        const cyPoint3f point2Verts = teapotTriMesh.VN(face->v[1]);
        const cyPoint3f point3Verts = teapotTriMesh.VN(face->v[2]);
        
        teapot_normals_Array.push_back(point1Verts);
        teapot_normals_Array.push_back(point2Verts);
        teapot_normals_Array.push_back(point3Verts);
        face++;
        
    }
    
    return teapot_normals_Array;
}


vector<cyPoint3f> loadTexCoord()

{
    const cyTriMesh::TriFace *face = &teapotTriMesh.FT(0);
    
    
    for(int i = 0; i < teapotTriMesh.NF(); i++)
    {
        const cyPoint3f point1Verts = teapotTriMesh.VT(face->v[0]);
        const cyPoint3f point2Verts = teapotTriMesh.VT(face->v[1]);
        const cyPoint3f point3Verts = teapotTriMesh.VT(face->v[2]);
        
        teapot_texture_coordinates_Array.push_back(point1Verts);
        teapot_texture_coordinates_Array.push_back(point2Verts);
        teapot_texture_coordinates_Array.push_back(point3Verts);
        face++;
        
    }
    
    return teapot_texture_coordinates_Array;
}

GLuint createCubeMap()
{
    
    //Get cubemap
    unsigned int W = 2048;
    unsigned int H = 2048;
    vector<unsigned char> image_negX;
    vector<unsigned char> image_negY;
    vector<unsigned char> image_negZ;
    vector<unsigned char> image_posX;
    vector<unsigned char> image_posY;
    vector<unsigned char> image_posZ;
    
    
    //Load the six different images for the textures
    lodepng::decode(image_negX, W, H, "/Users/CharlieBarber/Desktop/cubemap/cubemap_negx.png");
    lodepng::decode(image_negY, W, H, "/Users/CharlieBarber/Desktop/cubemap/cubemap_negy.png");
    lodepng::decode(image_negZ, W, H, "/Users/CharlieBarber/Desktop/cubemap/cubemap_negz.png");
    lodepng::decode(image_posX, W, H, "/Users/CharlieBarber/Desktop/cubemap/cubemap_posx.png");
    lodepng::decode(image_posY, W, H, "/Users/CharlieBarber/Desktop/cubemap/cubemap_posy.png");
    lodepng::decode(image_posZ, W, H, "/Users/CharlieBarber/Desktop/cubemap/cubemap_posz.png");
    
    
    //Create the six different textures for the walls of the cube
    glGenTextures(1, &cubeMapTextureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureId);
    
    
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_posX.data());//Right of cube
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_negX.data());//Left side of cube
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_posY.data());//Top of cube
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_negY.data());//Bottom of cube
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_posZ.data());//Back of cube
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_negZ.data());//Front of cube
    
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    
    return cubeMapTextureId;
   
}
