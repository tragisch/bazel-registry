"""Runs a Graphviz header generator with the execution platform's Python."""

_PYTHON_EXEC_TOOLS = "@rules_python//python:exec_tools_toolchain_type"

def _generate_header_impl(ctx):
    python = ctx.toolchains[_PYTHON_EXEC_TOOLS].exec_tools.exec_interpreter
    args = ctx.actions.args()
    args.add(ctx.file.script)
    args.add_all(ctx.files.args)
    args.add(ctx.outputs.out)

    ctx.actions.run(
        executable = python[DefaultInfo].files_to_run,
        arguments = [args],
        inputs = [ctx.file.script] + ctx.files.args + ctx.files.data,
        outputs = [ctx.outputs.out],
        mnemonic = "GraphvizGenerateHeader",
        toolchain = _PYTHON_EXEC_TOOLS,
    )

generate_header = rule(
    implementation = _generate_header_impl,
    attrs = {
        "args": attr.label_list(allow_files = True),
        "data": attr.label_list(allow_files = True),
        "out": attr.output(mandatory = True),
        "script": attr.label(allow_single_file = [".py"], mandatory = True),
    },
    toolchains = [_PYTHON_EXEC_TOOLS],
)
