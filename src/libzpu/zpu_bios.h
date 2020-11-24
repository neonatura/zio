
/*
 * ref: https://en.wikipedia.org/wiki/BIOS_interrupt_call
 * ref: https://en.wikipedia.org/wiki/INT_10H
 */
#define ZBIOS_INT_SHIFT_PRINT_SCREEN 0x05
#define ZBIOS_INT_VIDEO 0x10

#define ZBIOS_VIDEO_MODE 0x0

/**
 * Set the video device's cursor position.
 * @params a0 page
 * @params a1 row
 * @params a2 col
 * @note INT 10h,2h
 */
#define ZBIOS_VIDEO_CURSOR_POS 0x02

#define ZBIOS_MEM_ADDR 0x00FFFFF0

/* notes:
 * use PRIVATE mmap for I/O ..
  (zdb_t *) compatible
  map "zdb_get_ref()" direct references to mmap into fifo->fifo[X]

bios
  shadow all "option roms" defined in bios and from devices
  register i/o devices
  inline vga driver
  BCV table function pointers to option roms
    options roms are copied from mem to stack and ran
  interrupt vector
    a mapping of bios system call addresses on the stack [or possible mem addr]
  POST
    print "ZIO BIOS v5.0 (built XX/XX/XX)\n" to video output "console"
    self-test of rom
    self-test of ram
      for() print "ZIO MEM ADDR [0x0000 - 0x0FFFF] ram (pages 0x0000 - 0xFFFF)\n"
    perform vid/did conf request on all i/o to verify, update 'status' conf register (NONE -> INIT || NONE -> DISABLE)
      for() print "ZIO I/O ADDR [0x1001] video (VGA@1001 320x200 16bit)\n"
  call devices's "AddOn ROM"
  configure i/o devices
  --
  initialize/setup CSRs
  --
  PrintChar(x)
    rem prints char to video output
    mov cmd: print char on screen
    mov page number (where/offset)
    mov fg/bg
    mov char
    int video
    rts
  PrintLine(s)

  --
  bootloader 0xaa55

io-device:
  configuration registers
    mapped to memory locations
    device id (did)
    vendor id (vid)
    status
    command


*/
