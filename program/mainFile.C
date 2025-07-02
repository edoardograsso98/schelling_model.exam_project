#include "Neighborhood.h"

#include "TH1D.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TStopwatch.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <fstream>

constexpr int max_it = 10; //set to 10
constexpr int n_exp = 30; //set to 30
constexpr char file_name_0_5[] = "data_pts_0_5.txt";
constexpr char file_name_0_7[] = "data_pts_0_7.txt";

//utility function to print the data files on the standard output
std::ostream& print_line(std::ostream &os, double x , double y, double ey);
//given a neighborhood with certain range and tolerance, makes attempts on making it reach stability, it default stops
//after 10 attempts (10*10000 iterations) and returns the segregation coefficient. If verbose is set to 1, prints out
//on the default output if the neighborhood has reached stability and, if not, prints the fraction of happy people
//if print_neighborhood = 1 prints on the terminal the people disposition after the iterations 
double launch_experiment(Neighborhood &my_neigh, int max_iterations = max_it, bool verbose = 0, bool print_neighborhood = 0);
//create a histogram for a default of thirty repeated experiments
TH1D* create_hist(Neighborhood &my_neigh, int n_of_experiments = n_exp, int max_iterations = max_it);
//creates 14 histogram, one for each possible combination of range and tolerance among the selected ones, also creates the
//txt files with data points containing all the segragation coefficients for every value of range and for both values of tolerance
TH1D** get_all_data();
//executes the macro, that is, creates all the histograms, achieves the data (i.e. the segregation coefficient with relative error) for
//all the values of range and vision and creates two TGraphErrors that will hopefully resemble the graph from the paper we're trying to recreate
//if return_histos is set to True it also returns the pointer to the heap allocated histograms, if it is set to False it deletes the memory
//allocated by them and returns nullptr
TH1D** launch_macro(bool return_histos = 0);

double launch_experiment(Neighborhood &my_neigh, int max_iterations, bool verbose, bool print_neighborhood)
{
    if (max_iterations <= 0) throw std::range_error("INVALID VALUE FOR MAX_ITERATIONS");
    int range = my_neigh.get_range();
    double tolerance = my_neigh.get_tolerance();
    //re-initializes given neighborhood with the same values
    my_neigh = Neighborhood(range, tolerance);
    int counter = 0;
    //it is not always guaranteed that stability is achievable, stops after 10 iterations
    while (++counter <= max_iterations) {
        my_neigh.iterate(); //each call of iterate() tries to move 10000 people
        if (my_neigh.is_stationary())
            break;
    }
    if (print_neighborhood)
        my_neigh.print_neigh();
    double seg_coeff = my_neigh.get_seg_coeff();
    if (!verbose)
        return seg_coeff;
    //if verbose:
    if (counter > max_iterations) {
        std::cout << "Didn't reach stability" << std::endl
                  << "fraction of happy people: " << my_neigh.fraction_happy() 
                  << std::endl;
        return seg_coeff;
    }
    //if counter <= max_iterations:
    std::cout << "Reached stability in " << counter << " iterations" <<  std::endl;
    return seg_coeff;
}

TH1D* create_hist(Neighborhood &my_neigh, int n_of_experiments, int max_iterations)
{
    if (max_iterations <= 0) throw std::range_error("INVALID VALUE FOR MAX_ITERATIONS");
    if (n_of_experiments <= 0) throw std::range_error("INVALID VALUE FOR N_OF_EXPERIMENTS");
    //static variable to keep track of how many histogram we create and to differenciate their names
    static int name_num = 1;
    //stringstreams are useful to dinamically write names
    std::stringstream name_s;
    std::stringstream title_s;
    name_s << "hist" << name_num++;
    title_s << "range: " << my_neigh.get_range() << " - tolerance: " << my_neigh.get_tolerance();
    //the .c_str() member returns a const char* object from a string
    TH1D *hist = new TH1D(name_s.str().c_str(), title_s.str().c_str(), 50, -1., 1.);
    hist->GetXaxis()->SetTitle("Segregation Coefficient");
    hist->GetYaxis()->SetTitle("Number of Entries");
    while (n_of_experiments-- != 0)
        hist->Fill(launch_experiment(my_neigh, max_iterations));
    return hist;
}

TH1D** get_all_data()
{
    //to writa data into extern files
    std::ofstream out_stream1, out_stream2;
    out_stream1.open(file_name_0_5);
    out_stream2.open(file_name_0_7);
    //creates default neighborhood, initializes grid if it's the first instance of Neighborhood
    Neighborhood neigh;
    //allocate an array of 14 pointers to TH1D objects
    TH1D* *hists = new TH1D*[14];
    std::size_t index = 0;
    int ranges[7] = {1, 2, 3, 4, 5, 6, 7};
    double tolerances[2] = {0.5, 0.7};
    for (auto tol : tolerances)
        for (auto range : ranges) {
            neigh = Neighborhood(range, tol);
            hists[index] = create_hist(neigh);
            //write data into the proper file
            if (tol == 0.5) {
                out_stream1 << range << " ";
                out_stream1 << hists[index]->GetMean() << " ";
                out_stream1 << 0 << " ";
                out_stream1 << hists[index++]->GetStdDev() << std::endl;
            }
            else if (tol == 0.7) {
                out_stream2 << range << " ";
                out_stream2 << hists[index]->GetMean() << " ";
                out_stream2 << 0 << " ";
                out_stream2 << hists[index++]->GetStdDev() << std::endl;
            }
        }
    out_stream1.close();
    out_stream2.close();
    return hists;
}

TH1D** launch_macro(bool return_histos)
{
    TStopwatch clock;
    //write the data files and save the memory addresses of the histograms
    auto histos = get_all_data();
    //if there's no need of returning them, delete the memory in which they're allocated
    if (!return_histos) {
        for (int i = 0; i < 14; ++i)
            delete histos[i];
        //also the pointers to histos were dinamically allocated
        delete histos;
    }
    //creates graphs and multigraph
    TGraphErrors *gr1 = new TGraphErrors();
    TGraphErrors *gr2 = new TGraphErrors();
    TMultiGraph *mg = new TMultiGraph();
    //manage input from extern files
    std::ifstream in_stream1, in_stream2;
    in_stream1.open(file_name_0_5);
    in_stream2.open(file_name_0_7);
    //initialize variables that will contain the data stored in the files
    double x(0.), y(0.), ex(0.), ey(0.);
    int n(0);
    std::cout << "---------------------------------------------------------\n" 
              << "SEGREGATION COEFFICIENT RESULTS:\n\nTOLERANCE = 0.5\nR:\tS:" << std::endl;
    //build graph for first file
    while (in_stream1 >> x >> y >> ex >> ey) {
        //get the point number we're about to insert in the graph
        n = gr1->GetN();
        //set point and point error
        gr1->SetPoint(n, x, y);
        gr1->SetPointError(n, ex, ey);
        print_line(std::cout, x, y, ey) << std::endl;
    }
    //build graph for second file
    std::cout << "\nTOLERANCE = 0.7\nR:\tS:" << std::endl;
    while (in_stream2 >> x >> y >> ex >> ey) {
        n = gr2->GetN();
        gr2->SetPoint(n, x, y);
        gr2->SetPointError(n, ex, ey);
        print_line(std::cout, x, y, ey) << std::endl;
    }
    in_stream1.close();
    in_stream2.close();
    //------------------------------------------------------------------------------------
    //BUILD THE GRAPH
    TCanvas *c = new TCanvas();
    gr1->SetMarkerStyle(20);
    gr1->SetMarkerColor(6);
    gr1->SetTitle("tol=0.5");
    gr1->GetXaxis()->SetRangeUser(0.5, 7.5);
    gr1->GetYaxis()->SetRangeUser(-0.1, 1.1);
    gr2->SetMarkerStyle(21);
    gr2->SetMarkerColor(4);
    gr2->SetTitle("tol=0.7");
    gr2->GetXaxis()->SetRangeUser(0.5, 7.5);
    gr2->GetYaxis()->SetRangeUser(-0.1, 1.1);
    mg->Add(gr1);
    mg->Add(gr2);
    mg->SetTitle("Segregation Results");
    mg->GetXaxis()->SetTitle("Range of Vision (R)");
    mg->GetYaxis()->SetTitle("Segregation Coefficient (S)");
    mg->Draw("AP");
    c->BuildLegend(0.75, 0.45, 0.9, 0.55);
    c->SetGrid();
    //------------------------------------------------------------------------------------

    clock.Stop();
    clock.Print();
    if (return_histos)
        return histos;
    return nullptr;
}

//utility funcition definition
std::ostream& print_line(std::ostream &os, double x , double y, double ey)
{
    os << x << "\t"; printf("%.2f", y);
    os << " +/- "; printf("%.2f", ey);
    return os;
}