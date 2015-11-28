#include"main.h"

int main(void){

try
    {

    int dim=2;
    char *file="Dots2D.txt";

    RadialNeuron rn(file,dim);

    rn.plot_C();
    rn.plot_sigm();


    std::vector<double> x;
    x.resize(dim,0.0);
    std::string str;
    std::cout<<"classify >";

    while(std::getline(std::cin,str))
    {
        if(str==std::string("e"))
           break;

             std::sscanf(str.c_str(),"%lf %lf",&x[0],&x[1]);

        std::cout<<"classify >";
        if(str.size()<3)
                continue;
        rn.classify(x);
    }

    return 0;
}
catch(std::string m)
    {

    std::cout<<m;
    return -1;

    }
}
