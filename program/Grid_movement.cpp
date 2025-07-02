#include "Grid_movement.h"
#include "Utilities.h"
#include "Box.h"
#include <algorithm>
#include <map>


Grid_movement::Grid_movement(Box* my_pos, double my_dist):
        start_pos(my_pos), current_pos(my_pos), max_distance(my_dist)
{ 
    if (std::find(Grid::boxes.cbegin(), Grid::boxes.cend(), start_pos) == Grid::boxes.cend())
        throw std::runtime_error("STARTING POINT PROVIDED IS NOT IN THE SET OF Grid::boxes");
}
//the sequent functions throw exceptions if the movement isn't allowed, so they should be
//called in try & catch blocks like try_movement does
void Grid_movement::move_up()
{
    current_pos->up(); //tries moving upwards
    //if the movement is allowed, modify the current position accordingly
    //if the movement isn't allowed, exception is thrown and the sequent line isn't executed
    current_pos = *(std::find(Grid::boxes.cbegin(), Grid::boxes.cend(), current_pos)-get_cols());
}
void Grid_movement::move_down()
{
    current_pos->down(); //tries moving downwards
    current_pos = *(std::find(Grid::boxes.cbegin(), Grid::boxes.cend(), current_pos)+get_cols());
}
void Grid_movement::move_right()
{
    current_pos->right(); //tries moving rightwards
    current_pos = *(std::find(Grid::boxes.cbegin(), Grid::boxes.cend(), current_pos)+1);
}
void Grid_movement::move_left()
{
    current_pos->left(); //tries moving leftwards
    current_pos = *(std::find(Grid::boxes.cbegin(), Grid::boxes.cend(), current_pos)-1);
}
//span a whole row
void Grid_movement::make_row()
{
    auto temp_pos = current_pos;
    while (status) 
        try_movement('l');
    restore_status(temp_pos);
    while (status)
        try_movement('r');
    restore_status(temp_pos);
}
//span the whole grid till we reach max_dist in every direction
const Grid_movement& Grid_movement::move_through_all()
{
    auto temp_pos = current_pos;
    while (status) {
        make_row();
        try_movement('u');
    }
    restore_status(temp_pos);
    try_movement('d');
    while (status) {
        make_row();
        try_movement('d');
    }
    return *this;
}
//funciton that "does the work"
void Grid_movement::try_movement(const char& dir)
{
    //static variables that just bind the chars to integers (switch condition requires int)
    static std::map<char, int> dir_map{{'r',1}, {'u',2}, {'l',3}, {'d',4}};
    static std::set<char> dirs({'r', 'u', 'l', 'd'});
    if (dirs.find(dir) == dirs.cend()) 
        throw std::runtime_error("INVALID ARGUMENT FOR MOVEMENT FUNCTION");
    int dir_num = dir_map[dir]; 
    switch (dir_num)
    {
    case 1:
        try { move_right(); } 
            catch (std::range_error err)
                //if exception is thrown it means that we have to come out of the other side because we are on a thorus
                { current_pos = Grid::boxes.at(get_position(current_pos)-get_cols()+1); }
        break;
    case 2:
        try { move_up(); }
            catch (std::range_error err)
                { current_pos = Grid::boxes.at(get_position(current_pos)+get_size()-get_cols()); }
        break;
    case 3:
        try { move_left(); }
            catch (std::range_error err)
                { current_pos = Grid::boxes.at(get_position(current_pos)+get_cols()-1); }
        break;
    case 4:
        try { move_down(); }
            catch (std::range_error err)
                { current_pos = Grid::boxes.at(get_position(current_pos)-get_size()+get_cols()); }
    }
    //sets status according to how much distance we made
    control_stop();
    //controls wether we have to add the house we are currently on to the visited houses, based on the distance of this house
    //from the starting point
    if (control_distance()) {
        visited_houses.insert(current_pos);
    }
}
double Grid_movement::get_distance() const 
    { return *current_pos-*start_pos; }