#include "pcv_scene.h"
#include "Global_Variables.h"

pcv_scene::~pcv_scene()
{
}

void pcv_scene::load_scene(const std::string scene_fname)
{
	// #TODO
}

void pcv_scene::setup_scene()
{
	// set up ppc given current pcs
	glm::vec3 center = { 0.0f,0.0f,0.0f };
	int num_pcs = int(_pcs.size());

	for (auto p : _pcs)
	{
		center += p->get_center() / float(num_pcs);
	}

	auto gv = Global_Variables::Instance();
	vec3 offset = { 0.0f,0.0f,10.0f };

	if (gv->cur_ppc)
	{
		gv->cur_ppc->PositionAndOrient(center + offset, center, glm::vec3(0.0f, 1.0f, 0.0f));
		std::cerr << "Initial ppc: " << *gv->cur_ppc;
	}

	// set up opengl envrionment
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND | GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(1.0f);
}

void pcv_scene::draw()
{
	auto gv = Global_Variables::Instance();

	// ppc contorl
	gv->cur_ppc->UpdatePPC();

	// render all the objects
	for (auto p : _pcs)
	{
		p->draw(_iteration, gv->cur_ppc->GetP(), gv->cur_ppc->GetV());
	}

	if(gv->is_update)
		_iteration++;
}
