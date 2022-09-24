/*
 * param.c
 *
 *  Created on: 18 Sept 2022
 *      Author: jwilson
 */

#include "param.h"
#include "ctrl.h"
#include "seq.h"

uint16_t param_value(ctrl_enum_t i) {
  ctrl_t *ctrl = ctrl_get_active();
  ctrl_t *step_ctrl = seq_get_active_step_ctrl(seq_get());
  if (step_ctrl && step_ctrl->value[i] != UINT16_MAX)
    return step_ctrl->value[i];
  else
    return ctrl->value[i];
}