#ifndef RIGHT_BOX_H
#define RIGHT_BOX_H

#include "Box.h"

class Right_box: public Box {
public:
    using Box::Box; //inherits Box's constructors
    ~Right_box() = default;
    void right() const override
            { throw_except("right"); }

};

#endif