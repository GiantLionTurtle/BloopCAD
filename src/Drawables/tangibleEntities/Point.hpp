
#ifndef POINT_HPP_
#define POINT_HPP_

#include <Drawables/Drawable.hpp>
#include <Geom/Geom3d/Point_abstr.hpp>
#include <ConstraintsSolver/Expression.hpp>

#include <Graphics_utils/VertexArray.hpp>
#include <Graphics_utils/VertexBuffer.hpp>
#include <Graphics_utils/Shader.hpp>

#include <glm/glm.hpp>

class Point : public Drawable, public Geom3d::Point_abstr {
private:
	const glm::vec3 mColor = glm::vec3(0.0f, 0.89f, 0.725f); // Point color

	var_ptr mX, mY, mZ;

	VertexArray* mVA;
	VertexBuffer* mVB;
	Shader* mShader;
public:
	Point(glm::vec3 const& basePoint);
	virtual ~Point();

	virtual int selection_rank() { return 5; }
	SelPoint closest(glm::vec2 cursor, Camera* cam, int filter);

	virtual void move(glm::vec3 from, glm::vec3 to);

	glm::vec3 pos() const;
	void set(glm::vec3 const& p);
protected:
	void init_impl();
	void draw_impl(Camera_ptr cam, int frame, draw_type type = draw_type::ALL);
	void update_impl();
};

#endif