
#include "PartDefault_tool.hpp"

#include <Utils/Debug_util.hpp>
#include <document.hpp>

PartDefault_tool::PartDefault_tool(partDesign* env):
	SimpleSelector_tool(env)
{
	if(!mEnv) {
		LOG_ERROR("Null environnement in tool.");
	}
}

void PartDefault_tool::act_on_entity(Drawable* drw)
{
	drw->invoke_workspace(mEnv->state()->doc);
}