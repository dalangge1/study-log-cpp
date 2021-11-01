#!/usr/bin/env zx
console.log(argv);
const { _, i, t = 'wasm', m = [] } = argv;
const basename = i.replace(path.extname(i), '');
const isC = path.extname(i) == '.c';
const startBuildTime = performance.now();
const CC = isC ? 'clang' : 'clang++';
const deps = (typeof m === 'string' ? [m] : m).map(i => 'src/' + i);

switch (t) {
  case 'wasm': {
    const flags = [
      `--target=wasm32`,
      // '-msimd128',
      '-nostdlib',
      '-O3',
      '-flto',
      '-DNDEBUG',
      // '-gdwarf',
      // '-gdwarf-4',
      '-Wl,--lto-O3',
      '-Wl,--no-entry',
      '-Wl,--export-all',
    ];

    await $`${CC} ${flags} -o ${`dist/${basename}.wasm`} src/${i} ${deps}`;
    await $`${CC} ${flags} -msimd128 -D__SIMD__=1 -o ${`dist/${basename}.simd.wasm`} src/${i} ${deps}`;
    $.verbose = false;
    await $`wasm-opt dist/${basename}.wasm --print -S -o dist/${basename}.wat`;
    await $`wasm-opt dist/${basename}.simd.wasm --print -S -o dist/${basename}.simd.wat`;
    break;
  }

  case 'exe': {
    await $`${CC} --target=x86_64-pc-windows-gnu -o ${`dist/${basename}.exe`} src/${i} ${deps}`;
    break;
  }
}

console.log(`build cost: ${(performance.now() - startBuildTime).toFixed(2)}ms`);
