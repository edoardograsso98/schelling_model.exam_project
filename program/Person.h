#ifndef PERSON_H
#define PERSON_H

//needed for friend functions declarations
#include "Neighborhood.h"
//needed because a data member will be a Ethnicity object
#include "Ethnicity.h"
class Box;

class Person {
//Box must see the color_home and blank_home members to make the friend declarations in Box.h valid
friend class Box;
//these Neighborhood functions must make neighbors move home
friend Neighborhood::Neighborhood(int, double);
friend bool Neighborhood::iterate(unsigned, Person*);
friend Neighborhood::~Neighborhood();
friend const Neighborhood& Neighborhood::operator=(const Neighborhood&);
friend void Neighborhood::recolor_houses() const;
public:
    //creates person with given home, tolerance and random ethnicity
    Person(Box*, double);
    //create person with all in-class initializer and randomm ethnicity
    Person();
    //creates person with given home, tolerance and ethnicity
    Person(Box*, double, char);
    //default destructor and copy control members are ok
    //return the level of likelyness of ANY spot
    double get_satisfaction(const Box*) const;
    bool get_happyness() const { return is_happy; }
    const Box* get_home() const { return home; }
    char get_ethn_char() const;
    double get_tolerance() const { return tolerance; }
private:
    bool is_happy = true;
    Box *home = nullptr;
    const Ethnicity ethn;
    double tolerance = 0.5;
    //person moves home 
    Person* moves(Box *new_home);
    //checks the color of the neighbors and sets happiness accordingly
    //also, returns 1 if is happy, 0 if is not
    bool watch_neighbors();
    //friend functions of class Box:
    //change color of the box on which they reside
    void color_home();
    //when leaves home, sets its color to Box::free
    void blank_home();
};
//functions used to calculate the segregation coefficient in the Neighborhood class
//they use the Von Neughmann neighborhood
double get_same_ethn_neigh_vn(Person*);
double get_total_neigh_vn(Person*);

#endif