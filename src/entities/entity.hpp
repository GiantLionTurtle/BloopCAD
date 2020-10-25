
#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "../graphics_utils/vertexBuffer.hpp"
#include "../graphics_utils/vertexBufferLayout.hpp"
#include "../graphics_utils/vertexArray.hpp"
#include "../graphics_utils/indexBuffer.hpp"
#include "../graphics_utils/shader.hpp"
#include "../graphics_utils/camera.hpp"

#include <string>
#include <memory>

class entity {
protected:
    bool mSelected, mHovered;
public:
    entity() {};

    virtual void draw(std::shared_ptr<camera> cam) { std::cout<<"Draw not implemented...\n"; };
    virtual void draw_selection(std::shared_ptr<camera> cam, glm::ivec3 ind) { std::cout<<"Selection draw not implemented...\n"; }

    void select() { set_selected(true); };
    void unselect() { set_selected(false); };
    void set_selected(bool select) { mSelected = select; };
    bool selected() { return mSelected; };

    void set_hover(bool hover) { mHovered = hover; };
    bool hovered() { return mHovered; };

    virtual bool can_focus() { return true; };

    virtual std::string type() {return "sdjfskjfn";};
};

#endif