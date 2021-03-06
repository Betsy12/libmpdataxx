/** 
 * @file
 * @copyright University of Warsaw
 * @section LICENSE
 * GPLv3+ (see the COPYING file or http://www.gnu.org/licenses/)
 *
 * \include "mpdata_1d_opt_eps/test_mpdata_1d_opt_eps.cpp"
 * \image html "../../tests/mpdata_1d_opt_eps/figure_iters=3.svg" TODO
 */

#include <libmpdata++/solvers/mpdata.hpp>
#include <libmpdata++/concurr/threads.hpp>
#include <libmpdata++/output/gnuplot.hpp>

// TODO: make a common file with the setopts and setup from below?

using namespace libmpdataxx;

using T = float;
int n = 11, nt = 10000;

template <class slv_t>
void setup(slv_t &solver, int n) 
{
  blitz::firstIndex i;
  int width = 2, center = 5;
  solver.advectee() = where(i <= center-width/2 || i >= center+width/2, -400, 400) * blitz::tiny(T(0)); 
  solver.advector() = .5; 
}

template <class T>
void setopts(T &p, const int nt, const std::string &fname, int n_iters)
{
  p.n_iters = n_iters;

  p.outfreq = nt; // displays initial condition and the final state
  p.gnuplot_output = fname + ".svg";    
  p.outvars = {{0, {.name = "psi", .unit = "1"}}};
  p.gnuplot_command = "plot";
  p.gnuplot_with = "histeps";
  //p.gnuplot_yrange = "[-2:5]";
}

template <opts::opts_t opt, class vec_t>
void add_solver(vec_t &slvs, const std::string &fname, int n_iters)
{
  struct ct_params_t : ct_params_default_t
  {
    using real_t = T;
    enum { n_dims = 1 };
    enum { n_eqns = 1 };
    enum { opts = opt };
  };
  using output_t = output::gnuplot<solvers::mpdata<ct_params_t>>;
  typename output_t::rt_params_t p;
  setopts(p, nt, fname, n_iters);
  p.grid_size = {n};
  slvs.push_back(new concurr::threads<output_t, bcond::cyclic, bcond::cyclic>(p));
  setup(slvs.back(), n);
}

int main() 
{
  const int n_dims = 1;
  boost::ptr_vector<concurr::any<T, n_dims>> slvs;

  add_solver<opts::abs>(slvs, "mpdata_iters=2", 2);
  add_solver<opts::abs | opts::eps>(slvs, "mpdata_iters=2_eps", 2);
  add_solver<opts::abs>(slvs, "mpdata_iters=3", 3);
  add_solver<opts::abs | opts::eps>(slvs, "mpdata_iters=3_eps", 3);

  for (auto &slv : slvs) slv.advance(nt);
}
