# study-log-cpp

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
