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

class particle
{
public:
    particle();
    ~particle();

    int id;
    dtype x[3];
    dtype v[3];
    dtype m;

    dtype v2();
    dtype Ekin();
};

std::ostream& operator<<( std::ostream& s, const particle &p);
