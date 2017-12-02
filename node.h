#pragma once

#include "mesh/mesh.h"
#include "camera.h"
#include <QMatrix4x4>

/*
 *  A Node encapsulates a mesh together with a model transformation.
 *
 *  Furthermore, it can have a vector of child nodes.
 *
 *  When drawing a node, child nodes will be drawn as well
 *  (in depth-first order), and transformations will be
 *  accumulated from parent to all children, in such
 *  way that the child transformation is multiplied
 *  from the right to the parent transformation.
 *
 *
 */
class Node
{
public:

    // constructor: requires a mesh, and takes an optional local transformation
    Node(std::shared_ptr<Mesh> mesh,
         QMatrix4x4 transformation = QMatrix4x4());

    // mesh: geometry + material information
    std::shared_ptr<Mesh> mesh;

    // 4x4 matrix: 3D transformation, applied to this mesh and all children
    QMatrix4x4 transformation;

    // vector of child nodes
    std::vector<std::shared_ptr<Node>> children;

    /*
     * draw the node by:
     * - calculating and setting the model matrix
     * - using the camera to set all transformation matrices in the material
     * - actually drawing the mesh
     */
    virtual void draw(const Camera& cam,
                      unsigned int light_pass = 0,
                      QMatrix4x4 parent_transform = QMatrix4x4());

    /*
     *  Finds node "child" recursively starting from the direct
     *  children of this node.
     *  Returns relative transformation from child's coords
     *  to this node's (parent) coords.
     *
     *  It is required that the child is references exactly once
     *  within the subtree. If the child not cannot be found, or
     *  is found multiple times, the result is undefined (assertion
     *  will fail in debug mode).
     *
     */
    QMatrix4x4 toParentTransform(std::shared_ptr<Node> child) const;


protected:

    // recursive helper for toParentTransform()
    void gatherChildrenTransformations(const Node& node,
                                       std::vector<QMatrix4x4>& result,
                                       QMatrix4x4 transform) const;


};

