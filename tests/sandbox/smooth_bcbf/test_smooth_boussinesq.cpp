/** 
 * @file
 * @copyright University of Warsaw
 * @section LICENSE
 * GPLv3+ (see the COPYING file or http://www.gnu.org/licenses/)
 */

#include "../../tutorial/8_boussinesq_2d/boussinesq.hpp"
#include <libmpdata++/concurr/threads.hpp>
#include <libmpdata++/output/gnuplot.hpp>
using namespace libmpdataxx;

const int nx = 201, ny = 201, nt = 1212;

int main() 
{
  // compile-time parameters
  struct ct_params_t : ct_params_default_t
  {
    using real_t = double;
    enum { n_dims = 2 };
    enum { n_eqns = 3 };
    enum { rhs_scheme = solvers::euler_b };
    enum { prs_scheme = solvers::cr };
    struct ix { enum {u, w, tht, vip_i=u, vip_j=w, vip_den=-1}; }; 
  };
  using ix = typename ct_params_t::ix;  

  using solver_t = output::gnuplot<boussinesq<ct_params_t>>;

  // run-time parameters
  solver_t::rt_params_t rt_params; 

  rt_params.prs_tol = 1e-5;
  rt_params.grid_size = {nx, ny};

  rt_params.dt = .5;
  rt_params.di = rt_params.dj = 10.;
  rt_params.Tht_ref = 300; // [K] ambient state (constant thoughout the domain)

  rt_params.outfreq = 1;
  rt_params.outvars = {
//    {ix::u,   {.name = "u",   .unit = "m/s"}}, 
//    {ix::w,   {.name = "w",   .unit = "m/s"}}, 
    {ix::tht, {.name = "tht", .unit = "K"  }}
  };
  rt_params.gnuplot_view = "map";
  rt_params.gnuplot_output = "figure_%s_%d.png";
  rt_params.gnuplot_with = "lines";
  rt_params.gnuplot_surface = false;
  rt_params.gnuplot_contour = true;
//  rt_params.gnuplot_cbrange = "[299:301.5]";
  rt_params.gnuplot_term = "png";

  concurr::threads<
    solver_t, 
    bcond::cyclic, bcond::cyclic,
    bcond::cyclic, bcond::cyclic
  > run(rt_params);

  // initial condition
  {
    blitz::firstIndex i;
    blitz::secondIndex j;

    run.advectee(ix::tht) = rt_params.Tht_ref 
      + .5 * exp( -sqr(i-(nx-1)/2.) / (2.*pow((nx-1)/20, 2))   // TODO: assumed dx=dy=1?
		  -sqr(j-(ny-1)/4.) / (2.*pow((ny-1)/20, 2)) )
    ;
    run.advectee(ix::u) = 0;
    run.advectee(ix::w) = 0; 
  }

  // integration
  run.advance(nt); 
};
