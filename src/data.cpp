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

pair::pair()
{
}

pair::~pair()
{
}

////////////////////////////////////////////////////////
data::data(const std::string &parameterfile)
    : parameter(parameterfile)
{
    ps = new particles[2]{N,N};
    t_0=0;
    t_1=1;
    N_pairs=0.5*(N*N-N);
    pairs = new unsigned int*[N_pairs];
    for(unsigned int loop=0;loop<N_pairs;++loop)
    {
        pairs[loop] = new unsigned int[2];
    }
    unsigned int loopp;
    for(int loopo=1;loopo<N;++loopo)
    {
        for(int loopi=0;loopi<loopo;++loopi)
        {
            loopp=0.5*(pow(loopo+1,2)-loopo-1)-(loopo-loopi);
            pairs[loopp][0]=loopo;
            pairs[loopp][1]=loopi;
        }
    }
}

data::~data()
{
    delete [] ps;
    for(unsigned int loop=0;loop<N_pairs;++loop)
    {
        delete [] pairs[loop];
    }
    delete [] pairs;
}

//////////////
std::ostream& operator<<( std::ostream& s, const pair &p)
{
    s<<p.x<<","<<p.y;
    return s;
}
