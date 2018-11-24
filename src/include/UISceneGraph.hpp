#ifndef UI_SCENE_GRAPH_HPP
#define UI_SCENE_GRAPH_HPP

#include <SceneGraph.hpp>

class UISceneGraph : public SceneGraph {

public:
    UISceneGraph(const shared_ptr<SceneObject>& root = make_shared<SceneObject>(string("World")));

    void draw(const mat4& ProjectionViewMatrix) const noexcept override;
};

#endif // !UI_SCENE_GRAPH_HPP
