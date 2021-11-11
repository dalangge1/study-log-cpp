# study-log-cpp

> 铁打的仓库，流水的日志

## 2021-11-11

0. determinant simd 性能比 no simd 慢, 到底是什么导致的? 先了解下 determinant 吧...
1. 额, 貌似 three.js 自己已经有了 determinant 改进算法在 invert 的函数里, 但是 determinant 方法里却不是改进版的
2. transpose 前后的 determinant 是相同的?
3. 终于有一版性能赶上非 simd 了, 不过只快了 1x% (有浮动)
4. loop in wasm 耗时有能显示出来, 基本吻合 loop in js
5. babylon.js 的 determinant 真离谱, chrome 能优化到和 wasm 相差 3ms
6. Float32Array 有性能问题, 只有循环数量大的时候对比 array 有优势, 比如 100_0000, 循环数量少的时候则比 array 差太多...
7. chrome 太强大了,只要有个 warmup, 触发 trubo fan 优化, 比 wasm 还强一些, 虽然有 js call 的损耗
8. 不过一个 determinant 函数 warmup 需要 2ms, 这么多函数都 warmup 的成本就会累加起来, 貌似也不是一个可靠的解法, 数据变化的时候是否依然有这样性能? 能
9. 可能也不需要刻意的 warmup, 数据量小, 不明显, 数据量大, 自动优化了, 不过可能提供一个人工加速自动优化的开始时机也时不错的选择, v8 都能优化比 wasm+simd 还强
10. 改为 float32Array 不是都能有提速...乘法就性能就明显下降了

https://github.com/toji/gl-matrix/issues/359
https://github.com/akira-cn/babel-plugin-transform-gl-matrix/blob/master/README-CN.md

> A determinant is a scalar number which is calculated from a matrix. This number can determine whether a set of linear equations are solvable, in other words whether the matrix can be inverted.

> matrix_determinant_benchmark_1000000:

| Key                            | Value            |
| ------------------------------ | ---------------- |
| wasm                           | 16.10ms (x1.643) |
| wasm2                          | 16.10ms (x1.643) |
| wasm_determinant_babylon       | 15.90ms (x1.622) |
| mat4_invert_det                | 18.30ms (x1.867) |
| mat4_det_glmatrix              | 16.90ms (x1.724) |
| wasm_hand_simd                 | 16.30ms (x1.663) |
| wasm_hand_simd2                | 31.60ms (x3.224) |
| wasm_hand_simd3                | 19.10ms (x1.949) |
| wasm_hand_simd4                | 22.60ms (x2.306) |
| wasm_hand_simd5                | 11.70ms (x1.194) |
| wasm_auto_simd                 | 14.90ms (x1.520) |
| wasm_auto_simd2                | 14.30ms (x1.459) |
| threejs_determinant            | 28.80ms (x2.939) |
| threejs_determinant2           | 19.20ms (x1.959) |
| threejs_determinant2_f32_array | 9.80ms (x1.000)  |
| threejs_determinant3           | 18.60ms (x1.898) |
| babylon_determinant            | 15.70ms (x1.602) |
| babylon_determinant_f32_array  | 9.90ms (x1.010)  |
| oasis_determinant              | 17.70ms (x1.806) |
| oasis_determinant_f32_array    | 9.90ms (x1.010)  |
| threejs_invert_det             | 28.70ms (x2.929) |

> matrix_determinant_benchmark_10000:

| Key                            | Value            |
| ------------------------------ | ---------------- |
| wasm                           | 0.50ms (x1.667)  |
| wasm2                          | 0.40ms (x1.333)  |
| wasm_determinant_babylon       | 0.40ms (x1.333)  |
| mat4_invert_det                | 0.30ms (x1.000)  |
| mat4_det_glmatrix              | 0.40ms (x1.333)  |
| wasm_hand_simd                 | 0.50ms (x1.667)  |
| wasm_hand_simd2                | 0.50ms (x1.667)  |
| wasm_hand_simd3                | 0.40ms (x1.333)  |
| wasm_hand_simd4                | 0.50ms (x1.667)  |
| wasm_hand_simd5                | 0.30ms (x1.000)  |
| wasm_auto_simd                 | 0.30ms (x1.000)  |
| wasm_auto_simd2                | 0.40ms (x1.333)  |
| threejs_determinant            | 1.90ms (x6.333)  |
| threejs_determinant2           | 1.40ms (x4.667)  |
| threejs_determinant2_f32_array | 3.10ms (x10.333) |
| threejs_determinant3           | 1.50ms (x5.000)  |
| babylon_determinant            | 1.50ms (x5.000)  |
| babylon_determinant_f32_array  | 2.40ms (x8.000)  |
| oasis_determinant              | 1.50ms (x5.000)  |
| oasis_determinant_f32_array    | 2.50ms (x8.333)  |
| threejs_invert_det             | 1.70ms (x5.667)  |

## 2021-11-9

0. 按照 glm 调整了顺序, 但是性能更差了些...很奇怪了
1. 增加了 make/shuffle/replace_lane 性能测试, 貌似没有明显的性能差别...determinant 为什么会比 wasm 慢呢...
2. wasm_i32_shuffle 最终编译为还是 i8x16.shuffle, wasm simd 规范找到的也只有这个, 发现有一段 shuffle 特别耗时 matrix.cpp:875

## 2021-11-8

0. 增加网页 log, 确认开 devtools, 对测试结果很有影响, 尤其是火狐, 关闭 devtools 时, 参考 glm 的实现最快的, 比 js 确实快了 4 倍, 并且 simd 的优势也体现出来了
1. 阅读 glm determinant 实现调整顺序

## 2021-11-5

0. 开了-msimd128 确实会自动把部分指令改为 simd,但是貌似带来提升体现不高
1. 迁移了 assemblyscript 时候的写版本, 性能和 js 差不多 1000000 loop
2. 比较奇怪是火狐打开 devtools wasm 耗时剧增, chrome 也有类似的情况, 等结果输出完毕后再打开 dev tools

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
