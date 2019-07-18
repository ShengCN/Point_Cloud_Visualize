#include <filesystem>
#include "pcv_scene.h"
#include "Global_Variables.h"
#include "Helplers.h"

pcv_scene::~pcv_scene()
{
}

void pcv_scene::add_pc(const std::string depth_img_path)
{
	auto gv = Global_Variables::Instance();
	_pcs.clear();

	// check if there exists final image, assume name convention: xxxx_depth, xxxx_final
	std::string prefix = depth_img_path.substr(0, depth_img_path.find("_depth"));
	auto final_fname = std::filesystem::path(prefix + "_final.png");

	if (!std::filesystem::exists(final_fname)) {
		// default
		std::cerr << "Did not find final image. Are you using name convention: xxxx_depth, xxxx_final? " << std::endl;
		std::shared_ptr<pc> new_pc = std::make_shared<pc>(gv->vertex_shader, gv->fragment_shader);
		new_pc->load_depth_img(depth_img_path);
		add_pc(new_pc);
	}
	else {
		std::cerr << "Find final image. " << std::endl;
		std::shared_ptr<pc> new_pc = std::make_shared<pc>(gv->vertex_shader, gv->fragment_shader);
		new_pc->load_depth_img(depth_img_path);
		new_pc->load_pixel(final_fname.string());
		add_pc(new_pc);
	}
}

void pcv_scene::load_scene(const std::string scene_fname)
{
	// #TODO
}

void pcv_scene::setup_scene()
{
	// set up ppc given current pcs
	glm::vec3 center = { 0.0f, 0.0f, 0.0f };
	int num_pcs = int(_pcs.size());

	for (auto p : _pcs)
	{
		center += p->get_center() / float(num_pcs);
	}

	auto gv = Global_Variables::Instance();
	vec3 offset = { 0.0f, 0.0f, -10.0f };

	if (gv->is_load_last_ppc && gv->cur_ppc->load(gv->default_ppc_file)) {
		std::cerr << "Load last time position \n";
	}
	else {
		gv->cur_ppc->PositionAndOrient(center + offset, center, glm::vec3(0.0f, 1.0f, 0.0f));
		std::cerr << "Center: " << center << "\nInitial ppc: " << *gv->cur_ppc;
	}

	// set up opengl envrionment
	// glDisable(GL_DEPTH_TEST);
	glEnable( GL_DEPTH_TEST);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(1.0);
}

void pcv_scene::draw()
{
	auto gv = Global_Variables::Instance();

	// ppc contorl

	// render all the objects
	for (auto p : _pcs)
	{
		p->draw(_iteration, gv->cur_ppc->GetP(), gv->cur_ppc->GetV());
	}

	if(gv->is_update)
		_iteration++;
}

void pcv_scene::reload_shaders()
{
	for (auto p : _pcs){
		p->reload_shader();
	}
}
