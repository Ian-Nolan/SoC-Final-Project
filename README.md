# SoC-Final-Project

Project Description:
I recently purchased a MIDI keyboard controller, the M-Audio Keystation 61 MK3. I can read the MIDI data through the USB with my laptop to play sounds out, but I also want to know how the 5-pin DIN MIDI output works and I want to visualize it using my Nexys 7 DDR FPGA. By searching for the MIDI standard online, I can find what each of the five pins does and what hardware is required to read the output. Once I have a readable signal, I can use the MIDI protocol to design a ASM chart to help me decode it. With my ASM chart finished, I will easily be able to implement it onto my FPGA. With the signals decoded, I will be able to print out key presses on the 8-digit 7-segment display.

Current Status:
After 3 hours of tinkering and ignoring the documentation, I finally found some good documentation and built the prescribed curcuit. With the circuit built, I connected the output to an oscilloscope and found that the output is actually inverted in comparison to what I was anticipating.

Next Step(s):
Since MIDI is similar to UART, being an asynchronous serial signal, I can also utilize a UART ASM chart to help me start making my MIDI ASM. I will write that out and compare with any MIDI ASM chart I find online. I will then write a small module to test the 7-segment display and a decoder module to translate the MIDI information into recognizable key presses using a map/database that I will make. I will then implement my MIDI module that will read the input data from a GPIO pin.


