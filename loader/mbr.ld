OUTPUT_FORMAT("binary")
ENTRY(boot)
SECTIONS
{
  .text : {
    *(.text)
  }
}

