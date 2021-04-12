#include "midi.h"
#include "main.h"
#include "stdbool.h"
#include "dac.h"

void update_gate_state(midi_state midi)
{
    HAL_GPIO_WritePin(NOTE_GATE_GPIO_Port, NOTE_GATE_Pin, midi.note_on);
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, midi.note_on);
}
