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

class pair
{
public:
    pair();
    ~pair();
        
    unsigned int o;
    unsigned int i;
    unsigned int &x=o;
    unsigned int &y=i;
};

class data : public parameter
{
public:
    data(const std::string &parameterfile);
    ~data();

    unsigned int t_0,t_1;
    particles* ps;
    unsigned int **pairs;
    unsigned int N_pairs;
};
