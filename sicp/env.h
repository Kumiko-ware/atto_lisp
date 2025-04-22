#ifndef ENV_H_INCLUDED
#define ENV_H_INCLUDED

#include "memory.h"

// From Ch. 4.1.3 - Operations on environments

cell enclosing_environment(cell env);
cell first_frame(cell env);
cell make_frame(cell variables, cell values);
cell frame_variables(cell frame);
cell frame_values(cell frame);
cell add_binding_to_frame(cell var, cell val, cell frame);
cell extend_environment(cell vars, cell vals, cell base_env);
cell lookup_variable_value(cell var, cell env);
cell set_variable_value(cell var, cell val, cell env);

#endif //ENV_H_INCLUDED
