# SPDX-License-Identifier: 0BSD

project('coda-bits', 'c',
	version : '0.1',
	license : '0BSD')

add_global_arguments('-Wall', language : 'c')

bitdis_sources = [
	'src/bitdis.c',
	'src/iset.h',
	'src/regs.h',
	'src/reorder.c',
	'src/reorder.h'
]
executable('bitdis', bitdis_sources, install : true)

bitspl_sources = [
	'src/bitspl.c',
	'src/reorder.c',
	'src/reorder.h'
]
executable('bitspl', bitspl_sources, install : true)

coda_fwinfo_sources = [
	'src/coda-fwinfo.c',
	'src/reorder.c',
	'src/reorder.h'
]
executable('coda-fwinfo', coda_fwinfo_sources, install : true)

codabits_docs = [
	'README.rst',
	'doc/bit-isa.rst',
	'doc/bit.rst'
]
if get_option('docs')
	install_data(codabits_docs, install_dir: 'share/doc/coda-bits')
endif
