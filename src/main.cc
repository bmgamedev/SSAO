////#include <iostream>
////#include <GL/glew.h>
////#include <GLFW/glfw3.h>
////#include <glm/glm.hpp>
////#include <glm/ext.hpp>
////#include <e3d/ogl/app.hh>
////
////#include <random>
////#define GLM_ENABLE_EXPERIMENTAL
////#define M_PI 3.1415926535897932384626433832795
////
////
////using namespace ogl;
////
////// Shader
////std::shared_ptr<Shader> static lambert;
////
//////std::shared_ptr<Shader> static SSAO_geom;
//////std::shared_ptr<Shader> static SSAO;
//////std::shared_ptr<Shader> static SSAO_lighting;
//////std::shared_ptr<Shader> static SSAO_blur;
////
////std::shared_ptr<Shader> static single_pass;
////
////// Meshes
////auto static ground = Mesh();
////auto static cube = Mesh();
////auto static sphere = Mesh();
////auto static screen_quad = Mesh();
////auto static dragon = Mesh();
//////std::vector<Mesh()> meshes;
////
///////
////unsigned int framebuffer; //framebuffer object
////unsigned int texColourBuffer;
////unsigned int renderbufferobject;
////
////
///////frame buffer stufffffff
//////void framebuffer_size_callback(GLFWwindow* window, int width, int height);
////void renderQuad(); 
//////void renderCube();
////
//////unsigned int gBuffer;
//////unsigned int gPosition, gNormal, gAlbedo;
////
//////std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
//////std::default_random_engine generator;
//////std::vector<glm::vec3> ssaoKernel;
////
//////unsigned int ssaoFBO, ssaoBlurFBO;
//////unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
//////std::vector<glm::vec3> ssaoNoise;
//////unsigned int noiseTexture;
//////glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
//////glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);
////
////// settings
////const unsigned int SCR_WIDTH = 1280;
////const unsigned int SCR_HEIGHT = 720;
////
/////*float lerp(float a, float b, float f)
////{
////	return a + f * (b - a);
////}*/
////
////
////
////
////// renderQuad() renders a 1x1 XY quad in NDC
////unsigned int quadVAO = 0;
////unsigned int quadVBO;
////void renderQuad()
////{
////	if (quadVAO == 0)
////	{
////		float quadVertices[] = {
////			// positions        // texture Coords
////			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
////			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
////			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
////			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
////		};
////
////
////
////
////		// setup plane VAO
////		glGenVertexArrays(1, &quadVAO);
////		glGenBuffers(1, &quadVBO);
////		glBindVertexArray(quadVAO);
////		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
////		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
////		glEnableVertexAttribArray(0);
////		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
////		glEnableVertexAttribArray(1);
////		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
////	}
////	glBindVertexArray(quadVAO);
////	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
////	glBindVertexArray(0);
////}
////
////
////
////void setup()
////{
////	// App settings
////	renderer::clear_colour(glm::vec4(0.2F, 0.3F, 0.3F, 1.0F));
////	renderer::camera.position = glm::vec3(0.0F, 20.0F, 5.0F);
////	renderer::camera.look_at(glm::vec3(0.0F));
////	renderer::camera.sensitivity = 0.001F;
////
////	// Shader
////	lambert = std::make_shared<Shader>("Lambert");
////	lambert->add(GL_VERTEX_SHADER, "resources/shaders/lambert.vert");
////	lambert->add(GL_FRAGMENT_SHADER, "resources/shaders/lambert.frag");
////	lambert->build();
////
////	/*SSAO_geom = std::make_shared<Shader>("SSAO_geom");
////	SSAO_geom->add(GL_VERTEX_SHADER, "resources/shaders/SSAO_geometry.vert");
////	SSAO_geom->add(GL_FRAGMENT_SHADER, "resources/shaders/SSAO_geometry.frag");
////	SSAO_geom->build();
////
////	SSAO = std::make_shared<Shader>("SSAO");
////	SSAO->add(GL_VERTEX_SHADER, "resources/shaders/SSAO.vert");
////	SSAO->add(GL_FRAGMENT_SHADER, "resources/shaders/SSAO.frag");
////	SSAO->build();
////
////	SSAO_lighting = std::make_shared<Shader>("SSAO_lighting");
////	SSAO_lighting->add(GL_VERTEX_SHADER, "resources/shaders/SSAO.vert");
////	SSAO_lighting->add(GL_FRAGMENT_SHADER, "resources/shaders/SSAO_lighting.frag");
////	SSAO_lighting->build();
////
////	SSAO_blur = std::make_shared<Shader>("SSAO_blur");
////	SSAO_blur->add(GL_VERTEX_SHADER, "resources/shaders/SSAO.vert");
////	SSAO_blur->add(GL_FRAGMENT_SHADER, "resources/shaders/SSAO_blur.frag");
////	SSAO_blur->build();*/
////
////	single_pass = std::make_shared<Shader>("single_pass");
////	single_pass->add(GL_VERTEX_SHADER, "resources/shaders/framebuffer_test.vert");
////	single_pass->add(GL_FRAGMENT_SHADER, "resources/shaders/framebuffer_test.frag");
////	single_pass->build();
////
////	// Meshes
////	// vec3 = x, z, y
////	ground.load(Mesh::Quad);
////	ground.scale(glm::vec3(15.0F));
////	ground.shader = lambert;
////
////	cube.load(Mesh::Cube);
////	cube.translate(glm::vec3(5.0F, 5.0F, 1.0F));
////	cube.shader = lambert;
////
////	sphere.load(Mesh::File, "resources/models/sphere.obj");
////	sphere.translate(glm::vec3(-5.0F, -5.0F, 1.0F));
////	sphere.shader = lambert;
////
////	dragon.load(Mesh::File, "resources/models/dragon.obj");
////	dragon.rotate(90.0f*(M_PI / 180), glm::vec3(1.0f, 0.0f, 0.0f));
////	dragon.scale(glm::vec3(0.5F));
////	dragon.shader = lambert;
////
////	screen_quad.load(Mesh::Quad);
////
////
////	/// BRAND NEW ATTEMPT
////
////	//create frame buffer
////	glGenFramebuffers(1, &framebuffer);
////	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); //bind as active framebuffer
////
////	
////	glGenTextures(1, &texColourBuffer);
////	glBindTexture(GL_TEXTURE_2D, texColourBuffer);
////
////	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); //texture's data paramter = null because only allocating memory rn, not filling it. 800x600 = screen size but apparently not necessary....
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////	glBindTexture(GL_TEXTURE_2D, 0);
////
////	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColourBuffer, 0);
////	//Args: target (framebuffer TYPE), attachment (TYPE and # - in case multiple), TYPE of texture wanting to attach, the ACTUAL text we're attaching, mipmap level 
////
////	
////	glGenRenderbuffers(1, &renderbufferobject);
////	glBindRenderbuffer(GL_RENDERBUFFER, renderbufferobject);
////	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
////	glBindRenderbuffer(GL_RENDERBUFFER, 0);
////
////	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferobject);
////
////	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
////		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;//is it complete? (reserves memory)
////	
////	glBindFramebuffer(GL_FRAMEBUFFER, 0); //rebind default framebuffer (so we can see stuff on screen)
////
////	renderQuad();
////
////	/*
////	///Frame buffer tests
////	glEnable(GL_DEPTH_TEST);
////
////	// g-buffer framebuffer
////	glGenFramebuffers(1, &gBuffer);
////	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
////
////	// position color buffer
////	glGenTextures(1, &gPosition);
////	glBindTexture(GL_TEXTURE_2D, gPosition);
////	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
////	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
////
////	// normal color buffer
////	glGenTextures(1, &gNormal);
////	glBindTexture(GL_TEXTURE_2D, gNormal);
////	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
////	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
////
////	// color + specular color buffer
////	glGenTextures(1, &gAlbedo);
////	glBindTexture(GL_TEXTURE_2D, gAlbedo);
////	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
////	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
////
////	// specify color attachments used 
////	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 }; //0 = position, 1 = normal, 2 = color/spec
////	glDrawBuffers(3, attachments);
////
////	// create and attach depth buffer (renderbuffer)
////	unsigned int rboDepth;
////	glGenRenderbuffers(1, &rboDepth);
////	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
////	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
////	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
////
////	// check if framebuffer is complete
////	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "Framebuffer not complete!" << std::endl;
////
////	glBindFramebuffer(GL_FRAMEBUFFER, 0);
////
////	// create framebuffer to hold SSAO processing stage 
////	glGenFramebuffers(1, &ssaoFBO);  glGenFramebuffers(1, &ssaoBlurFBO);
////	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
////
////	// SSAO color buffer
////	glGenTextures(1, &ssaoColorBuffer);
////	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
////	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
////	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
////	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "SSAO Framebuffer not complete!" << std::endl;
////
////	// blur stage
////	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
////	glGenTextures(1, &ssaoColorBufferBlur);
////	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
////	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
////	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
////	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
////	glBindFramebuffer(GL_FRAMEBUFFER, 0);
////
////	// generate sample kernel
////	for (unsigned int i = 0; i < 64; ++i)
////	{
////		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
////		sample = glm::normalize(sample);
////		sample *= randomFloats(generator);
////		float scale = float(i) / 64.0;
////
////		// scale samples so they're more aligned to center of kernel
////		scale = lerp(0.1f, 1.0f, scale * scale);
////		sample *= scale;
////		ssaoKernel.push_back(sample);
////	}
////
////	// generate noise texture
////	for (unsigned int i = 0; i < 16; i++)
////	{
////		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
////		ssaoNoise.push_back(noise);
////	}
////	noiseTexture = noiseTexture; glGenTextures(1, &noiseTexture); //TODO is this actually generating the noise texture??
////	//unsigned int noiseTexture; glGenTextures(1, &noiseTexture);
////	glBindTexture(GL_TEXTURE_2D, noiseTexture);
////	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
////
////	// shader config
////	SSAO_lighting->bind("gPosition", 0);
////	SSAO_lighting->bind("gNormal", 1);
////	SSAO_lighting->bind("gAlbedo", 2);
////	SSAO_lighting->bind("ssao", 3); //TODO is this messing with the output? Have I written SSAO somewhere?? (double check)
////
////	SSAO->bind("gPosition", 0);
////	SSAO->bind("gNormal", 1);
////	SSAO->bind("texNoise", 2);
////
////	SSAO_blur->bind("ssaoInput", 0);
////	// */
////}
////
////void render()
////{
////	 /*
////
////	//// render
////	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
////	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////
////	//// 1. geometry pass: render scene's geometry/color data into gbuffer
////	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
////	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////	glm::mat4 projection = app::camera.projection();
////	glm::mat4 view = app::camera.view();
////	glm::mat4 model = glm::mat4(1.0f);
////	SSAO_geom->use();
////	SSAO_geom->bind("projection", projection);
////	SSAO_geom->bind("view", view);
////
////	//// room cube
////	model = glm::mat4(1.0f);
////	model = glm::translate(model, glm::vec3(0.0, 7.0f, 0.0f));
////	model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
////
////	SSAO_geom->bind("model", model);
////
////	SSAO_geom->bind("invertedNormals", 1);// invert normals as we're inside the cube
////	renderCube();
////	SSAO_geom->bind("invertedNormals", 0);
////
////	//// dragon model on the floor
////	model = glm::mat4(1.0f);
////	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0));
////	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
////	model = glm::scale(model, glm::vec3(0.5f));
////	SSAO_geom->bind("model", model);
////
////	//dragon.Draw(SSAO_geom); //TODO set shader then draw??
////	dragon.shader = SSAO_geom;
////
////	///
////	glBindVertexArray(dragon.vao());
////	glDrawArrays(GL_TRIANGLES, 0, dragon.size());
////	glBindVertexArray(0);
////	///
////
////	app::draw(dragon);
////	glBindFramebuffer(GL_FRAMEBUFFER, 0);
////
////
////	//// 2. generate SSAO texture
////	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO); 
////	glClear(GL_COLOR_BUFFER_BIT);
////
////	SSAO->use();
////	// Send kernel + rotation 
////	for (unsigned int i = 0; i < 64; ++i) {
////		SSAO->bind("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
////	}
////
////	SSAO->bind("projection", projection);
////	glActiveTexture(GL_TEXTURE0);
////	glBindTexture(GL_TEXTURE_2D, gPosition);
////	glActiveTexture(GL_TEXTURE1);
////	glBindTexture(GL_TEXTURE_2D, gNormal);
////	glActiveTexture(GL_TEXTURE2);
////	glBindTexture(GL_TEXTURE_2D, noiseTexture);
////	renderQuad(); 
////	glBindFramebuffer(GL_FRAMEBUFFER, 0);
////
////
////	//// 3. blur SSAO texture to remove noise
////	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
////	glClear(GL_COLOR_BUFFER_BIT);
////
////	SSAO_blur->use();
////	glActiveTexture(GL_TEXTURE0);
////	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
////	renderQuad();
////	glBindFramebuffer(GL_FRAMEBUFFER, 0);
////
////
////	//// 4. lighting pass: traditional deferred Blinn-Phong lighting with added screen-space ambient occlusion
////	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////
////	SSAO_lighting->use();
////	// send light relevant uniforms
////	glm::vec3 lightPosView = glm::vec3(app::camera.view() * glm::vec4(lightPos, 1.0));
////	SSAO_lighting->bind("light.Position", lightPosView);
////	SSAO_lighting->bind("light.Color", lightColor);
////	// Update attenuation parameters
////	const float constant = 1.0; //TODO make uniform for this in shader files (hardcoded right now)
////	const float linear = 0.09;
////	const float quadratic = 0.032;
////	SSAO_lighting->bind("light.Linear", linear);
////	SSAO_lighting->bind("light.Quadratic", quadratic);
////	glActiveTexture(GL_TEXTURE0);
////	glBindTexture(GL_TEXTURE_2D, gPosition);
////	glActiveTexture(GL_TEXTURE1);
////	glBindTexture(GL_TEXTURE_2D, gNormal);
////	glActiveTexture(GL_TEXTURE2);
////	glBindTexture(GL_TEXTURE_2D, gAlbedo);
////	glActiveTexture(GL_TEXTURE3); // add extra SSAO texture to lighting pass
////	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
////	renderQuad();
////
////
////	// */
////	///***************************
////
////	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
////	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
////	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
////	glEnable(GL_DEPTH_TEST);
////	//DrawScene();
////
////	// Function to bind common uniforms for a mesh
////	auto const static bind = [](Mesh const& m, Shader const& s)
////	{
////		s.bind("translate", m.translate());
////		s.bind("rotate", m.rotate());
////		s.bind("scale", m.scale());
////	};
////
////	// Bind camera matrices
////	lambert->bind("projection", renderer::camera.projection());
////	lambert->bind("view", renderer::camera.view());
////
////	// Bind matrices and draw mesh
////	bind(sphere, *lambert);
////	renderer::draw(sphere);
////	
////	bind(cube, *lambert);
////	renderer::draw(cube);
////	
////	bind(ground, *lambert);
////	renderer::draw(ground);
////	
////	bind(dragon, *lambert);
////	renderer::draw(dragon);
////	
////	renderer::display();
////	
////
////	
////
////
////
////
////
////
////
////	// second pass
////	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
////	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
////	glClear(GL_COLOR_BUFFER_BIT);
////
////	single_pass->use();
////	glBindVertexArray(quadVAO);
////	//glBindVertexArray(screen_quad.vao());
////	glDisable(GL_DEPTH_TEST);
////	glBindTexture(GL_TEXTURE_2D, texColourBuffer);
////	glDrawArrays(GL_TRIANGLES, 0, 6);
////
////
////
////	///***************************
////
////	/*app::clear();
////	app::draw(sphere);
////	app::draw(cube);
////	app::draw(ground);
////	app::draw(dragon);
////	app::display();*/
////}
////
////auto main() -> int
////{
////	app::setup_function = setup;
////	app::render_function = render;
////	app::run();
////}
////
/////* WANT A BLANK SCREEN? RUN THIS! :D */
/////* int main()
////{
////	app::run();
////} */
////
////
////
////
////
/////*
////
////
////// renderCube() renders a 1x1 3D cube in NDC.
////unsigned int cubeVAO = 0;
////unsigned int cubeVBO = 0;
////void renderCube()
////{
////	// initialize (if necessary)
////	if (cubeVAO == 0)
////	{
////		float vertices[] = {
////			// back face
////			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
////			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
////			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
////			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
////			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
////			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
////			// front face
////			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
////			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
////			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
////			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
////			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
////			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
////			// left face
////			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
////			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
////			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
////			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
////			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
////			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
////			// right face
////			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
////			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
////			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
////			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
////			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
////			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
////			// bottom face
////			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
////			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
////			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
////			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
////			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
////			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
////			// top face
////			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
////			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
////			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
////			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
////			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
////			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
////		};
////		glGenVertexArrays(1, &cubeVAO);
////		glGenBuffers(1, &cubeVBO);
////		// fill buffer
////		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
////		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
////		// link vertex attributes
////		glBindVertexArray(cubeVAO);
////		glEnableVertexAttribArray(0);
////		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
////		glEnableVertexAttribArray(1);
////		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
////		glEnableVertexAttribArray(2);
////		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
////		glBindBuffer(GL_ARRAY_BUFFER, 0);
////		glBindVertexArray(0);
////	}
////	// render Cube
////	glBindVertexArray(cubeVAO);
////	glDrawArrays(GL_TRIANGLES, 0, 36);
////	glBindVertexArray(0);
////}
////
////*/
////
////
/////*
////
////// glfw: whenever the window size changed (by OS or user resize) this callback function executes
////void framebuffer_size_callback(GLFWwindow* window, int width, int height)
////{
////	// make sure the viewport matches the new window dimensions; note that width and 
////	// height will be significantly larger than specified on retina displays.
////	glViewport(0, 0, width, height);
////}
////
////
////*/

#include <iostream>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/ext.hpp>
#include <e3d/ogl/app.hh>

#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#define M_PI 3.1415926535897932384626433832795


using namespace ogl;


// Shader
std::shared_ptr<Shader> static lambert;
std::shared_ptr<Shader> static single_pass;

// Meshes
auto static ground = Mesh();
auto static cube = Mesh();
auto static sphere = Mesh();

auto static screen_quad = Mesh();


//framebuffers
unsigned int framebuffer; //framebuffer object
unsigned int texColourBuffer;
unsigned int renderbufferobject;

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		/*
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		*/

		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		float quadPositions[] = {
			// positions        
			-1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f
		};

		float texCoords[] = {
			// texture Coords
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
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
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void setup()
{
	// App settings
	renderer::clear_colour(glm::vec4(0.2F, 0.3F, 0.3F, 1.0F));
	renderer::camera.position = glm::vec3(0.0F, 20.0F, 5.0F);
	renderer::camera.look_at(glm::vec3(0.0F));
	renderer::camera.sensitivity = 0.001F;

	// Shader
	lambert = std::make_shared<Shader>("Lambert");
	lambert->add(GL_VERTEX_SHADER, "resources/shaders/lambert.vert");
	lambert->add(GL_FRAGMENT_SHADER, "resources/shaders/lambert.frag");
	lambert->build();

	single_pass = std::make_shared<Shader>("single_pass");
	single_pass->add(GL_VERTEX_SHADER, "resources/shaders/framebuffer_test.vert");
	single_pass->add(GL_FRAGMENT_SHADER, "resources/shaders/framebuffer_test.frag");
	single_pass->build();

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

	screen_quad.load(Mesh::Quad);

	//create frame buffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); //bind as active framebuffer

	
	glGenTextures(1, &texColourBuffer);
	glBindTexture(GL_TEXTURE_2D, texColourBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderer::width, renderer::height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); //texture's data paramter = null because only allocating memory rn, not filling it. 800x600 = screen size but apparently not necessary....
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColourBuffer, 0);
	//Args: target (framebuffer TYPE), attachment (TYPE and # - in case multiple), TYPE of texture wanting to attach, the ACTUAL text we're attaching, mipmap level 

	
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

void render()
{
	// Clear the screen
	//renderer::clear();

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);

	// Use the shader
	if (!lambert->use())
	{
		std::cerr <<
			"ERROR: Couldn't use " <<
			lambert->name << " shader." << std::endl;
		return;
	}

	// Function to bind common uniforms for a mesh
	auto const static bind = [](Mesh const& m, Shader const& s)
	{
		s.bind("translate", m.translate());
		s.bind("rotate", m.rotate());
		s.bind("scale", m.scale());
	};

	// Bind camera matrices
	lambert->bind("projection", renderer::camera.projection());
	lambert->bind("view", renderer::camera.view());

	// Bind matrices and draw mesh
	bind(sphere, *lambert);
	renderer::draw(sphere);

	bind(cube, *lambert);
	renderer::draw(cube);

	bind(ground, *lambert);
	renderer::draw(ground);

	// Display the render on screen
	renderer::display();

	// second pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	single_pass->use();
	glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, texColourBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

auto
main(
)
-> int
{
	app::setup_function = setup;
	app::render_function = render;
	app::run();
}