#include "ctrl.h"
#include "main.h"

const char *ctrl_get_name(ctrl_enum_t ctrl_enum) {
  switch (ctrl_enum) {

  // Osc 1
  case CTRL_OSC1_SAW_LVL:
    return "Osc1 Saw Level";
  case CTRL_OSC1_SQU_LVL:
    return "Osc1 Square Level";
  case CTRL_OSC1_SQU_PWM:
    return "Osc1 Square PWM";
  case CTRL_OSC1_TO_OSC1_MIX:
    return NULL;
  case CTRL_OSC1_TO_OSC2_MIX:
    return "Osc1 to Osc2 Mix";
  case CTRL_OSC1_TUNE_COARSE:
    return "Osc1 Coarse Tuning";
  case CTRL_OSC1_TUNE_FINE:
    return "Osc1 Fine Tuning";
  case CTRL_OSC1_FILT_CUTOFF:
    return "Osc1 Filter Cut-Off";
  case CTRL_OSC1_FILT_RES:
    return "Osc1 Filter Resonance";
  case CTRL_OSC1_DRIVE_AMT:
    return "Osc1 Drive Amount";

  // Osc 2
  case CTRL_OSC2_SAW_LVL:
    return "Osc2 Saw Level";
  case CTRL_OSC2_SQU_LVL:
    return "Osc2 Square Level";
  case CTRL_OSC2_SQU_PWM:
    return "Osc2 Square PWM";
  case CTRL_OSC2_NOISE_LVL:
    return "Osc2 Noise Level";
  case CTRL_OSC2_FILT_CUTOFF:
    return "Osc2 Filter Cut-Off";
  case CTRL_OSC2_FILT_RES:
    return "Osc2 Filter Resonance";
  case CTRL_OSC2_DRIVE_AMT:
    return "Osc2 Drive Amount";

  // Sub Oscillator
  case CTRL_SUB_LVL:
    return "Sub Square Level";
  case CTRL_SUB_NOISE_LVL:
    return "Sub Noise Level";
  case CTRL_SUB_TO_OSC2_MIX:
    return "Sub to Osc2 Mix";
  case CTRL_SUB_FILT_CUTOFF:
    return "Sub Filter Cut-Off";
  case CTRL_SUB_FILT_RES:
    return "Sub Filter Resonance";

  // Osc Filt Env 1
  case CTRL_OSC_FILT_ENV1_A:
    return "Osc Filter Env 1 Attack";
  case CTRL_OSC_FILT_ENV1_D:
    return "Osc Filter Env 1 Decay";
  case CTRL_OSC_FILT_ENV1_S:
    return "Osc Filter Env 1 Sustain";
  case CTRL_OSC_FILT_ENV1_R:
    return "Osc Filter Env 1 Release";
  case CTRL_OSC_FILT_ENV1_AMT:
    return "Osc Filter Env 1 Amount";

  // Osc Filt Env 2
  case CTRL_OSC_FILT_ENV2_A:
    return "Osc Filter Env 2 Attack";
  case CTRL_OSC_FILT_ENV2_D:
    return "Osc Filter Env 2 Decay";
  case CTRL_OSC_FILT_ENV2_S:
    return "Osc Filter Env 2 Sustain";
  case CTRL_OSC_FILT_ENV2_R:
    return "Osc Filter Env 2 Release";
  case CTRL_OSC_FILT_ENV2_AMT:
    return "Osc Filter Env 2 Amount";

  // Sub Filt Env 1
  case CTRL_SUB_FILT_ENV1_A:
    return "Sub Filter Env 1 Attack";
  case CTRL_SUB_FILT_ENV1_D:
    return "Sub Filter Env 1 Decay";
  case CTRL_SUB_FILT_ENV1_S:
    return "Sub Filter Env 1 Sustain";
  case CTRL_SUB_FILT_ENV1_R:
    return "Sub Filter Env 1 Release";
  case CTRL_SUB_FILT_ENV1_AMT:
    return "Sub Filter Env 1 Amount";

  // Sub Filt Env 2
  case CTRL_SUB_FILT_ENV2_A:
    return "Sub Filter Env 2 Attack";
  case CTRL_SUB_FILT_ENV2_D:
    return "Sub Filter Env 2 Decay";
  case CTRL_SUB_FILT_ENV2_S:
    return "Sub Filter Env 2 Sustain";
  case CTRL_SUB_FILT_ENV2_R:
    return "Sub Filter Env 2 Release";
  case CTRL_SUB_FILT_ENV2_AMT:
    return "Sub Filter Env 2 Amount";

  // Osc Amp Env
  case CTRL_OSC_AMP_ENV_A:
    return "Osc Amp Env Attack";
  case CTRL_OSC_AMP_ENV_D:
    return "Osc Amp Env Aecay";
  case CTRL_OSC_AMP_ENV_S:
    return "Osc Amp Env Austain";
  case CTRL_OSC_AMP_ENV_R:
    return "Osc Amp Env Aelease";
  case CTRL_OSC_AMP_ENV_AMT:
    return "Osc Amp Env Amount";

  // Sub Amp Env
  case CTRL_SUB_AMP_ENV_A:
    return "Sub Amp Env Attack";
  case CTRL_SUB_AMP_ENV_D:
    return "Sub Amp Env Aecay";
  case CTRL_SUB_AMP_ENV_S:
    return "Sub Amp Env Austain";
  case CTRL_SUB_AMP_ENV_R:
    return "Sub Amp Env Aelease";
  case CTRL_SUB_AMP_ENV_AMT:
    return "Sub Amp Env Amount";

  case CTRL_FX_WETDRY:
    return "FX Wet/Dry";
  case CTRL_FX_VAL1:
    return "FX Value 1";
  case CTRL_FX_VAL2:
    return "FX Value 2";
  case CTRL_FX_VAL3:
    return "FX Value 3";
  case CTRL_FX_FEEDBACK:
    return "FX Feedback";

  case CTRL_ENUM_MAX:
    return NULL;
  }

  return NULL;
}