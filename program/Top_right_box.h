#ifndef TOP_RIGHT_BOX_H
#define TOP_RIGHT_BOX_H

#include "Box.h"

class Top_right_box: public Box {
public:
    using Box::Box; //inherits Box's constructors
    ~Top_right_box() = default;
    void right() const override
            { throw_except("right"); }
    void up() const override
            { throw_except("top"); }
};

#endif