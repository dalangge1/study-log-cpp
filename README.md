# study-log-cpp

> 铁打的仓库，流水的日志

## 2022-2-10

cpp代码引入c代码, 头文件需要包一个`extern "C" {}`

```cpp
#ifdef __cplusplus
extern "C" {
#endif
#include "quickjs.h"
#ifdef __cplusplus
}
#endif
```

## 2022-1-18

0. thread demo 了解mutex condition_variable的使用方式

## 2022-1-6

0. [移动构造函数](https://stackoverflow.com/questions/4986673/c11-rvalues-and-move-semantics-confusion-return-statement)

## 2022-1-5

0. 区分常变量和常指针 const 的位置
1. struct/class 占用的空间,会有字节对齐,虚函数会存一个虚指针,每个集成的基类的虚指针都会有一个,几遍相同,也不会合并. 空 class 占 1 字节.
2. https://clang.llvm.org/docs/analyzer/developer-docs/IPA.html

## 2021-12-31

cmake debug 打印日志, 定义函数

```cmake
message(STATUS "cmake_debug------------------------------------------{")
message(STATUS ${googlebenchmark_SOURCE_DIR})
message(STATUS ${googlebenchmark_BINARY_DIR})

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

print_all_variables()
```

## 2021-12-27

0. 通过 explicit 防止隐式转换 https://www.cnblogs.com/this-543273659/archive/2011/08/02/2124596.html

explicit 作用:

在 C++中，explicit 关键字用来修饰类的构造函数，被修饰的构造函数的类，不能发生相应的隐式类型转换，只能以显示的方式进行类型转换。

explicit 使用注意事项:

    * explicit 关键字只能用于类内部的构造函数声明上。
    * explicit 关键字作用于单个参数的构造函数。
    * 在C++中，explicit关键字用来修饰类的构造函数，被修饰的构造函数的类，不能发生相应的隐式类型转换

## 2021-12-24

0. [List Initialization](https://blog.csdn.net/caoshangpa/article/details/79169930)
1. POD(plain old data，简单来说就是可以用 memcpy 复制的对象)类型
2. [拷贝构造函数](http://c.biancheng.net/view/151.html)

```cpp

```

## 2021-12-23

0. 匿名函数

```cpp
// h,s,l ranges are in 0.0 - 1.0
// @see HSL 到 RGB 替代方案 https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_RGB
void Color::setFromHSL(float h, float s, float l) {
    auto fn = [h, s, l](float n) -> float {
        float a = s * min(l, 1 - l);
        // float k = (n + h / 30); // [0,360]
        float k = (n + h * 12); // [0,1]
        return l - a * max(-1, min(k - 3, 9 - k, 1));
    };
    r = fn(0);
    g = fn(8);
    b = fn(4);
}
```

## 2021-12-22

const 的两种位置 https://docs.microsoft.com/en-us/cpp/cpp/const-cpp?view=msvc-170

```cpp
const declaration ;
member-function const ;

// constant_values3.cpp
int main() {
   char *mybuf = 0, *yourbuf;
   char *const aptr = mybuf; // const指针, 但指针指向非const
   *aptr = 'a';   // OK
   aptr = yourbuf;   // C3892
}
```

inline function 还有条件 https://www.geeksforgeeks.org/inline-functions-cpp/

Remember, inlining is only a request to the compiler, not a command. Compiler can ignore the request for inlining. Compiler may not perform inlining in such circumstances like:

1. If a function contains a loop. (for, while, do-while)
2. If a function contains static variables. (这个比较奇怪了)
3. If a function is recursive.
4. If a function return type is other than void, and the return statement doesn’t exist in function body.
5. If a function contains switch or goto statement.

## 2021-12-21

Granite 中 muglm 大量使用入参都是引用, 出参是新 struct

https://segmentfault.com/a/1190000023643326

函数参数传递中值传递、地址传递、引用传递有什么区别？
(1) 值传递，会为形参重新分配内存空间，将实参的值拷贝给形参，形参的值不会影响实参的值，函数调用结束后形参被释放；

(2) 引用传递，不会为形参重新分配内存空间，形参只是实参的别名，形参的改变会影响实参的值，函数调用结束后形参不会被释放；

(3) 地址传递，形参为指针变量，将实参的地址传递给函数，可以在函数中改变实参的值，调用时为形参指针变量分配内存，结束时释放指针变量。

## 2021-12-20

0. 初始化静态成员
1. 如果出现 pp_including_mainfile_in_preamble 头文件的相互循环依赖,可以把引用挪到 source file 里
2. 看到另一种指针代替写法 &T FN(K &A); 和上层使用用法相同(和指针有不少[区别](https://segmentfault.com/a/1190000023643326))

```cpp
// Header file
class Game {
  public:
    // Declaration:
    static const struct timespec UPDATE_TIMEOUT;
  ...
};

// Source file
const struct timespec Game::UPDATE_TIMEOUT = { 10 , 10 };  // Definition

// 引用
explicit inline tmat3(const tmat4<T> &m) noexcept
{
  for (int col = 0; col < 3; col++)
    for (int row = 0; row < 3; row++)
      vec[col][row] = m[col][row];
}
```

## 2021-12-16

0. CPP 函数默认值
1. CPP 不能静态检测 nullptr 只能注释...怪不得 nullptr 恐怖

```c++
void f(int, int);     // #1
void f(int, int = 7); // #2 OK: adds a default

void h() {
    f(3); // #1 and #2 are in scope; makes a call to f(3,7)
    void f(int = 1, int); // Error: inner scope declarations don't acquire defaults
}
```

https://en.cppreference.com/w/cpp/language/default_arguments

## 2021-12-15

0. 了解`#include`和`#include_next`的区别
1. inline function(不过 bx 还会增加个`constexpr` `__attribute__( (const) )`修饰符)

```c++
inline return-type function-name(parameters)
{
    // function code
}
```

https://www.geeksforgeeks.org/inline-functions-cpp/

## 2021-12-14

0. 增加打印 Class/Struct 内存布局 Demo
1. CMake Tools 支持 `.vscode/setting.json` `cmake.configureArgs` `cmake.buildArgs` 配置配置/构建参数

## 2021-12-08

`CMake`多个 executable 指定 CXX 标准需要用一下方法

```cmake
set_property(TARGET main PROPERTY CXX_STANDARD 14)
set_property(TARGET main_test PROPERTY CXX_STANDARD 14)
```

## 2021-12-07

`#include <stuf>`和`#include "stuf"`的区别

1. 系统自带的头文件用尖括号括起来，这样编译器会在系统文件目录下查找。

2. 用户自定义的文件用双引号括起来，编译器首先会在用户目录下查找，然后在到 C++安装目录（比如 VC 中可以指定和修改库文件查找路径，Unix 和 Linux 中可以通过环境变量来设定）中查找，最后在系统文件中查找。

https://gcc.gnu.org/onlinedocs/cpp/Search-Path.html

`#pragam once`与`#ifndef`区别

1. `#ifndef` 会使得编译时间相对较长

2. `#pragma once` 由编译器提供保证：同一个文件不会被包含多次。注意这里所说的“同一个文件”是指物理上的一个文件，而不是指内容相同的两个文件。你无法对一个头文件中的一段代码作 pragma once 声明，而只能针对文件。

https://www.cnblogs.com/hokyhu/archive/2009/03/30/1425604.html

`.inl`文件是什么

https://stackoverflow.com/questions/1208028/significance-of-a-inl-file-in-c/1208062

## 2021-11-24

0. 部门内流行躺平，不过我自己习惯还是得继续推进，手脑同步才行，即便流水账，习惯不能丢。
1. mac 上 chrome 和 firefox wasm simd 性能表现和我的主机性能差不多，i9 10900es 也太差了，比 19 年 macbook 的 i7 也没快多少
2. compile_commands.json 适配 mac

## 2021-11-23

0. mac 上跑起来，这边的日报也得继续了

## 2021-11-18

0. 加上 transpose 后慢了一些, js api 相比 wasm 只快了 30%, 但是相比 threejs 的还是快了一倍

## 2021-11-17

0. loop in wasm 的 simd 还是能快一倍
1. 矩阵关键的运算相关 3 个方法算是翻译完了, 准备编写内存管理了, 测试用例越来越多, 测试的结果报表估计需要美化一下

## 2021-11-16

0. simd 比繁琐的问题在于运算前的数据填充, 还是得看看 glm 学习下, glm 主要还是使用 shuffle, 但是 wasm-simd 翻译到对应平台的 simd 指令是组合指令...
1. 要不尝试只用 v128.load 读数据的方式
2. 翻译 gl-matrix 的版本的 invert 确实快不少, 但是还缺一步 transpose, 不过相比于 wasm 版本只有快了 40%

> [wasm-simd/Instructions.md](https://github.com/zeux/wasm-simd/blob/master/Shuffles.md) \
> [wasm-simd/Shuffles.md](https://github.com/zeux/wasm-simd/blob/master/Shuffles.md#unpacks)

应该使用最多应该时 unpacks 的方式

| op                 | v8 x64 | v8 arm |
| ------------------ | ------ | ------ |
| v8x16.shuffle      | 1-11   | 1-12?  |
| f32x4.extract_lane | 2      | 1      |
| f32x4.replace_lane | 1      | 1-2    |
| f32x4.splat        | 2      | 1      |
| f32x4.abs          | 3      | 1      |
| f32x4.neg          | 3      | 1      |
| f32x4.sqrt         | 1      | 1      |
| f32x4.add          | 1      | 1      |
| f32x4.sub          | 1      | 1      |
| f32x4.mul          | 1      | 1      |
| f32x4.div          | 1      | 1      |
| f32x4.min          | 6      | 1      |
| f32x4.max          | 6      | 1      |
| v128.load          | 1      | 1      |
| v128.store         | 1      | 1      |

> matrix_invert_benchmark_1000000:

| Key        | Value            |
| ---------- | ---------------- |
| wasm       | 42.00ms (x2.154) |
| wasm2      | 28.30ms (x1.451) |
| wasm_simd  | 41.30ms (x2.118) |
| wasm_simd2 | 19.50ms (x1.000) |
| threejs    | 44.70ms (x2.292) |

## 2021-11-15

0. 开始翻译 invert 了, 三个主要的运算翻译完后, 就开始处理内存分配的问题了
1. invert 这耗时又是没性能优势啊,这个 simd 的优化不简单,估计得找找相关规则,或者一些指令的耗时情况

## 2021-11-12

0. 测试代码有点凌乱, 版本也多, 需整理
1. 不同场景测试结果有差距 这个仓库里的 determinant 性能比 determinant2 快上 2ms https://github.com/deepkolos/matrix-f32-array-performace-issue/
2. vscode 的 c++的代码提示远不如 js 的...估计还是看看怎么配置, 连个分号缺少都不会报错...reference 也不太工作...这开发体验真的太差了(clangd 还行)

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
