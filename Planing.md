
Project started on september 27 2020

# Planing

## Version 0.0 planed feature set

* 2D sketch editor
    * Lines
        * Single line
        * Polylines
    * Circles
        * Circle
        * Three point arc
    * Bezier
        * n-points combined quadratic bezier
    * Constraints
        * Point-point coincidence
        * Point-line coincidence
        * Point-circle coincidence
        * Line-line angle
            * Parallelism 
            * Right angle
            * Horizontal 
            * Vertical
        * Line-line distance
        * Point-line distance
        * Line dimension
    * Regular polygons & rectangle (as constrained lines)
    * Saving sketch
        * Custom xml-like format
        * PDF
        * SVG (with or without constraint retention)
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

## Current developpement goals

* Get constraints out of Drawables again (bad move were done..)
* Stop relying on expression system for constraint solver (it will be kept for future expression parsing and dynamic input)
* Solve constraints rather than equations
* Make sketch geometries integrate rather derive from abstract geometries
* Constraints get to know about abstract geometries
* More speeed in the solver with a reduced number of jumps
* Move expressions out of constraint solving and into utils