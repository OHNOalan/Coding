#include <context.h>
#include <gcc-plugin.h>
#include <langhooks.h>
#include <plugin-version.h>
#include <stdio.h>
#include <tree-pass.h>
#include <tree.h>

// 必须声明 GPL 协议，否则 GCC 拒绝加载
int plugin_is_GPL_compatible;

static unsigned int my_pass_execute(void) {
    // 打印当前正在处理的函数名
    printf("编译插件正在处理函数: %s\n", lang_hooks.decl_printable_name(cfun->decl, 2));
    return 0;
}

// 1. 定义一个新的类继承自 gimple_opt_pass
class my_print_pass : public gimple_opt_pass {
public:
    my_print_pass(gcc::context *ctxt) :
        gimple_opt_pass(
                // 这里传入 pass_data 结构体
                {
                        GIMPLE_PASS,
                        "my_print_pass", // Pass 名字
                        OPTGROUP_NONE, // 优化组
                        TV_NONE, // Timevar
                        PROP_cfg, // 设置属性
                        0, // 清除属性
                        0, // 属性槽
                        0, // 这里的 TODO
                        0 // 这里的 TODO
                },
                ctxt) {}

    // 2. 实现 execute 虚函数
    virtual unsigned int execute(function *fun) override {
        printf("编译插件 (GCC 15) 正在处理函数: %s\n", lang_hooks.decl_printable_name(fun->decl, 2));
        return 0;
    }
};

// 3. 在注册信息中使用新的类实例
struct register_pass_info my_info = {.pass = new my_print_pass(g), // g 是全局编译器上下文
                                     .reference_pass_name = "cfg",
                                     .ref_pass_instance_number = 1,
                                     .pos_op = PASS_POS_INSERT_AFTER};

// 插件入口函数
int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version) {
    // 注册回调
    register_callback(plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &my_info);
    return 0;
}
