/* od-elf32_avr.c -- dump information about an elf32-avr object file.
   Copyright (C) 2011-2014 Free Software Foundation, Inc.

   This file is part of GNU Binutils.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"
#include <stddef.h>
#include <time.h>
#include <stdint.h>
#include "safe-ctype.h"
#include "bfd.h"
#include "objdump.h"
#include "bucomm.h"
#include "bfdlink.h"
#include "bfd.h"
#include "elf/external.h"
#include "elf/internal.h"
#include "elf32-avr.h"

/* Index of the options in the options[] array.  */
#define OPT_MEMUSAGE 0
#define OPT_AVRPROP 1

/* List of actions.  */
static struct objdump_private_option options[] =
  {
    { "mem-usage", 0 },
    { "avr-prop",  0},
    { NULL, 0 }
  };

/* Display help.  */

static void
elf32_avr_help (FILE *stream)
{
  fprintf (stream, _("\
For AVR ELF files:\n\
  mem-usage   Display memory usage\n\
  avr-prop    Display contents of .avr.prop section\n\
"));
}

/* Return TRUE if ABFD is handled.  */

static int
elf32_avr_filter (bfd *abfd)
{
  return bfd_get_flavour (abfd) == bfd_target_elf_flavour;
}

static void
elf32_avr_dump_avr_prop (bfd *abfd)
{
  struct avr_property_header *header;
  unsigned int i;

  header = avr_elf32_load_property_records (abfd);
  if (header == NULL)
    return;

  printf ("\nContents of `%s' section:\n\n", header->section->name);

  printf ("  Version: %d\n", header->version);
  printf ("  Flags:   %#x\n\n", header->flags);

  for (i = 0; i < header->record_count; ++i)
    {
      printf ("   %d %s @ %s + %#08lx (%#08lx)\n",
              i,
              avr_elf32_property_record_name_string (&header->records [i]),
              header->records [i].section->name,
              header->records [i].offset,
              (bfd_get_section_vma (abfd, header->records [i].section)
               + header->records [i].offset));
      switch (header->records [i].type)
        {
        case RECORD_ORG:
          /* Nothing else to print.  */
          break;
        case RECORD_ORG_AND_FILL:
          printf ("     Fill: %#08lx\n",
                  header->records [i].data.org.fill);
          break;
        case RECORD_ALIGN:
          printf ("    Align: %#08lx\n",
                  header->records [i].data.align.bytes);
          break;
        case RECORD_ALIGN_AND_FILL:
          printf ("    Align: %#08lx, Fill: %#08lx\n",
                  header->records [i].data.align.bytes,
                  header->records [i].data.org.fill);
          break;
        }
    }

  free (header);
}

static void
elf32_avr_dump (bfd *abfd)
{
  if (options[OPT_AVRPROP].selected)
    elf32_avr_dump_avr_prop (abfd);
}

const struct objdump_private_desc objdump_private_desc_elf32_avr =
  {
    elf32_avr_help,
    elf32_avr_filter,
    elf32_avr_dump,
    options
  };
