// Copyright (c) 2020 Sarun Rattanasiri
// under the 3-Clause BSD License
// https://opensource.org/licenses/BSD-3-Clause

#ifndef __JOANNE_CODER_H
#define __JOANNE_CODER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define JOANNE_CODER__UNINITIALIZED 0xffff
#define JOANNE_CODER__SYNC_MARGIN 1000

typedef struct _joanne_coder__config {
  uint32_t *tracking_state;
  uint8_t slot_count;
  void (*command_received)(uint8_t command);
  void (*tracking_changed)(uint32_t *tracking_state, uint8_t changed_slot);
} JoanneCoder_Config;

typedef struct _joanne_coder__state {
  bool auto_init;
} JoanneCoder_State;

void joanne_coder__init(const JoanneCoder_Config *config,
                        JoanneCoder_State *state, uint32_t *tracking_state);

void joanne_coder__push_command(const JoanneCoder_Config *config,
                                JoanneCoder_State *state, uint8_t slot,
                                uint32_t access_code, uint8_t command);

void joanne_coder__sync(const JoanneCoder_Config *config, uint8_t slot,
                        uint32_t access_code);

#endif
