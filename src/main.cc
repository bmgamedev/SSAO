#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <e3d/ogl/app.hh>

#define GLM_ENABLE_EXPERIMENTAL
#define M_PI 3.1415926535897932384626433832795

using namespace ogl;

int main()
{
	app::run();
}

/*
auto
main(
	)
	-> int
{
	// Application setup

	app::initialise_renderer();
	app::camera.position = glm::vec3(0.0F, 20.0F, 5.0F);
	app::camera.look_at(glm::vec3(0.0F));
	app::camera.sensitivity = 0.001F;



	// Generate shader
	auto lambert = std::make_shared<Shader>("Lambert");
	lambert->add(GL_VERTEX_SHADER, "res/shaders/lambert.vert");
	lambert->add(GL_FRAGMENT_SHADER, "res/shaders/lambert.frag");
	lambert->build();

	auto SSAO_geom = std::make_shared<Shader>("SSAO_geom");
	SSAO_geom->add(GL_VERTEX_SHADER, "res/shaders/SSAO_geometry.vert");
	SSAO_geom->add(GL_FRAGMENT_SHADER, "res/shaders/SSAO_geometry.frag");
	SSAO_geom->build();

	auto SSAO = std::make_shared<Shader>("SSAO");
	SSAO->add(GL_VERTEX_SHADER, "res/shaders/SSAO.vert");
	SSAO->add(GL_FRAGMENT_SHADER, "res/shaders/SSAO.frag");
	SSAO->build();

	auto SSAO_lighting = std::make_shared<Shader>("SSAO_lighting");
	SSAO_lighting->add(GL_VERTEX_SHADER, "res/shaders/SSAO.vert");
	SSAO_lighting->add(GL_FRAGMENT_SHADER, "res/shaders/SSAO_lighting.frag");
	SSAO_lighting->build();

	auto SSAO_blur = std::make_shared<Shader>("SSAO_blur");
	SSAO_blur->add(GL_VERTEX_SHADER, "res/shaders/SSAO.vert");
	SSAO_blur->add(GL_FRAGMENT_SHADER, "res/shaders/SSAO_blur.frag");
	SSAO_blur->build();
	


	// Mesh
	auto ground = Mesh(Mesh::Quad);
	ground.scale(glm::vec3(50.0F));
	ground.shader = lambert;

	auto cube = Mesh(Mesh::Cube);
	cube.translate(glm::vec3(2.0F, -10.0F, 2.0F)); //TODO does nothing. Because it's here or because it doesn't work??
	cube.shader = lambert;

	auto dragon = Mesh("res/models/dragon.obj");
	dragon.position(glm::vec3(2.0F, -10.0F, 2.0F)); //TODO does nothing. Because it's here or because it doesn't work??
	//dragon.translate(glm::vec3(2.0F, 10.0F, 2.0F)); //TODO does nothing. Because it's here or because it doesn't work??
	dragon.scale(glm::vec3(0.3F));
	dragon.shader = lambert;
	dragon.rotate(90.0f*(M_PI/180), glm::vec3(1.0f, 0.0f, 0.0f));



	// Time variables
	auto           time        = 0.0F;
	auto           accumulator = 0.0F;
	auto constexpr framerate   = 60.0F;
	auto const     delta_time   = 1.0F / framerate;
	auto           current_time = float(glfwGetTime());



	// Application loop
	while (!app::should_close()) {

		// Update time
		auto       new_time  = float(glfwGetTime());
		auto const frameTime = new_time - current_time;
		current_time         = new_time;
		accumulator         += frameTime;



		// Application update
		app::update(frameTime);
		app::show_fps(new_time);



		// Physics
		while (accumulator >= delta_time) {

			// Time
			accumulator -= delta_time;
			time        += delta_time;
		}



		// Render
		app::clear();
		app::draw(dragon);
		app::draw(cube);
		app::draw(ground);
		app::display();
	}



	// Close app
	app::close();
	exit(EXIT_SUCCESS);
}
*/