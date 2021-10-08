
Project started on september 27 2020

# Planing

## Version 1.0 planed feature set

* 2D sketch editor
    * Lines
        * Single line
        * Polylines
    * Circles
        * Circle
        * Three point arc
    * Constraints
        * Point-point coincidence
        * Line-line angle
            * Parallelism 
            * Right angle
            * Horizontal 
            * Vertical
        * Line-line distance
        * Point-line distance
        * Line dimension
    * Regular polygons & rectangle (as constrained lines)
* 3D part editor
    * Extrusion of faces
    * Selection of planes for 2D sketch editor
    * Save part as
        * .stl 
        * .obj
    * Load models for viewing
        * .stl
        * .obj
* Navigation
    * Orbit
    * Pan
    * Zoom

## TODO before merge with main

* Check if it is possible to simplify ConstraintsSystem's varState thingy
* Re-enable point-line and point-circle coincidence
* Re-enable double-click to enter sketch
* Solve updating problem when adding coincidence constraint between a free and fixed point
* Enforce a better constraint behavior when dragging a partially constrained line
* Comments for dragged point
* Analize the case of the get_single_var
* Replace expvec with alias to std::array
