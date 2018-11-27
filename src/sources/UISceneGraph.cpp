#include <UISceneGraph.hpp>

UISceneGraph::UISceneGraph(const shared_ptr<UIElement>& root):
    SceneGraph(root) {
    idListMap.insert(make_pair(root->getName(), vector<unsigned long>()));
    idListMap[root->getName()].push_back(root->subscribeToOnZindexChange([this, root](const UIElement::ON_ZINDEX_CHANGE& e) {
        orderedElements.erase(orderedElements.find(root));
        orderedElements.insert(root);
    }));
    idListMap[root->getName()].push_back(root->subscribeToOnChildAppended([this](const UIElement::ON_CHILD_APPENDED& e) {
        this->handleChildAppended(e);
    }));

    orderedElements.insert(root);
}

void UISceneGraph::draw() const noexcept {
    for (const shared_ptr<UIElement>& element : orderedElements) {
        UIUtils::draw(element, ortho(0.f, getWidth(), getHeight(), 0.f));
    }
}

void UISceneGraph::handleChildAppended(const UIElement::ON_CHILD_APPENDED& e) noexcept {
    UIElement const* parent = dynamic_cast<UIElement const*>(e.parent);
    shared_ptr<UIElement> child = shared_ptr<UIElement>(dynamic_cast<UIElement*>(e.child.get()));

    if (parent == nullptr || child == nullptr) {
        return;
    }

    idListMap.insert(make_pair(child->getName(), vector<unsigned long>()));
    idListMap[child->getName()].push_back(child->subscribeToOnZindexChange([this, parent, child](const UIElement::ON_ZINDEX_CHANGE& e) {
        orderedElements.erase(orderedElements.find(child));
        orderedElements.insert(child);
    }));
    idListMap[child->getName()].push_back(child->subscribeToOnChildAppended([this](const UIElement::ON_CHILD_APPENDED& e) {
        this->handleChildAppended(e);
    }));
    idListMap[child->getName()].push_back(child->subscribeToOnRemoveFromSceneObject([this, parent, child](const UIElement::ON_REMOVE_FROM_SCENE_OBJECT& e) {
        if (parent == e.parent) {
            orderedElements.erase(orderedElements.find(child));
            vector<unsigned long>& ids = idListMap[child->getName()];
            child->getManager().removeSubscription<UIElement::ON_ZINDEX_CHANGE>(ids[0]);
            child->getManager().removeSubscription<UIElement::ON_CHILD_APPENDED>(ids[1]);
            child->getManager().removeSubscription<UIElement::ON_REMOVE_FROM_SCENE_OBJECT>(ids[2]);
            idListMap.erase(child->getName());
        }
    }));

    orderedElements.insert(child);
}

void UISceneGraph::handleClick(const double& x, const double& y) {
    for (const shared_ptr<UIElement>& element : orderedElements) {
        if (element->isWithinElement(x, y)) {
            element->onClick(x, y);
            break;
        }
    }
}