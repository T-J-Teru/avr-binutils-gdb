#name: AVR, check link-relax flag is set final link (first input relaxable)
#as: -mmcu=avrxmega2
#ld: -relax -mavrxmega2
#source: relax-elf-flags-a.s -mlink-relax
#source: relax-elf-flags-b.s
#readelf: -h
#target: avr-*-*

ELF Header:
#...
  Flags:                             0xe6
#...