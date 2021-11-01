# study-log-cpp

## 2021-11-1

https://zhuanlan.zhihu.com/p/158586853

> 这时候原本的myadd函数名变成了__Z5myaddii，这对于js的使用方很不友好，因此我们需要关掉c++的name mangle处理。 通过extern "C"我们可以阻止c++的默认name mangle行为 

0. 貌似clang设置输出wasm source map比较困难, 找emcc的对clang命令行参数的拼接也没找到
1. 卧槽, 真的可以自动优化输出simd的指令, 这样太强了, 难怪之前simd和非no-simd测试相差非常小
2. 难道不是assemblyscript的锅, c++编译出来的add循环100_000都得20ms, rust的编译就那么强么
3. 看了下matrix.wat和gl-matrix-wasm的wast的区别, 前者用了f64, 后者没有, 这性能差距也太大了吧, 这原因值得深究了

## 2021-10-31

0. 尝试用 assemblyscript 编写 simd 矩阵库, 但是发现编译出来的性能比 jit 优化后的慢很多, 并且看到 rust 编写 gl-matrix-wasm 对比 jit 优化过的 js 稳定 2 倍性能, 所以尝试用传统强类型语言编写
1. 找到方便编译 c++到 wasm 的工具, 支持编译 c++到 wasm 或者 js, 并自动关联调用, 使用起来方便, 但是发现并不支持 simd 特性
2. emscripten 编译出的 wasm 和 js 包含体积有点大, 尤其是 js 部分, 感觉是把 web 当作一个程序类型: 把 c++程序 port 到 web, 不太强调于 js 的交互

https://surma.dev/things/c-to-webassembly/
https://pranjal9599.github.io/simd-in-webassembly 难道能自动转 simd 么, 不需要手动的??

奇怪的问题:

0. clang 编译 addc.c, 导出函数名是 add, 而 add.cpp 导出函数则是\_Z3addii
