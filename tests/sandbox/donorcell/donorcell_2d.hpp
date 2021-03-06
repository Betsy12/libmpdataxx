/** @file
* @copyright University of Warsaw
* @section LICENSE
* GPLv3+ (see the COPYING file or http://www.gnu.org/licenses/)
*/

#pragma once

#include <libmpdata++/solvers/detail/solver_2d.hpp>
#include <libmpdata++/formulae/donorcell_formulae.hpp>

namespace libmpdataxx
{
  namespace solvers
  {
    template<
      typename ct_params_t, 
      int minhalo = formulae::donorcell::halo
    >
    class donorcell_2d : public detail::solver<
      ct_params_t,
      2,
      formulae::donorcell::n_tlev,
      detail::max(minhalo, formulae::donorcell::halo)
    > 
    {
      using parent_t = detail::solver<
        ct_params_t,
        2,
        formulae::donorcell::n_tlev, 
        detail::max(minhalo, formulae::donorcell::halo)
      >;

      void advop(int e)
      {
        formulae::donorcell::op_2d<ct_params_t::opts>(
          this->mem->psi[e], 
	  this->mem->GC, 
	  *this->mem->G,
	  this->n[e], 
	  this->i, 
          this->j
        );
      }

      public:

      // ctor
      donorcell_2d(
        typename parent_t::ctor_args_t args, 
        const typename parent_t::params_t &p
      ) :
        parent_t(args, p)
      {}  
    };
  }; // namespace solvers
}; // namespace libmpdataxx
