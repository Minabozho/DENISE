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

class hermite : virtual public data
{
public:
    hermite(const std::string &parameterfile);
    ~hermite();

    void integrate_hermite();
private:
    dtype **a[2];
    dtype **adot[2];
    dtype calc_dt();
    void set_zero(short loopt);
    void calc_a_adot_dG(short loopt, int loopo, int loopi); //computes a/G and adot/G
    void calc_a_adot_dG(short loopt, pair p);
    void calc_all_a_adot(short loopt); //loops over above method and multiplies with G (gravitational constant)

    short loopt_calc;
    void thread_calc_a_adot_dG(unsigned int id);
    parallel_data ppair_data;

    //tdata hermite_threadarg
    bool hermite_start_threads();
    void hermite_stop_threads();

    // The C-callback has to be static.
    static void* hermite_invoke(void* self);

    thread_data* hermite_threadarg;
    pthread_t* hermite_thread_id;

    pthread_barrier_t barrier_a_adot_dG;
};