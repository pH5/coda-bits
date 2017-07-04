Disclaimer
----------

The following information has been obtained by staring at the CODA firmware
files distributed by Freescale, for a really long time, until patterns emerged.
None of this is validated, anything could be wrong. Read this document not as
a statement of fact, but as a snapshot of the author's understanding of the BIT
processor instruction set. The mnemonics are completely made up. Code addresses
and mailbox register offsets are in units of 16-bit words, unless otherwise
noted.

The possibility to read the program counter in a running system helped to
identify a tight loop where the BIT processor spins waiting for the host to
issue a new command by writing to the RUN_COMMAND mailbox register, helpfully
demonstrating a load into an accumulator register from a mailbox register and a
conditional jump. Further, the known 32-bit return value of the FIRMWARE_GET
command allowed to identify the location where the firmware moves two known
immediate 16-bit values into the low bits of both accumulator registers,
combines them with a left shift and an or instruction, and stores the result
into a mailbox register as return value. The rest followed from there.

Instruction set encoding overview
---------------------------------

Instructions consist of either a single 16-bit instruction word, or for some
operations, of an instruction word followed by a 16-bit parameter word.

+------------------------------------+--------------+--------------------------------+
| Instruction word                   |Parameter word| Description                    |
+=======+======+=====+=+=+==+==+==+==+==============+================================+
|15 - 12|11 - 8|7 - 6|5|4|3 - 2| 1| 0| 15 - 0       |                                |
+-------+------+-----+-+-+-----+--+--+--------------+--------------------------------+
| 1010  | op   | operand          | A| --           | `immediate 7-bit operations`_  |
+-------+------+-----+------------+--+--------------+--------------------------------+
| 1110  | 0000 | 00  | op         | A| operand      | `immediate 16-bit operations`_ |
+-------+------+-----+---------+--+--+--------------+--------------------------------+
| 1110  | 0000 | 01  | op      | B| A| --           | `binary operations`_           |
+-------+------+-----+---+-----+--+--+--------------+--------------------------------+
| 1110  | 0000 | 1100    | 00  | S| A| --           | `mailbox loads and stores`_    |
+-------+------+---------+-----+--+--+--------------+--------------------------------+
| 1110  | 0001 | 1001    | 0000      | --           | `no-op`_                       |
+-------+------+-----+-+-+-----+--+--+--------------+--------------------------------+
| 1110  | 0100 | 00  |0|L| cond   | 0| address      | `absolute jumps and calls`_    |
+-------+------+-----+-+-+--------+--+--------------+--------------------------------+
| 1110  | 0100 | 0010    | cond   | 0| --           | `returns`_                     |
+-------+------+---------+--------+--+--------------+--------------------------------+
| 1111  | offset         | cond   | 0| --           | `relative jumps`_              |
+-------+----------------+--------+--+--------------+--------------------------------+

Instructions that operate on one or both of the two accumulator registers
contain accumulator register selection bits *A*, *B*. The *S* bit determines
whether a memory access is a store, and the *L* bit in the branch instructions
determines whether an absolute jump is a call that stores the next program
counter address for a later return.

Immediate 7-bit Operations
--------------------------

Arithmetic and bitwise operations with an accumulator register *A* and a 7-bit
immediate value *operand*.

+----------------------------+----------+----------------+
| Instruction word           | Mnemonic | Description    |
+=======+======+=====+=====+=+==========+================+
|15 - 12|11 - 8|7 - 4|3 - 1|0|          |                |
+-------+------+-----+-----+-+----------+----------------+
| 1010  | 0000 | operand   |A|``add.b`` | A += operand   |
+-------+------+-----------+-+----------+----------------+
| 1010  | 0001 | operand   |A|``sub.b`` | A -= operand   |
+-------+------+-----------+-+----------+----------------+
| 1010  | 0010 | operand   |A|``mv.b``  | A = operand    |
+-------+------+-----------+-+----------+----------------+
| 1010  | 1000 | operand   |A|``and.b`` | A &= operand   |
+-------+------+-----------+-+----------+----------------+
| 1010  | 1110 | operand   |A|``lsl``   | A <<= operand  |
+-------+------+-----------+-+----------+----------------+
| 1010  | 1111 | operand   |A|``lsr``   | A >>= operand  |
+-------+------+-----------+-+----------+----------------+

Immediate 16-bit operations
---------------------------

Arithmetic and bitwise operations with an accumulator register *A* and a 16-bit
immediate value *operand*. The instruction word is followed by a parameter word
containing the 16-bit immediate operand.

+-----------------------------+--------------+---------+------------------+
| Instruction word            |Parameter word|Mnemonic | Description      |
+=======+======+======+=====+=+==============+=========+==================+
|15 - 12|11 - 8| 7 - 4|3 - 1|0| 15 - 0       |         |                  |
+-------+------+------+-----+-+--------------+---------+------------------+
| 1110  | 0000 | 0000 | 001 |A| operand      |``sub.h``| A -= operand     |
+-------+------+------+-----+-+--------------+---------+------------------+
| 1110  | 0000 | 0000 | 010 |A| operand      |``mv.h`` | A = operand      |
+-------+------+------+-----+-+--------------+---------+------------------+
| 1110  | 0000 | 0000 | 100 |A| operand      |``and.h``| A &= operand     |
+-------+------+------+-----+-+--------------+---------+------------------+
| 1110  | 0000 | 0000 | 101 |A| operand      |``or.h`` | A |= operand     |
+-------+------+------+-----+-+--------------+---------+------------------+

Binary operations
-----------------

Arithmetic and bitwise operations with two accumulator registers *A* and *B*

+-------------------------------+--------------+---------+--------------+
| Instruction word              |Parameter word|Mnemonic | Description  |
+=======+======+======+=====+=+=+==============+=========+==============+
|15 - 12|11 - 8| 7 - 4|3 - 2|1|0| 15 - 0       |         |              |
+-------+------+------+-----+-+-+--------------+---------+--------------+
| 1110  | 0000 | 0100 | 00  |B|A| operand      |``add.w``| A += B       |
+-------+------+------+-----+-+-+--------------+---------+--------------+
| 1110  | 0000 | 0101 | 00  |B|A| operand      |``and.w``| A &= B       |
+-------+------+------+-----+-+-+--------------+---------+--------------+
| 1110  | 0000 | 0101 | 01  |B|A| operand      |``or.w`` | A |= B       |
+-------+------+------+-----+-+-+--------------+---------+--------------+

Peripheral Loads and Stores
---------------------------
Loads of 32-bit values from an *address* on the internal register bus into
accumulator register *A* and stores of 32-bit values from accumulator register
*A* to an *address* on the internal register bus. The instruction word is
followed by a parameter word containing the register address.

+-----------------------------+--------------+--------+------------------+
| Instruction word            |Parameter word|Mnemonic| Description      |
+=======+======+======+=====+=+==============+========+==================+
|15 - 12|11 - 8| 7 - 4|3 - 1|0| 15 - 0       |        |                  |
+-------+------+------+-----+-+--------------+--------+------------------+
| 1110  | 0000 | 1100 | 000 |A| address      |``ld.w``| A = reg[address] |
+-------+------+------+-----+-+--------------+--------+------------------+
| 1110  | 0000 | 1100 | 001 |A| address      |``st.w``| reg[address] = A |
+-------+------+------+-----+-+--------------+--------+------------------+

The internal register bus is connected to the host interface (mailbox) register
block and to the registers of the peripheral acceleration units.

No-op
-----

+----------------------------+----------+-------------+
| Instruction word           | Mnemonic | Description |
+=======+======+======+======+==========+=============+
|15 - 12|11 - 8| 7 - 4| 3 - 0|          |             |
+-------+------+------+------+----------+-------------+
| 1110  | 0001 | 1001 | 0000 | ``nop``  | --          |
+-------+------+------+------+----------+-------------+

Absolute jumps and calls
------------------------

These instructions move the program counter *PC* to an absolute *address*, if a
condition is met. ``call`` instructions additionally store *PC + 1* as return
address *RA* before *PC* is changed. The instruction word is followed by a
parameter word containing the absolute address.

+----------------------------+--------------+----------+---------------------------------+
| Instruction word           |Parameter word| Mnemonic | Description                     |
+=======+======+======+======+==============+==========+=================================+
|15 - 12|11 - 8| 7 - 4| 3 - 0| 15 - 0       |          |                                 |
+-------+------+------+------+--------------+----------+---------------------------------+
| 1110  | 0100 | 0000 | 0010 | address      |``jumple``| if (le) PC = address            |
+-------+------+------+------+--------------+----------+---------------------------------+
| 1110  | 0100 | 0000 | 1000 | address      |``jumpnz``| if (!z) PC = address            |
+-------+------+------+------+--------------+----------+---------------------------------+
| 1110  | 0100 | 0000 | 1010 | address      |``jumpz`` | if (z) PC = address             |
+-------+------+------+------+--------------+----------+---------------------------------+
| 1110  | 0100 | 0000 | 1110 | address      |``jump``  | PC = address                    |
+-------+------+------+------+--------------+----------+---------------------------------+
| 1110  | 0100 | 0000 | 0010 | address      |``callle``| if (le) RA = PC+1, PC = address |
+-------+------+------+------+--------------+----------+---------------------------------+
| 1110  | 0100 | 0000 | 1000 | address      |``callnz``| if (!z) RA = PC+1, PC = address |
+-------+------+------+------+--------------+----------+---------------------------------+
| 1110  | 0100 | 0000 | 1010 | address      |``callz`` | if (z) RA = PC+1, PC = address  |
+-------+------+------+------+--------------+----------+---------------------------------+
| 1110  | 0100 | 0000 | 1110 | address      |``call``  | RA = PC+1, PC = address         |
+-------+------+------+------+--------------+----------+---------------------------------+

Returns
-------

Return instructions move the program counter to the return address *RA* stored
by the last ``call`` instruction, if a condition is met.

+----------------------------+----------+-----------------+
| Instruction word           | Mnemonic | Description     |
+=======+======+======+======+==========+=================+
|15 - 12|11 - 8| 7 - 4| 3 - 0|          |                 |
+-------+------+------+------+----------+-----------------+
| 1110  | 0100 | 0010 | 0010 |``retle`` | if (le) PC = RA |
+-------+------+------+------+----------+-----------------+
| 1110  | 0100 | 0010 | 1000 |``retnz`` | if (!z) PC = RA |
+-------+------+------+------+----------+-----------------+
| 1110  | 0100 | 0010 | 1010 |``retz``  | if (z) PC = RA  |
+-------+------+------+------+----------+-----------------+
| 1110  | 0100 | 0010 | 1110 |``ret``   | PC = RA         |
+-------+------+------+------+----------+-----------------+

Relative jumps
--------------

These instructions move the program counter *PC* relative to its current value,
by a signed 8-bit *offset*, if a condition is met.

+----------------------------+--------+----------------------+
| Instruction word           |Mnemonic| Description          |
+=======+======+======+======+========+======================+
|15 - 12|11 - 8| 7 - 4| 3 - 0|        |                      |
+-------+------+------+------+--------+----------------------+
| 1111  | offset      | 0010 |``jle`` | if (le) PC += offset |
+-------+-------------+------+--------+----------------------+
| 1111  | offset      | 1000 |``jnz`` | if (!z) PC += offset |
+-------+-------------+------+--------+----------------------+
| 1111  | offset      | 1010 |``jz``  | if (z) PC += offset  |
+-------+-------------+------+--------+----------------------+
| 1111  | offset      | 1110 |``j``   | PC += offset         |
+-------+-------------+------+--------+----------------------+
