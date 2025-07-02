#ifndef BOX_H
#define BOX_H

#include <set>
#include <vector>
//subsequent two are needed for the friend functions declarations
#include "Person.h"
#include "Neighborhood.h"

class Box {
friend double operator-(const Box&, const Box&);
//lets the person color the box, based on their ethnicity
friend void Person::color_home();
//if the person leaves, the box get's colored blank
friend void Person::blank_home();
//inside the neighborhood class, the neighbors of the boxes are setted
friend void Neighborhood::set_neighbors_from_vector(std::vector<std::set<Box*>>) const;
friend Neighborhood::Neighborhood(int, double);
friend void Neighborhood::recolor_houses() const;
public:
    //constructors
    Box() = delete;
    Box(unsigned r, unsigned c): 
            row_coord(r), col_coord(c) { }
    //pure virtual destructor
    virtual ~Box() = 0;
    //subsequent methods will be overridden in subclasses and will throw exceptions
    //on attempts to go out of bounds (if you are on the left border you can't go left)
    //since we consider thoroidal geometry, an exception of going out of the borders will be
    //handled by placing the cursor on the opposite side
    virtual void right() const { }
    virtual void left() const { }
    virtual void up() const { }
    virtual void down() const { }
    //members used by other classes
    char get_color() const { return color; }
    std::vector<char> get_neigh_colors() const { set_neigh_colors(); return neigh_colors; }
    //the sequent function returns neigh colors for Von Neumann Neighborhood (R = 1), is used to 
    //calculate the Von Neumann segregation coefficient in the Neighborhood class
    std::vector<char> get_vn_neigh_colors() const;
    //getter member for coordinates:
    unsigned get_row_coord() const { return row_coord; }
    unsigned get_col_coord() const { return col_coord; }
    //the char indicating a free house
    static constexpr char free = 'X';
protected:
    const unsigned row_coord;
    const unsigned col_coord;
    std::set<Box*> neighbors;
    //a vector containing chars indicating the colors of the R-neighbors, mutable means that the colour of its neighbor can
    //always change even though the object itself is constant, this (other than making sense) allows us to make the
    //set_neigh_colors and, in turn, get_neigh_colors const functions
    mutable std::vector<char> neigh_colors;
    //sets the colors of the current neighbors, will always be called before returning the vector of color of neighbors
    void set_neigh_colors() const;
    char color = free; //default: blank color
};
//non-member utility function
void throw_except(const std::string&);
//we'll use minus operator to get easily the distances between two boxes, remind that the geomethry of the grid is thoroidal
double operator-(const Box&, const Box&);

#endif