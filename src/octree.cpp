#include "octree.hpp"

Octree::Octree(const glm::vec3& origin, const glm::vec3& halfDimension):
  origin(origin),
  halfDimension(halfDimension),
  has_data(false)
{
    // Initially, there are no children
    for(int i=0; i<8; ++i) 
      children[i] = NULL;
}

Octree::~Octree() {
  // Recursively destroy octants
  for(int i=0; i<8; ++i) 
    delete children[i];
}

// Determine which octant of the tree would contain 'point'
int Octree::getOctantContainingPoint(const glm::vec3& point) const {
  int oct = 0;
  if(point.x >= origin.x) oct |= 4;
  if(point.y >= origin.y) oct |= 2;
  if(point.z >= origin.z) oct |= 1;
  return oct;
}

bool Octree::isLeafNode() const {
  // This is correct, but overkill. See below.
  /*
     for(int i=0; i<8; ++i)
     if(children[i] != NULL) 
     return false;
     return true;
   */

  // We are a leaf iff we have no children. Since we either have none, or 
  // all eight, it is sufficient to just check the first.
  return children[0] == NULL;
}

// void Octree::insert(OctreePoint* point) {
void Octree::insert(const glm::vec3& point) {
  // If this node doesn't have a data point yet assigned 
  // and it is a leaf, then we're done!
  if(isLeafNode()) {
    if (!has_data) {
      data = point;
      has_data = true;
      return;
    } else {
      // We're at a leaf, but there's already something here
      // We will split this node so that it has 8 child octants
      // and then insert the old data that was here, along with 
      // this new data point

      // Save this data point that was here for a later re-insert
      glm::vec3 oldPoint = data;
      has_data = false;

      // Split the current node and create new empty trees for each
      // child octant.
      for(int i=0; i<8; ++i) {
        // Compute new bounding box for this child
        glm::vec3 newOrigin = origin;
        newOrigin.x += halfDimension.x * (i&4 ? .5f : -.5f);
        newOrigin.y += halfDimension.y * (i&2 ? .5f : -.5f);
        newOrigin.z += halfDimension.z * (i&1 ? .5f : -.5f);
        children[i] = new Octree(newOrigin, halfDimension*.5f);
      }

      // Re-insert the old point, and insert this new point
      // (We wouldn't need to insert from the root, because we already
      // know it's guaranteed to be in this section of the tree)
      // TODO: no need for recursive call? (child.data = NULL in ctor)
      children[getOctantContainingPoint(oldPoint)]->insert(oldPoint);
      children[getOctantContainingPoint(point)]->insert(point);
    }
  } else {
    // We are at an interior node. Insert recursively into the 
    // appropriate child octant
    int octant = getOctantContainingPoint(point);
    children[octant]->insert(point);
  }
}

// This is a really simple routine for querying the tree for points
// within a bounding box defined by min/max points (bmin, bmax)
// All results are pushed into 'results'
void Octree::getPointsInsideBox(const glm::vec3& bmin, const glm::vec3& bmax, std::vector<glm::vec3>& results, int& n_calls) {
  // If we're at a leaf node, just see if the current data point is inside
  // the query bounding box
  if(isLeafNode()) {
    if (has_data) {
      // TODO: if point inside bbox
      glm::vec3 p = data;
      n_calls++;
      if(p.x>bmax.x || p.y>bmax.y || p.z>bmax.z) return;
      if(p.x<bmin.x || p.y<bmin.y || p.z<bmin.z) return;
      results.push_back(data);
    }
  } else {
    // We're at an interior node of the tree. We will check to see if
    // the query bounding box lies outside the octants of this node.
    for(int i=0; i<8; ++i) {
      // Compute the min/max corners of this child octant
      glm::vec3 cmax = children[i]->origin + children[i]->halfDimension;
      glm::vec3 cmin = children[i]->origin - children[i]->halfDimension;

      // If the query rectangle is outside the child's bounding box, 
      // then continue
      // TODO: intersection between 2 bboxes (see mozilla website)
      if(cmax.x<bmin.x || cmax.y<bmin.y || cmax.z<bmin.z) continue;
      if(cmin.x>bmax.x || cmin.y>bmax.y || cmin.z>bmax.z) continue;

      // At this point, we've determined that this child is intersecting 
      // the query bounding box
      n_calls++;
      children[i]->getPointsInsideBox(bmin,bmax,results, n_calls);
    } 
  }
}
