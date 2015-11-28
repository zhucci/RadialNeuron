#ifndef _NEURAL_H
#define _ENURAL_H

#include<iostream>
#include<fstream>
#include<cstdlib>
#include"point.h"
#include<cmath>
#include<sstream>
#include<string>

class RadialNeuron{

private:
    std::vector<double> C_N;
    double sigm;
    std::vector<Point> theachPoints;
    int dimension;
    const std::string gnufile{"GNUPlotFile.txt"};
    //const std::string gnufile{"gnuplot -persist"};
    std::ofstream ofile;
    FILE *gpipe;


public:
    RadialNeuron(const char *filename, int dim=2);
    ~RadialNeuron();

    double classify(std::vector<double> v);

    void plot_sigm(){
        std::cout<<"Sigma is "<< sigm<<'\n';
    }

    void plot_C(){
        for(int i=0;i<dimension;++i)
            {
	      std::cout<<"x"<<i<<" = "<<C_N[i]<<"\t";
            }
        std::cout<<'\n';
    }

private:

    bool is_ieee_nan(double m){
        if(m==m)
            return false;
        else
            return true;
    }

    int training(std::ifstream& f);
    double actFunc(const std::vector<double>& v,const std::vector<double>& par);

    long double norm(const std::vector<double>& v1,const std::vector<double>& v2);
    double tagFunc(const std::vector<Point>& v,const std::vector<double>& par);

    int optimizeRadialNeuron();

    void writeGNUFile(double time=0.5);
};

#endif //_NEURAL_H
