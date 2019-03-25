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

particles::particles(int N_in)
{
    p = new particle[N_in];
    N = N_in;
}

particles::~particles()
{
    delete [] p;
}


void particles::write_output(const std::string& dateiname)
{
    std::ofstream dat_out;
    dat_out.open(dateiname.c_str(),std::ios_base::out);
    if(!dat_out)
    {
        std::cout<<"Error: not able to write output("<<dateiname<<")"<<std::endl;
        std::cout<<std::endl;
    }

    if(dat_out)
    {
        dat_out<<"# id"<<"\t"<<"x_1"<<"\t"<<"x_2"<<"\t"<<"x_3"<<"\t"<<"v_1"<<"\t"<<"v_2"<<"\t";
        dat_out<<"v_3"<<"\t"<<"m"<<std::endl;
        for(int loop=0;loop<N;++loop)
        {
            dat_out<<p[loop]<<std::endl;
        }
    }
    dat_out.close(); 
}