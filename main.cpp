#include"main.h"

int main(int argc, char **argv){

  printf("Radial Neuron trainer\n\n"
	 "Input arguments [dimention] [optionaly: teacher's file name]\n"
	 "Write 'e' to pass classify mode\n\n");
  assert(argc>1);
try
    {

      int dim=atoi(argv[1]);
      const char *file= argc==3?argv[2]:"Dots2D.txt";

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

	int n =std::sscanf(str.c_str(),"%lf %lf",&x[0],&x[1]);

        std::cout<<"classify >";

        if(n<2)
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
