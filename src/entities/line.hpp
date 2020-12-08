
#ifndef LINE_HPP_
#define LINE_HPP_

#include <entities/entity.hpp>

class line_abstract {
protected:
	glm::vec3 mPointA, mPointB;
public:
	line_abstract(glm::vec3 ptA, glm::vec3 ptB): mPointA(ptA), mPointB(ptB) {}

	glm::vec3 pointA() const { return mPointA; }
	glm::vec3 pointB() const { return mPointB; }
};

class line : public entity, public line_abstract {
private:
	const glm::vec3 mColor = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec3 mVertices[4];
	unsigned int mIndices[6];

	std::shared_ptr<vertexBuffer> 	mVB;
	std::shared_ptr<vertexArray> 	mVA;
	std::shared_ptr<indexBuffer>	mIB, mOutlineIB;
	std::shared_ptr<shader> 		mShader, mSelectionShader;
public:
	line(glm::vec3 ptA, glm::vec3 ptB);

	static line from_2Points(glm::vec3 ptA, glm::vec3 ptB);

	static std::shared_ptr<line> from_2Points_ptr(glm::vec3 ptA, glm::vec3 ptB);
protected:
	virtual void draw_impl(std::shared_ptr<camera> cam);
	virtual void draw_selection_impl(std::shared_ptr<camera> cam);

	void init_buffers();
};

#endif