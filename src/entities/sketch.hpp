
#ifndef SKETCH_HPP_
#define SKETCH_HPP_

#include "entity.hpp"
#include "entitiesIndexer.hpp"
#include "plane_abstract.hpp"

#include <memory>
#include <vector>

/*
	@class sketch describes a sketch which is a bunch of geometries with constraints
	@parent : entity
	@parent : entitiesIndexer
*/
class sketch : public entity, public entitiesIndexer {
private:
	std::shared_ptr<plane_abstract> mBasePlane; // Plane onto which every geometry is added, maybe it should descend from plane_abstract..
public:
	/*
		@function sketch creates a sketch

		@param base_plane : The plane onto which everything is constructed in the sketch
	*/
	sketch(std::shared_ptr<plane_abstract> base_plane);
	/*
		@function sketch creates a sketch

		@param base_plane : The plane onto which everything is constructed in the sketch
		@param indexer : 	An indexer to follow, likely the part onto which it is added
	*/
	sketch(std::shared_ptr<plane_abstract> base_plane, std::shared_ptr<entitiesIndexer> indexer);

	/*
		@function basePlane

		@return : The base plane of the sketch
	*/
	std::shared_ptr<plane_abstract> basePlane() { return mBasePlane; }
protected:
	/*
		@function draw_impl draws the part on screen, it calls draw on all it's components

		@param cam : The camera used for rendering
	*/
	virtual void draw_impl(std::shared_ptr<camera> cam);
	/*
		@function draw_selection_impl draws the part on the selection buffer, it calls draw_selection on all it's components

		@param cam : The camera used for rendering
	*/
	virtual void draw_selection_impl(std::shared_ptr<camera> cam);
};

#endif