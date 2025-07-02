#ifndef BOTTOM_LEFT_BOX_H
#define BOTTOM_LEFT_BOX_H

#include "Box.h"

class Bottom_left_box: public Box {
public:
    using Box::Box; //inherits Box's constructors
    ~Bottom_left_box() = default;
    void left() const override
            { throw_except("left"); }
    void down() const override
            { throw_except("bottom"); }
};

#endif