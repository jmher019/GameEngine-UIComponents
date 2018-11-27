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

    void handleChildAppended(const UIElement::ON_CHILD_APPENDED& e) noexcept;

    void handleClick(const double& x, const double& y);
};

#endif // !UI_SCENE_GRAPH_HPP
