# CHIP 8 EMULATION
This project is going to be my entry into emulation development. Initially I wanted to get into
creating a gameboy emulator, however I am bit too overwhelmed so I decided on the *Hello World* 
of emulation. This README will serve as a small journal through my learnings. I will place the
references I use down below.

# SPECIFICATIONS
- Memory: CHIP-8 has direct access to up to 4 kilobytes of RAM
- Display: 64 x 32 pixels (or 128 x 64 for SUPER-CHIP) monochrome, ie. black or white
- A program counter, often called just “PC”, which points at the current instruction in memory
- One 16-bit index register called “I” which is used to point at locations in memory
- A stack for 16-bit addresses, which is used to call subroutines/functions and return from them
- An 8-bit delay timer which is decremented at a rate of 60 Hz (60 times per second) until it reaches 0
- An 8-bit sound timer which functions like the delay timer, but which also gives off a beeping sound as long as it’s not 0
- 16 8-bit (one byte) general-purpose variable registers numbered 0 through F hexadecimal, ie. 0 through 15 in decimal, called V0 through VF
- VF is also used as a flag register; many instructions will set it to either 1 or 0 based on some rule, for example using it as a carry flag


# IMPORTANT NOTES
Fetch, Decode, Execute loop an emulator does these three task:
- *Fetch* the instructions from memeory at the current PC (program timer)
  - Instructioins are 2 successive bytes 
  - Combines them into 16-bit instructions
- *Decode* the instructions to find out what the emulator should do
- *Execute* the instructions




# REFERENCES 
[Tobias V. Langhoff Article](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)
[Stack Abstract](https://en.wikipedia.org/wiki/Stack_abstract_data_type)




