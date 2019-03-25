/*******************************************************************************
 * This file is part of DENISE.
 * Copyright (c) 2019 Moritz Fischer (moritz.s.fischer@eclipso.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

parameter::parameter(const std::string& filename)
{
    ////
    G=1.0;
    ////
    directory="";
    directory_data=directory+"data/";
    make_dir("data");
    directory_plots=directory+"plots/";
    make_dir("plots");
    N = 10;
    total_mass=1;
    radius=10;
    alpha=5.0/3.0;
    dt = 0.01;
    fact_dt=0.1;
    smoothing_length=1;
    t=0;
    num_out=6;
    ic_number=0;
    N_threads=2;
    do_replot=false;
    read_parameter(filename);
    times_out = new float[num_out];
    float dt_out;
    if(num_out>1)
        dt_out=t_final/(num_out-1);
    else
        dt_out=1.0;
    for(short loop=0;loop<num_out;++loop)
    {
        times_out[loop]=dt_out*loop;
    }
    smoothing_length2=smoothing_length*smoothing_length;
    //check that N_threads is 2^n with n element of natural numbers;
    N_threads=smaller_2n(N_threads);
    //ensure suitable number of paritcles
    N=ceil((double)N/(2*N_threads))*2*N_threads;
    display_parameter();
}

parameter::~parameter()
{
    delete [] times_out;
}

void parameter::read_parameter(const std::string& filename)
{
    std::ifstream dat_in;
    dat_in.open(filename.c_str(),std::ios_base::in);
    if(!dat_in)
    {
        dat_in.open(("../"+filename).c_str(),std::ios_base::in);
        if(!dat_in)
        {
            std::cout<<"Error: not able to open parameter file ("<<filename<<")"<<std::endl;
            std::cout<<std::endl;
            throw std::exception();
        }
    }
    
    {
        std::string readin;
        dat_in>>readin;
        while(!dat_in.eof())
        {
            if(readin=="Num_particles")
            {
                dat_in>>N;
            }
            else if(readin=="TotalMass")
            {
                dat_in>>total_mass;
            }
            else if(readin=="Num_NeutronStars")
            {
                dat_in>>N_NS;
            }
            else if(readin=="dt_factor")
            {
                dat_in>>fact_dt;
            }
            else if(readin=="N_output")
            {
                dat_in>>num_out;
            }
            else if(readin=="t_final")
            {
                dat_in>>t_final;
            }
            else if(readin=="smoothing_length")
            {
                dat_in>>smoothing_length;
            }
            else if(readin=="InitialCondition")
            {
                dat_in>>ic_number;
            }
            else if(readin=="Radius")
            {
                dat_in>>radius;
            }
            else if(readin=="alpha")
            {
                dat_in>>alpha;
            }
            else if(readin=="N_threads")
            {
                dat_in>>N_threads;
            }
            else if(readin=="replot")
            {
                dat_in>>readin;
                if(readin=="true")
                    do_replot=true;
                else if(readin=="false")
                    do_replot=false;
            }
            else if(readin=="G")
            {
                dat_in>>G;
            }
            dat_in>>readin;
        }
    }
    dat_in.close();
}

void parameter::display_parameter()
{
std::cout<<"  Parameter:  "<<"N_particle = "<<N<<"\t\t"<<"M_total = "<<total_mass<<std::endl;
std::cout<<"              "<<"initial conditions "<<ic_number<<"\t"<<"Radius = "<<radius<<std::endl;
std::cout<<"              "<<"t_final = "<<t_final<<"  \t\t"<<"N_output = "<<num_out<<std::endl;
std::cout<<"              "<<"smoothing len = "<<smoothing_length<<"  \t"<<"dt_factor = "<<fact_dt<<std::endl;
std::cout<<"              "<<"N_threads = "<<N_threads<<"\t\t"<<"replot = "<<do_replot<<std::endl;
}