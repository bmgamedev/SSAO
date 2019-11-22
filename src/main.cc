#include <iostream>
#include <random>
#include <e3d/ogl/app.hh>
#define M_PI 3.1415926535897932384626433832795

using namespace ogl;

//forward declarations
void PostProcessSetUp();
void PostProcessRender();
void SSAOSetUp();
void SSAORender();
void TwoPassTestSetup();
void TwoPassTestRender();
float lerp(float a, float b, float f);

// Shaders
std::shared_ptr<Shader> static lambert;
std::shared_ptr<Shader> static post_process;
std::shared_ptr<Shader> static blur;

std::shared_ptr<Shader> static SSAO_geom;
std::shared_ptr<Shader> static SSAO;
std::shared_ptr<Shader> static SSAO_lighting;
std::shared_ptr<Shader> static SSAO_blur;


// Meshes
auto static quad = Mesh();
auto static ground = Mesh();
auto static cube = Mesh();
auto static sphere = Mesh();

//framebuffers - Post process
std::shared_ptr<Framebuffer> static fboPostProcess;

//framebuffers - two pass
std::shared_ptr<Framebuffer> static fbo;
std::shared_ptr<Framebuffer> static fbo2;

//framebuffers - SSAO
unsigned int rboDepth;
unsigned int gBuffer, gPosition, gNormal, gAlbedo;
unsigned int ssaoFBO, ssaoColorBuffer;
unsigned int ssaoBlurFBO, ssaoColorBufferBlur;

//hemisphere sampling
std::uniform_real_distribution<GLfloat> randomFloats(0.0f, 1.0f);
std::default_random_engine generator;
std::vector<glm::vec3> ssaoKernel;
std::vector<int> sampleUniforms;
std::vector<glm::vec3> ssaoNoise;
unsigned int noiseTexture;


void setup() {
	// App settings
	renderer::clear_colour(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
	renderer::camera.position = glm::vec3(0.0f, 20.0f, 5.0f);
	renderer::camera.look_at(glm::vec3(0.0f));
	renderer::camera.sensitivity = 0.001f;

	//PostProcessSetUp();
	//TwoPassTestSetup();
	SSAOSetUp();
}

void render() {
	//PostProcessRender();
	//TwoPassTestRender();
	SSAORender();
}

auto main()-> int {
	app::setup_function = setup;
	app::render_function = render;
	app::run();
}

void PostProcessSetUp() {
	// Shaders
	lambert = std::make_shared<Shader>("Lambert");
	lambert->add(GL_VERTEX_SHADER, Shader::File, "resources/shaders/lambert.vert");
	lambert->add(GL_FRAGMENT_SHADER, Shader::File, "resources/shaders/lambert.frag");
	lambert->build();

	post_process = std::make_shared<Shader>("post_process");
	post_process->add(GL_VERTEX_SHADER, Shader::File, "resources/shaders/post_process.vert");
	post_process->add(GL_FRAGMENT_SHADER, Shader::File, "resources/shaders/post_process.frag");
	post_process->build();

	// Meshes
	ground.load(Mesh::Quad);
	ground.scale = glm::vec3(5.0f);

	cube.load(Mesh::Cube);
	cube.position += glm::vec3(1.0F, 0.0F, 1.0F);

	sphere.load(Mesh::File, "resources/models/sphere.obj");
	sphere.position += glm::vec3(-1.0F, 0.0F, 1.0F);

	quad.load(Mesh::Quad);

	//create frame buffer
	fboPostProcess = std::make_shared<Framebuffer>(renderer::width, renderer::height);
}

void PostProcessRender() {
	
	renderer::target(fboPostProcess.get());
	renderer::clear_colour(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	renderer::clear();

	// Use the shader
	lambert->use();

	// Bind camera matrices
	lambert->bind("projection", renderer::camera.projection());
	lambert->bind("view", renderer::camera.view());

	// Bind matrices and draw mesh
	lambert->bind("translate", sphere.translate_matrix());
	lambert->bind("rotate", sphere.rotate_matrix());
	lambert->bind("scale", sphere.scale_matrix());
	renderer::draw(sphere);

	lambert->bind("translate", cube.translate_matrix());
	lambert->bind("rotate", cube.rotate_matrix());
	lambert->bind("scale", cube.scale_matrix());
	renderer::draw(cube);

	lambert->bind("translate", ground.translate_matrix());
	lambert->bind("rotate", ground.rotate_matrix());
	lambert->bind("scale", ground.scale_matrix());
	renderer::draw(ground);

	renderer::display();

	//post process - invert colours
	renderer::target();
	renderer::clear(); //But in some instances, might not want to clear depth buffer??

	post_process->use();
	post_process->bind("screenTexture", 0);
	renderer::bind(fboPostProcess->frame(), 0);

	renderer::draw(quad);
}

void TwoPassTestSetup() {
	// Shaders
	lambert = std::make_shared<Shader>("Lambert");
	lambert->add(GL_VERTEX_SHADER, Shader::File, "resources/shaders/lambert.vert");
	lambert->add(GL_FRAGMENT_SHADER, Shader::File, "resources/shaders/lambert.frag");
	lambert->build();

	post_process = std::make_shared<Shader>("post_process");
	post_process->add(GL_VERTEX_SHADER, Shader::File, "resources/shaders/post_process.vert");
	post_process->add(GL_FRAGMENT_SHADER, Shader::File, "resources/shaders/post_process.frag");
	post_process->build();

	blur = std::make_shared<Shader>("blur");
	blur->add(GL_VERTEX_SHADER, Shader::File, "resources/shaders/post_process.vert");
	blur->add(GL_FRAGMENT_SHADER, Shader::File, "resources/shaders/blur.frag");
	blur->build();

	// Meshes
	ground.load(Mesh::Quad);
	ground.scale = glm::vec3(5.0f);

	cube.load(Mesh::Cube);
	cube.position += glm::vec3(1.0F, 0.0F, 1.0F);

	sphere.load(Mesh::File, "resources/models/sphere.obj");
	sphere.position += glm::vec3(-1.0F, 0.0F, 1.0F);

	quad.load(Mesh::Quad);

	//New FBO code:
	fbo = std::make_shared<Framebuffer>(renderer::width, renderer::height);
	fbo2 = std::make_shared<Framebuffer>(renderer::width, renderer::height);
}

void TwoPassTestRender() {

	//New FBO code:
	renderer::target(fbo.get());
	renderer::clear_colour(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	renderer::clear();

	// Use the shader
	lambert->use();

	// Bind camera matrices
	lambert->bind("projection", renderer::camera.projection());
	lambert->bind("view", renderer::camera.view());

	// Bind matrices and draw mesh
	lambert->bind("translate", sphere.translate_matrix());
	lambert->bind("rotate", sphere.rotate_matrix());
	lambert->bind("scale", sphere.scale_matrix());
	renderer::draw(sphere);

	lambert->bind("translate", cube.translate_matrix());
	lambert->bind("rotate", cube.rotate_matrix());
	lambert->bind("scale", cube.scale_matrix());
	renderer::draw(cube);

	lambert->bind("translate", ground.translate_matrix());
	lambert->bind("rotate", ground.rotate_matrix());
	lambert->bind("scale", ground.scale_matrix());
	renderer::draw(ground);

	renderer::display();

	// New FBO code:
	// first pass - Invert colours
	renderer::target(fbo2.get());
	renderer::clear(); 

	post_process->use();
	post_process->bind("screenTexture", 0);
	renderer::bind(fbo->frame(), 0);

	renderer::draw(quad);

	// second pass - Blur
	renderer::target();
	renderer::clear();

	blur->use();
	blur->bind("screenTexture", 0);
	renderer::bind(fbo2->frame(), 0);

	renderer::draw(quad);
}

void SSAOSetUp() {
	// Shaders
	SSAO_geom = std::make_shared<Shader>("SSAO_geom");
	SSAO_geom->add(GL_VERTEX_SHADER, Shader::File, "resources/shaders/SSAO_geometry.vert");
	SSAO_geom->add(GL_FRAGMENT_SHADER, Shader::File, "resources/shaders/SSAO_geometry.frag");
	SSAO_geom->build();

	SSAO_lighting = std::make_shared<Shader>("SSAO_lighting");
	SSAO_lighting->add(GL_VERTEX_SHADER, Shader::File, "resources/shaders/SSAO.vert");
	SSAO_lighting->add(GL_FRAGMENT_SHADER, Shader::File, "resources/shaders/SSAO_lighting.frag");
	SSAO_lighting->build();

	SSAO = std::make_shared<Shader>("SSAO");
	SSAO->add(GL_VERTEX_SHADER, Shader::File, "resources/shaders/SSAO.vert");
	SSAO->add(GL_FRAGMENT_SHADER, Shader::File, "resources/shaders/SSAO.frag");
	SSAO->build();

	SSAO_blur = std::make_shared<Shader>("SSAO_blur");
	SSAO_blur->add(GL_VERTEX_SHADER, Shader::File, "resources/shaders/SSAO.vert");
	SSAO_blur->add(GL_FRAGMENT_SHADER, Shader::File, "resources/shaders/SSAO_blur.frag");
	SSAO_blur->build();

	// Meshes
	ground.load(Mesh::Quad);
	ground.scale = glm::vec3(5.0f);

	cube.load(Mesh::Cube);
	cube.position += glm::vec3(1.0F, 0.0F, 1.0F);

	sphere.load(Mesh::File, "resources/models/sphere.obj");
	sphere.position += glm::vec3(-1.0F, 0.0F, 1.0F);

	quad.load(Mesh::Quad);

	glEnable(GL_DEPTH_TEST);

	/// GEOM fbos
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	//tex 1 - Pos:
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, renderer::width, renderer::height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	
	//tex 2 - normals:
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, renderer::width, renderer::height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	
	//tex 3 - albedo:
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderer::width, renderer::height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
	
	// specify color attachments used 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 }; //0 = position, 1 = normal, 2 = color/spec
	glDrawBuffers(3, attachments);

	// create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, renderer::width, renderer::height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	///SSAO fbos
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
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0f;

		// scale samples so they're more aligned to center of kernel
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);

		//create uniform name array
		sampleUniforms.push_back(glGetUniformLocation(SSAO->ID(), ("samples[" + std::to_string(i) + "]").c_str()));
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
	glBindTexture(GL_TEXTURE_2D, 0);

	// Send kernel + rotation 
	for (unsigned int i = 0; i < 64; ++i) {
		//SSAO->bind("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
		glUniform3fv(sampleUniforms[i], 1, glm::value_ptr(ssaoKernel[i]));
	}
}

void SSAORender() {

	///Process:
	/*
	* glBindFramebuffer(cur frame buffer obj OR screen on last pass)
	* Set clear colour (at start - on first pass)
	* glClear everything
	*
	* Use shader
	*
	* bind the shader texture object to whatever index it is written to in set up //pass 2+
	*
	* glActiveTexture(whatever the index is for that tex) //pass 2+ (tex from PREV fbo)
	* glBindTex(that tex) //pass 2+ (tex from PREV fbo)
	* 
	* Bind relevant shader uniforms etc
	* Bind and draw meshes
	* renderer::display() //this is rendering to the texture bound to the cur fbo object (either screen of a specified tex from another fbo)
	* OR
	* renderer::draw(quad); //draw the quad containing the tex
	*/

	///Some set up because proj and view are used for calcs too
	glm::mat4 projection = renderer::camera.projection();
	glm::mat4 view = renderer::camera.view();

	///first pass - Geometry
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	renderer::clear_colour(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	

	SSAO_geom->use();
	SSAO_geom->bind("projection", projection);
	SSAO_geom->bind("view", view);

	// Bind matrices and draw mesh
	SSAO_geom->bind("model", sphere.model_matrix());
	renderer::draw(sphere);

	SSAO_geom->bind("model", cube.model_matrix());
	renderer::draw(cube);

	SSAO_geom->bind("model", ground.model_matrix());
	renderer::draw(ground);

	renderer::display();

	///second pass - SSAO
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SSAO->use();

	SSAO->bind("gPosition", 0);
	SSAO->bind("gNormal", 1);
	SSAO->bind("texNoise", 2);

	/*// Send kernel + rotation 
	for (unsigned int i = 0; i < 64; ++i) {
		SSAO->bind("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
	}*/

	SSAO->bind("projection", projection);

	//glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);

	renderer::draw(quad);

	
	///third pass - SSAO blur
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	SSAO_blur->use();
	SSAO_blur->bind("ssaoInput", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);

	renderer::draw(quad);

	/// fourth pass - Lighting
	renderer::target();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SSAO_lighting->use();

	SSAO_lighting->bind("gPosition", 0);
	SSAO_lighting->bind("gNormal", 1);
	SSAO_lighting->bind("gAlbedo", 2);
	SSAO_lighting->bind("ssao", 3);

	SSAO_lighting->bind("LightPos", glm::vec3(view * glm::vec4(2.0f, 4.0f, 10.0f, 1.0f)));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glActiveTexture(GL_TEXTURE3); // add extra SSAO texture to lighting pass
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);

	renderer::draw(quad);
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}