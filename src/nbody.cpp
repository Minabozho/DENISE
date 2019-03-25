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

nbody::nbody(const std::string &parameterfile)
    : hermite(parameterfile), initial_cond(parameterfile), data(parameterfile)
{
}

nbody::~nbody()
{
}

void nbody::set_IC()
{
    set_initial_cond();
}


void nbody::simulate()
{
    int nout=1;

    write_output(times_out[0]);

    while(t<t_final)
    {
        t_0=t_1;
        t_1=(t_0+1)%2;

        integrate_hermite();
        t+=dt;

        if(nout<num_out)
        {
            if(t>=times_out[nout]-0.5*dt)
            {
                write_output(nout);
                ++nout;
            }
        }
    }
}

void nbody::do_plot()
{
    std::string gp_script="gp_script.txt";
    std::string dateinameout,dateiname_plot;
    dateinameout=directory+gp_script;
    dateiname_plot="test";
    std::ofstream dat_out;
    dat_out.open(dateinameout.c_str(),std::ios_base::out);
    if(!dat_out)
    {
        std::cout<<"Error: not able to write script for gnuplot ("<<gp_script<<")"<<std::endl;
        std::cout<<std::endl;
    }
    else
    {
        std::string title_S, str_nout;
        title_S="Stars";

        dat_out<<"reset"<<std::endl;
        //dat_out<<"set terminal postscript landscape enhanced color dashed lw 1 \"Helvetica\" 20"<<std::endl;
        //dat_out<<"set terminal jpeg size 580,435 enhanced font \"Helvetica\" 13"<<std::endl;
        dat_out<<"set terminal jpeg size 640,480 enhanced font \"Helvetica\" 14"<<std::endl;
        //dat_out<<"set terminal jpeg size 960,720 enhanced font \"Helvetica\" 24"<<std::endl;
        //dat_out<<"set terminal jpeg size 1152,864 enhanced font \"Helvetica\" 24"<<std::endl;
        dat_out<<"set encoding iso_8859_1"<<std::endl;

        dat_out<<"set xrange[-10:10]"<<std::endl;
        dat_out<<"set yrange[-10:10]"<<std::endl;
        dat_out<<"set zrange[-10:10]"<<std::endl;
        for(short nout=0;nout<num_out;++nout)
        {
            str_nout=add_leading_zeros(nout);
            dat_out<<"set title \""<<"Particle Cluster   t="<<times_out[nout]<<"             "<<std::endl;
            dat_out<<"set output \""+directory_plots+dateiname_plot<<"_"<<nout<<".jpeg"<<std::endl;
            dat_out<<"splot ";

            dat_out<<"\""<<directory_data<<"output_"<<str_nout<<".txt\" using ($2):($3):($4) title \""<<title_S<<"\" with points lt 2 lw 2 lc 1 ,\\"<<std::endl;
            dat_out<<std::endl;
        }
    }
    dat_out.close();


    std::string command="gnuplot "+directory+gp_script;

    popen(command.c_str(),"w");
}

bool nbody::replot()
{
    return do_replot;
}

void nbody::write_output(const short& nout)
{
    std::cout<<"   output t="<<times_out[nout]<<std::endl;
    ps[t_1].write_output(directory_data+"output_"+add_leading_zeros(nout)+".txt");
    //std::cout<<"momentum="<<calc_total_momentum(ps[t_1])<<std::endl;
    //std::cout<<"velocity="<<calc_cluster_velocity(ps[t_1])<<std::endl;
    //std::cout<<"kinetic energy="<<calc_kinetic_energy(*this)<<std::endl;
    //std::cout<<"potential energy="<<calc_potential_energy(*this)<<std::endl;
    //std::cout<<"   total energy="<<calc_total_energy(*this)<<std::endl;
}