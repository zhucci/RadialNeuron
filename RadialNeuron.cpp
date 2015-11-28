#include"RadialNeuron.h"

RadialNeuron::RadialNeuron(const char *filename/*=GNUFile.txt*/,int dim/*=2*/)
{

    gpipe = popen("gnuplot -persist","w");
           if(!gpipe)
           {
	     exit(-1);
           }

    dimension=dim;
    for(int i=0;i<dim;++i)
        C_N.push_back(1.0);
        sigm=2;

   ofile.open(this->gnufile.c_str());

   if(!ofile.is_open())
       throw std::string("Can't create/open GNU file");
   std::fprintf(gpipe,"set xrange [0:16]\n");
   std::fprintf(gpipe,"set yrange [0:16]\n");
   if(dim==3)
   std::fprintf(gpipe,"set zrange [0:16]\n");
   std::ifstream infile(filename);

       if(!infile.is_open())
           throw std::string("Can't open InFile");

   training(infile);

   infile.close();


}

double RadialNeuron::classify(std::vector<double> v){
    switch(dimension){
        case 2 :
    std::fprintf(gpipe,"plot '-' using 1:2:3 w circles \n");
    break;
        case 3 :
    std::fprintf(gpipe,"splot '-' u 1:2:3:4 w p pt 7 \n");
    break;
default : break;
}
    typedef unsigned int _int;
for(_int i=0;i!=v.size();++i){
    for(_int j=0;j!=dimension;++j)
    std::fprintf(gpipe,"%6.3f ",v[j]);
std::fprintf(gpipe,"0.02\n");

}

        for(int j=0;j!=dimension;++j)
            std::fprintf(gpipe,"%f ",C_N[j]);

        std::fprintf(gpipe,"%f \n",1.17*sigm);
        std::fprintf(gpipe,"e\n");
        fflush(gpipe);
    std::vector<double> par{C_N};
    par.push_back(sigm);
   return actFunc(v,par);
}
int RadialNeuron::training(std::ifstream& f){
    try{

        std::string str;

        double inData[dimension+1];

        while(std::getline(f,str))
        {
            if(str.size()<5)
                continue;

            switch(dimension){

            case 2:
               sscanf(str.c_str(),"%lf %lf %lf",&inData[0],&inData[1],&inData[2]);

                break;
            case 3:
                sscanf(str.c_str(),"%lf %lf %lf %lf",&inData[0],&inData[1],&inData[2],&inData[3]);

                break;
            default: continue;
            }

            theachPoints.push_back(Point(dimension,inData));

            optimizeRadialNeuron();

            writeGNUFile(0.05);
        }

    return 0;
    }
    catch(std::string string){
        std::cout<<string;
        return -1;
    }
}

int RadialNeuron::optimizeRadialNeuron()
{
    try
    {
            std::vector<double> par;
            for(double c:C_N)
            {
                par.push_back(c);
            }
            par.push_back(sigm);
            double delta_E=10;
            double delta_par=10;
            double delta_E_min=1e-4;
            double E_prev=tagFunc(theachPoints,par);

            std::vector<double> par_plus_lambda{par};

            std::vector<double> par_minus_lambda{par};

            //std::vector<double> par_prev{par};

            std::vector<double> par_new{par};

            std::vector<double> lambda;
            lambda.resize(par.size(),1e-2);
            double increase=1.2;
            double reduce=0.8;

            std::vector<double> b_factor;
            b_factor.resize(par.size(),2.5);
            double b_factor_min=0.5;
            std::vector<bool> prev_dir={true,true,true};
            std::vector<bool> new_dir={true,true,true};

            double delta_par_min=1e-6;

            int success_step_count=0;
            int need_success_step=3;

            int max_iter=200;

        while(--max_iter)
        {
            int count_mis=0;

               for(int i=0;i<par.size();++i)
               {
                    par_plus_lambda=par_new;
                    par_minus_lambda=par_new;
                    par_plus_lambda[i]+=lambda[i];
                    par_minus_lambda[i]-=lambda[i];

                    double E1=tagFunc(theachPoints,par_plus_lambda);
                    double E2=tagFunc(theachPoints,par_minus_lambda);

                    if(E_prev>E1)
                    {
                        E_prev=E1;
                        par_new[i]=par_plus_lambda[i];
                        new_dir[i]=true;
                        //lambda[i]*=increase;
                    }
                    else if(E_prev>E2)
                    {
                        E_prev=E2;
                        par_new[i]=par_minus_lambda[i];
                        new_dir[i]=false;
                       // lambda[i]*=increase;
                    }
                    else
                    {
                        ++count_mis;
                        lambda[i]*=reduce;
                    }

               }

            if(count_mis==par.size())
            {
                par_new=par;
                new_dir=prev_dir;
                continue;
            }

            int max_num_iter=30;
            std::vector<double> next_par;

           while(--max_num_iter)
           {
               next_par.clear();
               next_par=par;
               for(int i=0;i<par_new.size();++i)
               {
                   if(prev_dir[i]!=new_dir[i])

                    b_factor[i]*=reduce;

                   else

                    b_factor[i]*=increase;

                   double n_par=next_par[i]+=b_factor[i]*(par_new[i]-par[i]);
                    ;
               }

               double E_next=tagFunc(theachPoints,next_par);
               //if(E_next<1e-6)
               // goto end_optimize;

               if(E_next<E_prev)
               {
                   break;
               }
               else
               {
                   for(double& b:b_factor)
                        b*=reduce;
                        continue;
               }
                double b_factor_max=0;
                for(auto b:b_factor)
                {
                    b_factor_max=b>b_factor_max?b:b_factor_max;
                }
               if(b_factor_max<b_factor_min)
                    goto end_optimize;
           }

            double next_E=tagFunc(theachPoints,next_par);

            if(next_E> E_prev)
                goto end_optimize;
            if(next_E>1e-10)

            delta_E=std::fabs(next_E-E_prev)/next_E;

            else
                break;
            delta_par=0;
            for(int i=0;i<par.size();++i)
            {
                if(std::fabs(par[i])>1e-10)
                    delta_par+=std::fabs((par[i]-next_par[i])/par[i]);
            }

            if(delta_par<delta_par_min)
                goto end_optimize;

            double s=next_par[dimension];
            if(next_par[dimension]>4)
            {
                next_par[dimension]=3;
                next_E=tagFunc(theachPoints,next_par);
                b_factor[dimension]=0.;
            }



            double Cn[2]={0.,0.};
            for(int i=0;i<2;++i)
                Cn[i]=next_par[i];
            par_new=next_par;
            par=next_par;
            prev_dir=new_dir;
            E_prev=next_E;
        }

    end_optimize:

    sigm=par[dimension];

    for(int i=0;i<dimension;++i)
    {
        C_N[i]=par[i];
    }

    }
    catch(std::string s)
    {
        std::cout<<s<<std::endl;
        return -1;
    }
    return 0;
}
std::vector<double>& operator+(std::vector<double> &v1,
                               std::vector<double> &v2)
{
    auto v3=new std::vector<double>;
    for(int i=0;i<std::min(v1.size(),v2.size());++i)
    {
        v3->push_back(v1[i]+v2[i]);
    }
    return *v3;
}

RadialNeuron::~RadialNeuron()
{
    fflush(gpipe);
}


double RadialNeuron::tagFunc(const std::vector<Point>& points,const std::vector<double>& par)
{
    double func=0.;
        for(auto p:points)
        {

        func+= std::abs(
                    actFunc(p.coordinate,par)-p.NeuronReturnValue
                       );
        }
        return func;
}

double RadialNeuron::actFunc(const std::vector<double> &v,const std::vector<double>& par)
{

    double deg= (-0.5)*norm(v,par)/(std::pow(par[dimension],2));
    double y= std::exp(deg);
   return y;

}
long double RadialNeuron::norm(const std::vector<double>& v1,const std::vector<double>& v2)
{

    long double sum=0;

    for(int i=0;i<dimension;++i)
        sum+=std::pow((v1[i]-v2[i]),2);

    //long double y=std::sqrt(sum);

        if(is_ieee_nan(sum))
            return 0.;

            if(sum<1e-15)
                return 0;
    return sum;
}
void RadialNeuron::writeGNUFile(double time)
{
    switch(dimension){
        case 2 :
            std::fprintf(gpipe,"plot '-' using 1:2:3:($4) w circles lc var \n");
            break;
        case 3 :
            std::fprintf(gpipe,"splot '-' u 1:2:3:4 w p pt 7 \n");
            break;
    default : break;
    }

    for(int i=0;i!=theachPoints.size();++i){
            for(int j=0;j!=dimension;j++)
            std::fprintf(gpipe,"%f ",theachPoints[i].coordinate[j]);
        std::fprintf(gpipe,"0.02 ");
        if(theachPoints[i].NeuronReturnValue>0.5)
            std::fprintf(gpipe,"1\n");
        else
            std::fprintf(gpipe,"2\n");
    }

    for(int j=0;j!=dimension;j++)
    std::fprintf(gpipe,"%f ",C_N[j]);
    std::fprintf(gpipe,"%f ",1.17741*sigm);
    std::fprintf(gpipe,"3\n");
    std::fprintf(gpipe,"e\n");
    int m=theachPoints.size();
    if(theachPoints[m-1].NeuronReturnValue>0.5)
    std::fprintf(gpipe,"pause %f\n",time);
    else
    std::fprintf(gpipe,"pause %f\n",time/5.);
    fflush(gpipe);
}
