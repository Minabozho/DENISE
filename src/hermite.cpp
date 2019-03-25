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

#define parallel true

hermite::hermite(const std::string &parameterfile)
    : data(parameterfile)
{ 
    ppair_data.initialize(N_threads,N);

    for(short loopt=0;loopt<2;++loopt)
    {
        a[loopt] = new dtype*[N];
        adot[loopt] = new dtype*[N];
        for(int loop=0;loop<N;++loop)
        {
            a[loopt][loop] = new dtype[3];
            adot[loopt][loop] = new dtype[3];
        }
    }

    hermite_thread_id = new pthread_t[N_threads];
    hermite_threadarg = new thread_data[N_threads];
    for(short loop=0;loop<N_threads;++loop)
    {
        hermite_threadarg[loop].thread_id=loop;
        hermite_threadarg[loop].SELF=this;
        hermite_thread_id[loop]=loop;
    }
}

hermite::~hermite()
{
    for(short loopt=0; loopt<2; ++loopt)
    {
        for(int loop=0;loop<N;++loop)
        {
            delete [] a[loopt][loop];
            delete [] adot[loopt][loop];
        }
        delete [] a[loopt];
        delete [] adot[loopt];
    }

    delete [] hermite_threadarg;
    delete [] hermite_thread_id;
}

void hermite::set_zero(short loopt)
{
    for(int loop=0;loop<N;++loop)
    {
        for(short loopc=0;loopc<3;++loopc)
        {
            a[loopt][loop][loopc]=0;
            adot[loopt][loop][loopc]=0;
        }
    }
}
void hermite::calc_a_adot_dG(short loopt, int loopo, int loopi) //for one pair
{
    dtype r[3],v[3],r1,Dr1,r2,Dr3,b,bdot,vr,bdot_term1,bdot_term2,b_ns,bdot_ns;
    float theta;
    r2=0;
    vr=0;
    for(int loopc=0;loopc<3;++loopc)
    {
        r[loopc]=ps[loopt].p[loopo].x[loopc]-ps[loopt].p[loopi].x[loopc];
        v[loopc]=ps[loopt].p[loopo].v[loopc]-ps[loopt].p[loopi].v[loopc];
        r2+=r[loopc]*r[loopc];
        vr+=r[loopc]*v[loopc];
    }
    r2=r2+smoothing_length2; //smoothing
    r1=sqrt(r2);
    Dr1=1.0/r1;
    Dr3=Dr1*Dr1*Dr1;

    // we compute the contribution to the acceleration, be carefull here a is the acceleration divided by G
    // in calc_all_a_adot it is multiplied with G
    for(short loopc=0;loopc<3;++loopc)
    {
        b=r[loopc]*Dr3;
        bdot_term1=v[loopc]*Dr3;
        bdot_term2=3.0*Dr3*Dr3*r1*vr*r[loopc];
        bdot=bdot_term1-bdot_term2;
        a[loopt][loopo][loopc]-=b*ps[t_0].p[loopi].m;
        a[loopt][loopi][loopc]+=b*ps[t_0].p[loopo].m;
        adot[loopt][loopo][loopc]-=bdot*ps[t_0].p[loopi].m;
        adot[loopt][loopi][loopc]+=bdot*ps[t_0].p[loopo].m;
    }
}

void hermite::calc_a_adot_dG(short loopt,pair p)
{
    calc_a_adot_dG(loopt,p.x,p.y);
}

void hermite::thread_calc_a_adot_dG(unsigned int id)
{
    unsigned int B=2*N_threads-1;
    for(unsigned int loopB=0;loopB<B;++loopB)
    {
        for(unsigned int loopp=0;loopp<ppair_data[id][loopB].N_pairs;++loopp)
        {
            calc_a_adot_dG(loopt_calc,ppair_data[id][loopB][loopp]);
        }
        pthread_barrier_wait(&barrier_a_adot_dG);
    }
}

void hermite::calc_all_a_adot(short loopt)  //loops over pairs
{
    set_zero(loopt);

#if parallel==false
    for(unsigned int loopp=0;loopp<N_pairs;++loopp)
    {
        calc_a_adot_dG(loopt,pairs[loopp][0],pairs[loopp][1]);
    }
#endif

#if parallel==true
    loopt_calc=loopt;
    pthread_barrier_init(&barrier_a_adot_dG, NULL, N_threads+1);
    hermite_start_threads();
    for(unsigned int loopB=0;loopB<2*N_threads-1;++loopB)
    {
        pthread_barrier_wait(&barrier_a_adot_dG);
    }
    hermite_stop_threads();
    pthread_barrier_destroy(&barrier_a_adot_dG);
#endif
  
    for(int loop=0;loop<N;++loop)
    {
        for(int loopc=0;loopc<3;++loopc)
        {
            a[loopt][loop][loopc]*=G;
            adot[loopt][loop][loopc]*=G;
        }
    }
}

dtype hermite::calc_dt()
{
    dtype a2,a2dot,a2Da2dot,a2Da2dot_min;
    a2Da2dot_min=1000;
    for(int loop=0;loop<N;++loop)
    {    
        a2=0;
        a2dot=0;
        for(short loopc=0;loopc<3;++loopc)
        {
            a2+=a[t_0][loop][loopc]*a[t_0][loop][loopc];
            a2dot+=adot[t_0][loop][loopc]*adot[t_0][loop][loopc];
        }
        a2Da2dot=a2/a2dot;
        if(a2Da2dot_min>a2Da2dot)
            a2Da2dot_min=a2Da2dot;
    }
    return fact_dt*pow(a2Da2dot_min,0.25);
}

void hermite::integrate_hermite()
{
    //compute a adot
    calc_all_a_adot(t_0);
    //compute time step
    dt=calc_dt();

    dtype dt2,dt3D6;
    dt2=dt*dt;
    dt3D6=dt2*dt/6.0;
    //compute predicted values (x, v)
    for(int loop=0;loop<N;++loop)
    {
        for(int loopc=0;loopc<3;++loopc)
        {
            ps[t_1].p[loop].v[loopc]=ps[t_0].p[loop].v[loopc]+a[t_0][loop][loopc]*dt+adot[t_0][loop][loopc]*dt2*0.5;
            ps[t_1].p[loop].x[loopc]=ps[t_0].p[loop].x[loopc]+ps[t_0].p[loop].v[loopc]*dt+a[t_0][loop][loopc]*dt2*0.5+adot[t_0][loop][loopc]*dt3D6;
        }
    }
    //compute a adot from predicted values
    calc_all_a_adot(t_1);
    //compute final values (x, v)
    dtype Ddt, Ddt2, Ddt3, dt4D24;
    Ddt=1.0/dt;
    Ddt2=Ddt*Ddt;
    Ddt3=Ddt2*Ddt;
    dt4D24=dt2*dt2/24.0;

    for(int loop=0;loop<N;++loop)
    {
        dtype adot2,adot3;
        for(int loopc=0;loopc<3;++loopc)
        {
            adot2=-6.0*Ddt2*(a[t_0][loop][loopc]-a[t_1][loop][loopc])-2.0*Ddt*(2.0*adot[t_0][loop][loopc]+adot[t_1][loop][loopc]);
            adot3=12.0*Ddt3*(a[t_0][loop][loopc]-a[t_1][loop][loopc])+6.0*Ddt2*(adot[t_0][loop][loopc]+adot[t_1][loop][loopc]);
            ps[t_1].p[loop].x[loopc]=ps[t_1].p[loop].x[loopc]+adot2*dt4D24+adot3*dt4D24*0.2*dt;
            ps[t_1].p[loop].v[loopc]=ps[t_1].p[loop].v[loopc]+adot2*dt3D6+adot3*dt4D24;
        }
    }
}


bool hermite::hermite_start_threads()
{
    for(int loop=0;loop<N_threads;++loop)
    {
        pthread_create(&hermite_thread_id[loop], 0, hermite_invoke, (void *)&hermite_threadarg[loop]);
    }
    return 0;
}

void hermite::hermite_stop_threads()
{
    void* no_result;
    for(int loop=0;loop<N_threads;++loop)
    {
        pthread_join(hermite_thread_id[loop], &no_result);
    }
}

void* hermite::hermite_invoke(void* self)
{
    struct thread_data *my_data;
    my_data = (struct thread_data *) self;
    // Note: The invocation takes place in start (passing this)
    static_cast<hermite*>(my_data->SELF)->thread_calc_a_adot_dG(my_data->thread_id);
    // No result passed to join.
    return 0;
}
