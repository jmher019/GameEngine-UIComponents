#ifndef UI_SCENE_GRAPH_HPP
#define UI_SCENE_GRAPH_HPP

#include <SceneGraph.hpp>
#include <UIUtils.hpp>

#include <set>

class UISceneGraph : public SceneGraph {
private:
    set<shared_ptr<UIElement>, UIElement> orderedElements;
    unordered_map<string, vector<unsigned long>> idListMap;

public:
    UISceneGraph(const shared_ptr<UIElement>& root = make_shared<UIElement>(string("World_") + SceneUtils::createId()));

    void draw(void) const noexcept override;

    const shared_ptr<Camera>& getCamera(void) const noexcept = delete;

    void getFov(void) const noexcept = delete;

    void getNearZ(void) const noexcept = delete;

    void getFarZ(void) const noexcept = delete;

    void setCamera(const shared_ptr<Camera>& camera) noexcept = delete;

    void setFov(const float& fov) noexcept = delete;

    void setNearZ(const float& nearZ) noexcept = delete;

    void setFarZ(const float& farZ) noexcept = delete;

    void handleChildAppended(const UIElement::ON_CHILD_APPENDED& e) noexcept;

    void handleClick(const double& x, const double& y);
};

#endif // !UI_SCENE_GRAPH_HPP
