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

initial_cond::initial_cond(const std::string &parameterfile)
    : data(parameterfile)
{
    unsigned int seed;
    seed=2374767364;
    generator.seed(seed);

    //alpha=3.0/5.0; //for uniform sphere
    sigma_velocity=sqrt(alpha*G*total_mass/(3.0*radius));
}

initial_cond::~initial_cond()
{
}

particle initial_cond::uniform_sphere()
{
    double radius2=radius*radius;
    particle result;
    double &x=result.x[0];
    double &y=result.x[1];
    double &z=result.x[2];

    std::uniform_real_distribution<dtype> random_flat (-radius,radius);
    std::normal_distribution<dtype> random_gaus (0.0,sigma_velocity);

    do { 
        x=random_flat(generator);
        y=random_flat(generator);
        z=random_flat(generator);
    } while(x*x+y*y+z*z>radius2);
    for(short loopc=0;loopc<3;++loopc)
        result.v[loopc]=random_gaus(generator);

    return result;
}

void initial_cond::set_cluster_vel_zero(short time) //set the cluster velocity to zero
{
    dtype velocity[3] = {0,0,0};
    for(unsigned int loop=0;loop<N;++loop)
    {
        for(int loopc=0;loopc<3;++loopc)
        {
            velocity[loopc]+=ps[time].p[loop].m*ps[time].p[loop].v[loopc]; //this is the momentum
        }
    }
    for(short loopc=0;loopc<3;++loopc)
    {
        velocity[loopc]/=total_mass; //here it becomes a velocity
    }
    //set velocity to zero
    for(unsigned int loop=0;loop<N;++loop)
    {
        for(int loopc=0;loopc<3;++loopc)
        {
            ps[time].p[loop].v[loopc]-=velocity[loopc];
        } 
    }
}

void initial_cond::set_initial_cond()
{
    switch(ic_number)
    {
        case 0:
            for(int loop=0;loop<N;++loop)
            {
                for(int loopc=0;loopc<3;++loopc)
                {
                    ps[0].p[loop].x[loopc]=loop-loopc;
                    ps[0].p[loop].v[loopc]=(loop+loopc)%3-1.0;
                }
            }
            break;
        case 1:
            std::cout<<"   Create uniform sphere, sigma="<<sigma_velocity<<std::endl;
            for(int loop=0;loop<N;++loop)
            {
                ps[0].p[loop]=uniform_sphere();
            }
            break;
        default:
            std::cout<<"Initial conditions unkown."<<std::endl;            
    }
    dtype particle_mass=total_mass/N;
    for(int loop=0;loop<N;++loop)
    {
        ps[0].p[loop].id=loop;
        ps[0].p[loop].m=particle_mass;
    }
    set_cluster_vel_zero(0);
    //copy to other array for other time
    for(int loop=0;loop<N;++loop)
    {       
        ps[1].p[loop]=ps[0].p[loop];
    }
}
