
#include "part.hpp"

#include <document.hpp>
#include <utils/errorLogger.hpp>
#include <entities/line.hpp>
#include <actions/sketchDesign/addLine_action.hpp>

part::part():
	entity(glm::ivec3(0, 0, 1)),
	mColumns(nullptr),
	mRefTreeModel(nullptr)
{
	init_scene();
	set_name("Part");
}
part::part(entity* parent):
	entity(parent),
	mColumns(nullptr),
	mRefTreeModel(nullptr)
{
	init_scene();
	set_name("Part");
}


void part::init_scene()
{
	// Maybe this whole function should just be in the constructor
	
	// Create the 3 planes of the origin
	mXY = std::shared_ptr<plane>(new plane(
		plane_abstract::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))));
	mXY->set_name("XY plane");
	mYZ = std::shared_ptr<plane>(new plane(
		plane_abstract::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), true))); // Inverted due to openGL's z axis being kind of backward
	mYZ->set_name("YZ plane");
	mZX = std::shared_ptr<plane>(new plane(
		plane_abstract::from_1Point2Vectors(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f))));
	mZX->set_name("ZX plane");
	add(std::shared_ptr<plane>(new plane(
		plane_abstract::from_1Point2Vectors(glm::vec3(-3.0f, 0.5f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f))))); // Temporary test plane

	// Add the three points
	add(mXY);
	add(mYZ);
	add(mZX);
}

void part::set_tree(modelColumns* columns, Gtk::TreeModel::Row row, Glib::RefPtr<Gtk::TreeStore> treeModel)
{
	// TODO: add cleanup if those are already set
	mColumns = columns;
	mRefTreeModel = treeModel;
	mRow = row;

	for_each([this](std::shared_ptr<entity> ent) {
		add_child_to_tree(ent);
	});
}

void part::add_child_to_tree(std::shared_ptr<entity> child)
{
	Gtk::TreeModel::Row childrow = *(mRefTreeModel->append(mRow.children()));
	childrow[mColumns->mColName] = child->name();
	childrow[mColumns->mPtr] = child;
}

void part::add(std::shared_ptr<entity> elem)
{
	if(mColumns && elem) {
		add_child_to_tree(elem);
	}
	entity::add(elem);
}

void part::add_sketch(std::shared_ptr<sketch> sketch_)
{
	if(sketch_) {
		BLOOP_MARKER;
		mSketches.push_back(sketch_);
		BLOOP_MARKER;
		add(sketch_);
		BLOOP_MARKER;
	} else {
		LOG_WARNING("Trying to add null sketch.");
	}
}
std::shared_ptr<sketch> part::get_sketch(int ind)
{
	// It's kind of a funny behavior, I might change it to just return nullptr on out of bounds
	if(ind < 0 || ind >= mSketches.size()) {
		if(mSketches.empty())
			return nullptr;
		return mSketches.back();
	}
	return mSketches[ind]; 
}

void part::draw_impl(std::shared_ptr<camera> cam, int frame)
{

}

void part::draw_selection_impl(std::shared_ptr<camera> cam, int frame)
{

}