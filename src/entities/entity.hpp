
#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <graphics_utils/vertexBuffer.hpp>
#include <graphics_utils/vertexBufferLayout.hpp>
#include <graphics_utils/vertexArray.hpp>
#include <graphics_utils/indexBuffer.hpp>
#include <graphics_utils/shader.hpp>
#include <graphics_utils/camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <memory>

class entity {
protected:
	bool mSelected, mHovered, mExists;
	glm::vec3 mSelectionColor;
public:
	entity();

	void draw(std::shared_ptr<camera> cam);
	void draw_selection(std::shared_ptr<camera> cam);

	void select();
	void unselect();
	void set_selected(bool select);
	bool selected() const;

	void set_hover(bool hover);
	bool hovered() const;

	void set_exists(bool exists_);
	bool exists() const { return mExists; }

	void setSelectionColor(glm::vec3 color) { mSelectionColor = color; }

	operator bool() const { return exists(); }
protected:
	virtual void draw_impl(std::shared_ptr<camera> cam) {};
	virtual void draw_selection_impl(std::shared_ptr<camera> cam) {};
};

#endif