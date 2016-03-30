#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xa139692d, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x1a28d412, __VMLINUX_SYMBOL_STR(kobject_put) },
	{ 0xc247dd25, __VMLINUX_SYMBOL_STR(kset_create_and_add) },
	{ 0xfdd250d8, __VMLINUX_SYMBOL_STR(d_path) },
	{ 0x20cdf864, __VMLINUX_SYMBOL_STR(kmem_cache_destroy) },
	{ 0xa11d68b3, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xda3e43d1, __VMLINUX_SYMBOL_STR(_raw_spin_unlock) },
	{ 0xe355059f, __VMLINUX_SYMBOL_STR(__mark_inode_dirty) },
	{ 0xb3c9d5d5, __VMLINUX_SYMBOL_STR(mntget) },
	{ 0x754d539c, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x80a9bdcd, __VMLINUX_SYMBOL_STR(dget_parent) },
	{ 0xed69b123, __VMLINUX_SYMBOL_STR(kobject_uevent) },
	{ 0x52cbb014, __VMLINUX_SYMBOL_STR(lockref_get) },
	{ 0xf67ea7bf, __VMLINUX_SYMBOL_STR(dput) },
	{ 0xaebba10c, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xe0cb0001, __VMLINUX_SYMBOL_STR(kobject_del) },
	{ 0x49bdde2e, __VMLINUX_SYMBOL_STR(mutex_trylock) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0xe754075f, __VMLINUX_SYMBOL_STR(kobject_create_and_add) },
	{ 0x97ea9ec7, __VMLINUX_SYMBOL_STR(kern_path) },
	{ 0xd7cc65c0, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0x449ad0a7, __VMLINUX_SYMBOL_STR(memcmp) },
	{ 0xa1c76e0a, __VMLINUX_SYMBOL_STR(_cond_resched) },
	{ 0x9166fada, __VMLINUX_SYMBOL_STR(strncpy) },
	{ 0xc795bcf7, __VMLINUX_SYMBOL_STR(kmem_cache_free) },
	{ 0xfc3492c1, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0x147869d1, __VMLINUX_SYMBOL_STR(setattr_copy) },
	{ 0xea4725b8, __VMLINUX_SYMBOL_STR(kobject_add) },
	{ 0xd8c335ef, __VMLINUX_SYMBOL_STR(mntput) },
	{ 0x8d7b193b, __VMLINUX_SYMBOL_STR(module_put) },
	{ 0x4d9f7dd1, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x6bfc2a7e, __VMLINUX_SYMBOL_STR(fs_kobj) },
	{ 0x1657e225, __VMLINUX_SYMBOL_STR(sysfs_remove_file_ns) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0xd7af2116, __VMLINUX_SYMBOL_STR(inode_change_ok) },
	{ 0xcbdc27ae, __VMLINUX_SYMBOL_STR(path_put) },
	{ 0xd88656ed, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xd52bf1ce, __VMLINUX_SYMBOL_STR(_raw_spin_lock) },
	{ 0x45b2a440, __VMLINUX_SYMBOL_STR(kmem_cache_create) },
	{ 0xa46c3de2, __VMLINUX_SYMBOL_STR(iput) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xd1ffcfdf, __VMLINUX_SYMBOL_STR(generic_permission) },
	{ 0xa07ddcd6, __VMLINUX_SYMBOL_STR(kobject_init) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xb0e602eb, __VMLINUX_SYMBOL_STR(memmove) },
	{ 0x936407bb, __VMLINUX_SYMBOL_STR(sysfs_create_file_ns) },
	{ 0x1314e6c, __VMLINUX_SYMBOL_STR(try_module_get) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "84201260D711686236244DC");
