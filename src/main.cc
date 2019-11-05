#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <e3d/ogl/app.hh>

#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#define M_PI 3.1415926535897932384626433832795

using namespace ogl;

//forward declarations
void PostProcessSetUp();
void PostProcessRender();
void SSAOSetUp();
void SSAORender();
void FBOTestSetUp();
void FBOTestRender();
void renderQuad();
float lerp(float a, float b, float f);

// Shaders
std::shared_ptr<Shader> static lambert;
std::shared_ptr<Shader> static post_process;
std::shared_ptr<Shader> static fbo_test;
std::shared_ptr<Shader> static SSAO_geom;
std::shared_ptr<Shader> static SSAO;
std::shared_ptr<Shader> static SSAO_lighting;
std::shared_ptr<Shader> static SSAO_blur;

// Meshes
auto static ground = Mesh();
auto static cube = Mesh();
auto static sphere = Mesh();

//framebuffers - Post process
unsigned int framebuffer;
unsigned int texColourBuffer;
unsigned int renderbufferobject;

//framebuffers - SSAO
unsigned int gBuffer;
unsigned int gPosition, gNormal, gAlbedo;
unsigned int ssaoFBO;
unsigned int ssaoBlurFBO;
unsigned int ssaoColorBuffer;
unsigned int ssaoColorBufferBlur;

//hemisphere sampling
std::uniform_real_distribution<GLfloat> randomFloats(0.0f, 1.0f);
std::default_random_engine generator;
std::vector<glm::vec3> ssaoKernel;
std::vector<glm::vec3> ssaoNoise;
unsigned int noiseTexture;

//light
glm::vec3 lightPos = glm::vec3(2.0f, 4.0f, -2.0f);
glm::vec3 lightColor = glm::vec3(0.2f, 0.2f, 0.7f);

//screen quad
unsigned int quadVAO = 0;
unsigned int quadVBO;

void setup() {
	// App settings
	renderer::clear_colour(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
	renderer::camera.position = glm::vec3(0.0f, 20.0f, 5.0f);
	renderer::camera.look_at(glm::vec3(0.0f));
	renderer::camera.sensitivity = 0.001f;

	PostProcessSetUp();
	//SSAOSetUp();
	//FBOTestSetUp();
	
}

void render() {
	PostProcessRender();
	//SSAORender();
	//FBOTestRender();

}

auto main()-> int {
	app::setup_function = setup;
	app::render_function = render;
	app::run();
}

void PostProcessSetUp() {
	lambert = std::make_shared<Shader>("Lambert");
	lambert->add(GL_VERTEX_SHADER, "resources/shaders/lambert.vert");
	lambert->add(GL_FRAGMENT_SHADER, "resources/shaders/lambert.frag");
	lambert->build();

	post_process = std::make_shared<Shader>("post_process");
	post_process->add(GL_VERTEX_SHADER, "resources/shaders/post_process.vert");
	post_process->add(GL_FRAGMENT_SHADER, "resources/shaders/post_process.frag");
	post_process->build();

	// Meshes
	ground.load(Mesh::Quad);
	ground.scale(glm::vec3(5.0F));
	ground.shader = lambert;

	cube.load(Mesh::Cube);
	cube.translate(glm::vec3(1.0F, 0.0F, 1.0F));
	cube.shader = lambert;

	sphere.load(Mesh::File, "resources/models/sphere.obj");
	sphere.translate(glm::vec3(-1.0F, 0.0F, 1.0F));
	sphere.shader = lambert;

	//create frame buffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); //bind as active framebuffer

	glGenTextures(1, &texColourBuffer);
	glBindTexture(GL_TEXTURE_2D, texColourBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderer::width, renderer::height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); //texture's data paramter = null because only allocating memory rn, not filling it
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColourBuffer, 0);
	//Args: target (framebuffer TYPE), attachment (TYPE and # - in case multiple), TYPE of text wanting to attach, the ACTUAL tex we're attaching, mipmap level 

	glGenRenderbuffers(1, &renderbufferobject);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbufferobject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, renderer::width, renderer::height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferobject);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;//is it complete? (reserves memory)

	glBindFramebuffer(GL_FRAMEBUFFER, 0); //rebind default framebuffer (so we can see stuff on screen)

	renderQuad();
}

void PostProcessRender() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);

	// Use the shader
	if (!lambert->use()) {
		std::cerr << "ERROR: Couldn't use " << lambert->name << " shader." << std::endl;
		return;
	}

	// Bind camera matrices
	lambert->bind("projection", renderer::camera.projection());
	lambert->bind("view", renderer::camera.view());

	// Bind matrices and draw mesh
	lambert->bind("translate", sphere.translate());
	lambert->bind("rotate", sphere.rotate());
	lambert->bind("scale", sphere.scale());
	renderer::draw(sphere);

	lambert->bind("translate", cube.translate());
	lambert->bind("rotate", cube.rotate());
	lambert->bind("scale", cube.scale());
	renderer::draw(cube);

	lambert->bind("translate", ground.translate());
	lambert->bind("rotate", ground.rotate());
	lambert->bind("scale", ground.scale());
	renderer::draw(ground);

	renderer::display();

	// second pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default i.e. screen
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	post_process->use();
	glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, texColourBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6); //tested - needed
}

void SSAOSetUp() {
	glEnable(GL_DEPTH_TEST);

	// g-buffer framebuffer
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, renderer::width, renderer::height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0); ///**
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, renderer::width, renderer::height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0); ///**
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// color + specular color buffer
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderer::width, renderer::height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0); ///**
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);

	// specify color attachments used 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 }; //0 = position, 1 = normal, 2 = color/spec
	glDrawBuffers(3, attachments);

	// create and attach depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, renderer::width, renderer::height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// create framebuffer to hold SSAO processing stage 
	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	// SSAO color buffer
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, renderer::width, renderer::height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "SSAO Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// blur stage
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, renderer::width, renderer::height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// generate sample kernel
	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));
		//glm::vec3 sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator), randomFloats(generator) * 2.0f - 1.0f);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0f;

		// scale samples so they're more aligned to center of kernel
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	// generate noise texture
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0); ///**

	// lighting info
	lightPos = glm::vec3(2.0f, 4.0f, -2.0f);
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	// shader config
	SSAO_lighting->use();
	SSAO_lighting->bind("gPosition", 0);
	SSAO_lighting->bind("gNormal", 1);
	SSAO_lighting->bind("gAlbedo", 2);
	SSAO_lighting->bind("ssao", 3);

	SSAO->use();
	SSAO->bind("gPosition", 0);
	SSAO->bind("gNormal", 1);
	SSAO->bind("texNoise", 2);

	SSAO_blur->use();
	SSAO_blur->bind("ssaoInput", 0);

	renderQuad();
}

void SSAORender() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = renderer::camera.projection();
	glm::mat4 view = renderer::camera.view();

	/// 1. geometry pass: render scene's geometry/color data into gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SSAO_geom->use();
	SSAO_geom->bind("projection", projection);
	SSAO_geom->bind("view", view);

	SSAO_geom->bind("invertedNormals", 0); //uniform exists for a room cube

	// Bind matrices and draw mesh
	sphere.shader = SSAO_geom;
	SSAO_geom->bind("model", sphere.translate() * sphere.rotate() * sphere.scale());
	renderer::draw(sphere);

	cube.shader = SSAO_geom;
	SSAO_geom->bind("model", cube.translate() * cube.rotate() * cube.scale());
	renderer::draw(cube);

	ground.shader = SSAO_geom;
	SSAO_geom->bind("model", ground.translate() * ground.rotate() * ground.scale());
	renderer::draw(ground);

	renderer::display();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	/// 2. generate SSAO texture
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	SSAO->use();
	// Send kernel + rotation 
	for (unsigned int i = 0; i < 64; ++i) {
		SSAO->bind("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
	}
	SSAO->bind("projection", projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	renderQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




	/// 3. blur SSAO texture to remove noise
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	SSAO_blur->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	renderQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	/// 4. lighting pass
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SSAO_lighting->use();

	glBindVertexArray(quadVAO);

	// send light relevant uniforms
	glm::vec3 lightPosView = glm::vec3(view * glm::vec4(lightPos, 1.0f));
	SSAO_lighting->bind("light.Position", lightPosView);
	SSAO_lighting->bind("light.Color", lightColor);
	// Update attenuation parameters
	//const float constant = 1.0; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
	const float linear = 0.09f;
	const float quadratic = 0.032f;
	SSAO_lighting->bind("light.Linear", linear);
	SSAO_lighting->bind("light.Quadratic", quadratic);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glActiveTexture(GL_TEXTURE3); // add extra SSAO texture to lighting pass
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	renderQuad();


	//glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//renderer::display();
}

void FBOTestSetUp() {
	//TODO : Not complete

	// Shader
	lambert = std::make_shared<Shader>("Lambert");
	lambert->add(GL_VERTEX_SHADER, "resources/shaders/lambert.vert");
	lambert->add(GL_FRAGMENT_SHADER, "resources/shaders/lambert.frag");
	lambert->build();

	post_process = std::make_shared<Shader>("post_process");
	post_process->add(GL_VERTEX_SHADER, "resources/shaders/post_process.vert");
	post_process->add(GL_FRAGMENT_SHADER, "resources/shaders/post_process.frag");
	post_process->build();

	fbo_test = std::make_shared<Shader>("fbo_test");
	fbo_test->add(GL_VERTEX_SHADER, "resources/shaders/framebuffer_test.vert");
	fbo_test->add(GL_FRAGMENT_SHADER, "resources/shaders/framebuffer_test.frag");
	fbo_test->build();

	SSAO_geom = std::make_shared<Shader>("SSAO_geom");
	SSAO_geom->add(GL_VERTEX_SHADER, "resources/shaders/SSAO_geometry.vert");
	SSAO_geom->add(GL_FRAGMENT_SHADER, "resources/shaders/SSAO_geometry.frag");
	SSAO_geom->build();

	SSAO = std::make_shared<Shader>("SSAO");
	SSAO->add(GL_VERTEX_SHADER, "resources/shaders/SSAO.vert");
	SSAO->add(GL_FRAGMENT_SHADER, "resources/shaders/SSAO.frag");
	SSAO->build();

	SSAO_lighting = std::make_shared<Shader>("SSAO_lighting");
	SSAO_lighting->add(GL_VERTEX_SHADER, "resources/shaders/SSAO.vert");
	SSAO_lighting->add(GL_FRAGMENT_SHADER, "resources/shaders/SSAO_lighting.frag");
	SSAO_lighting->build();

	SSAO_blur = std::make_shared<Shader>("SSAO_blur");
	SSAO_blur->add(GL_VERTEX_SHADER, "resources/shaders/SSAO.vert");
	SSAO_blur->add(GL_FRAGMENT_SHADER, "resources/shaders/SSAO_blur.frag");
	SSAO_blur->build();

	// Meshes
	ground.load(Mesh::Quad);
	ground.scale(glm::vec3(5.0f));
	ground.shader = SSAO_geom;

	cube.load(Mesh::Cube);
	cube.translate(glm::vec3(1.0f, 0.0f, 1.0f));
	cube.shader = SSAO_geom;

	sphere.load(Mesh::File, "resources/models/sphere.obj");
	sphere.translate(glm::vec3(-1.0f, 0.0f, 1.0f));
	sphere.shader = SSAO_geom;

	renderQuad();
}

void FBOTestRender() {
	//TODO : Not complete

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = renderer::camera.projection();
	glm::mat4 view = renderer::camera.view();

	/// 1. geometry pass: render scene's geometry/color data into gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SSAO_geom->use();
	SSAO_geom->bind("projection", projection);
	SSAO_geom->bind("view", view);

	SSAO_geom->bind("invertedNormals", 0); //uniform exists for a room cube

	// Bind matrices and draw mesh
	sphere.shader = SSAO_geom;
	SSAO_geom->bind("model", sphere.translate() * sphere.rotate() * sphere.scale());
	renderer::draw(sphere);

	cube.shader = SSAO_geom;
	SSAO_geom->bind("model", cube.translate() * cube.rotate() * cube.scale());
	renderer::draw(cube);

	ground.shader = SSAO_geom;
	SSAO_geom->bind("model", ground.translate() * ground.rotate() * ground.scale());
	renderer::draw(ground);

	renderer::display();

	//second pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	fbo_test->use();
	glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}