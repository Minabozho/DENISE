/*******************************************************************************
 * This file is part of DENISE.
 * Copyright (c) 2019 Moritz Fischer (mail@moritzfischer.world)
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

template<typename NTYPE>
std::string convert_to_string(NTYPE number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

template<typename DTYPE>
double vector_length(DTYPE &vec)
{
    return sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
}

std::string add_leading_zeros(short number)
{
    std::string result=convert_to_string(number);
    short naz;
    if(number!=0)
        naz=3-trunc(log10(number));
    else
        naz=3;
    for(short loop=0; loop<naz;++loop)
    {
        result="0"+result;
    }
    return result;
}

unsigned int smaller_2n(unsigned int number)
{
    return pow(2,trunc(log2(number)));
}

void make_dir(const std::string &name)
{
    std::string command="mkdir -p "+name;
    popen(command.c_str(),"w");
    sleep(1); //because it takes a while until the directory is created 
}
