

template<typename Impl>
Circle_abstr<Impl>::Circle_abstr()
	: Geom2d_abstr<Circle_abstr<Impl>>(CIRCLE)
{

}

template<typename Impl>
glm::vec2 Circle_abstr<Impl>::center_pos()
{
	return static_cast<Impl*>(this)->center_pos_impl();
}
template<typename Impl>
float Circle_abstr<Impl>::radius_val()
{
	return static_cast<Impl*>(this)->radius_val_impl();
}

template<typename Impl>
glm::vec2 Circle_abstr<Impl>::at_geom(float t)
{
	float angle = t * (M_PI * 2.0f);
	return center_pos() + glm::vec2(std::cos(angle), std::sin(angle)) * (float)radius_val();
}
template<typename Impl>
bool Circle_abstr<Impl>::within_geom(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
{
	if(contained) {
		glm::vec2 c = center_pos();
		float r = radius_val();
		return top_left.x <= c.x - r && top_left.y >= c.y + r && bottom_right.x >= c.x + r && bottom_right.y <= c.y - r;
	}
	Geom2d::Simple_line up(top_left, glm::vec2(bottom_right.x, top_left.y)),
								down(glm::vec2(top_left.x, bottom_right.y), bottom_right),
								// right(a, glm::vec2(a.x, b.y)),
								left(glm::vec2(top_left.x, bottom_right.y), top_left);
	return intersects(up) || intersects(down) /*|| intersects(&right)*/ || intersects(left) || within_geom(top_left, bottom_right, true);
}
template<typename Impl>
float Circle_abstr<Impl>::closest_to_point_interp_geom(glm::vec2 const& pt)
{
	// Closest point radialy then do inverse of at()
	glm::vec2 pt_norm = glm::normalize(pt - center_pos());
	float angle = std::asin(pt_norm.y);
	return angle / (M_PI * 2.0f);
}
template<typename Impl>
glm::vec2 Circle_abstr<Impl>::closest_to_point_geom(glm::vec2 const& pt)
{
	glm::vec2 center_to_point = glm::normalize(pt - center_pos());
	return center_pos() + center_to_point * (float)radius_val();  
}
template<typename Impl>
float Circle_abstr<Impl>::dist_to_point_geom(glm::vec2 const& pt)
{
	float dist_to_center = glm::distance2(center_pos(), pt);
	if(dist_to_center < radius_val()) {
		return radius_val() - dist_to_center;
	} else {
		return dist_to_center - radius_val();
	}
}
template<typename Impl>
bool Circle_abstr<Impl>::intersects(Simple_line& l)
{
	// Maths from https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm

	glm::vec2 line_dir = l.posB() - l.posA(); // line start to end
	glm::vec2 start_center = l.posA() - center_pos(); // line start to circle center

	// Three params of quadratic equation
	float a = glm::dot(line_dir, line_dir);
	float b = 2 * glm::dot(start_center, line_dir);
	float c = glm::dot(start_center, start_center) - radius_val() * radius_val();

	float determinant = b*b - 4 * a * c;

	if(determinant < 0) { // No solution, it is invalid for sqrt()
		return false; 
	}

	float sqrt_determinant = std::sqrt(determinant);

	float t0 = (-b - sqrt_determinant) / (2*a);

	if(t0 >= 0 && t0 <= 1)
		return true;

	float t1 = (-b + sqrt_determinant) / (2*a);

	if(t1 >= 0 && t1 <= 1) 
		return true;
	return false;
}


Circle::Circle(glm::vec2 c, float r)
	: mCenter(c)
	, mRadius(r)
{
	init_params();
}
Circle::Circle()
	: mCenter(glm::vec2(0.0f, 0.0f))
	, mRadius(0.0)
{
	init_params();
}
void Circle::init_params()
{
	mParams[0] = mCenter.x_raw();
	mParams[1] = mCenter.y_raw();
	mParams[2] = mRadius.val_ptr();
}