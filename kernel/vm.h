#pragma once

#include <stddef.h>
#include <stdint.h>
#include "mem.h"

int map_phys_range(pde_t *pgdir, uintptr_t va, uintptr_t pa, size_t size, int perm);