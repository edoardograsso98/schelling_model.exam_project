#include "Box.h"
#include <stdexcept>
//needed for the two_pnt_dist function and the grid dimensions
#include "Utilities.h"

//mandatory definition of pure virtual destructor
Box::~Box() { }
//overloading binary minus operator, returns the distance, since the geometry of the grid is thoroidal, the definition
//of the distance is much more complicated than one would expect
double operator-(const Box &lhs, const Box &rhs) 
{
    if (lhs.col_coord == rhs.col_coord && lhs.row_coord == rhs.row_coord)
        return 0.;
    //on a thorus there are many ways to draw straight lines connecting two points, the distance will be the lenght of the shortest
    //of those lines. We store the distances on a multiset that allows us to store the same-valued key more than once (was useful for debugging)
    //and automatically store them in ascending order.
    std::multiset<double> distances;
    double dist1(0.), dist2(0.), dist3(0.), dist4(0.);
    //the first distance is just the path connecting the point as if they were on a flat bounded grid
    dist1 = two_pnt_dist(lhs.row_coord, lhs.col_coord, rhs.row_coord,rhs.col_coord);
    distances.insert(dist1);
    //if the points are allinged with a circle of latitude or longitude there are just two ways to go from one point to another
    if (lhs.row_coord == rhs.row_coord) {
        (lhs.col_coord < rhs.col_coord) ? dist2 = two_pnt_dist(lhs.row_coord, lhs.col_coord + get_cols(), rhs.row_coord, rhs.col_coord) :
                    dist2 = two_pnt_dist(lhs.row_coord, lhs.col_coord, rhs.row_coord, rhs.col_coord + get_cols());
        distances.insert(dist2);
    }
    else if (lhs.col_coord == rhs.col_coord) {
        (lhs.row_coord < rhs.row_coord) ? dist2 = two_pnt_dist(lhs.row_coord + get_rows(), lhs.col_coord, rhs.row_coord, rhs.col_coord) :
                    dist2 = two_pnt_dist(lhs.row_coord, lhs.col_coord, rhs.row_coord + get_rows(), rhs.col_coord);
        distances.insert(dist2);
    }
    //in every other case there are four ways
    else {
        if (lhs.row_coord < rhs.row_coord && lhs.col_coord < rhs.col_coord) {
            dist2 = two_pnt_dist(lhs.row_coord + get_rows(), lhs.col_coord + get_cols(), rhs.row_coord,rhs.col_coord);
            dist3 = two_pnt_dist(lhs.row_coord, lhs.col_coord + get_cols(), rhs.row_coord,rhs.col_coord);
            dist4 = two_pnt_dist(lhs.row_coord + get_rows(), lhs.col_coord, rhs.row_coord,rhs.col_coord);
        }
        else if (lhs.row_coord < rhs.row_coord && rhs.col_coord < lhs.col_coord) {
            dist2 = two_pnt_dist(lhs.row_coord + get_rows(), lhs.col_coord, rhs.row_coord,rhs.col_coord + get_cols());
            dist3 = two_pnt_dist(lhs.row_coord, lhs.col_coord, rhs.row_coord,rhs.col_coord + get_cols());
            dist4 = two_pnt_dist(lhs.row_coord + get_rows(), lhs.col_coord, rhs.row_coord,rhs.col_coord);
        }
        else if (rhs.row_coord < lhs.row_coord && lhs.col_coord < rhs.col_coord) {
            dist2 = two_pnt_dist(lhs.row_coord, lhs.col_coord + get_cols(), rhs.row_coord + get_rows(),rhs.col_coord);
            dist3 = two_pnt_dist(lhs.row_coord, lhs.col_coord, rhs.row_coord + get_rows(),rhs.col_coord);
            dist4 = two_pnt_dist(lhs.row_coord, lhs.col_coord + get_cols(), rhs.row_coord,rhs.col_coord);
        }
        else {
            dist2 = two_pnt_dist(lhs.row_coord, lhs.col_coord, rhs.row_coord + get_rows(),rhs.col_coord + get_cols());
            dist3 = two_pnt_dist(lhs.row_coord, lhs.col_coord, rhs.row_coord + get_rows(), rhs.col_coord);
            dist4 = two_pnt_dist(lhs.row_coord, lhs.col_coord, rhs.row_coord,rhs.col_coord + get_cols());
        }
        distances.insert(dist2);
        distances.insert(dist3);
        distances.insert(dist4);
    }
    //returns the first element in the set, that is the shortest
    return *distances.cbegin();
}
//resets the colors to those of the current neighbors
void Box::set_neigh_colors() const
        { neigh_colors.clear(); 
    for (auto box : neighbors) if (box->color != free) neigh_colors.push_back(box->color); }
//returns a vector of chars containing at most 4 items, that are the von neughmann neighbors
std::vector<char> Box::get_vn_neigh_colors() const
{
    set_neigh_colors();
    std::vector<char> vn_colors;
    for (auto elem : neighbors) {
        if (*elem - *this > 1.)
            continue;
        if (elem->color != Box::free)
            vn_colors.push_back(elem->color);
    }
    return vn_colors;
}
//the utility function used in the derived classes
void throw_except(const std::string &msg)
    { throw std::range_error("No box on the " + msg); }

//static member definition
constexpr char Box::free;