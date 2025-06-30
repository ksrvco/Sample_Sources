#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timekeeping.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kthread.h>

static int __init sum_module_init(void) {
    unsigned long long sum = 0;
    ktime_t start, end;
    u64 delta_ns;
    char cmd[256];

    pr_info("Kernel Module: Starting summation\n");

    start = ktime_get();
    for (unsigned int i = 1; i <= 1000000; ++i) {
        sum += i;
    }
    end = ktime_get();

    delta_ns = ktime_to_ns(ktime_sub(end, start));

    u64 delta_us = delta_ns / 1000;
    u64 delta_ms = delta_ns / 1000000;

    pr_info("Kernel Mode Sum: %llu\n", sum);
    pr_info("Time Taken (Kernel Mode): %llu ns (%llu us, %llu ms)\n",
            delta_ns, delta_us, delta_ms);

    snprintf(cmd, sizeof(cmd),
             "echo \"Kernel Mode: Sum = %llu, Time = %llu ns (%llu us, %llu ms)\" >> /root/timecaptured",
             sum, delta_ns, delta_us, delta_ms);

    {
        static char *envp[] = {
            "HOME=/root",
            "PATH=/sbin:/bin:/usr/sbin:/usr/bin",
            NULL
        };
        char *argv[] = { "/bin/sh", "-c", cmd, NULL };
        call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
    }

    return 0;
}

static void __exit sum_module_exit(void) {
    pr_info("Kernel Module: Unloaded\n");
}

module_init(sum_module_init);
module_exit(sum_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kamran");
MODULE_DESCRIPTION("Kernel sum with time logging to /root/timecaptured (integer only)");