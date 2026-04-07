#pragma once

#include "cpu/gdt.h"
#include "cpu/isr.h"
#include "kernel/mem.h"

struct context {
    uint32_t edi, esi, ebp, ebx;
    uint32_t eip;
};

struct kstack {
    uint32_t space[400];
    struct context context;
    registers_t trapframe;
    char bottom[];
};

struct task {
    struct taskstate tss;
    pde_t *pgdir;
    struct kstack stack;
};

struct vm {
    void *kernel_thread;
    struct task *user_task;
};

extern struct vm vm;

void run_elf(const char* name);
_Noreturn void killproc();