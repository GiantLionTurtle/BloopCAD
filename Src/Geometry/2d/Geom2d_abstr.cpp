

template<typename Geom>
glm::vec2 Geom2d_abstr<Geom>::at(float t)
{
	return static_cast<Geom*>(this)->at_geom(t);
}

template<typename Geom>
bool Geom2d_abstr<Geom>::within(glm::vec2 top_left, glm::vec2 bottom_right, bool contained)
{
	return static_cast<Geom*>(this)->within_geom(top_left, bottom_right, contained);
}

template<typename Geom>
float Geom2d_abstr<Geom>::closest_to_point_interp(glm::vec2 const& pt)
{
	return static_cast<Geom*>(this)->closest_to_point_interp_geom(pt);
}

template<typename Geom>
glm::vec2 Geom2d_abstr<Geom>::closest_to_point(glm::vec2 const& pt)
{
	return static_cast<Geom*>(this)->closest_to_point_geom(pt);
}

template<typename Geom>
float Geom2d_abstr<Geom>::dist_to_point(glm::vec2 const& pt)
{
    return static_cast<Geom*>(this)->dist_to_point_geom(pt);
}
