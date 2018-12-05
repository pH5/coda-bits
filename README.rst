CODA bits
=========

This repository contains bits and pieces of information about the CODA series
of video processing units found in i.MX application processors. There is a
`firmware identification tool`_, a `firmware splitter`_, a rudimentary
disassembler_, and some unfinished documentation:

* `BIT processor overview`_
* `Instruction set description`_

.. _`BIT processor overview`: doc/bit.rst
.. _`Instruction set description`: doc/bit-isa.rst

Setup and build
---------------

To configure the build system and build the tools, follow the standard Meson_
build procedure::

  $ meson build
  $ cd build
  $ ninja

.. _Meson: http://mesonbuild.com

Firmware identification tool
----------------------------

The coda-fwinfo tool matches a machine code template against the firmware file
to determine the product id, firmware version number, and code revision
(CODA960 only) as returned by the FIRMWARE_VERNUM command if run on the
hardware::

  $ coda-fwinfo vpu_fw_imx6q.bin
  Firmware in host memory order
  Product ID: 0xf020, CODA960 (i.MX6)
  Version Number: 0x3101 (3.1.1)
  Code Revision: 46072

Firmware splitter
-----------------

The bitspl tool looks for reset vector signatures and splits the firmware into
individual components at those boundaries::

  $ bitspl vpu_fw_imx6q.bin
  vector at 0x00000, writing 00000.bin
  vector at 0x05000, writing 05000.bin
  [...]

For example, the i.MX6Q firmware version 3.1.1 splits into the following parts.
Their function can be determined from the code at address 0x01c4, which checks
the RUN_COD_STD and RUN_AUX_STD parameters:

========= ================
Part      Function
========= ================
00000.bin h.264 AVC decode
05000.bin
0a000.bin h.264 MVC decode
0f000.bin VC1 decode
13000.bin MPEG-2 decode
16000.bin MPEG-4 decode
1a000.bin DivX 3 decode
1d000.bin RV decode
1f800.bin AVS decode
22800.bin MJPEG decode
24800.bin Theora decode
28800.bin VP6 decode
2c800.bin VP8 decode
31800.bin h.264 AVC encode
35000.bin MPEG-4 encode
38000.bin
3a000.bin h.264 MVC encode
========= ================

Note that the output of bitspl is always in BIT processor order, even if the
input is in host memory order::

  $ coda-fwinfo 00000.bin
  Firmware in BIT processor memory order
  [...]

The parts can be put back together using cat::

  $ cat ?????.bin > vpu_fw_imx6q.bin

Disassembler
------------

The bitdis tool is a rudimentary disassembler that translates the firmware
machine code into made up mnemonics, as far as the instruction set is
understood::

  $ bitdis 00000.bin
  Firmware in host memory order
  Disassembly of 00000.bin:

    0000: e40e 0020       jump    $0, 0x40
  [...]

Firmware download
-----------------

The CODA firmware binaries can be downloaded as part of the i.MX firmware
package provided by NXP. Here is how to download and extract a few versions of
the CODA VPU firmware files::

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-3.0.35-4.0.0.bin
  $ dd if=firmware-imx-3.0.35-4.0.0.bin bs=15586 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-3.5.7-1.0.0.bin
  $ dd if=firmware-imx-3.5.7-1.0.0.bin bs=15584 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-3.10.17-1.0.0.bin
  $ dd if=firmware-imx-3.10.17-1.0.0.bin bs=28056 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-3.14.28-1.0.0.bin
  $ dd if=firmware-imx-3.14.28-1.0.0.bin bs=33936 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-5.0.bin
  $ dd if=firmware-imx-5.0.bin bs=34172 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-5.2.bin
  $ dd if=firmware-imx-5.2.bin bs=34172 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-5.3.bin
  $ dd if=firmware-imx-5.3.bin bs=34479 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-5.4.bin
  $ dd if=firmware-imx-5.4.bin bs=34087 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-6.0.bin
  $ dd if=firmware-imx-6.0.bin bs=36566 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-7.1.bin
  $ dd if=firmware-imx-7.1.bin bs=36909 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-7.2.bin
  $ dd if=firmware-imx-7.2.bin bs=36909 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-7.4.bin
  $ dd if=firmware-imx-7.4.bin bs=36909 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-7.5.bin
  $ dd if=firmware-imx-7.5.bin bs=37083 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-7.6.bin
  $ dd if=firmware-imx-7.6.bin bs=37623 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-7.7.bin
  $ dd if=firmware-imx-7.7.bin bs=37622 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-7.8.bin
  $ dd if=firmware-imx-7.8.bin bs=38868 skip=1 | tar xj

  $ wget http://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-imx-7.9.bin
  $ dd if=firmware-imx-7.9.bin bs=38868 skip=1 | tar xj

These all contain the same firmware versions for i.MX 27 to i.MX53 in the
firmware/vpu subdirectory:

========== ======== ==================== =======
SoC        CODA     Firmware             Version
========== ======== ==================== =======
i.MX27 TO1 CodaDx6  vpu_fw_imx27_TO1.bin 1.7.1
i.MX27 TO2 CodaDx6  vpu_fw_imx27_TO2.bin 2.2.2
i.MX51     CodaHx14 vpu_fw_imx51.bin     1.4.50
i.MX53     CODA7541 vpu_fw_imx53.bin     1.4.50
========== ======== ==================== =======

The firmware versions for i.MX6Q/D and i.MX6DL/S differ between the packages:

========= ======= ================
SoC       CODA    Firmware
========= ======= ================
i.MX6Q/D  CODA960 vpu_fw_imx6q.bin
i.MX6DL/S CODA960 vpu_fw_imx6d.bin
========= ======= ================

=============== =======================
Package version Firmware version
=============== =======================
3.0.38-4.0.0    2.1.9 r36350
3.5.7-1.0.0     2.3.10 r40778
3.10.17-1.0.0   3.1.1 r46056
3.14.28-1.0.0   3.1.1 r46063
5.0             3.1.1 r46067
5.2             3.1.1 r46070
5.3             3.1.1 r46070
5.4             3.1.1 r46072
6.0-7.1         3.1.1 r46075 (0xb3fb)
7.2-7.9         3.1.1 r570363 (0x8b3fb)
=============== =======================
