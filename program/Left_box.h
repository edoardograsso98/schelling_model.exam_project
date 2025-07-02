#ifndef LEFT_BOX_H
#define LEFT_BOX_H

#include "Box.h"

class Left_box: public Box {
public:
    using Box::Box; //inherits Box's constructors
    ~Left_box() = default;
    void left() const override
            { throw_except("left"); }

};

#endif