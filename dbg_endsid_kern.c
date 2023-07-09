#include <linux/types.h>
#include <bpf/bpf_helpers.h>

SEC("main")
int bpf_prog(struct __sk_buff *skb)
{
    const char fmt_str[] = "End.BPF test\n";
    bpf_trace_printk(fmt_str, 14);
    return 0;
}

char License[] SEC("license") = "GPL";
