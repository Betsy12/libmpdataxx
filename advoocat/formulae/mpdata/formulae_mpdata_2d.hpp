/** @file
* @copyright University of Warsaw
* @section LICENSE
* GPLv3+ (see the COPYING file or http://www.gnu.org/licenses/)
*/

#pragma once

#include "formulae_mpdata_common.hpp"

namespace advoocat 
{ 
  namespace formulae 
  { 
    namespace mpdata 
    {
      // 2D
      template<int d, class arr_2d_t>
      inline auto A(
        const arr_2d_t &psi, 
        const rng_t &i, 
        const rng_t &j
      ) return_macro(,
        frac(
            abs(psi(pi<d>(i+1, j))) 
          - abs(psi(pi<d>(i,   j))),
          // ----------------------
            abs(psi(pi<d>(i+1, j))) 
          + abs(psi(pi<d>(i,   j)))
        ) 
      ) 

      template<int d, class arr_2d_t>
      inline auto B(
        const arr_2d_t &psi, 
        const rng_t &i, 
        const rng_t &j
      ) return_macro(,
        frac( 
            abs(psi(pi<d>(i+1, j+1))) 
          + abs(psi(pi<d>(i,   j+1))) 
          - abs(psi(pi<d>(i+1, j-1))) 
          - abs(psi(pi<d>(i,   j-1))),
	  // ------------------------
            abs(psi(pi<d>(i+1, j+1))) 
          + abs(psi(pi<d>(i,   j+1))) 
          + abs(psi(pi<d>(i+1, j-1))) 
          + abs(psi(pi<d>(i,   j-1)))
        ) / 2
      )

      template<int d, class arr_2d_t>
      inline auto C_bar(
        const arr_2d_t &C, 
        const rng_t &i, 
        const rng_t &j
      ) return_macro(,
        (
          C(pi<d>(i+1, j+h)) + 
          C(pi<d>(i,   j+h)) +
          C(pi<d>(i+1, j-h)) + 
          C(pi<d>(i,   j-h)) 
        ) / 4
      )

      template <int dim, class arr_2d_t>
      inline auto antidiff(
        const arr_2d_t &psi, 
        const rng_t &i, 
        const rng_t &j,
        const arrvec_t<arr_2d_t> &C
      ) return_macro(,
        abs(C[dim](pi<dim>(i+h, j))) 
        * (1 - abs(C[dim](pi<dim>(i+h, j)))) 
        * A<dim>(psi, i, j) 
        - C[dim](pi<dim>(i+h, j)) 
        * C_bar<dim>(C[dim-1], i, j)
        * B<dim>(psi, i, j)
      ) 
    }; // namespace mpdata
  }; // namespace formulae
}; // namespcae advoocat 