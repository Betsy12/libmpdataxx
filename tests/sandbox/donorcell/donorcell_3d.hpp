/** @file
* @copyright University of Warsaw
* @section LICENSE
* GPLv3+ (see the COPYING file or http://www.gnu.org/licenses/)
*/

#pragma once

#include <libmpdata++/solvers/detail/solver_3d.hpp>
#include <libmpdata++/formulae/donorcell_formulae.hpp>

namespace libmpdataxx
{
  namespace solvers
  {
    template<
      typename ct_params_t,
      int halo = formulae::donorcell::halo
    > 
    class donorcell_3d : public detail::solver<
      ct_params_t,
      3,
      formulae::donorcell::n_tlev, 
      detail::max(halo, formulae::donorcell::halo)
    >
    {
      using parent_t = detail::solver<
        ct_params_t,
        3,
        formulae::donorcell::n_tlev, 
        detail::max(halo, formulae::donorcell::halo)
      >;

      void advop(int e)
      {
	formulae::donorcell::op_3d<ct_params_t::opts>( 
	  this->mem->psi[e], 
          this->mem->GC, 
          *this->mem->G, 
          this->n[e], 
          this->i, this->j, this->k
	);
      }

      public:

      // ctor
      donorcell_3d(
	typename parent_t::ctor_args_t args, 
	const typename parent_t::params_t &p
      ) :
	parent_t(args, p)
      {}  
    }; // class donorcell_3d
  }; // namespace solvers
}; // namespace libmpdataxx
