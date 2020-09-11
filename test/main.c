#include "joanne_coder.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define CODE_SLOT 2

uint32_t tracking_memory[CODE_SLOT];

static void command_handler(uint8_t command);
static const JoanneCoder_Config coder_config = {
    .tracking_state = tracking_memory,
    .slot_count = CODE_SLOT,
    .command_received = &command_handler,
};
static JoanneCoder_State coder_state;

const uint32_t initial_tracking_state[CODE_SLOT] = {
    0,
    ~1,
};

static uint8_t command_count = 0;
static uint8_t latest_command = 0;

int main(void) {
  joanne_coder__init(&coder_config, &coder_state,
                     (uint32_t *)initial_tracking_state);
  joanne_coder__push_command(&coder_config, &coder_state, 0, 10, 1);
  if ((command_count == 1) && (latest_command == 1)) {
    puts("Basic retrieve\tPASSED");
  } else {
    puts("Basic retrieve\tFAILED");
  }
  joanne_coder__push_command(&coder_config, &coder_state, 0, 10, 2);
  if ((command_count == 1) && (latest_command == 1)) {
    puts("Replay immunity\tPASSED");
  } else {
    puts("Replay immunity\tFAILED");
  }
  joanne_coder__push_command(&coder_config, &coder_state, 0, 11, 3);
  if ((command_count == 2) && (latest_command == 3)) {
    puts("Operation after replay\tPASSED");
  } else {
    puts("Operation after replay\tFAILED");
  }
  joanne_coder__push_command(&coder_config, &coder_state, 0,
                             11 + JOANNE_CODER__SYNC_MARGIN + 1, 4);
  if ((command_count == 2) && (latest_command == 3)) {
    puts("Rejection of distant time traveler\tPASSED");
  } else {
    puts("Rejection of distant time traveler\tFAILED");
  }
  joanne_coder__push_command(&coder_config, &coder_state, 1, 2, 5);
  if ((command_count == 3) && (latest_command == 5)) {
    puts("Allow rotation\tPASSED");
  } else {
    puts("Allow rotation\tFAILED");
  }
  joanne_coder__push_command(&coder_config, &coder_state, 2,
                             JOANNE_CODER__SYNC_MARGIN, 6);
  if ((command_count == 3) && (latest_command == 5)) {
    puts("Rotation safety\tPASSED");
  } else {
    puts("Rotation safety\tFAILED");
  }
  // autoinit
  joanne_coder__init(&coder_config, &coder_state, NULL);
  joanne_coder__push_command(&coder_config, &coder_state, 0,
                             JOANNE_CODER__SYNC_MARGIN * 5, 1);
  joanne_coder__push_command(&coder_config, &coder_state, 0,
                             JOANNE_CODER__SYNC_MARGIN * 5 + 1, 2);
  if ((command_count == 5) && (latest_command == 2)) {
    puts("Autoinit\tPASSED");
  } else {
    puts("Autoinit\tFAILED");
  }
  joanne_coder__push_command(&coder_config, &coder_state, 1,
                             JOANNE_CODER__UNINITIALIZED, 3);
  joanne_coder__push_command(&coder_config, &coder_state, 1,
                             JOANNE_CODER__SYNC_MARGIN * 5, 4);
  if ((command_count == 6) && (latest_command == 3)) {
    puts("Autoinit safety\tPASSED");
  } else {
    puts("Autoinit safety\tFAILED");
  }
  joanne_coder__sync(&coder_config, &coder_state, 0,
                     JOANNE_CODER__SYNC_MARGIN * 2);
  bool sync_success = (command_count == 6); // no change
  joanne_coder__push_command(&coder_config, &coder_state, 1,
                             JOANNE_CODER__SYNC_MARGIN * 2 + 1, 5);
  sync_success &= (command_count == 6);
  if ((command_count == 6) && (latest_command == 3)) {
    puts("Sequence sync\tPASSED");
  } else {
    puts("Sequence sync\tFAILED");
  }

  puts("\nDone!");
  return 0;
}

static void command_handler(uint8_t command) {
  command_count++;
  latest_command = command;
}
