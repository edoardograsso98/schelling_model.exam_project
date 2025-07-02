#include "Neighborhood.h"
//must construct people and ethnicities
#include "Person.h"
#include "Ethnicity.h"
//needed for the print_that_ethn function
#include "Utilities.h"
//"Utilities.h" alread includes "Grid.h" and "Box.h"
//"Box.h" is needed to access Box::free member and "Grid.h" for Grid::boxes
//"mynamespace.h" contains the functions to manage the memory occupied by the Grid, they should
//be aviable only inside this .cpp file
#include "mynamespace.h"

#include <iostream>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <TRandom3.h>
#include <TStopwatch.h>

//defined in Ethnicity.cpp
extern std::map<char,std::string> color_map;
//a bool to decide wether to print on the screen the cpu time of a call to some different functions
constexpr bool print_time = 0;

Neighborhood::Neighborhood(int my_range, double my_tolerance):
        neigh_distance(my_range)
{
    if (my_tolerance < 0 || my_tolerance > 1) throw std::out_of_range("Insert tolerance value between 0 and 1");
    ++instances_control;
    //initialize grid only if grid_ptr is the nullptr, grid_ptr is set to point to the first box of the grid inside the
    //initialize_grid function
    if (!grid_ptr) initialize_grid();
    //shuffle the array of boxes
    Grid::grid_arr shuffled(Grid::boxes);
    std::random_shuffle(shuffled.begin(), shuffled.end());
    auto it = shuffled.begin();
    std::size_t n_people = get_size()*density;
    while (n_people-- != 0)
        //for n_people times, a person with given tolerance and random ethnicity is created and moved into a random house
        //each person automatically colors their new home 
        people.push_back(new Person(*it++, my_tolerance)); //generates random ethnicity person with given home
    //initialize the free houses set
    while (it != shuffled.end()) {
        (*it)->color = Box::free;
        free_houses.insert(*it++);
    }
    //initialize the neighbor of each box according to the provided range
    set_all_neighbors(my_range);
    //set start happyness
    for (auto pers : people)
        pers->watch_neighbors();
}
const Neighborhood& Neighborhood::operator=(const Neighborhood &rhs)
{
    //make it safe in case of self assignment
    std::vector<Person*> copying_people;
    for (auto person : rhs.people) {
        copying_people.push_back(new Person(*person));
    }
    for (auto person : people) delete person;
    neigh_distance = rhs.neigh_distance;
    people = copying_people;
    free_houses = rhs.free_houses;
    return *this;
}
Neighborhood::Neighborhood(const Neighborhood &orig):
    neigh_distance(orig.neigh_distance), free_houses(orig.free_houses)
{ 
    ++instances_control; 
    for (auto person : orig.people)
        people.push_back(new Person(*person));
}
//notice: if your terminal doesn't support ANSI colour codes, you should use print_neigh(0) and maybe change the
//constexpr char "free" in the Box.h file from 'X' to ' ' to make the visualization clearer
void Neighborhood::print_neigh(bool use_colors) const
{
    if (check_if_recolor()) 
        recolor_houses();
    if (use_colors) {
        for (auto house : Grid::boxes) {
            if (house->get_color() != Box::free)
                print_that_ethn(house->get_color(), color_map.at(house->get_color()));
            else std::cout << Box::free;
            if (house->get_col_coord() == get_cols()) std::cout << std::endl;
        }
        return;
    }
    //use this if not use_colors
    for (auto house : Grid::boxes) {
        std::cout << house->get_color();
        if (house->get_col_coord() == get_cols()) std::cout << std::endl;
    }
}
Neighborhood::~Neighborhood()
{
    for (auto person : people)
        delete person;
    //destroy the grid if it's the last instance living
    if (instances_control == 1) {
        if (grid_ptr != nullptr)
            destroy_grid_memory();
        grid_ptr = nullptr;
    }
    //if this is not the last neighborhood left, change just_deleted to true
    else just_deleted = true;
    --instances_control;
}
bool Neighborhood::iterate(unsigned times, Person *test_pers)
{
    TStopwatch clock;
    if (check_if_recolor())
        recolor_houses();
    //something happened variable
    bool smt_happened = false;
    std::size_t n_people = get_size()*density;
    std::size_t n_free_houses = free_houses.size();
    //repeat the iterations given times
    while (times-- != 0) {
        Person *a_person;
        //select a random person among the set of people
        //if test_pers is not the nullptr, use it
        if (test_pers) a_person = test_pers;
        else a_person = people.at(gRandom->Integer(n_people-1));
        //if it's happy do nothing
        if (a_person->watch_neighbors()) continue;
        //save starting home
        auto start_home = a_person->home;
        //get current satisfaction
        double current_sat = a_person->get_satisfaction(start_home);
        //initialize a counter, the person will try to move exactly n_free_houses times
        std::size_t counter = 0;
        while (counter++ < n_free_houses) {
            auto it = free_houses.cbegin();
            //select a random house among the free houses
            //we use advance because free_houses is a set, hence random_access_iterators are not allowed
            std::advance(it, gRandom->Integer(n_free_houses-1));
            //person checks if satisfaction is higher, if so they move there
            if(a_person->get_satisfaction(*it) < current_sat) {
                a_person->moves(*it);
                free_houses.erase(a_person->home);
                free_houses.insert(start_home);
                smt_happened = true;
                //breaks from the free_houses while loop and continue with the next iteration
                break;
            }
        }
    }
    //if it returns false, almost surely the neighborhood became stationary
    clock.Stop();
    if (print_time) {
        std::cout << "A call of iterate function, time elapsed:" << std::endl;
        clock.Print();
    }
    return smt_happened;
}
double Neighborhood::get_tolerance() const { return people.at(0)->get_tolerance(); }
double Neighborhood::get_seg_coeff() const
{
    if (check_if_recolor()) 
        recolor_houses();
    //expanded definition of the Von Neumann segregation coefficient, to consider the case
    //of more than two ethnicities living in the neighborhood
    std::map<char, double> n_ethns;
    auto it = Ethnicity::ethnicities.cbegin();
    for (unsigned n = 0; n < Ethnicity::ethnicities.size(); ++n) {
        double n_ethn = 0;
        for (auto person : people)
            if (person->get_ethn_char() == *it) ++n_ethn;
        n_ethns.insert({*it++, n_ethn});
    }
    double sum = 0.;
    for (auto person : people) {
        //in a completely integrated neighborhood the expected fraction of neighbors of ethncity a should be
        //(total number of a)/(total number of people)
        double expected_fr = n_ethns.at(person->get_ethn_char())/people.size();;
        double real_fr;
        //for calculating the real fraction, the Von Neughamann neighborhood is used
        if (get_total_neigh_vn(person) == 0)
            real_fr = 0;
        //within the four nearest neighbors, the number of person of a certain ethnicity above the total number
        //of occupants of these for houses is real_fr
        else real_fr = get_same_ethn_neigh_vn(person)/(get_total_neigh_vn(person));
        sum += (real_fr-expected_fr)/(1-expected_fr);
    }
    return sum/people.size();
}
double Neighborhood::fraction_happy() const
{ 
    double n_happy = 0; 
    for (auto pers : people) 
        if (pers->get_happyness()) ++n_happy; 
    return n_happy/(density*get_size()); 
}
bool Neighborhood::is_stationary() const
{
    if (check_if_recolor()) 
        recolor_houses();
    for (auto person : people) {
        //don't consider happy people
        if (person->get_happyness()) continue;
        double start_sat = person->get_satisfaction(person->get_home());
        for (auto house : free_houses)
            //for unhappy people, if there exist a house among the free houses, for which our satisfaction
            //is higher than in the present moment, than the Neighborhood is NOT stationary
            if (person->get_satisfaction(house) < start_sat) return false;
    }
    return true;
}
void Neighborhood::set_neighbors_from_vector(Grid::neighbors_grid my_vec) const
{
    std::size_t index = 0;
    for (auto house : Grid::boxes)
        house->neighbors = my_vec.at(index++);
}
void Neighborhood::set_all_neighbors(int distance) const{
    switch (distance) {
        case 2:
            set_neighbors_from_vector(Grid::second_order);
            break;
        case 3:
            set_neighbors_from_vector(Grid::third_order);
            break;
        case 4:
            set_neighbors_from_vector(Grid::fourth_order);
            break;
        case 5:
            set_neighbors_from_vector(Grid::fifth_order);
            break;
        case 6:
            set_neighbors_from_vector(Grid::sixth_order);
            break;
        case 7:
            set_neighbors_from_vector(Grid::seventh_order);
            break;
        default:
            set_neighbors_from_vector(Grid::first_order);
            break;
    }
}
void Neighborhood::recolor_houses() const
{
    TStopwatch clock;
    for (auto person : people)
        person->color_home();
    for (auto house : free_houses)
        house->color = Box::free;
    //also the neighbors should be reset
    set_all_neighbors(neigh_distance);
    clock.Stop();
    if (print_time) {
        std::cout << "A call to recolor_houses, time elapsed:" << std::endl;
        clock.Print();
    }
}
bool Neighborhood::check_if_recolor() const
{
    if (instances_control > 1)
        return true;
    if (just_deleted) {
        just_deleted = false;
        return true;
    }
    return false;
}
void Neighborhood::initialize_grid()
{
    allocate_grid_memory();
    grid_ptr = Grid::boxes.at(0); 
}

//outside-class definition for static data members
Box *Neighborhood::grid_ptr = nullptr;
unsigned Neighborhood::instances_control = 0;
bool Neighborhood::just_deleted = false;
