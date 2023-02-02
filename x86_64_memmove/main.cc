#ifndef __x86_64__
#error Only x86_64 is supported
#endif

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// rdi, rsi, rdx, rcx, r8d, r9d;

const size_t kNumMoves = 10000;

void mexec_memmove(char* dst, char* src, size_t n) {
    __asm__ volatile (
        "cld;"                // Clear Direction Flag

        // Compare the source and destination pointers. Memmove starts from the
        // front of the buffer and copies forward if src > dst otherwise it
        // starts at the end of the buffer and copies backwards.
        "cmp %%rsi, %%rdi;"   // if rdi < rsi then CF = 1 
        "jbe .do_copy%=;\n"   // if CF = 1, Branch to .do_copy
        "mov %%rcx, %%rax;"   // Copy count into rax
        "sub $1, %%rax;"      // Increment rax by 1
        "add %%rax, %%rdi;"   // Move rdi to the end of the buffer
        "add %%rax, %%rsi;"   // Also move rsi to the end of the buffer
        "std;"                // Set direction flag so rep movsb decrements 
        ".do_copy%=:\n"
        "rep movsb;"          // Copy rcx bytes from rsi to rdi
        "cld;"                // Clear the direction flag.
        : "=D" (dst),
          "=S" (src),
          "=c" (n)
        : "0" (dst),
          "1" (src),
          "2" (n)
        : "memory","%rax"
    );
}

int main(int argc, char* argv[]) {
    for (size_t i = 0; i < kNumMoves; i++) {
        char mine[] = "abcdefghijklmnopqrstuvwxyz";
        char reference[] = "abcdefghijklmnopqrstuvwxyz";

        // Generate random memmov params
        size_t src_index = rand() % sizeof(mine);
        size_t dst_index = rand() % sizeof(mine);

        // Don't allow the source or the destination index to go off of the end
        // of the string
        size_t src_max = (sizeof(mine) - 1) - src_index;
        size_t dst_max = (sizeof(mine) - 1) - dst_index;

        if (!src_max || !dst_max) continue;

        size_t length = rand() % (MIN(src_max, dst_max));

        // printf("Moving %lu bytes from %lu to %lu\n", length, src_index, dst_index);

        mexec_memmove(mine + dst_index, mine + src_index, length);
        memmove(reference + dst_index, reference + src_index, length);

        if (strncmp(mine, reference, sizeof(mine))) {
            printf("Memmov failed! '%s' '%s'\n", mine, reference);
        }
    }

    return 0;
}