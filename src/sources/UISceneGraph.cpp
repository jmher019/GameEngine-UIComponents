#include <UISceneGraph.hpp>

UISceneGraph::UISceneGraph(const shared_ptr<SceneObject>& root):
    SceneGraph(root) {}

void UISceneGraph::draw(const mat4& ProjectionViewMatrix) const noexcept {
    root->draw(ProjectionViewMatrix);
}