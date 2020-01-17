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

struct thread_data
{
    void* SELF;
    int thread_id;
};

class pair_decomp: public pair
{
public:
    pair_decomp(unsigned int T_in, unsigned int B_in, unsigned int u_in);
    ~pair_decomp();

    void calc_index(unsigned int t_in, unsigned int  b_in);

private:
    unsigned int t, b, t_s, b_s, l, r, T, B, u, T_s;

    void calc_T_s();
    void calc_t_s();
    void calc_b_s();
    void q_x();
    void q_y();
    void sum();
};

class domain
{
public:
    domain();
    ~domain();
    void initialize(unsigned int N_pairs_in);
    void triangle(unsigned int x_first, unsigned int y_first);
    void square(unsigned int x_first, unsigned int y_first);

    unsigned int N_pairs;
    pair& operator[](const unsigned int i);
private:
    pair *a;
};

class parallel_data
{
public:
    parallel_data();
    ~parallel_data();
    void initialize(short N_threads_in, unsigned int N);
    domain* &operator[](const unsigned i);
private:
    short N_threads;
    domain **a;
};
