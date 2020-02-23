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
std::string convert_to_string(NTYPE number);

template<typename DTYPE>
double vector_length(DTYPE &vec);

std::string add_leading_zeros(short number);

unsigned int smaller_2n(unsigned int number);

void make_dir(const std::string &name);
