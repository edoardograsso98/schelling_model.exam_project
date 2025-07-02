#ifndef GRID_H
#define GRID_H

#include <array>
#include <set>
class Box;

namespace Grid
{
    //sizes of the grid
    constexpr std::size_t n_cols = 50;
    constexpr std::size_t n_rows = 50;
    constexpr std::size_t grid_size = n_cols*n_rows;
    //types definitions
    typedef std::array<Box*, grid_size> grid_arr;
    typedef std::vector<std::set<Box*>> neighbors_grid;
    //the only function that is able to instanciate a Grid_movement object and collects R-neighbors
    //for each box that's passed to it
    std::set<Box*> move_through_grid(Box* start_pnt, int R);
    //the rest are global variables that should be accessible from anywere but must be modified
    //only through the functions in the anonymous namespace (that are accessible only inside the
    //Neighborhood.cpp file)
    extern neighbors_grid first_order;
    extern neighbors_grid second_order;
    extern neighbors_grid third_order;
    extern neighbors_grid fourth_order;
    extern neighbors_grid fifth_order;
    extern neighbors_grid sixth_order;
    extern neighbors_grid seventh_order;

    extern grid_arr boxes;
}

#endif