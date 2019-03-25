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

class parameter
{
public:
    parameter(const std::string& filename);
    ~parameter();

    std::string directory,directory_data,directory_plots;
    int N,N_NS,num_out;
    dtype total_mass;
    dtype radius;
    dtype alpha;

    short ic_number;
    short N_threads;
    
    dtype dt;
    dtype fact_dt;
    dtype t;
    dtype smoothing_length;
    dtype smoothing_length2;

    float t_final;
    float* times_out;

    bool do_replot;

    dtype G;

private:
    void read_parameter(const std::string& filename);
    void display_parameter();
};