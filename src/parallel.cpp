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

pair_decomp::pair_decomp(unsigned int T_in, unsigned int B_in, unsigned int u_in)
{
    T=T_in;
    B=B_in;
    u=u_in;
    l=log2(T);
    T_s=T;
}

pair_decomp::~pair_decomp()
{

}

void pair_decomp::calc_index(unsigned int t_in, unsigned int b_in)
{
    x=0;
    y=0;
    t=t_in;
    b=b_in;

    sum();
}

void pair_decomp::sum()
{
    for(r=1;r<=l;++r)
    {
        calc_T_s();
        calc_t_s();
        calc_b_s();
        q_x();
        q_y();
    }
}

void pair_decomp::q_x()
{
    x+=u*T_s*(b_s*t_s+(1-b_s)*(1-t_s));
}

void pair_decomp::q_y()
{
    y+=u*T_s*t_s;
}

void pair_decomp::calc_t_s()
{
    t_s=(unsigned int)floor(t/T_s)%2;
}

void pair_decomp::calc_b_s()
{
    b_s=(unsigned int)floor((b-1)/T_s)%2;
}

void pair_decomp::calc_T_s()
{
    T_s=pow(2,l-r);
}

///////////////////////////////////////////////////////
domain::domain()
{
    
}

void domain::initialize(unsigned int N_pairs_in)
{
    N_pairs=N_pairs_in;
    a = new pair[N_pairs];
}

domain::~domain()
{
    delete [] a;
}

void domain::triangle(unsigned int x_first, unsigned int y_first)
{
    unsigned int loopp=0;
    for(unsigned int loopx=x_first;loopp<N_pairs;++loopx)
    {
        for(unsigned int loopy=y_first;loopy<y_first+(loopx-x_first)+1;++loopy)
        {
            a[loopp].x=loopx;
            a[loopp].y=loopy;
            ++loopp;
        }
    }
}

void domain::square(unsigned int x_first, unsigned int y_first)
{
    unsigned int length=sqrt(N_pairs);
    unsigned int loopp=0;
    for(unsigned int loopx=x_first;loopx<x_first+length;++loopx)
    {
        for(unsigned int loopy=y_first;loopy<y_first+length;++loopy)
        {
            a[loopp].x=loopx;
            a[loopp].y=loopy;
            ++loopp;
        }
    }
}

pair &domain::operator [](const unsigned int i)
{
    return a[i];
}

std::ostream& operator<<( std::ostream& s, domain &d)
{
    s<<d[0];
    for(unsigned int loop=1;loop<d.N_pairs;++loop)
    {
        s<<" "<<d[loop];
    }
    return s;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

parallel_data::parallel_data()
{

}

void parallel_data::initialize(short N_threads_in, unsigned int N)
{
    N_threads=N_threads_in;
    unsigned int NDD=N/N_threads;
    short B=2*N_threads-1; //Anzahl an Bereichen, die hintereinander ausgeführt werden müssen
    unsigned int pairs_pD=0.5*NDD*(NDD-1);    //Paare pro Dreieck
    unsigned int pairs_pQ=pow(0.5*NDD,2);      //Paare pro Quadrat
    a = new domain*[N_threads];

    for(short loopT=0;loopT<N_threads;++loopT)
    {
        a[loopT] = new domain[B];
        a[loopT][0].initialize(pairs_pD);
        for(unsigned int loopB=1;loopB<B;++loopB)
        {
            a[loopT][loopB].initialize(pairs_pQ);
        }
    }

    unsigned int u=0.5*NDD;
  
    unsigned int loopT_r;
    pair_decomp pd(2*(unsigned int)N_threads,B,u);
    for(short loopT=0;loopT<N_threads*2;++loopT)
    {
        //triangles
        if(loopT<N_threads)
            a[loopT][0].triangle(NDD*loopT+1,NDD*loopT);
        //squares
        for(unsigned int loopB=1;loopB<B;++loopB)
        {
            pd.calc_index(loopT,loopB);
            if(pd.x>pd.y) 
            {
                if(loopT>=N_threads) //these require a different thread number
                    loopT_r=2*N_threads-loopT-1;
                else
                    loopT_r=loopT;
                a[loopT_r][loopB].square(pd.x,pd.y);
            }
        }
    }
}

parallel_data::~parallel_data()
{
    for(short loop=1;loop<N_threads;++loop)
    {
        delete [] a[loop];
    }
    delete [] a;
}


domain* &parallel_data::operator [](const unsigned int i)
{
    return a[i];
}
