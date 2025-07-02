#ifndef TOP_LEFT_BOX_H
#define TOP_LEFT_BOX_H

#include "Box.h"

class Top_left_box: public Box {
public:
    using Box::Box; //inherits Box's constructors
    ~Top_left_box() = default;
    void left() const override
            { throw_except("left"); }
    void up() const override
            { throw_except("top"); }
};

#endif