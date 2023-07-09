# End.BPF test 用リポジトリ

## 準備

必要なツール・ライブラリをインストールする。
LLVM・Clang を使ってコードのコンパイルを行う想定。
```
sudo apt install build-essential libbpf-dev clang llvm 
```

## テスト環境確認

**kernel 情報**
```
motok1@server:~$ uname -a
Linux pe02-notb 6.4.2-060402-generic #202307070731 SMP PREEMPT_DYNAMIC Fri Jul  7 18:21:08 UTC 2023 x86_64 x86_64 x86_64 GNU/Linux
```

**Clang バージョン**
```
motok1@server:~$ clang -v
Ubuntu clang version 14.0.0-1ubuntu1
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
Found candidate GCC installation: /usr/bin/../lib/gcc/x86_64-linux-gnu/11
Selected GCC installation: /usr/bin/../lib/gcc/x86_64-linux-gnu/11
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

**LLVM バージョン**
```
motok1@server:~$ llc --version
Ubuntu LLVM version 14.0.0

  Optimized build.
  Default target: x86_64-pc-linux-gnu
  Host CPU: cascadelake
```


**iproute2 バージョン**
```
motok1@server:~$ ip -V
ip utility, iproute2-5.15.0, libbpf 0.5.0
```

## 動作確認

1. プログラムのコンパイル

```
motok1@server:~/workspace/motoki/test_end_bpf$ make
clang -O2 -emit-llvm -c dbg_endsid_kern.c -o - | llc -march=bpf -filetype=obj -o dbg_endsid_kern.o
```

2. End.BPF の登録

```
motok1@server:~/workspace/motoki/test_end_bpf$ sudo ip -6 r add dead::beef encap seg6local action End.BPF endpoint object dbg_endsid_kern.o section main dev eno6
```

※ 登録の削除

```
motok1@server:~/workspace/motoki/test_end_bpf$ sudo ip -6 r del dead::beef encap seg6local action End.BPF endpoint object dbg_endsid_kern.o section main dev eno6
```

3. 登録の確認

```
motok1@server:~/workspace/motoki/test_end_bpf$ ip -6 r
(snip)
dead::beef encap seg6local action End.BPF endpoint dbg_endsid_kern.o:[main] dev eno6 metric 1024 pref medium
```

4. 動作確認 

※ End.BPF として登録した SID がついたパケットを転送すると eBPF のデバッグプリントが実行されていることを確認できる

```
motok1@server:~/workspace/motoki/test_end_bpf$  sudo cat  /sys/kernel/debug/tracing/trace_pipe



          <idle>-0       [004] ..s31  2141.724309: bpf_trace_printk: End.BPF test

          <idle>-0       [004] ..s31  2142.725042: bpf_trace_printk: End.BPF test

          <idle>-0       [004] ..s31  2143.728516: bpf_trace_printk: End.BPF test
```
