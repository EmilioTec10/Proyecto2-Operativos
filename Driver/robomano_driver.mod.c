#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x2c635209, "module_layout" },
	{ 0xcfe1e2f9, "class_unregister" },
	{ 0xdff7669f, "device_destroy" },
	{ 0x3ee2b36d, "class_destroy" },
	{ 0xcf2a881c, "device_create" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x9b0fb107, "__class_create" },
	{ 0x8120e312, "__register_chrdev" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0xf8c7264e, "filp_close" },
	{ 0xf435dd31, "kernel_write" },
	{ 0x98303217, "filp_open" },
	{ 0x92997ed8, "_printk" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D1131407F9A238442AB8A18");
