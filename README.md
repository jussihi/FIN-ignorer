# FIN ignorer

This ko simply ignores FIN flags in TCP streams. 

**Usage:**

I heard that some pay-for-WLAN providers use the FIN flag to restrict the WLAN connectivity of non-paying users.

To run it, simply compile using Makefile and load with `# insmod fin_ignorer.ko`

Huge thanks to [Hansformer's VR-Wifi-Simulator](https://github.com/Hansformer/VR-Wifi-Simulator) for getting me started with Linux kernel drivers.
