# study-log-cpp

> 铁打的仓库，流水的日志

## 2021-11-5

0. 开了-msimd128 确实会自动把部分指令改为 simd,但是貌似带来提升体现不高
1. 迁移了 assemblyscript时候的写版本, 性能和js差不多1000000 loop
2. 比较奇怪是火狐打开devtools wasm耗时剧增, chrome也有类似的情况, 等结果输出完毕后再打开dev tools

https://emcc.zcopy.site/docs/porting/simd/

> Emscripten supports the WebAssembly SIMD proposal when using the WebAssembly LLVM backend. To enable SIMD, pass the -msimd128 flag at compile time. This will also turn on LLVM’s **autovectorization** passes, so no source modifications are necessary to benefit from SIMD.

## 2021-11-4

0. 有点小奇怪, loop in js 确实 glm 直接存储 v128 的方式快 2ms, 但是 loop in wasm 却不是最快的, 慢了 6ms
1. 翻译了 determinant, 但是手写的 simd 版本比 非 simd 还慢, simd 的编写不简单呐

## 2021-11-3

0. 感觉 shuffle 指令大有作为, 极大方便数据的设置, 得学一下;
1. 先阅读 glm, 看看 simd 指令 是怎么用的; 卧槽，存储的是 v128
2. C++ 代码格式化风格改为 Google
3. 看了 glb 的 matrix_mul 重新整理 mat4_multiply_simd2, 快了 8ms

https://stackoverflow.com/questions/57032331/understanding-the-simd-shuffle-control-mask
https://github.com/g-truc/glm/blob/master/glm/simd/matrix.h#L36

wasm_f32x4_make 是下面的语法糖，和我现在的写法是一样的，还是挺方便的

```wasm
(f32x4.replace_lane 3
  (f32x4.replace_lane 2
  (f32x4.replace_lane 1
    (f32x4.splat
    (local.get $0)
    )
    (local.get $1)
  )
  (local.get $2)
  )
  (local.get $3)
)
```

```json
{
  "multiply matrix_benchmark_1000000 loop in js": {
    "wasm_auto_simd": "34.30ms (x1.885)",
    "wasm_hand_simd": "26.40ms (x1.451)",
    "wasm_hand_simd2": "18.20ms (x1.000)",
    "wasm_bench_simd": "34.90ms (x1.918)",
    "wasm": "32.10ms (x1.764)",
    "rust_wasm": "41.90ms (x2.302)",
    "threejs": "34.60ms (x1.901)"
  },
  "multiply matrix_benchmark_1 loop in wasm": {
    "wasm_auto_simd": "30.30ms (x4.734)",
    "wasm_hand_simd": "20.00ms (x3.125)",
    "wasm_hand_simd2": "6.40ms (x1.000)",
    "wasm_bench_simd": "17.30ms (x2.703)"
  }
}
```

## 2021-11-2

0. 真相大白了, 是没缓存 instance.exports 导致, 性能与 rust 差不多了, 稍微慢一些, 估计是用了 f64 导致, 用了 double 导致, 改为 float 就比 rust 的快了
1. 优化测试空转耗时
2. simd 的好处还是没明显体现出来, 有点尴尬; simd 再频繁调用的情况下确实没啥亮眼, 但是在 wasm 里循环是无敌的存在; wat 里只有两个 call; 不对, 只要输入存在变化耗时就正常了

## 2021-11-1

https://zhuanlan.zhihu.com/p/158586853

> 这时候原本的 myadd 函数名变成了\_\_Z5myaddii，这对于 js 的使用方很不友好，因此我们需要关掉 c++的 name mangle 处理。 通过 extern "C"我们可以阻止 c++的默认 name mangle 行为

0. 貌似 clang 设置输出 wasm source map 比较困难, 找 emcc 的对 clang 命令行参数的拼接也没找到
1. 卧槽, 真的可以自动优化输出 simd 的指令, 这样太强了, 难怪之前 simd 和非 no-simd 测试相差非常小
2. 难道不是 assemblyscript 的锅, c++编译出来的 add 循环 100_000 都得 20ms, rust 的编译就那么强么
3. 看了下 matrix.wat 和 gl-matrix-wasm 的 wast 的区别, 前者用了 f64, 后者没有, 这性能差距也太大了吧, 这原因值得深究了

## 2021-10-31

0. 尝试用 assemblyscript 编写 simd 矩阵库, 但是发现编译出来的性能比 jit 优化后的慢很多, 并且看到 rust 编写 gl-matrix-wasm 对比 jit 优化过的 js 稳定 2 倍性能, 所以尝试用传统强类型语言编写
1. 找到方便编译 c++到 wasm 的工具, 支持编译 c++到 wasm 或者 js, 并自动关联调用, 使用起来方便, 但是发现并不支持 simd 特性
2. emscripten 编译出的 wasm 和 js 包含体积有点大, 尤其是 js 部分, 感觉是把 web 当作一个程序类型: 把 c++程序 port 到 web, 不太强调于 js 的交互

https://surma.dev/things/c-to-webassembly/
https://pranjal9599.github.io/simd-in-webassembly 难道能自动转 simd 么, 不需要手动的??

奇怪的问题:

0. clang 编译 addc.c, 导出函数名是 add, 而 add.cpp 导出函数则是\_Z3addii
