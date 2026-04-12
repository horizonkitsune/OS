#pragma once

#include "stdint.h"
#include "isr.h"


void keyboard_init(void);
void keyboard_key(register_t* regs);