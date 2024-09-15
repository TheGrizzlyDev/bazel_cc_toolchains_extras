CcTestInfo = provider()
GetRunnerInfo = provider()

_WRAPPER_LABEL_ARG_NAME = "wrapper-label-arg"
_TOOLCHAIN_TARGET_ARG_NAME = "toolchain-target-arg"

def _get_test_runner(ctx, binary_info, processed_environment, **args):
    wrapper = args[_WRAPPER_LABEL_ARG_NAME]
    bin = ctx.actions.declare_file("test_runner_symlink" + args[_TOOLCHAIN_TARGET_ARG_NAME] + "_" + ctx.attr.name)
    ctx.actions.symlink(output=bin, target_file=wrapper[DefaultInfo].files_to_run.executable, is_executable=True)
    runfiles = ctx.runfiles([]).merge(wrapper.default_runfiles).merge(binary_info.runfiles)

    bin_path = binary_info.executable.short_path

    env = RunEnvironmentInfo(
        environment={
            "TEST_BIN": bin_path
        }
    )

    return [
        DefaultInfo(
            executable=bin,
            files=depset(transitive=[wrapper.files]),
            runfiles=runfiles,
        ),
        env,
    ]

def _cc_test_toolchain(ctx):
    toolchain_info = platform_common.ToolchainInfo(
        cc_test_info = CcTestInfo(
            linkopts = ctx.attr.linkopts,
            linkstatic = ctx.attr.linkstatic,
            get_runner = GetRunnerInfo(
                func = _get_test_runner,
                args = {
                    _WRAPPER_LABEL_ARG_NAME: ctx.attr.wrapper,
                    _TOOLCHAIN_TARGET_ARG_NAME: ctx.attr.name,
                },
            )
        )
    )
    return [
        toolchain_info,
    ]

cc_test_toolchain = rule(
    _cc_test_toolchain,
    attrs = {
        "linkopts": attr.string_list(default = []),
        "linkstatic": attr.bool(default = False),
        "wrapper": attr.label(executable = True, mandatory = True, providers = [DefaultInfo], cfg = "exec"),
    }
)
