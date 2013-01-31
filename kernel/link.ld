OUTPUT_FORMAT("binary")
ENTRY(start)
SECTIONS
{
  .text : {
    *(.text)
  }
}
