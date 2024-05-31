// F20GA Coursework Source.cpp file by Drew Tomkins
// Uses the F20GA OpenGL template as a basis for the coursework
// This uses Visual Studio 2019 and other libraries - see notes in code and linker files.
// Last Changed 22/11/2021

#pragma comment(linker, "/NODEFAULTLIB:MSVCRT")
#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

#include <GL/glew.h>			// Add library to extend OpenGL to newest version
#include <GLFW/glfw3.h>			// Add library to launch a window
#include <GLM/glm.hpp>			// Add helper maths library
#include <GLM/gtx/transform.hpp>

#include <stb_image.h>			// Add library to load images for textures

#include "Mesh.h"				// Simplest mesh holder and OBJ loader - can update more - from https://github.com/BennyQBD/ModernOpenGLTutorial


// MAIN FUNCTIONS
void setupRender();
void startup();
void update(GLfloat currentTime);
void render(GLfloat currentTime);
void endProgram();

// HELPER FUNCTIONS OPENGL
void hintsGLFW();
string readShader(string name);
void checkErrorShader(GLuint shader);
void errorCallbackGLFW(int error, const char* description);
void debugGL();
static void APIENTRY openGLDebugCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const GLvoid* userParam);

// CALLBACK FUNCTIONS FOR WINDOW
void onResizeCallback(GLFWwindow* window, int w, int h);
void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void onMouseMoveCallback(GLFWwindow* window, double x, double y);
void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset);


// VARIABLES
GLFWwindow*		window;											// Keep track of the window
int				windowWidth = 640;				
int				windowHeight = 480;
bool			running = true;									// Are we still running?
glm::mat4		proj_matrix;									// Projection Matrix
glm::vec3		cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);	// Week 5 lecture
glm::vec3		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float           aspect = (float)windowWidth / (float)windowHeight;
float			fovy = 45.0f;
bool			keyStatus[1024];
GLfloat			deltaTime = 0.0f;
GLfloat			lastTime = 0.0f;
GLuint			program;
GLint			proj_location;

//Positions for the model, rotation and various meshes positions
glm::vec3		modelPosition;
glm::vec3		modelRotation;
//Rotation amounts for certain objects as needed in the scene
glm::vec3		windmillbladesRotation, islandRotation, windmillbaseRotation, rocksRotation, doorRotation;
//Positions for regular meshes within the scene
glm::vec3       windmillbladesPos, benchPos, pondPos, rocksPos, islandPos, housePos, gravePos, dirtpathPos, windmillbasePos, flowersPos, witchPos, doorPos;

//Add corresponding mesh objects
Mesh bench;
Mesh rocks;
Mesh island;
Mesh house;
Mesh pond;
Mesh grave;
Mesh dirtpath;
Mesh windmillbase;
Mesh windmillblades;
Mesh flowers;
Mesh witch;
Mesh door;
// Attempt to initialise a texture for the dirt path
//GLuint dirtTexture;


/*
  Basic lighting system (note:probably non-functional due to lack of proper shaders
*/
glm::vec4 ia = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
GLfloat ka = 1.0f;
glm::vec4 id = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 is = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 lightDisp = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


// Camera variables to enable its movement
GLfloat			yaw = -90.0f;					// init pointing to inside
GLfloat			pitch = 0.0f;					// start centered
GLfloat			lastX = windowWidth / 2.0f;	// start middle screen
GLfloat			lastY = windowHeight / 2.0f;	// start middle screen
bool			firstMouse = true;


int main()
{
	if (!glfwInit()) {							// Checking for GLFW
		cout << "Could not initialise GLFW...";
		return 0;
	}

	glfwSetErrorCallback(errorCallbackGLFW);	// Setup a function to catch and display all GLFW errors.

	hintsGLFW();								// Setup glfw with various hints.		

												// Start a window using GLFW
	string title = "My OpenGL Application";

	// Fullscreen
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	// windowWidth = mode->width; windowHeight = mode->height; //fullscreen
	// window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), glfwGetPrimaryMonitor(), NULL); // fullscreen

	// Window
	window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);
	if (!window) {								// Window or OpenGL context creation failed
		cout << "Could not initialise GLFW...";
		endProgram();
		return 0;
	}

	glfwMakeContextCurrent(window);				// making the OpenGL context current

												// Start GLEW (note: always initialise GLEW after creating your window context.)
	glewExperimental = GL_TRUE;					// hack: catching them all - forcing newest debug callback (glDebugMessageCallback)
	GLenum errGLEW = glewInit();
	if (GLEW_OK != errGLEW) {					// Problems starting GLEW?
		cout << "Could not initialise GLEW...";
		endProgram();
		return 0;
	}

	debugGL();									// Setup callback to catch openGL errors.	

												// Setup all the message loop callbacks.
	glfwSetWindowSizeCallback(window, onResizeCallback);		// Set callback for resize
	glfwSetKeyCallback(window, onKeyCallback);					// Set allback for keys
	glfwSetMouseButtonCallback(window, onMouseButtonCallback);	// Set callback for mouse click
	glfwSetCursorPosCallback(window, onMouseMoveCallback);		// Set callback for mouse move
	glfwSetScrollCallback(window, onMouseWheelCallback);		// Set callback for mouse wheel.
																//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);	// Set mouse cursor. Fullscreen
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	// Set mouse cursor FPS fullscreen.

	setupRender();								// setup some render variables.
	startup();									// Setup all necessary information for startup (aka. load texture, shaders, models, etc).

	do {										// run until the window is closed
		GLfloat currentTime = (GLfloat)glfwGetTime();		// retrieve timelapse
		deltaTime = currentTime - lastTime;		// Calculate delta time
		lastTime = currentTime;					// Save for next frame calculations.
		glfwPollEvents();						// poll callbacks
		update(currentTime);					// update (physics, animation, structures, etc)
		render(currentTime);					// call render function.

		glfwSwapBuffers(window);				// swap buffers (avoid flickering and tearing)

		running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);	// exit if escape key pressed
		running &= (glfwWindowShouldClose(window) != GL_TRUE);
	} while (running);

	endProgram();			// Close and clean everything up...

	cout << "\nPress any key to continue...\n";
	cin.ignore(); cin.get(); // delay closing console to read debugging errors.

	return 0;
}

void errorCallbackGLFW(int error, const char* description) {
	cout << "Error GLFW: " << description << "\n";
}

void hintsGLFW() {
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);			// Create context in debug mode - for debug message callback
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
}

void endProgram() {
	glfwMakeContextCurrent(window);		// destroys window handler
	glfwTerminate();	// destroys all windows and releases resources.
}

void setupRender() {
	glfwSwapInterval(1);	// Ony render when synced (V SYNC)

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 2);
	glfwWindowHint(GLFW_STEREO, GL_FALSE);
}

void startup() {

	// Load main object model and shaders
	// Note: the model loader only accepts triangles and quads, anything more it will ignore.
	// So, if you find holes in your model, then please triangulate your model before importing or update loading code.
	bench.LoadModel("bench.obj"); 
	rocks.LoadModel("rocks.obj");
	island.LoadModel("island.obj");
	house.LoadModel("housealt.obj");
	pond.LoadModel("pondalt.obj");
	grave.LoadModel("grave.obj");
	dirtpath.LoadModel("dirtpath.obj");
	windmillblades.LoadModel("bladesalt2.obj");
	windmillbase.LoadModel("basealt2.obj");
	flowers.LoadModel("flowers.obj");
	witch.LoadModel("witchalt.obj");
	door.LoadModel("door.obj");

	/*
	* Code to attempt to try and load in the texture for a dirt path - this seemed to work
	* when shaders weren't modified so I assume it worked, however without working shaders this
	* was essentially non-functional
	/*
		
	glCreateTextures(GL_TEXTURE_2D, 1, &dirtTexture);
	stbi_set_flip_vertically_on_load(true);

	glBindTexture(GL_TEXTURE_2D, dirtTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int iWidth, iHeight, iChannels;
	//unsigned char* iData = stbi_load(dirt.c_str(), &iWidth, &iHeight, &iChannels, 0);
	unsigned char* iData = stbi_load("Ground_Dirt_008_baseColor.png", &iWidth, &iHeight, &iChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, iData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(iData);
	program = glCreateProgram();
	glLinkProgram(program);
	*/

	program = glCreateProgram();

	string vs_text = readShader("vs_model.glsl"); const char* vs_source = vs_text.c_str();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);
	checkErrorShader(vs);
	glAttachShader(program, vs);

	string fs_text = readShader("fs_model.glsl"); const char* fs_source = fs_text.c_str();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_source, NULL);
	glCompileShader(fs);
	checkErrorShader(fs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glUseProgram(program);

	// Start from the centre
	modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	// Set each meshes starting position to be the same so that interactivity functions as intended
	benchPos = glm::vec3(0.5f, 0.5f, 0.5f);
	pondPos = glm::vec3(0.5f, 0.5f, 0.5f);
	rocksPos = glm::vec3(0.5f, 0.5f, 0.5f);
	gravePos = glm::vec3(0.5f, 0.5f, 0.5f);
	dirtpathPos = glm::vec3(0.5f, 0.5f, 0.5f);
	flowersPos = glm::vec3(0.5f, 0.5f, 0.5f);
	witchPos = glm::vec3(0.5f, 0.5f, 0.5f);
	housePos = glm::vec3(0.5f, 0.5f, 0.5f);
	islandPos = glm::vec3(0.5f, 0.5f, 0.5f);
	doorPos = glm::vec3(0.5f, 0.5f, 0.5f);
	windmillbladesPos = glm::vec3(0.5f, 0.5f, 0.5f);
	windmillbasePos = glm::vec3(0.5f, 0.5f, 0.5f);

	//Set starting positions for various meshes rotations
	modelRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	windmillbladesRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	doorRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	//islandRotation = glm::vec3(0.0f, 0.0f, 0.0f);

	
	// A few optimizations.
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Calculate proj_matrix for the first time.
	aspect = (float)windowWidth / (float)windowHeight;
	proj_matrix = glm::perspective(glm::radians(fovy), aspect, 0.1f, 1000.0f);
}

void update(GLfloat currentTime) {
	// Calculate camera movement
	GLfloat cameraSpeed = 1.0f * deltaTime;
	if (keyStatus[GLFW_KEY_W]) cameraPosition += cameraSpeed * cameraFront;
	if (keyStatus[GLFW_KEY_S]) cameraPosition -= cameraSpeed * cameraFront;
	if (keyStatus[GLFW_KEY_A]) cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keyStatus[GLFW_KEY_D]) cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	// Rotate the island as necessary
	if (keyStatus[GLFW_KEY_LEFT])			modelRotation.y += 0.05f;
	if (keyStatus[GLFW_KEY_RIGHT])			modelRotation.y -= 0.05f;
	if (keyStatus[GLFW_KEY_UP])				modelRotation.x += 0.05f;
	if (keyStatus[GLFW_KEY_DOWN])			modelRotation.x -= 0.05f;
	if (keyStatus[GLFW_KEY_0])			    modelPosition.z += 0.10f;
	if (keyStatus[GLFW_KEY_1])	            modelPosition.z -= 0.10f;

	// Move rocks up the y axis
	if (keyStatus[GLFW_KEY_2])              rocksPos.y += 0.10f;
	// Move the bench through the x axis
	if (keyStatus[GLFW_KEY_3])              benchPos.x += 0.10f;
	// Move the island down the y axis
	if (keyStatus[GLFW_KEY_4])              islandPos.y -= 0.10f;
	// Rotate the windmill blades in one way
	if (keyStatus[GLFW_KEY_5])              windmillbladesRotation.x += 0.05f;
	// Rotate the blades the other way
	if (keyStatus[GLFW_KEY_6])              windmillbladesRotation.x -= 0.05f;
	// Rotate the rocks around the y axis
	if (keyStatus[GLFW_KEY_7])              rocksRotation.y += 0.10f;

	// Deconstruct the scene so objects go flying in various directions
	if (keyStatus[GLFW_KEY_8]) {
		islandPos.y -= 0.01f;
		housePos.z += 0.01f;
		doorPos.z += 0.01f;
		windmillbasePos.x += 0.01f;
		windmillbladesPos.x += 0.01f;
		rocksPos.y += 0.01f;
		witchPos.z += 0.01f;
		gravePos.x -= 0.01f;
		flowersPos.x += 0.01f;
		benchPos.x += 0.01f;
		housePos.y += 0.01f;
		pondPos.z -= 0.01f;
	}

	// Reconstruct the scene to bring all the objects back together
	if (keyStatus[GLFW_KEY_9]) {
		islandPos.y += 0.01f;
		housePos.z -= 0.01f;
		doorPos.z -= 0.01f;
		windmillbasePos.x -= 0.01f;
		windmillbladesPos.x -= 0.01f;
		rocksPos.y -= 0.01f;
		witchPos.z -= 0.01f;
		gravePos.x += 0.01f;
		flowersPos.x -= 0.01f;
		benchPos.x -= 0.01f;
		housePos.y -= 0.01f;
		pondPos.z += 0.01f;
	}

	// Move the houses door to the side on the x axis within a certain distance
	if (keyStatus[GLFW_KEY_O]) {
		if (doorPos.x >= 0.01f)
			doorPos.x -= 0.001f;
		}
	
	// The opposite direction of the above
	if (keyStatus[GLFW_KEY_P]) {
		if (doorPos.x <= 0.01f)
			doorPos.x += 0.001f;
	}

	
}

void render(GLfloat currentTime) {
	glViewport(0, 0, windowWidth, windowHeight);

	// Clear colour buffer
	glm::vec4 backgroundColor = glm::vec4(0.1f, 0.1f, 0.3f, 1.0f); glClearBufferfv(GL_COLOR, 0, &backgroundColor[0]);
	//glm::vec4 backgroundColor = glm::vec4(fs_in.tc.s, fs_in.tc.t, 0.3f, 1.0f); glClearBufferfv(GL_COLOR, 0, &backgroundColor[0]);

	// Clear deep buffer
	static const GLfloat one = 1.0f; glClearBufferfv(GL_DEPTH, 0, &one);

	// Enable blend
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use our shader programs
	glUseProgram(program);

	// Setup camera
	glm::mat4 viewMatrix = glm::lookAt(cameraPosition,					// eye
		cameraPosition + cameraFront,									// centre
		cameraUp);					
	
	// Set up a lighting system - most likely non-functional due to lack of working shaders but its there
	glUniform4f(glGetUniformLocation(program, "viewPosition"), cameraPosition.x, cameraPosition.y, cameraPosition.z, 1.0);
	glUniform4f(glGetUniformLocation(program, "lightPosition"), lightDisp.x, lightDisp.y, lightDisp.z, 1.0);
	glUniform4f(glGetUniformLocation(program, "ia"), ia.r, ia.g, ia.b, 1.0);
	glUniform1f(glGetUniformLocation(program, "ka"), ka);
	glUniform4f(glGetUniformLocation(program, "id"), id.r, id.g, id.b, 1.0);
	glUniform1f(glGetUniformLocation(program, "kd"), 1.0f);
	glUniform4f(glGetUniformLocation(program, "is"), is.r, is.g, is.b, 1.0);
	glUniform1f(glGetUniformLocation(program, "ks"), 1.0f);
	glUniform1f(glGetUniformLocation(program, "shininess"), 32.0f);;
	

	// Do some translations, rotations and scaling for each mesh
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), modelPosition);
		modelMatrix = glm::rotate(modelMatrix, modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mv_matrix = viewMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);

	// Bench adjustments

	glm::mat4 modelMatrix2 = glm::translate(glm::mat4(1.0f), benchPos);
	modelMatrix2 = glm::rotate(modelMatrix2, modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix2 = glm::rotate(modelMatrix2, modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mv_matrix2 = viewMatrix * modelMatrix2;

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix2[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);
	bench.Draw();

	// Rock adjustments

	glm::mat4 modelMatrix3 = glm::translate(glm::mat4(1.0f), rocksPos);
	modelMatrix3 = glm::rotate(modelMatrix3, modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix3 = glm::rotate(modelMatrix3, modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix3 = glm::rotate(modelMatrix3, rocksRotation.x, glm::vec3(0.01f, 0.0f, 0.0f));
	modelMatrix3 = glm::rotate(modelMatrix3, rocksRotation.y, glm::vec3(0.0f, 0.01f, 0.0f));
	modelMatrix3 = glm::rotate(modelMatrix3, rocksRotation.z, glm::vec3(0.0f, 0.0f, 0.01f));
	modelMatrix3 = glm::scale(modelMatrix3, glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mv_matrix3 = viewMatrix * modelMatrix3;

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix3[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);
	rocks.Draw();

	// island adjustments

	glm::mat4 modelMatrix4 = glm::translate(glm::mat4(1.0f), islandPos);
	modelMatrix4 = glm::rotate(modelMatrix4, modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix4 = glm::rotate(modelMatrix4, modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//modelMatrix4 = glm::rotate(modelMatrix4, islandRotation.x, glm::vec3(0.01f, 0.0f, 0.0f));
	//modelMatrix4 = glm::rotate(modelMatrix4, islandRotation.y, glm::vec3(0.0f, 0.01f, 0.0f));
	//modelMatrix4 = glm::rotate(modelMatrix4, islandRotation.z, glm::vec3(0.0f, 0.0f, 0.01f));
	modelMatrix4 = glm::scale(modelMatrix4, glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mv_matrix4 = viewMatrix * modelMatrix4;

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix4[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);

	island.Draw();

	// House adjustments

	glm::mat4 modelMatrix5 = glm::translate(glm::mat4(1.0f), housePos);
	modelMatrix5 = glm::rotate(modelMatrix5, modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix5 = glm::rotate(modelMatrix5, modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix5 = glm::scale(modelMatrix5, glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mv_matrix5 = viewMatrix * modelMatrix5;

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix5[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);
	house.Draw();

	// Pond adjustments

	glm::mat4 modelMatrix6 = glm::translate(glm::mat4(1.0f), pondPos);
	modelMatrix6 = glm::rotate(modelMatrix6, modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix6 = glm::rotate(modelMatrix6, modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix6 = glm::scale(modelMatrix6, glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mv_matrix6 = viewMatrix * modelMatrix6;

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix6[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);
	pond.Draw();

	// Grave adjustments

	glm::mat4 modelMatrix7 = glm::translate(glm::mat4(1.0f), gravePos);
	modelMatrix7 = glm::rotate(modelMatrix7, modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix7 = glm::rotate(modelMatrix7, modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix7 = glm::scale(modelMatrix7, glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mv_matrix7 = viewMatrix * modelMatrix7;

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix7[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);
	grave.Draw();

	//Dirt path adjustments - there was an attempt to load the texture in for this but it wasn't successful

	/* 
	GLint dirt_location = glGetUniformLocation(program, "dirt");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, dirtTexture);
	glUniform1i(dirt_location, 0);
	*/

	glm::mat4 modelMatrix8 = glm::translate(glm::mat4(1.0f), dirtpathPos);
	modelMatrix8 = glm::rotate(modelMatrix8, modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix8 = glm::rotate(modelMatrix8, modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix8 = glm::scale(modelMatrix8, glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mv_matrix8 = viewMatrix * modelMatrix8;

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix8[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);
	dirtpath.Draw();

	// Windmill base adjustments

	glm::mat4 modelMatrix9 = glm::translate(glm::mat4(1.0f), windmillbasePos);
	modelMatrix9 = glm::rotate(modelMatrix9, modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix9 = glm::rotate(modelMatrix9, modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix9 = glm::scale(modelMatrix9, glm::vec3(0.2f, 0.2f, 0.2f));
	modelMatrix9 = glm::rotate(modelMatrix9, windmillbaseRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix9 = glm::rotate(modelMatrix9, windmillbaseRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix9 = glm::rotate(modelMatrix9, windmillbaseRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 mv_matrix9 = viewMatrix * modelMatrix9;

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix9[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);
	windmillbase.Draw();

	// Windmill blade adjustments

	glm::mat4 modelMatrix10 = glm::translate(glm::mat4(1.0f), windmillbladesPos);
	modelMatrix10 = glm::rotate(modelMatrix10, modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix10 = glm::rotate(modelMatrix10, modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix10 = glm::rotate(modelMatrix10, windmillbladesRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix10 = glm::rotate(modelMatrix10, windmillbladesRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix10 = glm::rotate(modelMatrix10, windmillbladesRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix10 = glm::scale(modelMatrix10, glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mv_matrix10 = viewMatrix * modelMatrix10;

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix10[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);
	windmillblades.Draw();

	// Flower adjustments

	glm::mat4 modelMatrix11 = glm::translate(glm::mat4(1.0f), flowersPos);
	modelMatrix11 = glm::rotate(modelMatrix11, modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix11 = glm::rotate(modelMatrix11, modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix11 = glm::scale(modelMatrix11, glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mv_matrix11 = viewMatrix * modelMatrix11;

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix11[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);
	flowers.Draw();

	// Witch adjustments

	glm::mat4 modelMatrix12 = glm::translate(glm::mat4(1.0f), witchPos);
	modelMatrix12 = glm::rotate(modelMatrix12, modelRotation.x, glm::vec3(0.5f, 0.0f, 0.0f));
	modelMatrix12 = glm::rotate(modelMatrix12, modelRotation.y, glm::vec3(0.0f, 0.5f, 0.0f));
	modelMatrix12 = glm::scale(modelMatrix12, glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mv_matrix12 = viewMatrix * modelMatrix12;

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix12[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);
	witch.Draw();

	// Door adjustments

	glm::mat4 modelMatrix13 = glm::translate(glm::mat4(1.0f), doorPos);
	modelMatrix13 = glm::rotate(modelMatrix13, modelRotation.x, glm::vec3(0.5f, 0.0f, 0.0f));
	modelMatrix13 = glm::rotate(modelMatrix13, modelRotation.y, glm::vec3(0.0f, 0.5f, 0.0f));
	modelMatrix13 = glm::rotate(modelMatrix13, doorRotation.x, glm::vec3(0.01f, 0.0f, 0.0f));
	modelMatrix13 = glm::rotate(modelMatrix13, doorRotation.y, glm::vec3(0.0f, 0.01f, 0.0f));
	modelMatrix13 = glm::rotate(modelMatrix13, doorRotation.z, glm::vec3(0.0f, 0.0f, 0.01f));
	modelMatrix13 = glm::scale(modelMatrix13, glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mv_matrix13 = viewMatrix * modelMatrix13;

	glUniformMatrix4fv(glGetUniformLocation(program, "model_matrix"), 1, GL_FALSE, &modelMatrix13[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);
	door.Draw();

}

void onResizeCallback(GLFWwindow* window, int w, int h) {
	windowWidth = w;
	windowHeight = h;

	if (windowWidth > 0 && windowHeight > 0) { // Avoid issues when minimising window - it gives size of 0 which fails division.
		aspect = (float)w / (float)h;
		proj_matrix = glm::perspective(glm::radians(fovy), aspect, 0.1f, 1000.0f);
	}
}

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) keyStatus[key] = true;
	else if (action == GLFW_RELEASE) keyStatus[key] = false;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

}

void onMouseMoveCallback(GLFWwindow* window, double x, double y) {
	int mouseX = static_cast<int>(x);
	int mouseY = static_cast<int>(y);

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(front);
}

static void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset) {
	int yoffsetInt = static_cast<int>(yoffset);
}

void debugGL() {
	//Output some debugging information
	cout << "VENDOR: " << (char *)glGetString(GL_VENDOR) << endl;
	cout << "VERSION: " << (char *)glGetString(GL_VERSION) << endl;
	cout << "RENDERER: " << (char *)glGetString(GL_RENDERER) << endl;

	// Enable Opengl Debug
	//glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback((GLDEBUGPROC)openGLDebugCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
}

static void APIENTRY openGLDebugCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const GLvoid* userParam) {

	cout << "---------------------opengl-callback------------" << endl;
	cout << "Message: " << message << endl;
	cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << " --- ";

	cout << "id: " << id << " --- ";
	cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "HIGH";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		cout << "NOTIFICATION";
	}
	cout << endl;
	cout << "-----------------------------------------" << endl;
}

string readShader(string name) {
	string vs_text;
	std::ifstream vs_file(name);

	string vs_line;
	if (vs_file.is_open()) {

		while (getline(vs_file, vs_line)) {
			vs_text += vs_line;
			vs_text += '\n';
		}
		vs_file.close();
	}
	return vs_text;
}

void  checkErrorShader(GLuint shader) {
	// Get log lenght
	GLint maxLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	// Init a string for it
	std::vector<GLchar> errorLog(maxLength);

	if (maxLength > 1) {
		// Get the log file
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		cout << "--------------Shader compilation error-------------\n";
		cout << errorLog.data();
	}
}
