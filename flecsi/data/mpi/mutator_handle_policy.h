/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_data_mpi_mutator_handle_policy_h
#define flecsi_data_mpi_mutator_handle_policy_h

#include "flecsi/data/common/data_types.h"

//----------------------------------------------------------------------------//
/// @file
/// @date Initial file creation: Apr 04, 2017
//----------------------------------------------------------------------------//

namespace flecsi {

//----------------------------------------------------------------------------//
//! The mpi_mutator_handle_policy_t type provides backend storage for
//! interfacing to the Legion runtime.
//!
//! @ingroup data
//----------------------------------------------------------------------------//

struct mpi_mutator_handle_policy_t
{
  using offset_t = data::sparse_data_offset_t;

  mpi_mutator_handle_policy_t(){}

  mpi_mutator_handle_policy_t(const mpi_mutator_handle_policy_t& p) = default;

  std::vector<offset_t>* offsets;    
  std::vector<uint8_t>* entries;
  size_t* reserve;
  size_t reserve_chunk;
  size_t* num_exclusive_entries;
}; // class mpi_mutator_handle_policy_t

} // namespace flecsi

#endif // flecsi_data_mpi_mutator_handle_policy_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
