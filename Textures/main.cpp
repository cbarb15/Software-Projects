//
//  main.cpp
//  HW4_Graphics
//
//  Created by Charlie Barber on 2/12/17.
//  Copyright © 2017 Charlie Barber. All rights reserved.
//

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLFW/glfw3.h>
#  include "cyTriMesh.h"
#  include "cyGL.h"
#  include "cyMatrix.h"
#else
#  include <GL/glew.h>
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GLFW/glfw3.h>
#  include "cyTriMesh.h"
#  include "cyGL.h"
#  include "cyMatrix.h"

#endif

#include "lodepng.hpp"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

using namespace std;
using namespace glm;
using namespace lodepng;


const int WIDTH = 1000;
const int HEIGHT = 800;
cyTriMesh triMesh;
cyGLSLShader vertexShaderObj;
cyGLSLShader fragmentShaderObj;
cyGLSLProgram program;
GLuint vertexArrayObject;
GLuint vertexBufferObject;
GLuint normalBufferObject;
GLuint shader_program;
GLFWwindow* window;
GLfloat scaleX = 0.05f;
GLfloat scaleY = 0.05f;
GLfloat scaleZ = 0.05f;
GLdouble mouseX = 0;
GLdouble mouseY = 0;
GLdouble prev_MouseX = 0;
GLdouble prev_MouseY = 0;
GLuint matrix_location;
GLuint rotateX_matrix_location;
GLuint scale_matrix_location;
GLuint rotateY_matrix_location;
GLuint rotateZ_matrix_location;
GLfloat angle = 20.0f;
GLfloat angleY = 0.0f;
GLfloat angleZ = 0.4f;
vector<cyPoint3f> vertArray;
vector<cyPoint3f> normalsArray;
vector<cyPoint3f> vertCoordArray;
GLuint view_matrix_location;
GLuint projection_matrix_location;
const double PI = 3.14159265358979323846;
vector<unsigned char> image;
unsigned int IMAGE_WIDTH;
unsigned int IMAGE_HEIGHT;
GLuint textureId;
GLuint texture;
const char* fileName;

GLfloat viewMatrix[] =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, -1.7f, 1.0f
};

GLfloat translationMatrix[] =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, -0.3f, 0.0f, 1.0f
};

GLfloat scaleMatrix[] =
{
    scaleX, 0.0f, 0.0f, 0.0f,
    0.0f, scaleY, 0.0f, 0.0f,
    0.0f, 0.0f, scaleZ, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

GLfloat rotateXMatrix[] =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, cosf(angle), -sinf(angle), 0.0f,
    0.0f, sinf(angle), cosf(angle), 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

GLfloat rotateYMatrix[] =
{
    cosf(angleY), 0.0f, sinf(angleY), 0.0f,
    0.0f, 1.0f, 0.0, 0.0f,
    -sinf(angleY), 0.0f, cosf(angleY), 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

GLfloat rotateZMatrix[] =
{
    cosf(angleZ), -sinf(angleZ), 0.0f, 0.0f,
    sinf(angleZ), cosf(angleZ), 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};


void initWindow();
int generateVertexBufferAndArray();
void loadShadersAndDraw();
void draw();
void cursor_pos();
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow*, double, double);
vector<cyPoint3f> makeArrayOfTriangleVerts(vector<cyPoint3f>);
vector<cyPoint3f> loadNormals();
vector<cyPoint3f> loadTexCoord();

int main(int argc, const char * argv[])
{
    const char* file = argv[1];
    triMesh.LoadFromFileObj(file);
    
    cyTriMesh::Mtl map = triMesh.M(0);
    
     fileName = map.map_Kd.name;
    
    vertArray = makeArrayOfTriangleVerts(vertArray);
    
    normalsArray = loadNormals();
    
    vertCoordArray = loadTexCoord();
    
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
    window = glfwCreateWindow( WIDTH, HEIGHT, "HW3", NULL, NULL);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
}

int generateVertexBufferAndArray()
{
    glewExperimental = GL_TRUE;
    glewInit();
    
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    
    
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cyPoint3f)*vertArray.size(), &vertArray.at(0), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cyPoint3f)*normalsArray.size(), &normalsArray.at(0), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glGenBuffers(1, &textureId);
    glBindBuffer(GL_ARRAY_BUFFER, textureId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cyPoint3f)*vertCoordArray.size(), &vertCoordArray.at(0), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    
    return 0;
}

////Register the uniform variables.  Look at cem's code
////Search for glUniform_something
void loadShadersAndDraw()
{
    const char* vertex_shader =
    "#version 410\n"
    "layout (location = 0) in vec3 vertex_position;"
    "layout (location = 1) in vec3 normal;"
    "layout (location = 2) in vec3 textureCoordinates;"
    "uniform mat4 translationMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, projection, view;"
    "out vec3 posVectorInCamCoord, normVectorInCamCoord;"
    "out vec3 normalColor;"
    "out mat4 model;"
    "out vec3 textCoord;"
    "void main(){"
    "   model = (translationMatrix * (rotateXMatrix * rotateYMatrix * rotateZMatrix) * scaleMatrix);"
    "   posVectorInCamCoord = vec3(view * model * vec4(vertex_position, 1.0));"
    "   normVectorInCamCoord = vec3(view * model * vec4(normal, 0.0f));"
    "   gl_Position = projection * vec4(posVectorInCamCoord, 1.0);"
    "   normalColor = normal;"
    "   textCoord = textureCoordinates;"
    "}";
    
    
    const char* fragment_shader =
    "#version 410\n"
    "out vec4 frag_color;"
    "in vec3 posVectorInCamCoord, normVectorInCamCoord;"
    "uniform mat4 view;"
    "in vec3 normalColor;"
    "in mat4 model;"
    "in vec3 textCoord;"
    "uniform sampler2D textImage;"
    "void main() {"
    
//    "   vec3 objectColor = vec3(0.0, 0.0, 0.8);"
    "   vec3 light_pos = vec3(0.0, 0.5, 2.0);"
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
    "float spec = pow(max(dot(normVector, halfwayVector), 0.0), 10.0);"
    "vec3 specular =  vec3(1.5) * spec * lightColor;"

      "vec4 texel = texture(textImage, textCoord.xy);"
    "frag_color = vec4((ambient + diffuse + specular), 1.0) * texel;"
    "}";
    
    
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    
    
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);
    glLinkProgram(shader_program);
    
    
    matrix_location = glGetUniformLocation(shader_program, "translationMatrix");
    glUseProgram(shader_program);
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, translationMatrix);
    
    rotateX_matrix_location = glGetUniformLocation(shader_program, "rotateXMatrix");
    glUseProgram(shader_program);
    glUniformMatrix4fv(rotateX_matrix_location, 1, GL_FALSE, rotateXMatrix);
    
    scale_matrix_location = glGetUniformLocation(shader_program, "scaleMatrix");
    glUseProgram(shader_program);
    glUniformMatrix4fv(scale_matrix_location, 1, GL_FALSE, scaleMatrix);
    
    rotateY_matrix_location = glGetUniformLocation(shader_program, "rotateYMatrix");
    glUseProgram(shader_program);
    glUniformMatrix4fv(rotateY_matrix_location, 1, GL_FALSE, rotateYMatrix);
    
    rotateZ_matrix_location = glGetUniformLocation(shader_program, "rotateZMatrix");
    glUseProgram(shader_program);
    glUniformMatrix4fv(rotateZ_matrix_location, 1, GL_FALSE, rotateZMatrix);
    
    
    view_matrix_location = glGetUniformLocation(shader_program, "view");
    glUseProgram(shader_program);
    glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE, viewMatrix);
    
    //Texture
    //***************
    unsigned int width, height;
    
    unsigned error = lodepng::decode(image, width, height, "/Users/CharlieBarber/Desktop/teapot/brick.png");

    
    if(error != 0)
    {
        cout << "error";
    }
    
    glGenTextures(1, &texture);
    glBindTexture(1, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    
    //***************
    
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
    // input variables
    float near = 0.1f; // clipping plane
    float far = 100.0f; // clipping plane
    float fov = 67.0f * ONE_DEG_IN_RAD; // convert 67 degrees to radians
    float aspect = (float)WIDTH / (float)HEIGHT; // aspect ratio
    // matrix components
    float range = tan (fov * 0.5f) * near;
    float Sx = (2.0f * near) / (range * aspect + range * aspect);
    float Sy = near / range;
    float Sz = -(far + near) / (far - near);
    float Pz = -(2.0f * far * near) / (far - near);
    GLfloat proj_mat[] = {
        Sx, 0.0f, 0.0f, 0.0f,
        0.0f, Sy, 0.0f, 0.0f,
        0.0f, 0.0f, Sz, -1.0f,
        0.0f, 0.0f, Pz, 0.0f
    };
    
    projection_matrix_location = glGetUniformLocation(shader_program, "projection");
    glUseProgram(shader_program);
    glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE, proj_mat);
    
    while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 )
    {
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program);
        glBindVertexArray(vertexBufferObject);
        glDrawArrays(GL_TRIANGLES, 0, vertArray.size());
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        printf("Left button pressed\n");
        
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        
        glfwGetCursorPos(window, &mouseX, &mouseY);
        
        while(mouseY > prev_MouseY)
        {
            scaleX += 0.02;
            scaleY += 0.02;
            scaleZ += 0.02;
            printf("scaleX = %f\n", scaleX);
            printf("scaleY = %f\n", scaleY);
            printf("scaleZ = %f\n", scaleZ);
            printf("scaleZ = %f\n", scaleZ);
            printf("scaleZ = %f\n", scaleZ);
            printf("scaleZ = %f\n", scaleZ);
        }
        prev_MouseY = mouseY;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    
    //    glfwGetCursorPos(window, &mouseX, &mouseY);
    //    printf("mouseY = %f\n", mouseY);
    //    printf("prev_MouseY = %f\n", prev_MouseY);
    //    if(mouseY > prev_MouseY)
    //    {
    scaleMatrix[0] += 0.0001;
    scaleMatrix[5] += 0.0001;
    scaleMatrix[10] += 0.0001;
    glUseProgram(shader_program);
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, scaleMatrix);
    //    }
}

vector<cyPoint3f> makeArrayOfTriangleVerts(vector<cyPoint3f> vertArray)
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

vector<cyPoint3f> loadNormals()
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

vector<cyPoint3f> loadTexCoord()
{
    const cyTriMesh::TriFace *face = &triMesh.FT(0);
    
    for(int i = 0; i < triMesh.NF(); i++)
    {
        const cyPoint3f point1Verts = triMesh.VT(face->v[0]);
        const cyPoint3f point2Verts = triMesh.VT(face->v[1]);
        const cyPoint3f point3Verts = triMesh.VT(face->v[2]);
        vertCoordArray.push_back(point1Verts);
        vertCoordArray.push_back(point2Verts);
        vertCoordArray.push_back(point3Verts);
        face++;
    }
    
    return vertCoordArray;
}










