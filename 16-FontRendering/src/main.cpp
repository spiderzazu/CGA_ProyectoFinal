#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

// contains new std::shuffle definition
#include <algorithm>
#include <random>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

// Font rendering
#include "Headers/FontTypeRendering.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// ShadowBox include
#include "Headers/ShadowBox.h"

// OpenAL include
#include <AL/alut.h>


#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
//Shader para las particulas de fountain
Shader shaderParticlesFountain;
//Shader para las particulas de fuego
Shader shaderParticlesFire;
//Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;
//Shader para dibujar objeto con solo textura
Shader shaderTexture;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 7.0;

Sphere skyboxSphere(20, 20);
Box boxCollider;
Sphere sphereCollider(10, 10);
Box boxViewDepth;
Box boxLightViewBox;
Box boxMenu;

ShadowBox *shadowBox;

//Game Variables
bool vivo = false, muestraFinal = false, principal = true, resultadoPartida = false, onSalida = false;
float dificultad = 0.1;
int maxmonedas = 15;

//Control type
int tipoControl = 0;

// Models complex instances
Model modelEnemy;

//Arboles
Model modelArbol;

//trineos
Model modelTrineo;

//snowman
Model modelSnowman;

//estrellas
Model modelEstrella;


//monedas
Model modelMoneda;

// Hierba
Model modelGrass;
// Fountain
Model modelFountain;

//Menu
Model menuImage;
// Model animate instance
// 
// Mario
Model marioModelAnimate;


// Terrain model instance
Terrain terrain(-1, -1, 250, 16, "../Textures/Terreno.png");

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID,
		textureLandingPadID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID,
		textureTerrainBID, textureTerrainBlendMapID;
GLuint textureParticleFountainID, textureParticleFireID, texId;
GLuint skyboxTextureID;
GLuint textureMenu0_Ini, textureMenu0_Out, textureMenu1_Ini, textureMenu1_Out, textureGO_Ini, textureGO_Out, textureWin_Ini, textureWin_Out, textureActivaID;

// Modelo para el redener de texto
FontTypeRendering::FontTypeRendering *modelText;
FontTypeRendering::FontTypeRendering *damageText;
FontTypeRendering::FontTypeRendering *menuP;
FontTypeRendering::FontTypeRendering *menuP2;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_frost/a_rt.tga",
		"../Textures/mp_frost/a_lf.tga",
		"../Textures/mp_frost/a_up.tga",
		"../Textures/mp_frost/a_dn.tga",
		"../Textures/mp_frost/a_ft.tga",
		"../Textures/mp_frost/a_bk.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 modelMatrixEnemy = glm::mat4(1.0);
glm::mat4 modelMatrixMario = glm::mat4(1.0f);
glm::mat4 modelMatrixFountain = glm::mat4(1.0f);
glm::mat4 modelMatrixArbol = glm::mat4(1.0);
glm::mat4 modelMatrixSnowman = glm::mat4(1.0);
glm::mat4 modelMatrixTrineo = glm::mat4(1.0);
glm::mat4 modelMatrixMenu = glm::mat4(1.0);

int animationIndex = 2;
int modelSelected = 2;
bool enableCountSelected = true;

int contador_txt=0;
int contador_damage = 5;

//// Variables to animations keyframes
//bool saveFrame = false, availableSave = true;
//std::ofstream myfile;
//std::string fileName = "";
//bool record = false;

// Var animate enemy
int stateEnemy = 0;
float EnemySteep = 0.0;

//Estrellas posicion

std::vector<glm::vec3> estrellaPosition = { 
	glm::vec3(0,0,0),
	glm::vec3(55,0,-19.89),
	glm::vec3(25,0,38),
	glm::vec3(-96,0,69),
	glm::vec3(-124.4,0,17.6),
	glm::vec3(-61.6,0,-89.1),
	glm::vec3(-43.5,0,-143.5)
	
};


std::vector<float> estrellaOrientation = { 21.37 + 90, -65.0 + 90 };

// monedas positions
std::vector<glm::vec3> monedaPosition = {

	glm::vec3(80,0,19),
	glm::vec3(20,0,-54.1),
	glm::vec3(-53.9,0,-35.2),
	glm::vec3(-38.4,0,32.9),
	glm::vec3(-118.6,0,-101.8),
	glm::vec3(3.6,0,-99.9),
	glm::vec3(-129,0,-29),
	glm::vec3(39,0,22),
	glm::vec3(-0.32,0,-46.16),
	glm::vec3(88,0,-141),

	glm::vec3(-96.1,0,70.3),
	glm::vec3(-91.1,0,-57.4),
	glm::vec3(5,0,-40),
	glm::vec3(42.8,0,-81.64),


	glm::vec3(-61.27,0,-91.92),
	glm::vec3(18.6,0,88.5),

	glm::vec3(0,0,0),
	glm::vec3(61.48,0,10.96),
	glm::vec3(-14.79,0,-143.63),

};

//monedas render
bool monedasRender[19] = { true,true,true,true, true, true,true,true, true,true,true,true,true, true, true,true,true, true,true };

// Enemy positions
std::vector<glm::vec3> enemyPosition = { 
	glm::vec3(80,0,19.5),
	glm::vec3(20,0,-53.6),
	glm::vec3(-53.9,0,-34.7),
	glm::vec3(-38.4,0,33.4),
	glm::vec3(-118.6,0,-101.3),
	glm::vec3(3.6,0,-99.4),
	glm::vec3(-129,0,-28.5),
	glm::vec3(39,0,22.5),
	glm::vec3(-0.32,0,-45.66),
	glm::vec3(88,0,-140.5),

	glm::vec3(-96.1,0,70.8),
	glm::vec3(-91.1,0,-56.9),
	glm::vec3(42.8,0,-81.14),

	glm::vec3(-61.27,0,-91.42),
	glm::vec3(18.6,0,89),

};
std::vector<glm::vec3> enemyPosition2 = { 
	glm::vec3(80.5,0,19),
	glm::vec3(20.5,0,-54.1),
	glm::vec3(-53.4,0,-35.2),
	glm::vec3(-37.9,0,32.9),
	glm::vec3(-118.1,0,-101.8),
	glm::vec3(4.1,0,-99.9),
	glm::vec3(-128.5,0,-29),
	glm::vec3(39.5,0,22),
	glm::vec3(0.18,0,-46.16),
	glm::vec3(88.5,0,-141),


	glm::vec3(-95.6,0,70.3),
	glm::vec3(-90.6,0,-57.4),
	glm::vec3(43.3,0,-81.64),
	glm::vec3(-60.77,0,-91.92),
	glm::vec3(19.1,0,88.5),



};

std::vector<glm::vec3> enemyPosition3 = {
	glm::vec3(80,0,18.5),
	glm::vec3(20,0,-54.6),
	glm::vec3(-53.9,0,-35.7),
	glm::vec3(-38.4,0,32.4),
	glm::vec3(-118.6,0,-102.3),
	glm::vec3(3.6,0,-100.4),
	glm::vec3(-129,0,-29.5),
	glm::vec3(39,0,21.5),
	glm::vec3(-0.32,0,-46.66),
	glm::vec3(88,0,-141.5),


	glm::vec3(-96.1,0,69.8),
	glm::vec3(-91.1,0,-57.9),
	glm::vec3(42.8,0,-82.14),
	glm::vec3(-61.27,0,-92.42),
	glm::vec3(18.6,0,88),


};

std::vector<glm::vec3> enemyPosition4 = {
	glm::vec3(79.5,0,19),
	glm::vec3(19.5,0,-54.1),
	glm::vec3(-54.4,0,-35.2),
	glm::vec3(-38.9,0,32.9),
	glm::vec3(-119.1,0,-101.8),
	glm::vec3(3.1,0,-99.9),
	glm::vec3(-129.5,0,-29),
	glm::vec3(38.5,0,22),
	glm::vec3(-0.82,0,-46.16),
	glm::vec3(87.5,0,-141),

	glm::vec3(-96.6,0,70.3),
	glm::vec3(-91.6,0,-57.4),
	glm::vec3(42.3,0,-81.64),
	glm::vec3(-61.77,0,-91.92),
	glm::vec3(18.1,0,88.5),



};


// Enemy renders

bool enemyRender[15] = { true, true, true,true, true, true,true, true, true, true,true, true, true,true, true };
bool enemyRender2[15] = { true, true, true,true, true, true,true, true, true, true,true, true, true,true, true };
bool enemyRender3[15] = { true, true, true,true, true, true,true, true, true, true,true, true, true,true, true };
bool enemyRender4[15] = { true, true, true,true, true, true,true, true, true, true,true, true, true,true, true };

//Arbol position
std::vector<glm::vec3> arbolPosition = { 
	glm::vec3(0, 0, -19),
	glm::vec3(0,0,-30), 
	glm::vec3(45,0,-29),
	glm::vec3(44,0,10),
	glm::vec3(73,0,-110),
	glm::vec3(11,0,-78),
	glm::vec3(-80,0,-40),
	glm::vec3(-16,0,-19),
	glm::vec3(-84,0,-42),
	glm::vec3(-110,0,-57),
	glm::vec3(-122,0,-122),
	glm::vec3(86,0,-129),
	glm::vec3(-40,0,-121),
	glm::vec3(32,0,-107),
	glm::vec3(54,0,-27),
	glm::vec3(33,0,58),
	glm::vec3(17,0,-66),
	glm::vec3(61,0,-25),
	glm::vec3(44,0,-10),
	glm::vec3(46,0,65),
	glm::vec3(-49,0,85),
	glm::vec3(-45,0,-37),
	glm::vec3(17,0,-59),
	glm::vec3(-107.4,0, 31),
	glm::vec3(-57,0, 22),
	glm::vec3(-23,0, 43)
};

std::vector<glm::vec3> snowmanPosition = {
	glm::vec3(10, 0, -19),
	glm::vec3(10,0,0.19),
	glm::vec3(58.3,0,-13.27),
	glm::vec3(-34,0,-13),
	glm::vec3(-67,0,-75),
	glm::vec3(108,0,-124),
	glm::vec3(-44,0,49),
	glm::vec3(-140,0,93),
	glm::vec3(-70,0,-136),
	glm::vec3(-17,0,-120)

};

std::vector<glm::vec3> trineoPosition = {
	glm::vec3(-105,0,-136),
	glm::vec3(25.4,0,60),
	glm::vec3(-100,0,83.6)

};

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = { { "fountain", glm::vec3(5.0, 0.0, -40.0) }, { "fire", glm::vec3(0.0, 0.0, 0.0) } };

double deltaTime;
double currTime, lastTime;

// Jump variables
bool isJump = false;
float GRAVITY = 3.5;
double tmv = 0;
double startTimeJump = 0;

// Definition for the particle system
GLuint initVel, startTime;
GLuint VAOParticles;
GLuint nParticles = 8000;
double currTimeParticlesAnimation, lastTimeParticlesAnimation;

// Definition for the particle system fire
GLuint initVelFire, startTimeFire;
GLuint VAOParticlesFire;
GLuint nParticlesFire = 2000;
GLuint posBuf[2], velBuf[2], age[2];
GLuint particleArray[2];
GLuint feedback[2];
GLuint drawBuf = 1;
float particleSize = 0.5, particleLifetime = 3.0;
double currTimeParticlesAnimationFire, lastTimeParticlesAnimationFire;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// Framesbuffers
GLuint depthMap, depthMapFBO;

/**********************
 * OpenAL config
 */

// OpenAL Defines
#define NUM_BUFFERS 7
#define NUM_SOURCES 7
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
// Source 3
ALfloat source3Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source3Vel[] = { 0.0, 0.0, 0.0 };
// Source 4
ALfloat source4Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source4Vel[] = { 0.0, 0.0, 0.0 };
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = { true, true, false, false, false };

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
void prepareScene();
void prepareDepthScene();
void renderScene(bool renderParticles = true);
void resetGame();

void initParticleBuffers() {
	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL,
			GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f,
				((float) rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float) rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float) rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticles);
	glBindVertexArray(VAOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void initParticleBuffersFire() {
	// Generate the buffers
	glGenBuffers(2, posBuf);    // position buffers
	glGenBuffers(2, velBuf);    // velocity buffers
	glGenBuffers(2, age);       // age buffers

	// Allocate space for all buffers
	int size = nParticlesFire * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

	// Fill the first age buffer
	std::vector<GLfloat> initialAge(nParticlesFire);
	float rate = particleLifetime / nParticlesFire;
	for (unsigned int i = 0; i < nParticlesFire; i++) {
		int index = i - nParticlesFire;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine { };
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArray);

	// Set up particle array 0
	glBindVertexArray(particleArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Set up particle array 1
	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize(
			"../Shaders/iluminacion_textura_animation_shadow.vs",
			"../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs",
			"../Shaders/terrain_shadow.fs");
	shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs",
			"../Shaders/particlesFountain.fs");
	shaderParticlesFire.initialize("../Shaders/particlesFire.vs",
			"../Shaders/particlesFire.fs", { "Position", "Velocity", "Age" });
	shaderViewDepth.initialize("../Shaders/texturizado.vs",
			"../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs",
			"../Shaders/shadow_mapping_depth.fs");
	shaderTexture.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(100.0f, 100.0f, 100.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	boxLightViewBox.init();
	boxLightViewBox.setShader(&shaderViewDepth);

	//Enemy
	modelEnemy.loadModel("../models/tortuga/tortuga1.fbx");
	modelEnemy.setShader(&shaderMulLighting);

	//arboles
	modelArbol.loadModel("../models/arbol/arbol.fbx");
	modelArbol.setShader(&shaderMulLighting);

	//trineo
	modelTrineo.loadModel("../models/trineo/trineo.fbx");
	modelTrineo.setShader(&shaderMulLighting);

	//snowman
	modelSnowman.loadModel("../models/muneco/muneco.fbx");
	modelSnowman.setShader(&shaderMulLighting);

	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));

	//estrella models
	modelEstrella.loadModel("../models/estrella/estrella.fbx");
	modelEstrella.setShader(&shaderMulLighting);

	//moneda
	modelMoneda.loadModel("../models/moneda/moneda.fbx");
	modelMoneda.setShader(&shaderMulLighting);

	//Grass
	modelGrass.loadModel("../models/grass/grassModel.obj");
	modelGrass.setShader(&shaderMulLighting);

	//Fountain
	modelFountain.loadModel("../models/hidrante/hidrante.fbx");
	modelFountain.setShader(&shaderMulLighting);

	//Mario
	marioModelAnimate.loadModel("../models/mario/marioAnimate2.fbx");
	marioModelAnimate.setShader(&shaderMulLighting);

	//Menu
	menuImage.loadModel("../models/menu/menu.fbx");
	menuImage.setShader(&shaderMulLighting);

	boxMenu.init();
	boxMenu.setShader(&shaderTexture);
	boxMenu.setScale(glm::vec3(2.0, 2.0, 1.0));

	camera->setPosition(glm::vec3(0.0, 0.0, 10.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);

	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	FIBITMAP *bitmap;
	unsigned char *data;

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
				imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

	// Definiendo la textura a utilizar
	Texture textureTerrainBackground("../Textures/nieve2.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBackground.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBackground.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBackgroundID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBackground.freeImage(bitmap);


	// Definiendo la textura a utilizar
	Texture textureTerrainR("../Textures/pasto3.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainR.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainR.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainRID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainR.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainG("../Textures/hielo.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainG.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainG.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainGID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainG.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainB("../Textures/piedra.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainB.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainB.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainB.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainBlendMap("../Textures/Terreno3.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBlendMap.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBlendMap.convertToData(bitmap, imageWidth,
			imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBlendMapID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap.freeImage(bitmap);

	Texture textureParticlesFountain("../Textures/bluewater.png");
	bitmap = textureParticlesFountain.loadImage();
	data = textureParticlesFountain.convertToData(bitmap, imageWidth,
			imageHeight);
	glGenTextures(1, &textureParticleFountainID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	textureParticlesFountain.freeImage(bitmap);

	Texture textureParticleFire("../Textures/fire.png");
	bitmap = textureParticleFire.loadImage();
	data = textureParticleFire.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFireID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	textureParticleFire.freeImage(bitmap);

	Texture textureMenu1("../Textures/M0_in.jpg");
	bitmap = textureMenu1.loadImage();
	data = textureMenu1.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureMenu0_Ini);
	glBindTexture(GL_TEXTURE_2D, textureMenu0_Ini);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureMenu1.freeImage(bitmap);

	Texture textureMenu2("../Textures/M0_out.jpg");
	bitmap = textureMenu2.loadImage();
	data = textureMenu2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureMenu0_Out);
	glBindTexture(GL_TEXTURE_2D, textureMenu0_Out);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureMenu2.freeImage(bitmap);

	Texture textureMenu3("../Textures/M1_in.jpg");
	bitmap = textureMenu3.loadImage();
	data = textureMenu3.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureMenu1_Ini);
	glBindTexture(GL_TEXTURE_2D, textureMenu1_Ini);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureMenu3.freeImage(bitmap);

	Texture textureMenu4("../Textures/M1_out.jpg");
	bitmap = textureMenu4.loadImage();
	data = textureMenu4.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureMenu1_Out);
	glBindTexture(GL_TEXTURE_2D, textureMenu1_Out);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureMenu4.freeImage(bitmap);

	Texture textureGO1("../Textures/GO_in.jpg");
	bitmap = textureGO1.loadImage();
	data = textureGO1.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureGO_Ini);
	glBindTexture(GL_TEXTURE_2D, textureGO_Ini);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureGO1.freeImage(bitmap);

	Texture textureGO2("../Textures/GO_out.jpg");
	bitmap = textureGO2.loadImage();
	data = textureGO2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureGO_Out);
	glBindTexture(GL_TEXTURE_2D, textureGO_Out);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureGO2.freeImage(bitmap);

	Texture textureWin1("../Textures/Win_in.jpg");
	bitmap = textureWin1.loadImage();
	data = textureWin1.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureWin_Ini);
	glBindTexture(GL_TEXTURE_2D, textureWin_Ini);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureWin1.freeImage(bitmap);

	Texture textureWin2("../Textures/Win_out.jpg");
	bitmap = textureWin2.loadImage();
	data = textureWin2.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureWin_Out);
	glBindTexture(GL_TEXTURE_2D, textureWin_Out);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureWin2.freeImage(bitmap);

	std::uniform_real_distribution<float> distr01 =
			std::uniform_real_distribution<float>(0.0f, 1.0f);
	std::mt19937 generator;
	std::random_device rd;
	generator.seed(rd());
	int size = nParticlesFire * 2;
	std::vector<GLfloat> randData(size);
	for (int i = 0; i < randData.size(); i++) {
		randData[i] = distr01(generator);
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_1D, texId);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT,
			randData.data());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	shaderParticlesFire.setInt("Pass", 1);
	shaderParticlesFire.setInt("ParticleTex", 0);
	shaderParticlesFire.setInt("RandomTex", 1);
	shaderParticlesFire.setFloat("ParticleLifetime", particleLifetime);
	shaderParticlesFire.setFloat("ParticleSize", particleSize);
	shaderParticlesFire.setVectorFloat3("Accel",
			glm::value_ptr(glm::vec3(0.0f, 0.1f, 0.0f)));
	shaderParticlesFire.setVectorFloat3("Emitter",
			glm::value_ptr(glm::vec3(0.0f)));

	glm::mat3 basis;
	glm::vec3 u, v, n;
	v = glm::vec3(0, 1, 0);
	n = glm::cross(glm::vec3(1, 0, 0), v);
	if (glm::length(n) < 0.00001f) {
		n = glm::cross(glm::vec3(0, 1, 0), v);
	}
	u = glm::cross(v, n);
	basis[0] = glm::normalize(u);
	basis[1] = glm::normalize(v);
	basis[2] = glm::normalize(n);
	shaderParticlesFire.setMatrix3("EmitterBasis", 1, false,
			glm::value_ptr(basis));

	/*******************************************
	 * Inicializacion de los buffers de la fuente
	 *******************************************/
	initParticleBuffers();

	/*******************************************
	 * Inicializacion de los buffers del fuego
	 *******************************************/
	initParticleBuffersFire();

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
			SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
			depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	/*******************************************
		 * OpenAL init
		 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Config source 0
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/fountain.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/fire.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/moneda.wav");
	buffer[3] = alutCreateBufferFromFile("../sounds/mario_oh.wav");
	buffer[4] = alutCreateBufferFromFile("../sounds/mario_salta.wav");
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR) {
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 1.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 1.0f); //ganancia intencidad
	alSourcefv(source[1], AL_POSITION, source1Pos); //pos
	alSourcefv(source[1], AL_VELOCITY, source1Vel); //velocidad
	alSourcei(source[1], AL_BUFFER, buffer[1]); //bufer
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 2000); //humbral

	//moneda
	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 2.0f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_FALSE);
	alSourcef(source[2], AL_MAX_DISTANCE, 500);

	//daño
	alSourcef(source[3], AL_PITCH, 1.0f);
	alSourcef(source[3], AL_GAIN, 1.0f);
	alSourcefv(source[3], AL_POSITION, source3Pos);
	alSourcefv(source[3], AL_VELOCITY, source3Vel);
	alSourcei(source[3], AL_BUFFER, buffer[3]);
	alSourcei(source[3], AL_LOOPING, AL_FALSE);
	alSourcef(source[3], AL_MAX_DISTANCE, 500);

	//salto
	alSourcef(source[4], AL_PITCH, 1.0f);
	alSourcef(source[4], AL_GAIN, 0.1f);
	alSourcefv(source[4], AL_POSITION, source4Pos);
	alSourcefv(source[4], AL_VELOCITY, source4Vel);
	alSourcei(source[4], AL_BUFFER, buffer[4]);
	alSourcei(source[4], AL_LOOPING, AL_FALSE);
	alSourcef(source[4], AL_MAX_DISTANCE, 500);


	// Se inicializa el modelo de texeles.
	modelText = new FontTypeRendering::FontTypeRendering(screenWidth,
			screenHeight);
	modelText->Initialize();
	damageText = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	damageText->Initialize();
	menuP = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	menuP->Initialize();
	menuP2 = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	menuP2->Initialize();
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	shaderParticlesFountain.destroy();
	shaderParticlesFire.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	boxViewDepth.destroy();
	boxLightViewBox.destroy();

	// Terrains objects Delete
	terrain.destroy();

	modelEnemy.destroy();

	//arbol
	modelArbol.destroy();

	//trineo
	modelTrineo.destroy();

	//snowman
	modelSnowman.destroy();

	//estrellas
	modelEstrella.destroy();
	//monedas
	modelMoneda.destroy();
	modelGrass.destroy();
	modelFountain.destroy();
	menuImage.destroy();


	// Custom objects animate
	marioModelAnimate.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);
	glDeleteTextures(1, &textureTerrainBackgroundID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureParticleFountainID);
	glDeleteTextures(1, &textureParticleFireID);
	//glDeleteTextures(1, &textureMenu0_Out);
	//glDeleteTextures(1, &textureMenu0_Ini);
	//glDeleteTextures(1, &textureMenu1_Out);
	//glDeleteTextures(1, &textureMenu1_Ini);
	//glDeleteTextures(1, &textureWin_Out);
	//glDeleteTextures(1, &textureWin_Ini);
	//glDeleteTextures(1, &textureGO_Out);
	//glDeleteTextures(1, &textureGO_Ini);


	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);

	// Remove the buffer of the fountain particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &initVel);
	glDeleteBuffers(1, &startTime);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticles);

	// Remove the buffer of the fire particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, posBuf);
	glDeleteBuffers(2, velBuf);
	glDeleteBuffers(2, age);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDeleteTransformFeedbacks(2, feedback);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticlesFire);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	bool keyEnterStatus;
	//Activar tipo de control
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		tipoControl = 0;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		tipoControl = 1;
	//Movimiento menú
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		onSalida = false;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		onSalida = true;
	if (!vivo && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		keyEnterStatus = true;
	else
		keyEnterStatus = false;
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		//std::cout << "Released" << std::endl;

	if (vivo) {
		if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE) {
			//std::cout << "Esta presente el joystick" << std::endl;
			int axesCount, buttonCount;
			const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
			//std::cout << "Número de ejes disponibles :=>" << axesCount << std::endl;
			//std::cout << "Left Stick X axis: " << axes[0] << std::endl;
			//std::cout << "Left Stick Y axis: " << axes[1] << std::endl;
			//std::cout << "Left Trigger/L2: " << axes[2] << std::endl;
			//std::cout << "Right Stick X axis: " << axes[3] << std::endl;
			//std::cout << "Right Stick Y axis: " << axes[4] << std::endl;
			//std::cout << "Right Trigger/R2: " << axes[5] << std::endl;

			if (tipoControl == 0) {
				if (fabs(axes[1]) > 0.2) {
					modelMatrixMario = glm::translate(modelMatrixMario,
						glm::vec3(0, 0, -axes[1] * 0.3));
					if (!isJump)
						animationIndex = 1;
					else {
						animationIndex = 0;
					}
				}
				if (fabs(axes[0]) > 0.2) {
					modelMatrixMario = glm::rotate(modelMatrixMario,
						glm::radians(-axes[0] * 1.0f), glm::vec3(0, 1, 0));
					if (!isJump)
						animationIndex = 1;
					else {
						animationIndex = 0;
					}
				}
				if (fabs(axes[3]) > 0.2) {
					camera->mouseMoveCamera(axes[3], 0.0, deltaTime);
				}
				if (fabs(axes[4]) > 0.2) {
					camera->mouseMoveCamera(0.0, axes[4], deltaTime);
				}

				const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1,
					&buttonCount);
				/*std::cout << "Número de botones disponibles :=>" << buttonCount
					<< std::endl;*/
				if (buttons[3] == GLFW_PRESS)
					std::cout << "Se presiona x" << std::endl;
				if (!isJump && buttons[0] == GLFW_PRESS) {
					isJump = true;
					sourcesPlay[4] = true;
					startTimeJump = currTime;
					tmv = 0;
					animationIndex = 0;
				}
			}
			if (tipoControl == 1) {
				if (fabs(abs(axes[1]) > 0.2)) {
					modelMatrixMario = glm::translate(modelMatrixMario, glm::vec3(0, 0, axes[1] * 0.3));
					if (!isJump)
						animationIndex = 1;
					else
						animationIndex = 0;
				}
				if (fabs(axes[0]) > 0.2) {
					modelMatrixMario = glm::rotate(modelMatrixMario, glm::radians(-axes[0] * 1.0f), glm::vec3(0, 1, 0));
					if (!isJump)
						animationIndex = 1;
					else
						animationIndex = 0;
				}

				if (fabs(axes[2]) > 0.2) {
					camera->mouseMoveCamera(axes[2], 0.0, deltaTime);
				}
				if (fabs(axes[3]) > 0.2) {
					camera->mouseMoveCamera(0, axes[3], deltaTime);
				}

				const unsigned char * botones = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
				//std::cout << "Botones disponibles :=>" << buttonCount << std::endl;
				if (botones[0] == GLFW_PRESS && !isJump) {
					/*std::cout << "Se presiona A" << std::endl;*/
					isJump = true;
					sourcesPlay[4] = true;
					startTimeJump = currTime;
					tmv = 0;
					animationIndex = 0;
				}
			}
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			camera->mouseMoveCamera(0.0, offsetY, deltaTime);
		offsetX = 0;
		offsetY = 0;

		if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			modelMatrixMario = glm::rotate(modelMatrixMario, glm::radians(1.0f),
				glm::vec3(0, 1, 0));
			if (!isJump)
				animationIndex = 1;
			else
				animationIndex = 0;
		}
		else if (modelSelected
			== 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			modelMatrixMario = glm::rotate(modelMatrixMario, glm::radians(-1.0f),
				glm::vec3(0, 1, 0));
			if (!isJump)
				animationIndex = 1;
			else
				animationIndex = 0;
		}
		if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			modelMatrixMario = glm::translate(modelMatrixMario,
				glm::vec3(0, 0, 0.3));
			if (!isJump)
				animationIndex = 1;
			else
				animationIndex = 0;
		}
		else if (modelSelected
			== 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			modelMatrixMario = glm::translate(modelMatrixMario,
				glm::vec3(0, 0, -0.3));
			if (!isJump)
				animationIndex = 1;
			else
				animationIndex = 0;
		}

		bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
		if (!isJump && keySpaceStatus) {
			isJump = true;
			startTimeJump = currTime;
			tmv = 0;
			sourcesPlay[4] = true;
		}

		if (isJump) {
			animationIndex = 0;
		}
	}   else {
			
			 //Activar bool que muestre texto
			 if (principal) {
				 if (!onSalida && tipoControl == 0) //Start and PS
					 textureActivaID = textureMenu0_Ini;
				 else if (!onSalida && tipoControl == 1)  //Start and Xbox
					 textureActivaID = textureMenu1_Ini;
				 else if (onSalida && tipoControl == 0)  //Exit and PS
					 textureActivaID = textureMenu0_Out;
				 else if (onSalida && tipoControl == 1)  //Exit and Xbox
					 textureActivaID = textureMenu1_Out;
				 else 
					 textureActivaID = textureMenu0_Ini;

				 if (keyEnterStatus) { // Se podrían añadir los botones de un control
					 if (onSalida)
						exitApp = true;
					 else
						vivo = true;
					 keyEnterStatus = false;
				 }
			 }
			 else if (muestraFinal) {
				 //Mostramos textos del GameOver Victoria
				 if (resultadoPartida) { //Victoria
					 if (!onSalida)
						 textureActivaID = textureWin_Ini;
					 else
						 textureActivaID = textureWin_Out;
					 if (keyEnterStatus) { // Se podrían añadir los botones de un control
						 if (!onSalida) {
							 vivo = true;
							 muestraFinal = false;
						 }
						 else
							 exitApp = true;
						 keyEnterStatus = false;
					 }
				 }
				 else { //Derrota
					 if (!onSalida)
						 textureActivaID = textureGO_Ini;
					 else
						 textureActivaID = textureGO_Out;
					 if (keyEnterStatus) { // Se podrían añadir los botones de un control
						 if (!onSalida) {
							 vivo = true;
							 muestraFinal = false;
						 }
						 else
							 exitApp = true;
						 keyEnterStatus = false;
					 }
				 }
			 }
	}


	glfwPollEvents();
	return continueApplication;
}

void resetGame() {
	contador_damage = 5; //Vidas al máximo
	contador_txt = 0; //Monedas a 0
	//Reiniciar el render de 
	//enemyRender
	for (int i = 0; i < enemyPosition.size(); i++)
		enemyRender[i] = true;
	for (int i = 0; i < enemyPosition2.size(); i++)
		enemyRender2[i] = true;
	for (int i = 0; i < enemyPosition3.size(); i++)
		enemyRender3[i] = true;
	for (int i = 0; i < enemyPosition4.size(); i++)
		enemyRender4[i] = true;
	for (int i = 0; i < monedaPosition.size(); i++)
		monedasRender[i] = true;
	//muestraFinal = false;
	modelMatrixMario = glm::translate(glm::mat4(1.0f),
		glm::vec3(13.0f, 0.05f, -5.0f));
	modelMatrixMario = glm::rotate(modelMatrixMario, glm::radians(-90.0f),
		glm::vec3(0, 1, 0));
	camera->setPosition(glm::vec3(0.0, 0.0, 10.0));
	camera->setDistanceFromTarget(7);
	//enemyRender2
	//monedasRender
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	modelMatrixMario = glm::translate(modelMatrixMario,
			glm::vec3(13.0f, 0.05f, -5.0f));
	modelMatrixMario = glm::rotate(modelMatrixMario, glm::radians(-90.0f),
			glm::vec3(0, 1, 0));
	//modelMatrixMario = glm::scale(modelMatrixMario,
		//glm::vec3(0.5f, 0.5f, 0.5f));


	modelMatrixFountain = glm::translate(modelMatrixFountain,
			glm::vec3(5.0, 0.0, -40.0));
	modelMatrixFountain[3][1] = terrain.getHeightTerrain(
			modelMatrixFountain[3][0], modelMatrixFountain[3][2]);
	modelMatrixFountain = glm::scale(modelMatrixFountain,
			glm::vec3(10.0f, 10.0f, 10.0f));

	modelMatrixMenu = glm::translate(glm::mat4(1.0f), glm::vec3(16.0f, 0.05f, -5.0f));
	modelMatrixMenu = glm::rotate(modelMatrixMenu, glm::radians(90.0f), glm::vec3(0, 1, 0));
	modelMatrixMenu = glm::rotate(modelMatrixMenu, glm::radians(-35.0f), glm::vec3(1, 0, 0));
	modelMatrixMenu = glm::scale(modelMatrixMenu, glm::vec3(5.0, 3.0, 3.0));
	modelMatrixMenu[3][1] = terrain.getHeightTerrain(modelMatrixMenu[3][0], modelMatrixMenu[3][2]) + 1.5f;


	lastTime = TimeManager::Instance().GetTime();

	// Time for the particles animation
	currTimeParticlesAnimation = lastTime;
	lastTimeParticlesAnimation = lastTime;

	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);

	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1f, 45.0f);

	textureActivaID = textureMenu0_Ini;

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if (currTime - lastTime < 0.016666667) {
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;


		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.1f, 100.0f);

		if (modelSelected == 1) {

		} else {
			axis = glm::axis(glm::quat_cast(modelMatrixMario));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixMario));
			target = modelMatrixMario[3];
		}

		if (std::isnan(angleTarget))
			angleTarget = 0.0;
		if (axis.y < 0)
			angleTarget = -angleTarget;
		if (modelSelected == 1)
			angleTarget -= glm::radians(90.0f);
		camera->setCameraTarget(target);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();
		view = camera->getViewMatrix();

		shadowBox->update(screenWidth, screenHeight);
		glm::vec3 centerBox = shadowBox->getCenter();

		// Projection light shadow mapping
		glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(
				1.0f);
		glm::mat4 lightSpaceMatrix;

		lightProjection[0][0] = 2.0f / shadowBox->getWidth();
		lightProjection[1][1] = 2.0f / shadowBox->getHeight();
		lightProjection[2][2] = -2.0f / shadowBox->getLength();
		lightProjection[3][3] = 1.0f;

		lightView = glm::lookAt(centerBox,
				centerBox + glm::normalize(-lightPos),
				glm::vec3(0.0, 1.0, 0.0));

		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false, glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false, glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader para el fountain
		shaderParticlesFountain.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderParticlesFountain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader para el fuego
		shaderParticlesFire.setInt("Pass", 2);
		shaderParticlesFire.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(view));
		shaderParticlesFire.setVectorFloat3("colorHumo", glm::value_ptr(glm::vec3(1, 1, 1)));

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		
		shaderMulLighting.setVectorFloat3("fogColor",
			glm::value_ptr(glm::vec3(0.6, 0.6, 0.8)));
		shaderMulLighting.setFloat("density", 0.05);
		shaderTerrain.setVectorFloat3("fogColor",
			glm::value_ptr(glm::vec3(0.6, 0.6, 0.8)));
		shaderTerrain.setFloat("density", 0.05);
		shaderSkybox.setVectorFloat3("fogColor",
			glm::value_ptr(glm::vec3(0.6, 0.6, 0.8)));
		shaderSkybox.setFloat("density", 0.05);
		/*
		shaderMulLighting.setVectorFloat3("fogColor",
			glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderMulLighting.setFloat("density", 0.0001);
		shaderTerrain.setVectorFloat3("fogColor",
			glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderTerrain.setFloat("density", 0.0001);
		shaderSkybox.setVectorFloat3("fogColor",
			glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		shaderSkybox.setFloat("density", 0.000);
		*/


		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos",
				glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular",
				glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction",
				glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades Luz direccional Terrain
		 *******************************************/
		shaderTerrain.setVectorFloat3("viewPos",
				glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse",
				glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular",
				glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction",
				glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		//Propiedades de estrellas

		shaderMulLighting.setInt("pointLightCount",
			estrellaPosition.size());
		shaderTerrain.setInt("pointLightCount",
			estrellaPosition.size());


		for (int i = 0; i < estrellaPosition.size(); i++) {
			glm::mat4 matrixAdjustEstrella = glm::mat4(1.0f);
			matrixAdjustEstrella = glm::translate(matrixAdjustEstrella,
				estrellaPosition[i]);
			matrixAdjustEstrella = glm::rotate(matrixAdjustEstrella,
				glm::radians(estrellaOrientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustEstrella = glm::scale(matrixAdjustEstrella,
				glm::vec3(0.5, 0.5, 0.5));
			//matrixAdjustEstrella = glm::translate(matrixAdjustEstrella,
				//glm::vec3(0, 10.3585, 0));
			glm::vec3 estrellaPosition = glm::vec3(matrixAdjustEstrella[3]);
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.5,1,1)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.diffuse",
				glm::value_ptr(glm::vec3(0.5,1,1)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.specular",
				glm::value_ptr(glm::vec3(1,1,1)));
			shaderMulLighting.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].position",
				glm::value_ptr(estrellaPosition));
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat(
				"pointLights[" + std::to_string(i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.ambient",
				glm::value_ptr(glm::vec3(0.5,1,1)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.diffuse",
				glm::value_ptr(glm::vec3(0.5, 1, 1)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].light.specular",
				glm::value_ptr(glm::vec3(1,1,1)));
			shaderTerrain.setVectorFloat3(
				"pointLights[" + std::to_string(i) + "].position",
				glm::value_ptr(estrellaPosition));
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat(
				"pointLights[" + std::to_string(i) + "].quadratic", 0.01);
		}




		//posicion monedas
		for (int i = 0; i < monedaPosition.size(); i++) {
			glm::mat4 matrixAdjustmoneda = glm::mat4(1.0f);
			matrixAdjustmoneda = glm::translate(matrixAdjustmoneda,
				monedaPosition[i]);
			//matrixAdjustmoneda = glm::scale(matrixAdjustmoneda,
				//glm::vec3(0.5, 0.5, 0.5));
			matrixAdjustmoneda = glm::translate(matrixAdjustmoneda,
				glm::vec3(0, 10.3585, 0));
			glm::vec3 monedaPosition = glm::vec3(matrixAdjustmoneda[3]);
		}

		//posicion enemigos
		for (int i = 0; i < enemyPosition.size(); i++) {
			glm::mat4 matrixAdjustEnemigo = glm::mat4(1.0f);
			matrixAdjustEnemigo = glm::translate(matrixAdjustEnemigo, enemyPosition[i]);
			//matrixAdjustEnemigo = glm::translate(matrixAdjustEnemigo, glm::vec3(0, 0, 0));
			glm::vec3 enemyPosition = glm::vec3(matrixAdjustEnemigo[3]);
		}
		for (int i = 0; i < enemyPosition2.size(); i++) {
			glm::mat4 matrixAdjustEnemigo2 = glm::mat4(1.0f);
			matrixAdjustEnemigo2 = glm::translate(matrixAdjustEnemigo2, enemyPosition2[i]);
			glm::vec3 enemyPosition2 = glm::vec3(matrixAdjustEnemigo2[3]);
		}
		for (int i = 0; i < enemyPosition3.size(); i++) {
			glm::mat4 matrixAdjustEnemigo3 = glm::mat4(1.0f);
			matrixAdjustEnemigo3 = glm::translate(matrixAdjustEnemigo3, enemyPosition3[i]);
			glm::vec3 enemyPosition3 = glm::vec3(matrixAdjustEnemigo3[3]);
		}
		for (int i = 0; i < enemyPosition4.size(); i++) {
			glm::mat4 matrixAdjustEnemigo4 = glm::mat4(1.0f);
			matrixAdjustEnemigo4 = glm::translate(matrixAdjustEnemigo4, enemyPosition4[i]);
			glm::vec3 enemyPosition4 = glm::vec3(matrixAdjustEnemigo4[3]);
		}

		//posicion arboles
		
		for (int i = 0; i < arbolPosition.size(); i++) {
			glm::mat4 matrixAdjustArbol = glm::mat4(1.0f);
			matrixAdjustArbol = glm::translate(matrixAdjustArbol, arbolPosition[i]);
			glm::vec3 arbolPosition = glm::vec3(matrixAdjustArbol[3]);
		}

		//posicion trineo

		for (int i = 0; i < trineoPosition.size(); i++) {
			glm::mat4 matrixAdjustTrineo = glm::mat4(1.0f);
			matrixAdjustTrineo = glm::translate(matrixAdjustTrineo, trineoPosition[i]);
			glm::vec3 trineoPosition = glm::vec3(matrixAdjustTrineo[3]);
		}

		//posicion snowman

		for (int i = 0; i < snowmanPosition.size(); i++) {
			glm::mat4 matrixAdjustSnowman = glm::mat4(1.0f);
			matrixAdjustSnowman = glm::translate(matrixAdjustSnowman, snowmanPosition[i]);
			glm::vec3 SnowmanPosition = glm::vec3(matrixAdjustSnowman[3]);
		}

		if (!vivo) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, screenWidth, screenHeight);
			shaderTexture.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
			shaderTexture.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureActivaID);
			shaderTexture.setInt("outTexture", 0);
			boxMenu.render();
			glfwSwapBuffers(window);
			continue;
		}

		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene(false);
		//glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*******************************************
		 * Debug to view the texture view map
		 *******************************************/
		// reset viewport
		/*glViewport(0, 0, screenWidth, screenHeight);
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 // render Depth map to quad for visual debugging
		 shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		 shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, depthMap);
		 boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
		 boxViewDepth.render();*/

		/*******************************************
		 * 2.- We render the normal objects
		 *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);
		renderScene();
		/*******************************************
		 * Debug to box light box
		 *******************************************/
		/*glm::vec3 front = glm::normalize(-lightPos);
		 glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), front));
		 glm::vec3 up = glm::normalize(glm::cross(front, right));
		 glDisable(GL_CULL_FACE);
		 glm::mat4 boxViewTransform = glm::mat4(1.0f);
		 boxViewTransform = glm::translate(boxViewTransform, centerBox);
		 boxViewTransform[0] = glm::vec4(right, 0.0);
		 boxViewTransform[1] = glm::vec4(up, 0.0);
		 boxViewTransform[2] = glm::vec4(front, 0.0);
		 boxViewTransform = glm::scale(boxViewTransform, glm::vec3(shadowBox->getWidth(), shadowBox->getHeight(), shadowBox->getLength()));
		 boxLightViewBox.enableWireMode();
		 boxLightViewBox.render(boxViewTransform);
		 glEnable(GL_CULL_FACE);*/

		/*******************************************
		 * Creacion de colliders
		 * IMPORTANT do this before interpolations
		 *******************************************/

		//monedas collider
		for (int i = 0; i < monedaPosition.size(); i++) {
				AbstractModel::OBB monedaCollider;
				glm::mat4 modelMatrixColliderMoneda = glm::mat4(1.0);
				modelMatrixColliderMoneda = glm::translate(modelMatrixColliderMoneda,
					monedaPosition[i]);
				if (i <= 9) {
					addOrUpdateColliders(collidersOBB, "moneda0" + std::to_string(i), monedaCollider, modelMatrixColliderMoneda);
				}
				else {
					addOrUpdateColliders(collidersOBB, "moneda" + std::to_string(i), monedaCollider, modelMatrixColliderMoneda);
				}
					
				// Set the orientation of collider before doing the scale
				monedaCollider.u = glm::quat_cast(modelMatrixColliderMoneda);
				//modelMatrixColliderMoneda = glm::scale(modelMatrixColliderMoneda,
					//glm::vec3(0.5, 0.5, 0.5));
				modelMatrixColliderMoneda = glm::translate(modelMatrixColliderMoneda,
					modelMoneda.getObb().c);
				monedaCollider.c = glm::vec3(modelMatrixColliderMoneda[3]);
				monedaCollider.e = modelMoneda.getObb().e * glm::vec3(0.5, 0.5, 0.5);
				if (i <= 9) {
					std::get<0>(collidersOBB.find("moneda0" + std::to_string(i))->second) = monedaCollider;
				}
				else {
					std::get<0>(collidersOBB.find("moneda" + std::to_string(i))->second) = monedaCollider;
				}
		}

		//enemy colliders
		for (int i = 0; i < enemyPosition.size(); i++) {
			AbstractModel::SBB enemyCollider;
			glm::mat4 modelMatrixColliderEnemy = glm::mat4(1.0);
			modelMatrixColliderEnemy = glm::translate(modelMatrixColliderEnemy, enemyPosition[i]);
			if (i <= 9) {
				addOrUpdateColliders(collidersSBB, "enemy0" + std::to_string(i), enemyCollider, modelMatrixColliderEnemy);
			}
			else {
				addOrUpdateColliders(collidersSBB, "enemy" + std::to_string(i), enemyCollider, modelMatrixColliderEnemy);
			}
			modelMatrixColliderEnemy = glm::translate(modelMatrixColliderEnemy, glm::vec3(modelEnemy.getSbb().c.x, modelEnemy.getSbb().c.y, modelEnemy.getSbb().c.z + EnemySteep));
			enemyCollider.c = glm::vec3(modelMatrixColliderEnemy[3]);
			enemyCollider.ratio = modelEnemy.getSbb().ratio*0.5;
			if (i <= 9) {
				std::get<0>(collidersSBB.find("enemy0" + std::to_string(i))->second) = enemyCollider;
			}
			else {
				std::get<0>(collidersSBB.find("enemy" + std::to_string(i))->second) = enemyCollider;
			}
		}
		for (int i = 0; i < enemyPosition2.size(); i++) {
			AbstractModel::SBB enemyCollider2;
			glm::mat4 modelMatrixColliderEnemy2 = glm::mat4(1.0);
			modelMatrixColliderEnemy2 = glm::translate(modelMatrixColliderEnemy2, enemyPosition2[i]);
			if (i <= 9) {
				addOrUpdateColliders(collidersSBB, "enemx0" + std::to_string(i), enemyCollider2, modelMatrixColliderEnemy2);
			}
			else {
				addOrUpdateColliders(collidersSBB, "enemx" + std::to_string(i), enemyCollider2, modelMatrixColliderEnemy2);

			}
			modelMatrixColliderEnemy2 = glm::translate(modelMatrixColliderEnemy2, glm::vec3(modelEnemy.getSbb().c.x + EnemySteep, modelEnemy.getSbb().c.y, modelEnemy.getSbb().c.z));
			enemyCollider2.c = glm::vec3(modelMatrixColliderEnemy2[3]);
			enemyCollider2.ratio = modelEnemy.getSbb().ratio*0.5;
			if (i <= 9) {
				std::get<0>(collidersSBB.find("enemx0" + std::to_string(i))->second) = enemyCollider2;
			}
			else {
				std::get<0>(collidersSBB.find("enemx" + std::to_string(i))->second) = enemyCollider2;

			}
		}
		for (int i = 0; i < enemyPosition3.size(); i++) {
			AbstractModel::SBB enemyCollider3;
			glm::mat4 modelMatrixColliderEnemy3 = glm::mat4(1.0);
			modelMatrixColliderEnemy3 = glm::translate(modelMatrixColliderEnemy3, enemyPosition3[i]);
			if (i <= 9) {
				addOrUpdateColliders(collidersSBB, "enem-y0" + std::to_string(i), enemyCollider3, modelMatrixColliderEnemy3);
			}
			else {
				addOrUpdateColliders(collidersSBB, "enem-y" + std::to_string(i), enemyCollider3, modelMatrixColliderEnemy3);

			}
			modelMatrixColliderEnemy3 = glm::translate(modelMatrixColliderEnemy3, glm::vec3(modelEnemy.getSbb().c.x, modelEnemy.getSbb().c.y, modelEnemy.getSbb().c.z - EnemySteep));
			enemyCollider3.c = glm::vec3(modelMatrixColliderEnemy3[3]);
			enemyCollider3.ratio = modelEnemy.getSbb().ratio * 0.5;
			if (i <= 9) {
				std::get<0>(collidersSBB.find("enem-y0" + std::to_string(i))->second) = enemyCollider3;
			}
			else {
				std::get<0>(collidersSBB.find("enem-y" + std::to_string(i))->second) = enemyCollider3;

			}

		}
		for (int i = 0; i < enemyPosition4.size(); i++) {
			AbstractModel::SBB enemyCollider4;
			glm::mat4 modelMatrixColliderEnemy4 = glm::mat4(1.0);
			modelMatrixColliderEnemy4 = glm::translate(modelMatrixColliderEnemy4, enemyPosition4[i]);
			if (i <= 9) {
				addOrUpdateColliders(collidersSBB, "enem-x0" + std::to_string(i), enemyCollider4, modelMatrixColliderEnemy4);
			}
			else {
				addOrUpdateColliders(collidersSBB, "enem-x" + std::to_string(i), enemyCollider4, modelMatrixColliderEnemy4);

			}

			modelMatrixColliderEnemy4 = glm::translate(modelMatrixColliderEnemy4, glm::vec3(modelEnemy.getSbb().c.x - EnemySteep, modelEnemy.getSbb().c.y, modelEnemy.getSbb().c.z));
			enemyCollider4.c = glm::vec3(modelMatrixColliderEnemy4[3]);
			enemyCollider4.ratio = modelEnemy.getSbb().ratio * 0.5;
			if (i <= 9) {
				std::get<0>(collidersSBB.find("enem-x0" + std::to_string(i))->second) = enemyCollider4;
			}
			else {
				std::get<0>(collidersSBB.find("enem-x" + std::to_string(i))->second) = enemyCollider4;
			}

		}


		// arbolcollider
		for (int i = 0; i < arbolPosition.size(); i++) {
			AbstractModel::OBB arbolCollider;
			glm::mat4 modelMatrixColliderArbol = glm::mat4(1.0);
			modelMatrixColliderArbol = glm::translate(modelMatrixColliderArbol,arbolPosition[i]);
			//modelMatrixColliderArbol = glm::rotate(modelMatrixColliderArbol, glm::radians(lamp1Orientation[i]), glm::vec3(0, 1, 0));
			if (i <= 9) {
				addOrUpdateColliders(collidersOBB, "arbol0" + std::to_string(i), arbolCollider, modelMatrixColliderArbol);
			}
			else {
				addOrUpdateColliders(collidersOBB, "arbol" + std::to_string(i), arbolCollider, modelMatrixColliderArbol);

			}
			// Set the orientation of collider before doing the scale
			arbolCollider.u = glm::quat_cast(modelMatrixColliderArbol);
			//modelMatrixColliderArbol = glm::scale(modelMatrixColliderArbol, glm::vec3(0.2, 1, 0.2));
			
			modelMatrixColliderArbol = glm::translate(modelMatrixColliderArbol,
					glm::vec3(modelArbol.getObb().c.x+0.1, //atras
						modelArbol.getObb().c.y, //altura
						modelArbol.getObb().c.z-0.15));
				;
			arbolCollider.c = glm::vec3(modelMatrixColliderArbol[3]);
			arbolCollider.e = modelArbol.getObb().e * glm::vec3(0.2, 1, 0.2);
			if (i <= 9) {
				std::get<0>(collidersOBB.find("arbol0" + std::to_string(i))->second) = arbolCollider;
			}else {
				std::get<0>(collidersOBB.find("arbol" + std::to_string(i))->second) = arbolCollider;

			}

		}

		// trineo collider
		for (int i = 0; i < trineoPosition.size(); i++) {
			AbstractModel::OBB trineoCollider;
			glm::mat4 modelMatrixColliderTrineo = glm::mat4(1.0);
			modelMatrixColliderTrineo = glm::translate(modelMatrixColliderTrineo, trineoPosition[i]);
			addOrUpdateColliders(collidersOBB, "trineo" + std::to_string(i), trineoCollider, modelMatrixColliderTrineo);
			// Set the orientation of collider before doing the scale
			trineoCollider.u = glm::quat_cast(modelMatrixColliderTrineo);
			modelMatrixColliderTrineo = glm::translate(modelMatrixColliderTrineo,
				glm::vec3(modelTrineo.getObb().c.x, //atras
					modelTrineo.getObb().c.y, //altura
					modelTrineo.getObb().c.z));
			;
			trineoCollider.c = glm::vec3(modelMatrixColliderTrineo[3]);
			trineoCollider.e = modelTrineo.getObb().e * glm::vec3(1, 1, 1);
			std::get<0>(collidersOBB.find("trineo" + std::to_string(i))->second) = trineoCollider;
		}


		// snowman collider
		for (int i = 0; i < snowmanPosition.size(); i++) {
			AbstractModel::OBB snowmanCollider;
			glm::mat4 modelMatrixColliderSnowman = glm::mat4(1.0);
			modelMatrixColliderSnowman = glm::translate(modelMatrixColliderSnowman, snowmanPosition[i]);
			if (i <= 9) {
				addOrUpdateColliders(collidersOBB, "snowman0" + std::to_string(i), snowmanCollider, modelMatrixColliderSnowman);
			}
			else {
				addOrUpdateColliders(collidersOBB, "snowman" + std::to_string(i), snowmanCollider, modelMatrixColliderSnowman);

			}

			// Set the orientation of collider before doing the scale
			snowmanCollider.u = glm::quat_cast(modelMatrixColliderSnowman);
			//modelMatrixColliderSnowman = glm::scale(modelMatrixColliderSnowman, glm::vec3(0.2, 1, 0.2));

			modelMatrixColliderSnowman = glm::translate(modelMatrixColliderSnowman,
				glm::vec3(modelSnowman.getObb().c.x, //atras
					modelSnowman.getObb().c.y, //altura
					modelSnowman.getObb().c.z ));
			;
			snowmanCollider.c = glm::vec3(modelMatrixColliderSnowman[3]);
			snowmanCollider.e = modelSnowman.getObb().e * glm::vec3(.5,1,1);
			if (i <= 9) {
				std::get<0>(collidersOBB.find("snowman0" + std::to_string(i))->second) = snowmanCollider;
			}
			else {
				std::get<0>(collidersOBB.find("snowman" + std::to_string(i))->second) = snowmanCollider;
			}
		}

		// Collider de Mario
		AbstractModel::OBB marioCollider;
		glm::mat4 modelmatrixColliderMario = glm::mat4(modelMatrixMario);
		modelmatrixColliderMario = glm::rotate(modelmatrixColliderMario,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		marioCollider.u = glm::quat_cast(modelmatrixColliderMario);
		modelmatrixColliderMario = glm::scale(modelmatrixColliderMario,
				glm::vec3(0.4, 0.8, 0.8));
		modelmatrixColliderMario = glm::translate(modelmatrixColliderMario,
				glm::vec3(marioModelAnimate.getObb().c.x-0.1,
						marioModelAnimate.getObb().c.y-0.1,
						marioModelAnimate.getObb().c.z+1));
		marioCollider.e = marioModelAnimate.getObb().e
				* glm::vec3(0.4, 0.8, 0.8)
				* glm::vec3(0.787401574, 0.787401574, 0.787401574);
		marioCollider.c = glm::vec3(modelmatrixColliderMario[3]);
		addOrUpdateColliders(collidersOBB, "Mario", marioCollider,
				modelMatrixMario);


		/*******************************************
		 * Render de colliders
		 *******************************************/
		/*for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider,
					std::get<0>(it->second).c);
			matrixCollider = matrixCollider
					* glm::mat4(std::get<0>(it->second).u);
			matrixCollider = glm::scale(matrixCollider,
					std::get<0>(it->second).e * 2.0f);
			boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			boxCollider.enableWireMode();
			boxCollider.render(matrixCollider);
		}

		for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			glm::mat4 matrixCollider = glm::mat4(1.0);
			matrixCollider = glm::translate(matrixCollider,
					std::get<0>(it->second).c);
			matrixCollider = glm::scale(matrixCollider,
					glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			sphereCollider.enableWireMode();
			sphereCollider.render(matrixCollider);
		}*/

		/*******************************************
		 * Test Colisions
		 *******************************************/
		for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt
						&& testOBBOBB(std::get<0>(it->second),
								std::get<0>(jt->second))) {
					std::cout << "3Colision " << it->first << " with "
							<< jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first,
					isCollision);
		}

		for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
					std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt
						&& testSphereSphereIntersection(std::get<0>(it->second),
								std::get<0>(jt->second))) {
					std::cout << "2Colision " << it->first << " with "
							<< jt->first << std::endl;
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first,
					isCollision);
		}

		for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.begin();
			for (; jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second),
						std::get<0>(jt->second))) {
					std::cout << "1Colision " << it->first << " with "
							<< jt->first << std::endl;
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first,
							isCollision);
					if (it->first.find("enemy") != std::string::npos) {


						std::string pos = it->first.substr(5, 2);
						//std::cout << "The enemy" << pos << std::endl;
						enemyRender[std::stoi(pos)] = false;
						contador_damage--;
						int a = std::stoi(pos);
						source3Pos[0] = enemyPosition[a].x;
						source3Pos[1] = enemyPosition[a].y;
						source3Pos[2] = enemyPosition[a].z;
						alSourcefv(source[3], AL_POSITION, source3Pos);
						sourcesPlay[3] = true;
				
					}
					if (it->first.find("enemx") != std::string::npos) {


						std::string pos = it->first.substr(5, 2);
						//std::cout << "The enemy" << pos << std::endl;
						enemyRender2[std::stoi(pos)] = false;
						contador_damage--;
						int a = std::stoi(pos);
						source3Pos[0] = enemyPosition2[a].x;
						source3Pos[1] = enemyPosition2[a].y;
						source3Pos[2] = enemyPosition2[a].z;
						alSourcefv(source[3], AL_POSITION, source3Pos);
						sourcesPlay[3] = true;
					}
					if (it->first.find("enem-y") != std::string::npos) {


						std::string pos = it->first.substr(6, 2);
						//std::cout << "The enemy" << pos << std::endl;
						enemyRender3[std::stoi(pos)] = false;
						contador_damage--;
						int a = std::stoi(pos);
						source3Pos[0] = enemyPosition3[a].x;
						source3Pos[1] = enemyPosition3[a].y;
						source3Pos[2] = enemyPosition3[a].z;
						alSourcefv(source[3], AL_POSITION, source3Pos);
						sourcesPlay[3] = true;
					}

					if (it->first.find("enem-x") != std::string::npos) {

						std::string pos = it->first.substr(6, 2);
						
						enemyRender4[std::stoi(pos)] = false;
						contador_damage--;
						int a = std::stoi(pos);
						source3Pos[0] = enemyPosition4[a].x;
						source3Pos[1] = enemyPosition4[a].y;
						source3Pos[2] = enemyPosition4[a].z;
						alSourcefv(source[3], AL_POSITION, source3Pos);
						sourcesPlay[3] = true;
					}
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first,
					isCollision);
		}

		std::map<std::string, bool>::iterator colIt;
		for (colIt = collisionDetection.begin();
				colIt != collisionDetection.end(); colIt++) {
			std::map<std::string,
					std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
					collidersSBB.find(colIt->first);
			std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.find(colIt->first);
			if (it != collidersSBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersSBB, it->first);
			}
			if (jt != collidersOBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersOBB, jt->first);
				else {
					if (jt->first.find("Mario") != std::string::npos) {
						modelMatrixMario = std::get<1>(jt->second);
					}
						
					//std::cout << "000000000000000000000000000" << std::endl;
					if (jt->first.find("moneda") != std::string::npos) {
						std::string pos = jt->first.substr(6, 2);
						std::cout << "Posicion" << pos << std::endl;
						int a = std::stoi(pos);
						monedasRender[std::stoi(pos)] = false;	
						contador_txt++;
						sourcesPlay[2] = true;
						dificultad += 0.01;

					}

					
					
						
				}
			}
		}

		//coordenadas

		//std::cout << "x" << modelMatrixMario[3].x << std::endl;
		//std::cout << "z" << modelMatrixMario[3].z << std::endl;
			
		

		/*******************************************
		 * Interpolation key frames with disconect objects
		 *******************************************/

		// Constantes de animaciones
		animationIndex = 2;

		/*******************************************
		 * State machines
		 *******************************************/

		// State machine for the lambo car

		switch (stateEnemy) {
		case 0:
			EnemySteep +=dificultad;
			//std::cout << "Posicion " << EnemySteep << std::endl;
			//std::cout << "Estado " << stateEnemy << std::endl;
			if (EnemySteep > 5) {
				stateEnemy = 1;
			}
			break;
		case 1:
			EnemySteep -= dificultad;
			//std::cout << "Posicion " << EnemySteep << std::endl;
			//std::cout << "Estado " << stateEnemy << std::endl;
			if (EnemySteep < 0) {
				EnemySteep = 0.0;
				stateEnemy = 0;
			}
			break;
		}

		//Quitar menu Render
		if (!vivo && principal) { //Inicia el juego y muestra menu principal
			//menuP->render("Mision", -0.25, 0.7, 48,0.77,0.31,0.09);
			//menuP->render("Encuentra las 9 monedas", -0.75, 0.25, 36, 0.77, 0.31, 0.09);

			//menuP->render("Enter para iniciar el juego o Esc para salir", -0.75, -0.3, 22, 0.21, 0.39, 0.81);
			//menuP2->render("Presiona", -0.75, -0.5, 22, 0.21, 0.39, 0.81);
			//menuP2->render("0 para control de Play", -0.75, -0.6, 22, 0.21, 0.39, 0.81);
			//menuP2->render("1 para control de Xbox", -0.75, -0.7, 22, 0.21, 0.39, 0.81);


			//menuP2->render("Mision: Encuentra las 9 monedas", -1.0, -0.8, 22, 1.0, 1.0, 1.0);
			//control de Play  1 para control de Xbox
		}
		else if (!vivo && !principal) {
			if (resultadoPartida) {
				damageText->render("**Ganaste**", -0.4, 0.0, 60, 0.28, 0.91,0.09);
				menuP->render("Enter para jugar de nuevo o Esc para salir", -0.75, -0.5, 22, 0.38, 0.76, 0.9);
			}
			else {
				damageText->render("**Perdiste**", -0.4, 0.0, 60, 0.77, 0.31, 0.09);
				menuP->render("Enter para jugar de nuevo o Esc para salir", -0.75, -0.5, 22, 0.38, 0.76, 0.9);
			}
		} else {
			principal = false;
			if (contador_damage > 0) {
				modelText->render("Monedas " + std::to_string(contador_txt), -0.2, 0.8, 22, 0.28, 0.91, 0.09);
				damageText->render("Vidas: " + std::to_string(contador_damage), -1, 0.8, 22, 0.77, 0.31, 0.09);
			}
			if (contador_damage <= 0) {
				muestraFinal = true;
				resultadoPartida = false;
				damageText->render("**Perdiste**", -0.4, 0.0, 60, 0.77, 0.31, 0.09);
				//menuP->render("Enter para jugar de nuevo o Esc para salir", -0.75, -0.5, 22, 0.38, 0.76, 0.9);
				vivo = false; //Bloquea controles
				resetGame();
			}
			if (contador_txt >= maxmonedas) {
				muestraFinal = true;
				resultadoPartida = true;
				vivo = false; //Bloquea controles
				damageText->render("**Ganaste**", -0.3, 0.0, 60, 0.28, 0.91, 0.09);
				//menuP->render("Enter para jugar de nuevo o Esc para salir", -0.75, -0.5, 22, 0.38, 0.76, 0.9);
				resetGame();
			}
		}

		glfwSwapBuffers(window);

		/*
		glfwSwapBuffers(window);*/
		/****************************+
		 * Open AL sound data
		 */
		source0Pos[0] = modelMatrixFountain[3].x;
		source0Pos[1] = modelMatrixFountain[3].y;
		source0Pos[2] = modelMatrixFountain[3].z;
		alSourcefv(source[0], AL_POSITION, source0Pos);

		source4Pos[0] = modelMatrixMario[3].x;
		source4Pos[1] = modelMatrixMario[3].y;
		source4Pos[2] = modelMatrixMario[3].z;
		alSourcefv(source[4], AL_POSITION, source4Pos);

		//monedas
		source2Pos[0] = modelMatrixMario[3].x;
		source2Pos[1] = modelMatrixMario[3].y;
		source2Pos[2] = modelMatrixMario[3].z;
		alSourcefv(source[2], AL_POSITION, source2Pos);



		// Listener for the Thris person camera
		listenerPos[0] = modelMatrixMario[3].x;
		listenerPos[1] = modelMatrixMario[3].y;
		listenerPos[2] = modelMatrixMario[3].z;
		alListenerfv(AL_POSITION, listenerPos);




		glm::vec3 upModel = glm::normalize(modelMatrixMario[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixMario[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;

		// Listener for the First person camera
		/*listenerPos[0] = camera->getPosition().x;
		 listenerPos[1] = camera->getPosition().y;
		 listenerPos[2] = camera->getPosition().z;
		 alListenerfv(AL_POSITION, listenerPos);
		 listenerOri[0] = camera->getFront().x;
		 listenerOri[1] = camera->getFront().y;
		 listenerOri[2] = camera->getFront().z;
		 listenerOri[3] = camera->getUp().x;
		 listenerOri[4] = camera->getUp().y;
		 listenerOri[5] = camera->getUp().z;*/
		alListenerfv(AL_ORIENTATION, listenerOri);

		for (unsigned int i = 0; i < sourcesPlay.size(); i++) {
			if (sourcesPlay[i]) {
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);
			}
		}
	}
}

void prepareScene() {

	skyboxSphere.setShader(&shaderSkybox);

	//Enemigo
	modelEnemy.setShader(&shaderMulLighting);

	//arboles
	modelArbol.setShader(&shaderMulLighting);

	//trineo
	modelTrineo.setShader(&shaderMulLighting);

	//snowman
	modelSnowman.setShader(&shaderMulLighting);

	terrain.setShader(&shaderTerrain);

	//estrellas
	modelEstrella.setShader(&shaderMulLighting);

	//monedas
	modelMoneda.setShader(&shaderMulLighting);

	//Grass
	modelGrass.setShader(&shaderMulLighting);

	//Mario
	marioModelAnimate.setShader(&shaderMulLighting);

	//Menu
	menuImage.setShader(&shaderMulLighting);
}

void prepareDepthScene() {

	skyboxSphere.setShader(&shaderDepth);

	//enemy
	modelEnemy.setShader(&shaderDepth);

	//arbol
	modelArbol.setShader(&shaderDepth);

	//trineo
	modelTrineo.setShader(&shaderDepth);

	//snowman
	modelSnowman.setShader(&shaderDepth);

	terrain.setShader(&shaderDepth);

	//estrellas
	modelEstrella.setShader(&shaderDepth);

	//Grass
	modelGrass.setShader(&shaderDepth);

	//Mario
	marioModelAnimate.setShader(&shaderDepth);

	//Menu
	menuImage.setShader(&shaderDepth);
}

void renderScene(bool renderParticles) {
	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	glm::mat4 modelCesped = glm::mat4(1.0);
	modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
	modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
	// Se activa la textura del background
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	shaderTerrain.setInt("backgroundTexture", 0);
	// Se activa la textura de tierra
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	// Se activa la textura de hierba
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	// Se activa la textura del camino
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	// Se activa la textura del blend map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Custom objects obj
	 *******************************************/

	// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	glActiveTexture(GL_TEXTURE0);

	// Render de estrellas
	for (int i = 0; i < estrellaPosition.size(); i++) {
		estrellaPosition[i].y = terrain.getHeightTerrain(estrellaPosition[i].x,
			estrellaPosition[i].z)+4;
		modelEstrella.setPosition(estrellaPosition[i]);
		modelEstrella.setScale(glm::vec3(0.5, 0.5, 0.5));
		modelEstrella.setOrientation(glm::vec3(0, estrellaOrientation[i], 0));
		modelEstrella.render();
	}

	//Render monedas
	for (int i = 0; i < monedaPosition.size(); i++) {

		//modelMoneda.setScale(glm::vec3(10, 10, 10));
		//modelmoneda.setOrientation(glm::vec3(0, monedaOrientation[i], 0));
		if (monedasRender[i]) {
			monedaPosition[i].y = terrain.getHeightTerrain(monedaPosition[i].x,
				monedaPosition[i].z);
			modelMoneda.setPosition(monedaPosition[i]);
			modelMoneda.render();
		} else {
			monedaPosition[i].y = -100;
			modelMoneda.setPosition(monedaPosition[i]);
		}
		
	}

	//Render enemigos
	for (int i = 0; i < enemyPosition.size(); i++) {
		if (enemyRender[i]) {
			enemyPosition[i].y = terrain.getHeightTerrain(enemyPosition[i].x, enemyPosition[i].z + EnemySteep);
			modelEnemy.setPosition(glm::vec3(enemyPosition[i].x, enemyPosition[i].y, enemyPosition[i].z + EnemySteep));
			modelEnemy.render();
		}
		else {
			enemyPosition[i].y = -100;
			modelEnemy.setPosition(enemyPosition[i]);
		}
	}
	for (int i = 0; i < enemyPosition2.size(); i++) {
		if (enemyRender2[i]) {
			enemyPosition2[i].y = terrain.getHeightTerrain(enemyPosition2[i].x + EnemySteep, enemyPosition2[i].z);
			modelEnemy.setPosition(glm::vec3(enemyPosition2[i].x + EnemySteep, enemyPosition2[i].y, enemyPosition2[i].z));
			modelEnemy.render();
		}
		else {
			enemyPosition2[i].y = -100;
			modelEnemy.setPosition(enemyPosition2[i]);
		}
	}

	for (int i = 0; i < enemyPosition3.size(); i++) {
		if (enemyRender3[i]) {
			enemyPosition3[i].y = terrain.getHeightTerrain(enemyPosition3[i].x, enemyPosition3[i].z - EnemySteep);
			modelEnemy.setPosition(glm::vec3(enemyPosition3[i].x, enemyPosition3[i].y, enemyPosition3[i].z - EnemySteep));
			modelEnemy.render();
		}
		else {
			enemyPosition3[i].y = -100;
			modelEnemy.setPosition(enemyPosition3[i]);
		}
	}
	for (int i = 0; i < enemyPosition4.size(); i++) {
		if (enemyRender4[i]) {
			enemyPosition4[i].y = terrain.getHeightTerrain(enemyPosition4[i].x - EnemySteep, enemyPosition4[i].z);
			modelEnemy.setPosition(glm::vec3(enemyPosition4[i].x - EnemySteep, enemyPosition4[i].y, enemyPosition4[i].z));
			modelEnemy.render();
		}
		else {
			enemyPosition4[i].y = -100;
			modelEnemy.setPosition(enemyPosition4[i]);
		}
	}


	//arboles render
	for (int i = 0; i < arbolPosition.size(); i++) {
		arbolPosition[i].y = terrain.getHeightTerrain(arbolPosition[i].x,
			arbolPosition[i].z);
		modelArbol.setPosition(arbolPosition[i]);
		modelArbol.render();
	}


	//trineo render
	for (int i = 0; i < trineoPosition.size(); i++) {
		trineoPosition[i].y = terrain.getHeightTerrain(trineoPosition[i].x,
			trineoPosition[i].z);
		modelTrineo.setPosition(trineoPosition[i]);
		modelTrineo.render();
	}


	//snowman render
	for (int i = 0; i < snowmanPosition.size(); i++) {
		snowmanPosition[i].y = terrain.getHeightTerrain(snowmanPosition[i].x,
			snowmanPosition[i].z);
		modelSnowman.setPosition(snowmanPosition[i]);
		modelSnowman.render();
	}


	////Enemy render

	//modelMatrixEnemy[3][1] = terrain.getHeightTerrain(modelMatrixEnemy[3][0],
	//	modelMatrixEnemy[3][2]);
	//glm::mat4 modelMatrixMEnemyBody = glm::mat4(modelMatrixEnemy);
	//modelMatrixMEnemyBody = glm::translate(modelMatrixMEnemyBody, glm::vec3(0, 0, EnemySteep));
	////modelMatrixEnemy = modelMatrixMEnemyBody;
	//modelEnemy.render(modelMatrixMEnemyBody);


	// Grass
	glDisable(GL_CULL_FACE);
	glm::vec3 grassPosition = glm::vec3(0.0, 0.0, 0.0);
	grassPosition.y = terrain.getHeightTerrain(grassPosition.x,
			grassPosition.z);
	modelGrass.setPosition(grassPosition);
	modelGrass.render();
	glEnable(GL_CULL_FACE);

	// Fountain
	glDisable(GL_CULL_FACE);
	modelFountain.render(modelMatrixFountain);
	glEnable(GL_CULL_FACE);

	if (!vivo)
		menuImage.render(modelMatrixMenu);

	// Se regresa el cull faces IMPORTANTE para la capa
	glEnable(GL_CULL_FACE);

	/*******************************************
	 * Custom Anim objects obj
	 *******************************************/
	modelMatrixMario[3][1] = -GRAVITY * tmv * tmv + 4.5 * tmv
			+ terrain.getHeightTerrain(modelMatrixMario[3][0],
					modelMatrixMario[3][2]);
	tmv = currTime - startTimeJump;
	if (modelMatrixMario[3][1]
			< terrain.getHeightTerrain(modelMatrixMario[3][0],
					modelMatrixMario[3][2])) {
		isJump = false;
		modelMatrixMario[3][1] = terrain.getHeightTerrain(
				modelMatrixMario[3][0], modelMatrixMario[3][2]);
	}
	//modelMatrixMario[3][1] = terrain.getHeightTerrain(modelMatrixMario[3][0], modelMatrixMario[3][2]);

	//limites
	if (modelMatrixMario[3].x > 99) {
		modelMatrixMario[3].x = 99;
	}

	if (modelMatrixMario[3].z > 99) {
		modelMatrixMario[3].z = 99;
	}

	if (modelMatrixMario[3].x < -150) {
		modelMatrixMario[3].x = -150;
	}

	if (modelMatrixMario[3].z < -150) {
		modelMatrixMario[3].z = -150;
	}
	glm::mat4 modelMatrixMarioBody = glm::mat4(modelMatrixMario);
	modelMatrixMarioBody = glm::scale(modelMatrixMarioBody,
			glm::vec3(0.021, 0.021, 0.021));
	marioModelAnimate.setAnimationIndex(animationIndex);
	marioModelAnimate.render(modelMatrixMarioBody);



	glActiveTexture(GL_TEXTURE0);





	/**********
	 * Update the position with alpha objects
	 */
	// Update the aircraft
	//blendingUnsorted.find("aircraft")->second = glm::vec3(
	//		modelMatrixAircraft[3]);
	//// Update the lambo
	//blendingUnsorted.find("lambo")->second = glm::vec3(modelMatrixLambo[3]);
	//// Update the helicopter
	//blendingUnsorted.find("heli")->second = glm::vec3(modelMatrixHeli[3]);

	/**********
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for (itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end();
			itblend++) {
		float distanceFromView = glm::length(
				camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first,
				itblend->second);
	}

	/**********
	 * Render de las transparencias
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	for (std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it =
			blendingSorted.rbegin(); it != blendingSorted.rend(); it++) {
		if (renderParticles
				&& it->second.first.compare("fountain") == 0) {
			/**********
			 * Init Render particles systems
			 */
			glm::mat4 modelMatrixParticlesFountain = glm::mat4(1.0);
			modelMatrixParticlesFountain = glm::translate(
					modelMatrixParticlesFountain, it->second.second);
			modelMatrixParticlesFountain[3][1] = terrain.getHeightTerrain(
					modelMatrixParticlesFountain[3][0],
					modelMatrixParticlesFountain[3][2]) + 0.25 * 10.0;
			modelMatrixParticlesFountain = glm::scale(
					modelMatrixParticlesFountain, glm::vec3(3.0, 3.0, 3.0));
			currTimeParticlesAnimation = TimeManager::Instance().GetTime();
			if (currTimeParticlesAnimation - lastTimeParticlesAnimation > 10.0)
				lastTimeParticlesAnimation = currTimeParticlesAnimation;
			//glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			// Set the point size
			glPointSize(10.0f);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
			shaderParticlesFountain.turnOn();
			shaderParticlesFountain.setFloat("Time",
					float(
							currTimeParticlesAnimation
									- lastTimeParticlesAnimation));
			shaderParticlesFountain.setFloat("ParticleLifetime", 3.5f);
			shaderParticlesFountain.setInt("ParticleTex", 0);
			shaderParticlesFountain.setVectorFloat3("Gravity",
					glm::value_ptr(glm::vec3(0.0f, -0.3f, 0.0f)));
			shaderParticlesFountain.setMatrix4("model", 1, false,
					glm::value_ptr(modelMatrixParticlesFountain));
			glBindVertexArray(VAOParticles);
			glDrawArrays(GL_POINTS, 0, nParticles);
			glDepthMask(GL_TRUE);
			//glEnable(GL_DEPTH_TEST);
			shaderParticlesFountain.turnOff();
			/**********
			 * End Render particles systems
			 */
		} else if (renderParticles && it->second.first.compare("fire") == 0) {
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time",
					currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT",
					currTimeParticlesAnimationFire
							- lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1 - drawBuf]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0);
			modelFireParticles = glm::translate(modelFireParticles,
					it->second.second);
			modelFireParticles[3][1] = terrain.getHeightTerrain(
					modelFireParticles[3][0], modelFireParticles[3][2]);
			shaderParticlesFire.setMatrix4("model", 1, false,
					glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

			/****************************+
			 * Open AL sound data
			 */
			source1Pos[0] = modelFireParticles[3].x;
			source1Pos[1] = modelFireParticles[3].y;
			source1Pos[2] = modelFireParticles[3].z;
			alSourcefv(source[1], AL_POSITION, source1Pos);

			/**********
			 * End Render particles systems
			 */
		}

	}
	glEnable(GL_CULL_FACE);
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
