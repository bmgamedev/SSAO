#include <iostream>
#include <e3d/ogl/app.hh>

#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#define M_PI 3.1415926535897932384626433832795

using namespace ogl;

// Shaders
std::shared_ptr<Shader> static lambert;
std::shared_ptr<Shader> static post_process;

// Meshes
auto static ground = Mesh();
auto static cube = Mesh();
auto static sphere = Mesh();

//framebuffers
unsigned int framebuffer;
unsigned int texColourBuffer;
unsigned int renderbufferobject;

auto static framebuffer2;

//screen quad
unsigned int quadVAO = 0;
unsigned int quadVBO;
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

void setup()
{
	// App settings
	renderer::clear_colour(glm::vec4(0.2F, 0.3F, 0.3F, 1.0F));
	renderer::camera.position = glm::vec3(0.0F, 20.0F, 5.0F);
	renderer::camera.look_at(glm::vec3(0.0F));
	renderer::camera.sensitivity = 0.001F;

	// Shader
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
	ground.shader = lambert;

	cube.load(Mesh::Cube);
	cube.position += glm::vec3(1.0F, 0.0F, 1.0F);
	cube.shader = lambert;

	sphere.load(Mesh::File, "resources/models/sphere.obj");
	sphere.position += glm::vec3(-1.0F, 0.0F, 1.0F);
	sphere.shader = lambert;


	///**** FramBuf tests
	//framebuffer2 = std::make_shared<Framebuffer>(renderer::width, renderer::height);
	/// Not finished

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

void render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);

	// Use the shader
	lambert->use();
	if (!lambert->is_active()) {
		std::cerr << "ERROR: Couldn't use " << lambert->name << " shader." << std::endl;
		return;
	}

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

auto main() -> int {
	app::setup_function = setup;
	app::render_function = render;
	app::run();
}