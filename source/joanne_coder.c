// Copyright (c) 2020 Sarun Rattanasiri
// under the 3-Clause BSD License
// https://opensource.org/licenses/BSD-3-Clause

#include "joanne_coder.h"
#include <stddef.h>

void joanne_coder__init(const JoanneCoder_Config *config,
                        JoanneCoder_State *state, uint32_t *tracking_state) {
  if (tracking_state == NULL) {
    state->auto_init = true;
    for (uint8_t i = 0; i < config->slot_count; i++)
      config->tracking_state[i] = JOANNE_CODER__UNINITIALIZED;
    return;
  }
  for (uint8_t i = 0; i < config->slot_count; i++)
    config->tracking_state[i] = tracking_state[i];
}

static void accept(const JoanneCoder_Config *config, JoanneCoder_State *state,
                   uint8_t slot, uint32_t access_code, uint8_t command);

void joanne_coder__push_command(const JoanneCoder_Config *config,
                                JoanneCoder_State *state, uint8_t slot,
                                uint32_t access_code, uint8_t command) {
  // slot overflow
  if (config->slot_count <= slot)
    return;
  uint32_t current_tracking = config->tracking_state[slot];
  // auto init
  if ((state->auto_init) && (current_tracking == JOANNE_CODER__UNINITIALIZED)) {
    accept(config, state, slot, access_code, command);
    return;
  }
  uint32_t upper_bound = current_tracking + JOANNE_CODER__SYNC_MARGIN;
  // rolled sequence
  if (upper_bound < current_tracking) {
    if (!((current_tracking < access_code) || (access_code <= upper_bound)))
      return;
    accept(config, state, slot, access_code, command);
    return;
  }
  // normal sequence
  if (!((current_tracking < access_code) && (access_code <= upper_bound)))
    return;
  accept(config, state, slot, access_code, command);
}

static void accept(const JoanneCoder_Config *config, JoanneCoder_State *state,
                   uint8_t slot, uint32_t access_code, uint8_t command) {
  // skip rolling collision
  if ((state->auto_init) && (access_code == JOANNE_CODER__UNINITIALIZED))
    access_code++;
  config->tracking_state[slot] = access_code;
  // update tracker
  if (config->tracking_changed)
    config->tracking_changed(config->tracking_state, slot);
  // process command
  if (config->command_received)
    config->command_received(command);
}

void joanne_coder__sync(const JoanneCoder_Config *config,
                        JoanneCoder_State *state, uint8_t slot,
                        uint32_t access_code) {
  // slot overflow
  if (config->slot_count <= slot)
    return;
  // skip rolling collision
  if ((state->auto_init) && (access_code == JOANNE_CODER__UNINITIALIZED))
    access_code++;
  // update tracker
  config->tracking_state[slot] = access_code;
  if (config->tracking_changed)
    config->tracking_changed(config->tracking_state, slot);
}
