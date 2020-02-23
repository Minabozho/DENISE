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

dtype calc_total_momentum(const particles &ps)
{
    dtype result[3]={0,0,0};

    for(unsigned int loop=0;loop<ps.N;++loop)
    {
        for(int loopc=0;loopc<3;++loopc)
        {
            result[loopc]+=ps.p[loop].m*ps.p[loop].v[loopc];
        }
    }

    return vector_length(result);
}

dtype calc_cluster_velocity(const particles &ps)
{
    dtype total_mass=0;
    for(unsigned int loop=0;loop<ps.N;++loop)
    {
        total_mass+=ps.p[loop].m;
    }
    return calc_total_momentum(ps)/total_mass;
}

dtype calc_kinetic_energy(const data &d)
{
    dtype Ekin=0;
    for(unsigned int loop=0;loop<d.N;++loop)
    {
        Ekin+=d.ps[d.t_0].p[loop].Ekin();
    }
    return Ekin;
}

dtype calc_potential_energy(const data &d)
{
    dtype Epot=0;
    unsigned int i,j,N_pairs;
    dtype r2,r[3];
    N_pairs=0.5*(d.N*d.N-d.N);
    for(unsigned int loop=0;loop<d.N_pairs;++loop)
    {
        j=d.pairs[loop][0];
        i=d.pairs[loop][1];
        r2=0;
        for(int loopc=0;loopc<3;++loopc)
        {
            r[loopc]=d.ps[d.t_0].p[j].x[loopc]-d.ps[d.t_0].p[i].x[loopc];
            r2+=r[loopc]*r[loopc];
        }
        Epot-=d.G*d.ps[d.t_0].p[j].m*d.ps[d.t_0].p[i].m/sqrt(r2);
    }
    return Epot;
}

dtype calc_total_energy(const data &d)
{   
    return calc_kinetic_energy(d)+calc_potential_energy(d);
}
