#ifndef CENTRAL_BOX_H
#define CENTRAL_BOX_H

#include "Box.h"

class Central_box: public Box {
public:
    using Box::Box; //inherits Box's constructors
    ~Central_box() = default;
};

#endif