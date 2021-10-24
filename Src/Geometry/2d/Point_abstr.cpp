

template <typename Impl>
Point_abstr<Impl>::Point_abstr():
	Geom2d_abstr<Point_abstr<Impl>>(POINT)
{

}

template <typename Impl>
glm::vec2 Point_abstr<Impl>::pos()
{
	return static_cast<Impl*>(this)->pos_impl();
}
template <typename Impl>
bool Point_abstr<Impl>::within_geom(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
{
	return point_within(pos(), top_left, bottom_right);
}
template <typename Impl>
float Point_abstr<Impl>::dist_to_point_geom(glm::vec2 const& pt)
{
	return glm::distance(pos(), pt);
}
template <typename Impl>
float Point_abstr<Impl>::dist_to_point_2(glm::vec2 const& pt)
{
	return glm::distance2(pos(), pt);
}

bool point_within(glm::vec2 pt, glm::vec2 top_left, glm::vec2 bottom_right)
{
	if(top_left.x <= pt.x && bottom_right.x >= pt.x && top_left.y >= pt.y && bottom_right.y <= pt.y)
		return true;
	return false;
}


Point::Point(glm::vec2 pt):
	mX(pt.x),
	mY(pt.y)
{
	init_params();
}
Point::Point():
	mX(0.0),
	mY(0.0)
{
	init_params();
}

glm::vec2 Point::pos_impl()
{
	return glm::vec2(mX.eval(), mY.eval());
}

void Point::init_params()
{
	mParams[0] = x_raw();
	mParams[1] = y_raw();
}