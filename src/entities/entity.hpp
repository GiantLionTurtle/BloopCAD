
#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "../graphics_utils/vertexBuffer.hpp"
#include "../graphics_utils/vertexBufferLayout.hpp"
#include "../graphics_utils/vertexArray.hpp"
#include "../graphics_utils/indexBuffer.hpp"
#include "../graphics_utils/shader.hpp"
#include "../graphics_utils/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <memory>

class entity {
protected:
    bool mSelected, mHovered;
    glm::vec3 mSelectionColor;
    glm::mat4 mTransform;
public:
    entity(): mSelected(false), mHovered(false), mSelectionColor(0.0f, 0.0f, 0.0f), mTransform(1.0f) {};

    virtual void draw(std::shared_ptr<camera> cam) { std::cout<<"Draw not implemented...\n"; };
    virtual void draw_selection(std::shared_ptr<camera> cam) { std::cout<<"Selection draw not implemented...\n"; }

    void select() { set_selected(true); };
    void unselect() { set_selected(false); };
    void set_selected(bool select) { mSelected = select; };
    bool selected() { return mSelected; };

    void set_hover(bool hover) { mHovered = hover; };
    bool hovered() { return mHovered; };

    void setSelectionColor(glm::vec3 color) { mSelectionColor = color; }

    virtual bool can_focus() { return true; };

    void rotate(float angle, glm::vec3 axis) { mTransform = glm::rotate(mTransform, angle, axis); }
    void translate(glm::vec3 translation) { mTransform = glm::translate(mTransform, translation); }
    void scale(glm::vec3 sc) { mTransform = glm::scale(mTransform, sc); }

    glm::mat4 transform() const { return mTransform; };
    void set_transform(glm::mat4 trans) { mTransform = trans; }

    virtual std::string type() {return "sdjfskjfn";};
};

#endif