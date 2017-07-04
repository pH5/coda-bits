BIT processor overview
======================

The CODA VPUs contain an embedded programmable 16-bit DSP processor called
BIT processor. According to a CodaDx6 Datasheet that was once freely
downloadable from the `Chips&Media web site`_, the BIT processor contains
separate program memory SRAM and data memory SRAM.

.. _`Chips&Media web site`: http://www.chipsnmedia.com

Instructions can be written directly into the boot code region (lower 4 KiB) of
the program memory while the BIT processor is not running, by storing address
and value into a special register in the host interface register region from
the host.

BIT processor units
-------------------

The BIT processor core consists of these units that are directly involved
in execution of the firmware code in program memory:

+-----+------------------------------------------------------------------------+
| Unit| Description                                                            |
+=====+========================================================================+
| PFU | The prefetch unit loads instructions from program memory. It contains  |
|     | the program counter *PC*, which is controlled with the ``jump``,       |
|     | ``call``, ``ret``, and ``j`` instructions.                             |
+-----+------------------------------------------------------------------------+
| CTU | The control unit contains the instruction decoder.                     |
+-----+------------------------------------------------------------------------+
| AGU | The address generation unit contains 8 16-bit address registers and an |
|     | arithmetic unit for the data memory.                                   |
+-----+------------------------------------------------------------------------+
| ALU | The arithmetic logic unit operates on two 36-bit accumulator           |
|     | registers. It handles arithmetic and logic operations such as ``add``, |
|     | ``sub``, ``mv``, ``and``, ``or``, ``lsl``, and ``lsr`` instructions.   |
+-----+------------------------------------------------------------------------+
| MPU | The multiply unit operates on two 16-bit 2's complement inputs,        |
|     | returning the product in a 32-bit value.                               |
+-----+------------------------------------------------------------------------+
| PCU | The peripheral control unit is connected to the                        |
|     | `internal peripheral bus`_ and handles loads and stores to the host    |
|     | interface register block and to the hardware accelerator unit          |
|     | registers. It handles the 32-bit ``ld.w`` and ``st.w`` instructions.   |
+-----+------------------------------------------------------------------------+
| MCU | The memory control unit connects to the data memory.                   |
+-----+------------------------------------------------------------------------+

The BIT processor further contains a hardware stack with 16 dedicated stack
registers, which probably handles the call stack for ``call`` and ``ret``
instructions and it has the ability to switch context on interrupts.

There is a bitstream read/write unit (GBU) that accelerates bitstream
(un)packing operations, supposedly as part of the core.

Internal Peripheral Bus
-----------------------

The internal peripheral bus (IPB) connects the peripheral control unit to the
following register blocks:

+------+-----------------------------------------------------------------------+
| HOST | The host interface (mailbox) register block is accessible from the    |
|      | host via the APB bus.                                                 |
+------+-----------------------------------------------------------------------+
| MMU  | The MMU interface connects to the AXI bus and provides a DMA          |
|      | controller for transfers between program/data memory and external     |
|      | SDRAM.                                                                |
+------+-----------------------------------------------------------------------+
| MVP  | Motion vector prediction and reconstruction unit                      |
+------+-----------------------------------------------------------------------+
| TC   | The transform and coefficient management block handles entropy        |
|      | encoding/decoding and interfaces with the video codec transform       |
|      | coefficient buffers.                                                  |
+------+-----------------------------------------------------------------------+
| NB   | Neighboring macroblock information unit for h.264 and MPEG4.          |
+------+-----------------------------------------------------------------------+
| NAL  | The NAL accelerator accelerates parsing and packing of h.264 Annex B  |
|      | NAL streams.                                                          |
+------+-----------------------------------------------------------------------+

The IPB is also connected to the host APB via a bridge. Its address bus is
12-bit wide and supports 32-bit word access only. The least significant two
bits of the address are ignored and should be zero.
