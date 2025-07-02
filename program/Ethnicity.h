#ifndef ETHNICITY_H
#define ETHNICITY_H

#include <set>
#include <iostream>
#include <string>

//the color code for white, it is used to provide default color code to the add_ethn function
constexpr char white_color[] = "0";

class Ethnicity {
friend std::ostream& operator<<(std::ostream&, const Ethnicity&);
public:
    //the default constructor uses a random number generator to generate a
    //random ethnicity among the static set of ethnicities
    Ethnicity();
    Ethnicity(const char&);
    //static member to add ethnicities to the set
    static void add_ethn(const char&, const std::string& = white_color);
    //static member to remove ethnicities from the set
    static void remove_ethn(const char&);
    char get_char() const { return ethn; }
    static std::set<char> ethnicities;
private:
    //in-class initialized as the null charachter, which is converted to boolean as false
    //when we create an Ethnicity object, the char is randomly picked among the set, this
    //is done at run time
    char ethn = '\0';
};
//print an ethnicity (prints the char)
std::ostream& operator<<(std::ostream &os, const Ethnicity &item);

#endif