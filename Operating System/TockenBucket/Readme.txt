Multi-threaded Token Bucket Emulation in C
++++++++++++++++++++++++++++++++++++++++++++++++++++

Description Link:= http://merlot.usc.edu/cs402-s18/projects/warmup2/


Project description• Emulated a traffic shaper which transmits packets controlled by a token bucket using multi-threading.
• Utilized 5 thread for packet arrival, token arrival, two servers, and a signal handling thread.
• Used mutex for synchronization and conditional variable to avoid busy-wait.