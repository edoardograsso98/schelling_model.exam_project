#ifndef GRID_MOVEMENT_H
#define GRID_MOVEMENT_H

#include "Grid.h"
#include "Neighborhood.h"
#include <set>

class Grid_movement {
//This class is a pure non-interface class, it should be instantiated only by the unique friend function of this class, and its purpose is 
//to create walkers on a grid that helps instantiating neighbors of the boxes according to the vision of the agents that we want to consider
//defined inside the Grid namespace
friend std::set<Box*> Grid::move_through_grid(Box*,int);
private:
    //constructor initialize start_pos and current_pos to the same box* and max_distance to given distance
    Grid_movement(Box*, double);
    //movement functions:
    const Grid_movement& move_through_all();
    //called by the move_through_all function
    void make_row();
    //move_up, move_down etc should only be called by try_movement, otherwise they'll throw exceptions and stop the program
    void move_up();
    void move_down();
    void move_left();
    void move_right();
    //boxes visited by the walker, will need to instantiate neighbors in the neighborhood
    std::set<Box*> visited_houses;
    //will always return positive value due to how we defined "-" in Box class
    //returns distance as calculated on a thoroidal geomethry
    double get_distance() const;
    //tries the actual one tile movement, if it catches an exception
    void try_movement(const char&);
    //set status to false if distance overcomes or equals max_dist, called by try_movement
    void control_stop() { status = get_distance() < max_distance; }
    //checks wether we need to add the current house to the visited houses set, called by try_movement
    //if the distance is <= max_distance, we need to add the house
    bool control_distance() { return get_distance() <= max_distance; }
    //restores status and returns to a previous position
    void restore_status(Box* &prev_pos) { current_pos = prev_pos; status = true; }
    //data members
    Box *start_pos;
    Box *current_pos;
    bool status = true; //false = stop walking, true = keep walking
    double max_distance;
};

#endif
