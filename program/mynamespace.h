#ifndef MYNAMESPACE_H
#define MYNAMESPACE_H

//All the boxes .h files are needed to instanciate them in the __proper_box__ function
#include "Bottom_box.h"
#include "Bottom_left_box.h"
#include "Bottom_right_box.h"
#include "Central_box.h"
#include "Left_box.h"
#include "Right_box.h"
#include "Top_box.h"
#include "Top_left_box.h"
#include "Top_right_box.h"
//functions inside this file manage the members of Grid
#include "Grid.h"

//this file contains an anonymous namespace, this means that the functions here listed are accessible only in the
//cpp that directly includes this .h, that is Neighborhood.cpp

namespace 
{
    //returns the address of the heap-allocated correctly-instanciated box, based on the position on the grid of that box
    Box* __proper_box__(unsigned row, unsigned col)
    {
        Box *new_box;
        switch(row) {
            case 1:
                //if position is (1,1) returns top left box, if it is (1,2), (1,3)... returns top box etc...
                (col == 1) ? new_box = new Top_left_box(row, col) :
                            (col == Grid::n_cols) ? new_box = new Top_right_box(row, col) :
                                                    new_box = new Top_box(row, col);
                break;
            case Grid::n_rows:
                (col == 1) ? new_box = new Bottom_left_box(row, col) :
                            (col == Grid::n_cols) ? new_box = new Bottom_right_box(row, col) :
                                                    new_box = new Bottom_box(row, col);
                break;
            default:
                (col == 1) ? new_box = new Left_box(row, col):
                            (col == Grid::n_cols) ? new_box = new Right_box(row, col) :
                                                    new_box = new Central_box(row, col);
        }
        return new_box;
    }
    //fills the sets of R-order neighbors for each box in a way that
    //first_order.at(0) contains all the 4 nearest neighbors of the top left box
    //(remember that the geometry is thoroidal)
    void set_order_neighbors()
    {
        for (auto house : Grid::boxes) {
            Grid::first_order.push_back(Grid::move_through_grid(house, 1));
            Grid::second_order.push_back(Grid::move_through_grid(house, 2));
            Grid::third_order.push_back(Grid::move_through_grid(house, 3));
            Grid::fourth_order.push_back(Grid::move_through_grid(house, 4));
            Grid::fifth_order.push_back(Grid::move_through_grid(house, 5));
            Grid::sixth_order.push_back(Grid::move_through_grid(house, 6));
            Grid::seventh_order.push_back(Grid::move_through_grid(house, 7));
        }
    }
    void allocate_grid_memory()
    {
        //construct the grid
        auto it = Grid::boxes.begin();
        for (std::size_t i = 1;  i <= Grid::n_rows; ++i)
            for (std::size_t j = 1; j <= Grid::n_cols; ++j)
                *it++ = __proper_box__(i,j);
        //sets neighbors up to seventh order for all the boxes
        set_order_neighbors();
    }
    void destroy_grid_memory()
    {
        //the gird gets destroyed when the last instance of Neighborhood goes out of scope
        Grid::first_order.clear(); Grid::second_order.clear(); Grid::third_order.clear();
        Grid::fourth_order.clear(); Grid::fifth_order.clear(); Grid::sixth_order.clear();
        Grid::seventh_order.clear();
        for (auto box : Grid::boxes)
            delete box;
    }
}


#endif