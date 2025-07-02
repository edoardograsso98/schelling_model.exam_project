#include "Ethnicity.h"
#include <TRandom3.h>
#include <map>

//outside class definition of static members
//warning: the std::set default orders the element in alphabetical order, indipendently of the order in which we list them
std::set<char> Ethnicity::ethnicities = {'b','w'};
//definition of color_map which is used in the print_neigh function
std::map<char,std::string> color_map = {{'b', "31"}, {'w', "34"}};

void Ethnicity::add_ethn(const char &my_ethn, const std::string &color)
{   if (!ethnicities.insert(my_ethn).second) { //insert returns a pair which second element is a
                                               //bool indicating whether the element was inserted or not
        std::cout << "That ethnicity was already present" << std::endl; 
        return;  
    }
    color_map.insert({my_ethn, color});
}
void Ethnicity::remove_ethn(const char &my_ethn)
    {   if (ethnicities.size() == 1) { std::cout 
                << "you can't remove another ethnicity" << std::endl;
                return; }
        if (!ethnicities.erase(my_ethn)) { std::cout  //erase returns 0 if the element was not found
                << "that ethnicity is not in the set" << std::endl; 
                return; }
        color_map.erase(my_ethn);
    }
//default constructor
Ethnicity::Ethnicity()
{
    do {
        //generate random number between 0 and 1
        double rand = gRandom->Rndm();
        //step is defined such that step*(num of ethnicities) = 1
        double step = 1./ethnicities.size();
        double bound = step;
        for (auto elem : ethnicities) {
            //evaluate the condition while at each step pushing the bound closer to 1
            if (rand <= bound) {
                ethn = elem;
                break;
            }
            bound += step;
        }
    } while (!ethn); //checks wether the assignment of an ethnicity was successfull,
                     //it should fail only due to very unfortunate approximations
}
//standard constructor
Ethnicity::Ethnicity(const char &my_ethn): ethn(my_ethn)
{ }
//overloaded output operator
std::ostream& operator<<(std::ostream &os, const Ethnicity &item) 
    { os << item.ethn; return os; }