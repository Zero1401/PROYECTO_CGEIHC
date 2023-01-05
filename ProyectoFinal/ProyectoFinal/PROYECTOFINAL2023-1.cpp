#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "modelAnim.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();
void animacion2();
void animacion3();
void animacion4();
void animacion5();
void animacion6();
void animacion7();
void animacion8();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-100.0f, 2.0f, -45.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;
float movCamera = 0.0f;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(0.0f, 0.0f, 0.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

bool active;

//Animación del coche
float movKitY = 0.0;
float movKitX = 0.0;
float rotKit = 0.0;

float movKitX2 = 0.0;
float movKitZ2 = 0.0;
float rotKit2 = 0.0;

float movKitX3 = 0.0;
float movKitZ3 = 0.0;
float rotKit3 = 0.0;


float rotcola = 0.0;
float rotcola2 = 0.0;
float rotcola3 = 0.0;

float PenX, PenZ = 0.0f;
float PenRotG, PenRotI = 0.0f;

float rotV, rotV2 = 0.0f;
float rotP = 0.0f;
float movXV, movZV = 0.0f;

bool circuito = false;
bool circuito3 = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;
bool recorrido6 = false;
bool bandera = false;

bool circuito2 = false;
bool recorrido12 = true;
bool recorrido22 = false;
bool recorrido32 = false;
bool recorrido42 = false;
bool recorrido52 = false;
bool recorrido62 = false;
bool bandera2 = false;

bool recorrido123 = true;
bool recorrido223 = false;
bool recorrido323 = false;
bool recorrido423 = false;
bool recorrido523 = false;
bool recorrido623 = false;

bool animcola = false;
bool animcola2 = false;

bool Pen = true;
bool Pen1 = true;
bool Pen2, Pen3, Pen4 = false;

bool PueAC1, PueAC2 = false;
bool PueCA1, PueCA2 = false;

float rotantena = 0;
float rotantena2 = 0;
bool animantena;
bool animantena2;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
float posX =PosIni.x, posY = PosIni.y, posZ = PosIni.z, rotRodIzq = 0;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotRodIzq;
	float rotInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;


float tiempo = 1.0f;
float speed = 1.0f;


// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(posX,posY,posZ),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0)
};

glm::vec3 LightP1;




void saveFrame(void)
{

	printf("posx %f\n", posX);
	
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;
	
	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;

}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;
	
	KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;

}




int main()
{
	// Init GLFW
	glfwInit();




	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.frag");
	Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");
	Shader Anim2("Shaders/anim2.vs", "Shaders/anim2.frag");

	Model BotaDer((char*)"Models/Personaje/bota.obj");
	Model Fuente((char*)"Models/Fish/Fuente.obj");
	Model Conchas((char*)"Models/Conchas.obj");
	Model Restaurante((char*)"Models/Fish/Restaurante.obj");
	Model Submarino((char*)"Models/Fish/Submarino1.obj");
	Model Submarino2((char*)"Models/Fish/Submarino2.obj");

	Model Suelo((char*)"Models/Fish/Suelo.obj");
	Model Domo((char*)"Models/Fish/Edificio/domo2.obj"); //
	Model Tiburon1((char*)"Models/Fish/Tiburon1.obj");
	Model Ballena1((char*)"Models/Fish/Ballena1.obj");
	Model Calamar1((char*)"Models/Fish/Calamar1.obj");
	Model banner((char*)"Models/Fish/banner.obj");
	Model caminos((char*)"Models/Fish/Edificio/caminos.obj");//
	Model domos((char*)"Models/Fish/Edificio/domos2.obj");//
	Model expo((char*)"Models/Fish/animales.obj");
	Model corales1((char*)"Models/Fish/Modelos/corales1.obj");
	Model corales2((char*)"Models/Fish/Modelos/corales2.obj");
	Model corales3((char*)"Models/Fish/Modelos/corales3.obj");
	Model corales4((char*)"Models/Fish/Modelos/corales4.obj");
	Model corales5((char*)"Models/Fish/Modelos/corales5.obj");
	Model corales6((char*)"Models/Fish/Modelos/corales6.obj");
	Model edificio((char*)"Models/Fish/Edificio/edificio.obj"); //
	Model Alga((char*)"Models/Fish/Modelos/alga.obj");
	Model welcome((char*)"Models/Fish/welcome.obj");
	Model adorno((char*)"Models/Fish/adorno.obj");
	Model banca((char*)"Models/Fish/banca.obj");
	Model cuerpot((char*)"Models/Fish/cuerpot.obj");
	Model colat((char*)"Models/Fish/colat.obj");
	Model mesas((char*)"Models/Fish/mesas.obj");
	Model pantalla((char*)"Models/Fish/pantalla.obj");
	Model burbuja1((char*)"Models/Fish/burbuja1.obj");
	Model burbuja2((char*)"Models/Fish/burbuja2.obj");
	Model burbuja3((char*)"Models/Fish/burbuja3.obj");
	Model pecera1((char*)"Models/Fish/pecera1.obj");
	Model pecera2((char*)"Models/Fish/pecera2.obj");
	Model pecera3((char*)"Models/Fish/pecera3.obj");
	Model adornotv((char*)"Models/Fish/adornotv.obj");
	Model mesacoral((char*)"Models/Fish/mesacoral.obj");
	Model inicio((char*)"Models/Fish/inicio.obj");
	Model baños((char*)"Models/Fish/baños.obj");
	Model peces((char*)"Models/Fish/peces.obj");
	Model peces2((char*)"Models/Fish/peces2.obj");
	Model antenasup((char*)"Models/Fish/antenasup.obj");
	Model antenainf((char*)"Models/Fish/antenainf.obj");
	Model base((char*)"Models/Fish/Jaula/Base.obj");
	Model ebase((char*)"Models/Fish/Jaula/EstanqueB.obj");
	Model isla((char*)"Models/Fish/Jaula/Isla.obj");
	Model jaula((char*)"Models/Fish/Jaula/Jaula.obj");
	Model sea((char*)"Models/Fish/Jaula/Sea.obj");
	Model penguin((char*)"Models/Fish/Penguin/PenguinC.obj");
	Model PiA((char*)"Models/Fish/Edificio/PiA.obj");
	Model PA((char*)"Models/Fish/Edificio/PA.obj");
	Model valvula((char*)"Models/Fish/Edificio/Valvula.obj");









	// Build and compile our shader program

	//Inicialización de KeyFrames
	
	for(int i=0; i<MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].rotInc = 0;
	}



	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");
	
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();
		animacion2();
		animacion3();
		animacion4();
		animacion5();
		animacion6();
		animacion7();
		animacion8();
		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/


		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp



		//Carga de modelo 
		//Personaje


		//view = camera.GetViewMatrix();
		//glm::mat4 model(1);
		//tmp = model = glm::translate(model, glm::vec3(0, 1, 0));
		//model = glm::translate(model,glm::vec3(posX,posY,posZ));
		//model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 90.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Fuente.Draw(lightingShader);
		glBindVertexArray(0);

		/*	model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-15.0f, 0.0f, -10.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Conchas.Draw(lightingShader);
			glBindVertexArray(0);*/

			/*	model = glm::mat4(1);
				model = glm::translate(model, glm::vec3(0.0f, 0.0f,	0.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				Restaurante.Draw(lightingShader);
				glBindVertexArray(0);*/


		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Suelo.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Submarino2.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		expo.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		corales1.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		corales2.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		corales3.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		corales4.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		corales5.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		corales6.Draw(lightingShader);
		glBindVertexArray(0);


		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		edificio.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		adorno.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		banca.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		welcome.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pantalla.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pecera1.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		baños.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		inicio.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pecera2.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pecera3.Draw(lightingShader);
		glBindVertexArray(0);
	
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		adornotv.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mesacoral.Draw(lightingShader);
		glBindVertexArray(0);


		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotantena), glm::vec3(0.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		antenasup.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		antenainf.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mesas.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		base.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ebase.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		isla.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		jaula.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(77.5f, -1.3f, 43.0f));
		model = glm::translate(model, glm::vec3(PenZ, 0.0f, PenX));
		model = glm::rotate(model, glm::radians(PenRotG), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		penguin.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PiA.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(27.5f, 0.0f, -12.0f));
		model = glm::rotate(model, glm::radians(-57.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotP), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PA.Draw(lightingShader);
		glBindVertexArray(0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(25.0f, 3.0f, -16.0f));
		model = glm::rotate(model, glm::radians(-57.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotV), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::translate(model, glm::vec3(0.0f, movXV, movZV));
		model = glm::rotate(model, glm::radians(-rotP), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		valvula.Draw(lightingShader);
		glBindVertexArray(0);


		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.05f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Domo.Draw(lightingShader);
		glDisable(GL_BLEND);
		glBindVertexArray(0);

		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.001f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		burbuja1.Draw(lightingShader);
		glDisable(GL_BLEND);
		glBindVertexArray(0);

		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.001f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		burbuja2.Draw(lightingShader);
		glDisable(GL_BLEND);
		glBindVertexArray(0);

		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.001f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		burbuja3.Draw(lightingShader);
		glDisable(GL_BLEND);
		glBindVertexArray(0);

		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.05f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		caminos.Draw(lightingShader);
		glDisable(GL_BLEND);
		glBindVertexArray(0);

		


		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.05f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		domos.Draw(lightingShader);
		glDisable(GL_BLEND);
		glBindVertexArray(0);


		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX, movKitY, 0));
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Submarino.Draw(lightingShader);
		glBindVertexArray(0);


		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX2, 0, movKitZ2));
		model = glm::rotate(model, glm::radians(rotKit2), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cuerpot.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX2, 0, movKitZ2));
		model = glm::rotate(model, glm::radians(rotcola2), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotcola), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		colat.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX2, 0, movKitZ2));
		model = glm::rotate(model, glm::radians(rotKit2), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		peces.Draw(lightingShader);
		
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX3, 0, movKitZ3));
		model = glm::rotate(model, glm::radians(rotKit3), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		peces2.Draw(lightingShader);




		Anim.Use();
		modelLoc = glGetUniformLocation(Anim.Program, "model");
		viewLoc = glGetUniformLocation(Anim.Program, "view");
		projLoc = glGetUniformLocation(Anim.Program, "projection");
		tiempo = glfwGetTime() * speed;
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Alga.Draw(Anim);
		glBindVertexArray(0);

		Anim2.Use();
		modelLoc = glGetUniformLocation(Anim.Program, "model");
		viewLoc = glGetUniformLocation(Anim.Program, "view");
		projLoc = glGetUniformLocation(Anim.Program, "projection");
		tiempo = glfwGetTime() * speed;
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		sea.Draw(Anim2);
		glBindVertexArray(0);


	
		

	
		//Traslucidez

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		//objTras.Draw(lightingShader);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default




		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();




	return 0;
}


void animacion()
{

		//Movimiento del personaje

		if (play)
		{
			if (i_curr_steps >= i_max_steps) //end of animation between frames?
			{
				playIndex++;
				if (playIndex>FrameIndex - 2)	//end of total animation?
				{
					printf("termina anim\n");
					playIndex = 0;
					play = false;
				}
				else //Next frame interpolations
				{
					i_curr_steps = 0; //Reset counter
									  //Interpolation
					interpolation();
				}
			}
			else
			{
				//Draw animation
				posX += KeyFrame[playIndex].incX;
				posY += KeyFrame[playIndex].incY;
				posZ += KeyFrame[playIndex].incZ;

				rotRodIzq += KeyFrame[playIndex].rotInc;

				i_curr_steps++;
			}

		}
	}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex<MAX_FRAMES)
		{
			saveFrame();
		}

		rot =-25.0f;//Variable que maneja el giro de la camara

	}


	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{
		range += 0.1;
		rot += 1;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_2])
	{
		range -= 0.1;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_3])
	{
		range += 0.1;
		printf("El spotangle es %f\n", range);
	}

	if (keys[GLFW_KEY_4])
	{
		range -= 0.1;
		printf("El spotangle es %f\n", range);
	}

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		//pointLightPositions[0].x -= 0.1f;
		//pointLightPositions[0].y -= 0.1f;
		pointLightPositions[0].z += 0.1f;
	}
	if (keys[GLFW_KEY_G])
	{
		//pointLightPositions[0].x -= 0.1f;
		//pointLightPositions[0].y -= 0.1f;
		pointLightPositions[0].z -= 0.1f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[1].x += 0.1f;
		pointLightPositions[1].y += 0.1f;
		pointLightPositions[1].z += 0.1f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[1].x -= 0.1f;
		pointLightPositions[1].y -= 0.1f;
		pointLightPositions[1].z -= 0.1f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[2].x += 0.1f;
		pointLightPositions[2].y += 0.1f;
		pointLightPositions[2].z += 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[2].x -= 0.1f;
		pointLightPositions[2].y -= 0.1f;
		pointLightPositions[2].z -= 0.1f;
	}
	if (keys[GLFW_KEY_I])
	{
		pointLightPositions[3].x += 0.1f;
		pointLightPositions[3].y += 0.1f;
		pointLightPositions[3].z += 0.1f;
		circuito = true;
		circuito2 = true;
		circuito3 = true;
		animcola = true;
		animcola2 = true;
		animantena = true;
		animantena2 = false;
	}

	if (keys[GLFW_KEY_O])
	{
		circuito = false;
		circuito2 = false;
		circuito3 = false;
		animcola = false;
		animcola2 = false;
	
	}
	if (keys[GLFW_KEY_K])
	{
		pointLightPositions[3].x -= 0.1f;
		pointLightPositions[3].y -= 0.1f;
		pointLightPositions[3].z -= 0.1f;
	}

	if (keys[GLFW_KEY_Z] && rotV < 270)
	{
		PueAC1 = true;
	}

	if (keys[GLFW_KEY_X] && rotV >= 270)
	{
		PueCA1 = true;
	}

}


void animacion2()
{

	if (circuito)
	{
		if (recorrido1)
		{
			movKitX -= 0.05f;
			if (movKitX > -10)
			{
				recorrido1 = false;

				if (bandera)
				{


					bandera = false;
					recorrido6 = true;

				}
				else 	recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			rotKit = 0;
			movKitX += 0.05f;
			if (movKitX > -5)
			{
				recorrido2 = false;
				recorrido3 = true;


			}
		}

		if (recorrido3)
		{
			rotKit = 0;
			movKitX += 0.05f;
			if (movKitX > -10)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}

		if (recorrido4)
		{
			rotKit = 0;
			movKitY += 0.05f;
			if (movKitY > 90)
			{
				recorrido4 = false;
				recorrido5 = true;
			}
		}

		if (recorrido5)
		{
			rotKit = 0;
			movKitY -= 0.05f;
			if (movKitY < 0)
			{
				recorrido5 = false;
				recorrido4 = true;
			}
		}


	}
}


void animacion3()
{

	if (circuito2)
	{
		if (recorrido12)
		{
			rotKit2 = 0;
			movKitZ2 -= 0.1f;
			if (movKitZ2 < -90)
			{
				recorrido12 = false;
				recorrido22 = true;
			}
		}
		if (recorrido22)
		{
			rotKit2 = 270;
			movKitX2 += 0.1f;
			if (movKitX2 > 20)
			{
				recorrido22 = false;
				recorrido32 = true;


			}
		}

		if (recorrido32)
		{
			rotKit2 = 180;
			movKitZ2 += 0.1f;
			if (movKitZ2 > 0)
			{
				recorrido32 = false;
				recorrido42 = true;
			}
		}

		if (recorrido42)
		{
			rotKit2 = 90;
			movKitX2 -= 0.1f;
			if (movKitX2 < 0)
			{
				recorrido42 = false;
				recorrido52 = true;
			}
		}

		if (recorrido52)
		{
			rotKit2 = 0;
			movKitZ2 -= 0.1f;
			if (movKitX2 < -90)
			{
				recorrido52 = false;
				recorrido22 = true;
			}
		}




	}
}

void animacion4() {



	if (animcola) {
		if (rotcola2 <= 1.0f) {
			rotcola2 += 0.05f;
			if (rotcola2 >= 1.0f) {
				animcola = false;
				animcola2 = true;
			}
		}


	}


	if (animcola2) {
		if (rotcola2 >= -1.0f) {
			rotcola2 -= 0.05f;
			if (rotcola3 <= -1.0f) {
				animcola = true;
				animcola2 = false;
			}
		}
	}


	if (circuito2)
	{
		if (recorrido12)
		{
			rotcola = 0;
			movKitZ2 -= 0.1f;
			if (movKitZ2 < -90)
			{
				recorrido12 = false;
				recorrido22 = true;
			}
		}
		if (recorrido22)
		{
			rotcola = 270;
			movKitX2 += 0.1f;
			if (movKitX2 > 20)
			{
				recorrido22 = false;
				recorrido32 = true;


			}
		}

		if (recorrido32)
		{
			rotcola = 180;
			movKitZ2 += 0.1f;
			if (movKitZ2 > 0)
			{
				recorrido32 = false;
				recorrido42 = true;
			}
		}

		if (recorrido42)
		{
			rotcola = 90;
			movKitX2 -= 0.1f;
			if (movKitX2 < 0)
			{
				recorrido42 = false;
				recorrido52 = true;
			}
		}

		if (recorrido52)
		{
			rotcola = 0;
			movKitZ2 -= 0.1f;
			if (movKitX2 < -90)
			{
				recorrido52 = false;
				recorrido22 = true;
			}
		}




	}

}


void animacion5()
{

	if (circuito3)
	{
		if (recorrido123)
		{
			rotKit3 = 0;
			movKitZ3 += 0.8f;
			if (movKitZ3 > 90)
			{
				recorrido123 = false;
				recorrido223 = true;
			}
		}
		if (recorrido223)
		{
			rotKit3 = 90;
			movKitX3 += 0.8f;
			if (movKitX3 > 20)
			{
				recorrido223 = false;
				recorrido323 = true;


			}
		}

		if (recorrido323)
		{
			rotKit3 = 180;
			movKitZ3 -= 0.8f;
			if (movKitZ3 < 0)
			{
				recorrido323 = false;
				recorrido423 = true;
			}
		}

		if (recorrido423)
		{
			rotKit3 = 270;
			movKitX3 -= 0.8f;
			if (movKitX3 < 0)
			{
				recorrido423 = false;
				recorrido523 = true;
			}
		}

		if (recorrido523)
		{
			rotKit3 = 0;
			movKitZ3 += 0.8f;
			if (movKitZ3 > 90)
			{
				recorrido523 = false;
				recorrido223 = true;
			}
		}

	}
}

void animacion6() {
	if (animantena) {
		if (rotantena <= 1.0f) {
			rotantena += 0.001f;
			if (rotantena >= 1.0f) {
				animantena = false;
				animantena2 = true;
			}
		}


	}


	if (animantena2) {
		if (rotantena >= -1.0f) {
			rotantena -= 0.001f;
			if (rotantena <= -1.0f) {
				animantena = true;
				animantena2 = false;
			}
		}
	}
}

void animacion7() {
	if (Pen) {
			if (Pen1) {
				PenX -= 0.1f;
				if (PenX <= -5.5f) {
					Pen1 = false;
					Pen2 = true;
				}
			}
			if (Pen2) {
				PenZ += 0.1f;
				PenRotG -= 6.0f;
				if (PenZ >= 3.0f) {
					Pen2 = false;
					Pen3 = true;
				}
			}
			if (Pen3) {
				PenX += 0.1f;
				if (PenX >= 0.0f) {
					Pen3 = false;
					Pen4 = true;
				}
			}
			if (Pen4) {
				PenZ -= 0.1f;
				PenRotG -= 6.0f;
				if (PenZ <= 0.0f) {
					Pen4 = false;
					Pen1 = true;
					PenRotG = 1.0f; //????
				}
			}
	}
}

void animacion8() {
	if (PueAC1) {
		rotV += 1.0f;
		if (rotV >= 270) {
			PueAC1 = false;
			PueAC2 = true;
		}
	}
	if (PueAC2) {
		rotP += 1.0f;
		movXV += 0.055f;
		movZV += 0.055f;
		if (rotP > 90) {
			PueAC2 = false;
		}
	}

	if (PueCA1) {
		rotP -= 1.0f;
		movXV -= 0.055f;
		movZV -= 0.055f;
		if (rotP < 0) {
			PueCA1 = false;
			PueCA2 = true;
		}
	}
	if (PueCA2) {
		rotV -= 1.0f;
		if (rotV <= 0) {
			PueCA2 = false;
		}
	}
}