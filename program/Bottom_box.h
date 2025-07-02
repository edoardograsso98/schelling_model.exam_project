#ifndef BOTTOM_BOX_H
#define BOTTOM_BOX_H

#include "Box.h"

class Bottom_box: public Box {
public:
    using Box::Box; //inherits Box's constructors
    ~Bottom_box() = default;
    void down() const override
            { throw_except("bottom"); }

};

#endif