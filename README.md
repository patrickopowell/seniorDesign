The loadable linux kernel modules are located in /lib/modules.
And the extension is *.ko
Loadable kernel modules in Linux are loaded (and unloaded) by the modprobe command.
The lsmod command lists the loaded kernel modules.

We configure our module in: 	/etc/modprobe.conf

/lib/modules/version/modules.dep specifies if other modules must be loaded before our module can be loaded

modules.dep is created by $ depmod -a

Linux distros provide modprobe, insmod and depmod as a package called module-init-tools.

A module compiled for one kernel won't load if you boot a different kernel unless you enable CONFIG_MODVERSIONS in the kernel

Modules can't print to the screen like printf() can, but they can log information and warnings.

Logs are stored here: /var/log/messages via printk().

Each printk() statement comes with a priority, which is the <1>.
There are 8 priorities and the kernel has macros for them, so you don't have to use cryptic numbers, 
and you can view them (and their meanings) in linux/kernel.h. 
If the priority is less than int console_loglevel, the message is printed on your current terminal.

Compiling:
 linux/Documentation/kbuild/modules.txt

You can insert your module into the kernel by running: $ insmod <file_name>
You can remove your module by running: $ rmmod <file_name>
