#include "navigator/nodenavigator.h"

using namespace std;


NodeNavigator::NodeNavigator(std::shared_ptr<Node> node,
                             std::shared_ptr<Node> world,
                             std::shared_ptr<Node> camera)
    : node_(node), world_(world), camera_(camera)
{}
