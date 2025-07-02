#ifndef BOTTOM_RIGHT_BOX_H
#define BOTTOM_RIGHT_BOX_H

#include "Box.h"

class Bottom_right_box: public Box {
public:
    using Box::Box; //inherits Box's constructors
    ~Bottom_right_box() = default;
    void right() const override
            { throw_except("right"); }
    void down() const override
            { throw_except("bottom"); }
};

#endif