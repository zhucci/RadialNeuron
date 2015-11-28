#ifndef POINT_H
#define POINT_H
#include<vector>

struct Point
{
public:
    Point(int dim,double *data){
        for(int i=0;i<dim;++i)
        coordinate.push_back(data[i]);
        NeuronReturnValue=data[dim];
    }

  std::vector<double> coordinate;
  double NeuronReturnValue;
};

#endif // POINT_H
