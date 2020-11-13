
#ifndef PLANE_HPP_
#define PLANE_HPP_

#include "entity.hpp"
#include "plane_abstract.hpp"

#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <iostream>

class plane : public entity, public plane_abstract {
private:
	const glm::vec3 mColorA = glm::vec3(1.0f, 0.0f, 0.95f), mColorB = glm::vec3(0.0f, 0.83f, 0.6f);

	glm::vec3 mVertices[4];
	unsigned int mIndices[6], mOutlineIndices[8];

	std::shared_ptr<vertexBuffer> 	mVB;
	std::shared_ptr<vertexArray> 	mVA;
	std::shared_ptr<indexBuffer>	mIB, mOutlineIB;
	std::shared_ptr<shader> 		mShader, mSelectionShader;
public:
	plane(plane_abstract const& plane_, std::string const& label = "");

	// static plane from_1Point1Vector(glm::vec3 origin, glm::vec3 n, std::string const& label);
	static plane from_1Point2Vectors(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w, std::string const& label = "");
	// static plane from_3Points(glm::vec3 origin, glm::vec3 pt1, glm::vec3 pt2, std::string const& label);

	// static std::shared_ptr<plane> from_1Point1Vector_ptr(glm::vec3 origin, glm::vec3 n, std::string const& label);
	static std::shared_ptr<plane> from_1Point2Vectors_ptr(glm::vec3 const& origin, glm::vec3 const& v, glm::vec3 const& w, std::string const& label = "");
	// static std::shared_ptr<plane> from_3Points_ptr(glm::vec3 origin, glm::vec3 pt1, glm::vec3 pt2, std::string const& label);

	virtual void draw(std::shared_ptr<camera> cam, glm::mat4 additionalTransform = glm::mat4(1.0f));
	virtual void draw_selection(std::shared_ptr<camera> cam, glm::mat4 additionalTransform = glm::mat4(1.0f));
private:
	void init_buffers();
};

#endif