
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

* More speeed in the solver with a reduced number of jumps
