
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
	
	glm::vec3 mTranslation, mScale;
	glm::mat4 mTransform;
public:
	entity();

	void draw(std::shared_ptr<camera> cam, glm::mat4 additionalTransform = glm::mat4(1.0f));
	void draw_selection(std::shared_ptr<camera> cam, glm::mat4 additionalTransform = glm::mat4(1.0f));

	void select();
	void unselect();
	void set_selected(bool select);
	bool selected() const;

	void set_hover(bool hover);
	bool hovered() const;

	void set_exists(bool exists_);
	bool exists() const { return mExists; }

	void setSelectionColor(glm::vec3 color) { mSelectionColor = color; }

	// void rotate(float angle, glm::vec3 axis) { mTransform = glm::rotate(mTransform, angle, axis); }
	void translate(glm::vec3 trans);
	void scale(float sc);

	void set_translation(glm::vec3 trans);
	void set_scale(glm::vec3 sc);

	float scale() const;
	glm::vec3 translation() const;

	glm::mat4 transform();
	void set_transform(glm::mat4 trans);
	void set_transform(glm::vec3 trans, glm::vec3 sc);

	operator bool() const { return exists(); }
protected:
	virtual void draw_impl(std::shared_ptr<camera> cam, glm::mat4 additionalTransform = glm::mat4(1.0f)) {};
	virtual void draw_selection_impl(std::shared_ptr<camera> cam, glm::mat4 additionalTransform = glm::mat4(1.0f)) {};
};

#endif