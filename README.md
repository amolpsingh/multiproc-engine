# multiproc-rpi
CS140E Final Project
Finish implementing the code in:
   - `get-code.c`, which has the routine `get_code` that does the actual loading.
     This is the only file you'll have to modify:

You'll have to look at:
   - `get-code.h`, which has a bunch of helpers.

   - `simple-boot.h`: the values used for the protocol (refer to the lab's `README` for
      how to use them). 
   - `staff-crc32.h`: code to compute the checksum.  You'll have to call this.
   - `main.c`: the trivial driver to call `get_code`.

The other files are from us:
   - `boot-start.S`:  look at how it differs from `libpi:staff-start.S`.

## The Unix Side (program.c)

In order to send multiple information to different Raspberry Pi's we would need to also write a function (e.g get_all_tty_usbs()) that returns a list of all the usb's connected to a machine that correspond to UARTs for a Raspberry Pi's. This would allow us to send bytes representing the binaries' directly to the PI's across different UARTs. 

We must come up with a file-parser that reads in a file that holds binaries and its dependencies that need to run before that binary is run on some Raspberry Pi.As an example consider the following file..


                **learn-to-talk.bin**: NONE


                **learn-to-speak-english.bin**: NONE


                **hello.bin**: learn-to-talk.bin, learn-to-speak-english.bin


                **say-goodbye.bin**: hello.bin

We would then like to read in from this file, and learn that there are two binaries that do not depend on any binaries to finish before they can, learn-to-talk.bin and earn-to-speak-english.bin. Thus, the file-parser should then be able to store this information in some graph like data structure which we will elaborate on later. Thus, we would then be able to directly send **learn-to-talk.bin** and** learn-to-speak-english.bin** to a Raspberry pi to immediately start executing.  And then, after learning that these binaries are finished, we should be able to see that we can now finally run our **hello.bin**. 

Additionally, not only do we need to write the parser itself but also need to hold the information in some class (e.g. Class Dependency) that holds all the dependencies in some directed graph that also stores which dependencies are finished running, currently running, or on queue. This is so that we receive a signal from the PI that was assigned some program X, that that program is now done. 

We will check whether or not a PI is available by continuously pinging the pi, asking for it to send the value at some address (e.g. 0x8000000) and if that byte is 1, we know it is free and if it is 0, we know it is currently running some program. Currently, the exact specifications are to be determined but they should be uncovered fairly soon.

 


## The Pi Side (image.bin)

The Raspberry Pi should have a bootloader that is continuously running, asking for work from the unix side of the program paradigm. It should also be able to do a number of things, such as indicate when it is done, indicate when it is running something, and also when it is idle. 

We currently postulate that the best way to know whether the Raspberry Pi is finished or free, is to hardcode a value at some address that allows the Unix Side to query the Raspberry Pi, and return the value at that address. Of course, we will have to consider the fact that at bootup, there is no guarantee for what values there are at that address. One way to mitigate this would be to hardcode into the image (which is continuously asking for work) to write some integer (e.g. 0) to a specific address where we are guaranteed not to overwrite. Thus, from the very moment the Raspberry Pi is turned on, it will have written a value to that address and thus can indicate that it is available. 

Additionally, the moment that the Raspberry Pi receives a program, it should write a value (e.g. 0xdab) so that when we query the pi, it indicates that it is currently running a program. There are plenty of other values we can use to indicate the different status's the pi could be running in. 

The MIracle:

The miracle in all of this; is to find a way to return values from the Raspberry PI. I feel that as long as we can show that this is faster than sequential programming, then we have a dub. Regardless of whether or not we can actually Pipe input from one pi to another, and feed it in as an argument to the PI. 