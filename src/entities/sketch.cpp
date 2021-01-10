
#include "sketch.hpp"

#include <entities/tangibleEntities/line.hpp>
#include <utils/errorLogger.hpp>

sketch::sketch(plane_abstract_ptr base_plane):
	mBasePlane(base_plane)
{
	set_name("sketch");
	create_origin();
}
sketch::sketch(plane_abstract_ptr base_plane, entity* parent):
	mBasePlane(base_plane),
	entity(parent) // Follow that entity
{
	set_name("sketch");
	create_origin();
}

void sketch::draw_impl(camera_ptr cam, int frame)
{

}
void sketch::draw_selection_impl(camera_ptr cam, int frame)
{

}

void sketch::create_origin()
{
	mOrigin = folder_ptr(new folder("skorigin"));
	mOrigin->add(std::make_shared<line>(line_abstract(mBasePlane->origin() - mBasePlane->v(), mBasePlane->origin() + mBasePlane->v())));
	mOrigin->add(std::make_shared<line>(line_abstract(mBasePlane->origin() - mBasePlane->w(), mBasePlane->origin() + mBasePlane->w())));
	add(mOrigin);
}

/*
child[0] => ivec3(0, 0, 2)(origin,  6)
child[1] => ivec3(0, 0, 3)(originPoint,  5)
child[2] => ivec3(0, 0, 4)(XY plane,  2)
child[3] => ivec3(0, 0, 5)(YZ plane,  3)
child[4] => ivec3(0, 0, 6)(ZX plane,  4)
child[5] => ivec3(0, 0, 7)(plane,  7)

child[6] => ivec3(0, 0, 10)(point,  11)
child[7] => ivec3(0, 0, 11)(point,  12)
child[8] => ivec3(0, 0, 9)(line,  10)
child[9] => ivec3(0, 0, 13)(point,  14)
child[10] => ivec3(0, 0, 14)(point,  15)
child[11] => ivec3(0, 0, 12)(line,  13)
child[12] => ivec3(0, 0, 8)(skorigin,  9)

child[13] => ivec3(0, 0, 18)(point,  11)
child[14] => ivec3(0, 0, 19)(point,  12)
child[15] => ivec3(0, 0, 17)(line,  10)
child[16] => ivec3(0, 0, 21)(point,  14)
child[17] => ivec3(0, 0, 22)(point,  15)
child[18] => ivec3(0, 0, 20)(line,  13)
child[19] => ivec3(0, 0, 16)(skorigin,  9)

child[20] => ivec3(0, 0, 15)(sketch,  8)

child[21] => ivec3(0, 0, 24)(sketchPoint,  16)
child[22] => ivec3(0, 0, 25)(sketchPoint,  17)
child[23] => ivec3(0, 0, 23)(line,  18)
child[24] => ivec3(0, 0, 27)(sketchPoint,  19)
child[25] => ivec3(0, 0, 28)(sketchPoint,  20)
child[26] => ivec3(0, 0, 26)(line,  21)

*/