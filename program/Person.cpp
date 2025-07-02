#include "Person.h"
//we have to access member of boxes
#include "Box.h"
//we have to construct Ethnicity objects
#include "Ethnicity.h"
#include <stdexcept>

Person::Person(Box *my_home, double my_tol):
        tolerance(my_tol), ethn(Ethnicity()), home(my_home)
                { color_home(); }
Person::Person(Box* my_home, double my_tol, char my_ethn):
        tolerance(my_tol), ethn(Ethnicity(my_ethn)), home(my_home)
                { color_home(); }
Person::Person(): ethn(Ethnicity())
    { }
bool Person::watch_neighbors()
    {
        if (home == nullptr) throw std::runtime_error("Checking neighbors of homeless person!");
        is_happy = get_satisfaction(home) <= tolerance;
        return is_happy;
    }
char Person::get_ethn_char() const { return ethn.get_char(); }
double Person::get_satisfaction(const Box *a_house) const
{
    auto neigh_ethns = a_house->get_neigh_colors();
    double diff_ethn = 0.;
    for (auto elem : neigh_ethns)
        if (elem != ethn.get_char())
            ++diff_ethn;
    return diff_ethn/neigh_ethns.size();
}
Person* Person::moves(Box *new_home) 
{   
    blank_home(); //the house they left becomes blank
    home = new_home; //change house
    color_home(); //color new house
    return this;
}
void Person::color_home()
    { if(home) home->color = ethn.get_char(); }
void Person::blank_home() 
        { if(home) home->color = Box::free; } //sets to char that indicate free house
//non-member functions
 double get_same_ethn_neigh_vn(Person *a_person)
 {
     double counter = 0;
     for (auto other_ethns : a_person->get_home()->get_vn_neigh_colors())
        if (other_ethns == a_person->get_ethn_char())
            ++counter;
    return counter;
 }
 double get_total_neigh_vn(Person *a_person)
        { return a_person->get_home()->get_vn_neigh_colors().size(); }