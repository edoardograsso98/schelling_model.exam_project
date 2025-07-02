#ifndef TOP_BOX_H
#define TOP_BOX_H

#include "Box.h"

class Top_box: public Box {
public:
    using Box::Box; //inherits Box's constructors
    ~Top_box() = default;
    void up() const override
            { throw_except("top"); }

};

#endif