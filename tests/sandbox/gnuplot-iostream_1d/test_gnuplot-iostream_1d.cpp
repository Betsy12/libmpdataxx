/** 
 * @file
 * @copyright University of Warsaw
 * @section LICENSE
 * GPLv3+ (see the COPYING file or http://www.gnu.org/licenses/)
 *
 * \include "gnuplot-iostream_1d/test_gnuplot-iostream_1d.cpp"
 * \image html "../../tests/gnuplot-iostream_1d/figure_iters=1.svg"
 * \image html "../../tests/gnuplot-iostream_1d/figure_iters=2.svg"
 * \image html "../../tests/gnuplot-iostream_1d/figure_iters=3.svg"
 */

#include <libmpdata++/solvers/mpdata.hpp>
#include <libmpdata++/concurr/threads.hpp>
#include <libmpdata++/output/gnuplot.hpp>

using namespace libmpdataxx;

struct ct_params_t : ct_params_default_t
{
  using real_t = float;
  enum { n_dims = 1 };
  enum { n_eqns = 1 };
  enum { opts = 0 };
};

int n = 20, nt = 20;

template <class slvs_t>
void add_solver(slvs_t &slvs, int n_iters)
{
  using solver_t = output::gnuplot<solvers::mpdata<ct_params_t>>;
  typename solver_t::rt_params_t p;

  // pre-instantiation stuff
  p.grid_size[0] = n;
  p.n_iters = n_iters;
  p.outfreq = nt / 10; 
  {
    std::ostringstream tmp;
    tmp << "figure_iters=" << n_iters << ".svg";
    p.gnuplot_output = tmp.str();    
  }
  p.outvars = {{0, {.name = "psi", .unit = "1"}}};
 
  // instantiation
  slvs.push_back(new concurr::threads<solver_t, bcond::cyclic, bcond::cyclic>(p));

  // post-instantiation stuff
  {
    blitz::firstIndex i;
    slvs.back().advectee() = exp(
      -sqr(i-(n-1)/2.) / (2.*pow((n-1)/10, 2)) // TODO: assumes dx=1
    );  
    slvs.back().advector() = .5; 
  }
}

int main() 
{
  boost::ptr_vector<concurr::any<ct_params_t::real_t, 1>> slvs;
  add_solver(slvs, 1);
  add_solver(slvs, 2);
  add_solver(slvs, 3);
  for (auto &slv : slvs) slv.advance(nt);
}
