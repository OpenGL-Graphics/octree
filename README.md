# SimpleOctree

A simple octree with good commenting for learning how octrees work. Blog post incoming with description, or read comments in Octree.h

# How to build
```terminal
$ mkdir build && cd build
$ cmake .. && make -j && ./main
```

# Use
- The octree will be used to quickly detect collisions between the player & nearby walls.
- Octrees should be a lot less frames-hungry than a naive brute force approach (i.e. compare against all walls each frame).

# Octree notions
- Root bounding box (bbox) = entire bbox.
- Each node (except leave nodes) is subdivided into 8 children (octants).
- 3D points stored in leaves (one point/leaf, NULL otherwise).
- If tree isn't balanced, the collision detections should be slow.
- Index of each octant (in binary = b2 b1 b0) given according to the following:
  - p.x >= center.x => b2 = 1
  - p.y >= center.y => b1 = 1
  - p.z >= center.z => b0 = 1

# Notes
- Depth of octree depends on # of points as well as their locations. E.g. In a same level, all points could be in the same child quadrant (in its descendents nodes actually), while other quadrants are empty.
- When querying with `getPointsInsideBox()`, the # of if-statements = 1117 for the octree (check if bboxes intersect then if node point inside query bbox), compared to 1M for the naive brute force approach (i.e. check if every point is inside the query bbox).
- IOW, the optimized exec. time comes from detecting if there was a collision against the enclosing bounding box (without exploring its children, if not), instead of checking each & every point individually.
