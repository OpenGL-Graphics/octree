#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <vector>
#include <glm/glm.hpp>
#include <array>
#include <memory>

/**!
 *
 */
class Octree {
private:
  // Physical position/size. This implicitly defines the bounding 
  // box of this node
  glm::vec3 origin;         //! The physical center of this node
  glm::vec3 halfDimension;  //! Half the width/height/depth of this node

  // The tree has up to eight children and can additionally store
  // a point, though in many applications only, the leaves will store data.
  // Octree *children[8]; //! Pointers to child octants
  // OctreePoint *data;   //! Data point to be stored at a node

  // hakim: field has_data added to avoid using pointer vec3 values
  // hakim: use smart pointers for children instead of raw pointers
  bool has_data;
  glm::vec3 data;   //! Data point to be stored at a node
  std::array<std::unique_ptr<Octree>, 8> children;

  /*
      Children follow a predictable pattern to make accesses simple.
      Here, - means less than 'origin' in that dimension, + means greater than.
      child:	0 1 2 3 4 5 6 7
      x:      - - - - + + + +
      y:      - - + + - - + +
      z:      - + - + - + - +
   */

public:
  Octree() = default;
  Octree(const glm::vec3& origin, const glm::vec3& halfDimension);

  int getOctantContainingPoint(const glm::vec3& point) const;
  bool isLeafNode() const;
  void insert(const glm::vec3& point);
  void getPointsInsideBox(const glm::vec3& bmin, const glm::vec3& bmax, std::vector<glm::vec3>& results);
};
#endif // OCTREE_HPP
