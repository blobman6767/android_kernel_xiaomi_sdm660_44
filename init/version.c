/*
 *  linux/init/version.c
 *
 *  Copyright (C) 1992  Theodore Ts'o
 *
 *  May be freely distributed as part of Linux.
 */

#include <generated/compile.h>
#include <linux/module.h>
#include <linux/uts.h>
#include <linux/utsname.h>
#include <generated/utsrelease.h>
#include <linux/version.h>
#include <linux/proc_ns.h>

/*
 * Spoof the kernel identity reported to userspace so /proc/version and uname()
 * match stock MIUI V12.5.3.0.QFGMIXM instead of revealing a custom build.
 *
 * /proc/version is rendered from linux_proc_banner with utsname() values
 * substituted at read time, so SUSFS's syscall-level set_uname cannot reach it;
 * patching init_uts_ns here covers both /proc/version and uname().
 *
 * UTS_RELEASE is deliberately NOT changed: module vermagic derives from it, and
 * keeping it at the real 4.4.365-PLTS-perf prevents MIUI's incompatible
 * prebuilt qca_cld3_wlan.ko (built for 4.4.192-perf+) from ever loading here.
 */
#define SPOOF_UTS_RELEASE  "4.4.192-perf+"
#define SPOOF_UTS_VERSION  "#1 SMP PREEMPT Fri Dec 10 13:53:37 WIB 2021"
#define SPOOF_COMPILE_BY   "builder"
#define SPOOF_COMPILE_HOST "c4-miui-ota-bd169.bj"
#define SPOOF_COMPILER     "gcc version 4.9.x 20150123 (prerelease) (GCC) "

#ifndef CONFIG_KALLSYMS
#define version(a) Version_ ## a
#define version_string(a) version(a)

extern int version_string(LINUX_VERSION_CODE);
int version_string(LINUX_VERSION_CODE);
#endif

struct uts_namespace init_uts_ns = {
	.kref = KREF_INIT(2),
	.name = {
		.sysname	= UTS_SYSNAME,
		.nodename	= UTS_NODENAME,
		.release	= SPOOF_UTS_RELEASE,
		.version	= SPOOF_UTS_VERSION,
		.machine	= UTS_MACHINE,
		.domainname	= UTS_DOMAINNAME,
	},
	.user_ns = &init_user_ns,
	.ns.inum = PROC_UTS_INIT_INO,
#ifdef CONFIG_UTS_NS
	.ns.ops = &utsns_operations,
#endif
};
EXPORT_SYMBOL_GPL(init_uts_ns);

/* FIXED STRINGS! Don't touch! */
const char linux_banner[] =
	"Linux version " SPOOF_UTS_RELEASE " (" SPOOF_COMPILE_BY "@"
	SPOOF_COMPILE_HOST ") (" SPOOF_COMPILER ") " SPOOF_UTS_VERSION "\n";

const char linux_proc_banner[] =
	"%s version %s"
	" (" SPOOF_COMPILE_BY "@" SPOOF_COMPILE_HOST ")"
	" (" SPOOF_COMPILER ") %s\n";
