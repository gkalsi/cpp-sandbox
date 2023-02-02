#include <iostream>

// See here for more details:
// https://lisha.ufsc.br/teaching/os/exercise/where_is_my_variable.html

constexpr bool kFail = true;

const int global_const = 10;  // This also ends up in ROdata
const int uninitialized_const;

int main(int argc, char* argv[]) {
  // static const variable ends up in the RODATA segment of the elf binary
  // and thus gets mapped in as a read only page.
  // 
  // See objdump -hrt ./bazel-bin/static_const_inline
  // for details:
  /*
    SYMBOL TABLE:
    0000000000000254 l     O .note.ABI-tag  0000000000000020              __abi_tag
    00000000000008b4 l     F .text  0000000000000014              call_weak_fn
    0000000000000000 l    df *ABS*  0000000000000000              crtstuff.c
    0000000000020010 l     O .tm_clone_table        0000000000000000              __TMC_LIST__
    00000000000008d0 l     F .text  0000000000000000              deregister_tm_clones
    0000000000000900 l     F .text  0000000000000000              register_tm_clones
    0000000000000940 l     F .text  0000000000000000              __do_global_dtors_aux
    0000000000020010 l     O .bss   0000000000000001              completed.0
    000000000001fd28 l     O .fini_array    0000000000000000              __do_global_dtors_aux_fini_array_entry
    0000000000000990 l     F .text  0000000000000000              frame_dummy
    000000000001fd30 l     O .init_array    0000000000000000              __frame_dummy_init_array_entry
    0000000000000000 l    df *ABS*  0000000000000000              main.cc
    0000000000020018 l     O .bss   0000000000000001              _ZStL8__ioinit
    0000000000000a88 l     O .rodata        0000000000000001              _ZL5kFail
    0000000000000a8c l     O .rodata        0000000000000004              _ZZ4mainE10some_const
    00000000000009f4 l     F .text  0000000000000060              _Z41__static_initialization_and_destruction_0ii
    0000000000000a54 l     F .text  000000000000001c              _GLOBAL__sub_I_main.cc
    0000000000000000 l    df *ABS*  0000000000000000              crtstuff.c
    0000000000000b98 l     O .eh_frame      0000000000000000              __FRAME_END__
    00000000000007a0 l     F .init  0000000000000000              .hidden _init
    0000000000000a70 l     F .fini  0000000000000000              .hidden _fini
    0000000000020008 l     O .data  0000000000000000              .hidden __dso_handle
    0000000000020010 l     O .tm_clone_table        0000000000000000              .hidden __TMC_END__
    000000000001fd40 l     O .dynamic       0000000000000220              .hidden _DYNAMIC
    000000000001ff60 l     O .got   0000000000000048              .hidden _GLOBAL_OFFSET_TABLE_
    0000000000020019 g       .fini_array    0000000000000000              _end
    0000000000020010 g       .fini_array    0000000000000000              __bss_start
    0000000000020010 g       .fini_array    0000000000000000              _edata
    0000000000000000       F *UND*  0000000000000000              abort
    0000000000000880 g     F .text  0000000000000034              _start
    0000000000000994 g     F .text  0000000000000060              main
    0000000000020000  w      .data  0000000000000000              data_start
    0000000000000a84 g     O .rodata        0000000000000004              _IO_stdin_used
    0000000000000000       F *UND*  0000000000000000              __libc_start_main
    0000000000020000 g       .data  0000000000000000              __data_start
    0000000000000000  w      *UND*  0000000000000000              __gmon_start__
    0000000000000000       F *UND*  0000000000000000              __cxa_atexit
    0000000000000000  w      *UND*  0000000000000000              _ITM_deregisterTMCloneTable
    0000000000000000  w      *UND*  0000000000000000              _ITM_registerTMCloneTable
    0000000000000000  w    F *UND*  0000000000000000              __cxa_finalize
    0000000000000000       O *UND*  0000000000000000              _ZSt4cout
    0000000000000000       F *UND*  0000000000000000              _ZNSolsEi
    0000000000000000       F *UND*  0000000000000000              _ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
    0000000000000000       F *UND*  0000000000000000              _ZNSolsEPFRSoS_E
    0000000000000000       F *UND*  0000000000000000              _ZNSt8ios_base4InitC1Ev
    0000000000000000       F *UND*  0000000000000000              _ZNSt8ios_base4InitD1Ev
  */


  // Note if this was not static, it would be fine.
  static const int some_const = 1;

  // This is going to crash.
  int* c = const_cast<int*>(&global_const);    // Non-const pointer to a static const

  std::cout << uninitialized_const << std::endl;

  if (kFail) {
    *c = 2;     // Writing to an RO page causes the binary to crash.
  }

  std::cout << *c << std::endl;

  return 0;
}