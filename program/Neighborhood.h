#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H

#include <vector>
#include <set>
#include <memory>
class Person;
class Box;

class Neighborhood {
//Box and Person must be friends to see the functions that validate the friend declaration inside Box.h and Person.h
friend class Box;
friend class Person;
public:
    Neighborhood(int my_range = 1, double my_tolerance = 0.5);
    //copy control members:
    //the destructor must destroy only the Person objects, allocated by the constructors, and, if it's the last remaining instance
    //of a nieghborhood in the program, it should destroy also the grid
    ~Neighborhood();
    Neighborhood(const Neighborhood&);
    const Neighborhood& operator=(const Neighborhood&);
    //a function to print the neighborhood on the terminal, use bool = 1 only if your terminals supports ANSI color codes
    void print_neigh(bool = 1) const;
    //move a (default random) person to a random house that is better than the previous one (if found), repeat for default 10000 times
    //returns false if after all the iterations nobody has moved
    bool iterate(unsigned = 10000, Person* = nullptr);
    //calculates and returns the Von Neumann segregation coefficent
    double get_seg_coeff() const;
    //returns the fraction of people that are happy, we may need to stop the program when a certain
    //fraction is reached, because to reach 100% of happiness (hence stability) sometimes
    //takes too long and others is impossible
    double fraction_happy() const;
    //check wether the neighborhood is stationary, that is when everyone is happy or when nobody can move
    //to a better place. Sometimes due to some people moving in circle it is impossible to reach stability
    bool is_stationary() const;
    //getter members
    double get_density() const { return density; }
    int get_range() const { return neigh_distance; }
    double get_tolerance() const;
private:
    //function called in the constructor, allocates grid memory and sets grid_ptr to non-nullptr value
    void initialize_grid();
    //functions that sets all the houses neighbors according to the given range
    //thanks to the R-order neighbors sets in the Grid namespace
    void set_neighbors_from_vector(std::vector<std::set<Box*>>) const;
    void set_all_neighbors(int) const;
    //since multiple vectors of people can live in the same grid, it is important that whenever an action that requires quarying the grid is about
    //to be made, we set the colors according to the people of the neighborhood that calls such action
    void recolor_houses() const;
    //if there is only one instance of a neighborhood living in the program it's almost always useless to recolor the houses, there's one
    //important exception, that is when a neighborhood has just been destroyed: if there are two neighborhoods in the program and at a certain
    //point one of them is used and than destroied, even though the remaining neighborhood is the only one remaining we must recolor the houses
    //for that neighborhood or we will keep using the colors of the one we destroyed
    bool check_if_recolor() const;
    //data members:
    //the percentage of occupied houses
    const double density = 0.9;
    //the distance we want to consider for neighbors searching
    int neigh_distance;
    //the pointers points to objects on the heap
    std::vector<Person*> people;
    //used to restrict the number of houses that each person will consider for moving
    std::set<Box*> free_houses;
    //static member that is used to check wether we need to create a new grid (must do only when we instance the first neighborhood)
    //or not, like whenever we create more instances of a neighborhoods
    static Box *grid_ptr;
    //controls the number of instances living in the program: it's augmented in the constructor, decremented in the destructor
    static unsigned instances_control;
    //is set to true every time we call the destructor, set back to false as soon as we call the recolor function
    static bool just_deleted;
};

#endif
