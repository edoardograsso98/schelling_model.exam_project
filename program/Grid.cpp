#include "Grid.h"
//move_through_grid is the only function in the program that instanciate a grid_movement object
#include "Grid_movement.h"

//definitions (made explicit) of global variables
Grid::neighbors_grid Grid::first_order = {};
Grid::neighbors_grid Grid::second_order = {};
Grid::neighbors_grid Grid::third_order = {};
Grid::neighbors_grid Grid::fourth_order = {};
Grid::neighbors_grid Grid::fifth_order = {};
Grid::neighbors_grid Grid::sixth_order = {};
Grid::neighbors_grid Grid::seventh_order = {};

Grid::grid_arr Grid::boxes = {};
//friend function of Grid_movement
std::set<Box*> Grid::move_through_grid(Box* start_pnt, int R)
    { return Grid_movement(start_pnt, R).move_through_all().visited_houses; }