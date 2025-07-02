#ifndef UTILITY_H
#define UTILITY_H

#include "Grid.h"
#include "Box.h"
#include <iostream>
#include <cmath> //for std::sqrt function

//a file containing functions used in various parts of the program

//utility functions to get size of the grid
inline constexpr std::size_t get_cols() { return Grid::n_cols; }
inline constexpr std::size_t get_rows() { return Grid::n_rows; }
inline constexpr std::size_t get_size() { return Grid::grid_size; }
//distance between any two points
inline double two_pnt_dist(double x_first, double y_first, double x_second, double y_second)
    { return std::sqrt((x_first-x_second)*(x_first-x_second)+(y_first-y_second)*(y_first-y_second)); }
//get position of box on the grid array from the coordinates
inline std::size_t get_position(Box* a_box) //friend of grid
{   
    const unsigned &row_crd = a_box->get_row_coord();
    const unsigned &col_crd = a_box->get_col_coord();
    return (row_crd-1)*Grid::n_cols + col_crd-1;
}
//print ethnicity with given ANSI color code
inline void print_that_ethn(const char &a_ethn, const std::string &color_code) //print char in bold with given color code, used in print_neighbor
{
    std::cout << "\033[" << color_code << ";1m" << a_ethn << "\033[0m";
}


#endif