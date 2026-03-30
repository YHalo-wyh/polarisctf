"""
Best-effort IDAPython renaming script for the stripped PolarisCTF ezheap binary.

Load the binary/DB in IDA and run:
  File -> Script file... -> ida_rename_ezheap.py
"""

try:
    import ida_name
    import ida_funcs
    import ida_bytes
except ImportError:
    print("This script must be run inside IDA.")
    raise


FUNCTIONS = [
    (0x2C50, "session_postproc_default", "Default session post-processing pipeline. XORs payload bytes with an index-derived mask."),
    (0x2F40, "task_runner_mul_ack", "Task runner variant #0. Updates desc->scratch/arg field and prints the mul ack banner."),
    (0x2FF0, "task_runner_xor_ack", "Task runner variant #1. XORs desc->scratch/arg field with a fixed constant and prints the xor ack banner."),
    (0x30A0, "task_runner_log_ack", "Task runner variant #2. Logs task_id, arg0 and tag."),
    (0x34E0, "session_postproc_clamp_negative", "Alternative post-processing pipeline. Clears payload bytes whose signed value is negative."),
    (0x3520, "session_postproc_shift_right", "Alternative post-processing pipeline. Shifts each payload byte right by 1."),
    (0x3570, "runtime_ctx_cleanup", "Frees artifact arena, session objects/payloads and clears runtime arrays."),
    (0x36A0, "bootstrap_async_scheduler", "Allocates queue control state and eight heap-backed task descriptors."),
    (0x3910, "complete_batch_inference", "Runs session postproc, frees payload/session, clears only the active flag, leaving a stale pointer."),
    (0x3DF0, "dispatch_async_task", "Validates task id, enforces handler whitelist when strict_policy is enabled, then calls desc->handler(desc->ctx)."),
    (0x4230, "register_model_artifact", "Parses artifact dimensions, allocates arena, stores artifact metadata in the runtime context."),
    (0x4870, "inspect_scheduler_queue", "Prints queue control pointer/flags and enumerates the eight task descriptors."),
    (0x4D60, "allocate_session_tensor", "Creates a 0x50 session object, allocates payload, stores alias, default postproc and session metadata."),
    (0x55B0, "patch_session_metadata", "Diagnostic UAF primitive: on recycled sessions, only qword_index 0 may be patched."),
    (0x5D60, "runtime_telemetry", "Leaks artifact state, scheduler control pointer, audit_sink address, session metadata and worker profile pointers."),
    (0x6750, "audit_sink_print_flag", "Opens /flag or ./flag, reads one line and prints it with the audit banner."),
    (0x6E90, "provision_worker_profile", "Allocates a 0x50 worker profile, parses six fields plus memo, then appends it to the worker vector."),
]


def apply():
    renamed = 0
    for ea, name, comment in FUNCTIONS:
        if ida_name.set_name(ea, name, ida_name.SN_NOWARN):
            renamed += 1
        func = ida_funcs.get_func(ea)
        if func:
            ida_funcs.set_func_cmt(func, comment, False)
        ida_bytes.set_cmt(ea, comment, False)
    print(f"Applied {renamed}/{len(FUNCTIONS)} function names.")


if __name__ == "__main__":
    apply()
