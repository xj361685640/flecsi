/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2015 Los Alamos National Security, LLC
 * All rights reserved.
 *~--------------------------------------------------------------------------~*/

#ifndef flecsi_execution_h
#define flecsi_execution_h

#include <functional>
// FIXME
#include <iostream>

#include "flecsi/utils/common.h"
#include "flecsi/execution/common/function_handle.h"
#include "flecsi/execution/task.h"
#include "flecsi/execution/function.h"
#include "flecsi/execution/kernel.h"

///
// \file execution.h
// \authors bergen
// \date Initial file creation: Aug 01, 2016
///

//----------------------------------------------------------------------------//
// Function Interface
//----------------------------------------------------------------------------//

///
// This macro registers a user function with the FleCSI runtime, which may
// then be passed as state data and executed in any task address space.
//
// \param name The function to register. This should be the plain-text
//              name of the function (not a string).
// \param return_type The function return type.
// \param ... The signature of the function (arguments).
///
#define register_function(name)                                                \
                                                                               \
  /* Function return type (trt) */                                             \
  using name ## _trt_t =                                                       \
    typename flecsi::function_traits__<decltype(name)>::return_type;           \
                                                                               \
  /* Function arguments type (tat) */                                          \
  using name ## _tat_t =                                                       \
    typename flecsi::function_traits__<decltype(name)>::arguments_type;        \
                                                                               \
  /* Wrapper to call user function with tuple arguments */                     \
  inline name ## _trt_t name ## _tuple_wrapper(name ## _tat_t && args) {       \
    return flecsi::tuple_function(name, args);                                 \
  } /* task ## _tuple_wrapper */                                               \
                                                                               \
  /* Make std::function delegate */                                            \
  std::function<name ## _trt_t(name ## _tat_t)>                                \
    name ## _function_delegate = name ## _tuple_wrapper;                       \
                                                                               \
  /* Define name handle type */                                                \
  using function_handle_ ## name ## _t =                                       \
    function_handle__<name ## _trt_t, name ## _tat_t>;                         \
                                                                               \
  /* Register the function delegate */                                         \
  bool name ## _function_registered =                                          \
    flecsi::execution::function_t::register_function<                          \
      name ## _trt_t, name ## _tat_t>(                                         \
      EXPAND_AND_STRINGIFY(name), name ## _function_delegate)

///
// Execute a user function.
//
// \param handle The function handle.
// \param ... The function arguments.
///
#define execute_function(handle, ...)                                          \
  flecsi::execution::function_t::execute_function(handle, ## __VA_ARGS__)

///
// FIXME
///
#define function_handle(name)                                                  \
  function_handle_ ## name ## _t(                                              \
    const_string_t{EXPAND_AND_STRINGIFY(name)}.hash())

///
// FIXME
///
#define define_function_type(name, return_type, ...)                          \
  using name = function_handle__<return_type, std::tuple<__VA_ARGS__>>

//----------------------------------------------------------------------------//
// Task Interface
//----------------------------------------------------------------------------//

// FIXME: Finish Doxygen

///
//
///
#define register_task(task, processor, mode)                                   \
                                                                               \
  /* Register the user task in the function table */                           \
  register_function(task);                                                     \
                                                                               \
  /* Register the user task */                                                 \
  bool task ## _task_registered =                                              \
    flecsi::execution::task_t::register_task<task ## _trt_t, task ## _tat_t>   \
    (reinterpret_cast<uintptr_t>(&task), processor, mode)

///
//
///
#define execute_task(task, processor, mode, ...)                               \
                                                                               \
  /* Execute the user task */                                                  \
  /* WARNING: This macro returns a future. Don't add terminations! */          \
  flecsi::execution::task_t::execute_task<task ## _trt_t>                      \
    (reinterpret_cast<uintptr_t>(&task), processor, mode,                      \
    task ## _function_delegate, ## __VA_ARGS__)

//----------------------------------------------------------------------------//
// Kernel Interface
//----------------------------------------------------------------------------//

///
//
///
#define for_each(index_space, index, kernel)                                   \
  flecsi::execution::for_each__(index_space, [&](auto * index) kernel)

///
//
///
#define reduce_each(index_space, index, variable, kernel)                      \
  flecsi::execution::reduce_each__(index_space, variable,                      \
    [&](auto * index, auto & variable) kernel)

#endif // flecsi_execution_h

/*~-------------------------------------------------------------------------~-*
 * Formatting options for vim.
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
