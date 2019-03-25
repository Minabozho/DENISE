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

particle::particle()
{
    m=1.0;
}

particle::~particle()
{

}

dtype particle::v2()
{
    return v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
}

dtype particle::Ekin()
{
    return 0.5*m*v2();
}

std::ostream& operator<<( std::ostream& s, const particle &p)
{
    s<<p.id<<" ";
    for(short loopc=0;loopc<3;++loopc)
    {
        s<<p.x[loopc]<<" ";
    }
    for(short loopc=0;loopc<3;++loopc)
    {
        s<<p.v[loopc]<<" ";
    }
    s<<p.m;
    return s;
}