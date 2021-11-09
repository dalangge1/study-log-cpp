import { test, expect, benchmark } from './libs/test.js';
import { load } from './libs/utils.js';
import { Matrix4 } from '../node_modules/three/src/math/Matrix4.js';

export async function matrix() {
  const { wasmAPI, wasmSIMDAPI } = await load('matrix');
  const glMatrixWasm = window.glMatrixWasm;

  await glMatrixWasm.init();

  // prettier-ignore
  const ms = [
    glMatrixWasm.Matrix4.fromValues(1, 2, 3, 4, 5, 6, 7, 8, 8, 2, 1, 2, 7, 1, 3, 2),
    glMatrixWasm.Matrix4.fromValues(1, 2, 3, 1, 5, 6, 7, 2, 8, 2, 1, 2, 9, 1, 3, 2),
    glMatrixWasm.Matrix4.fromValues(2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2),
  ];

  /**
   * @param {WebAssembly.Exports} wasmAPI
   */
  function init(wasmAPI) {
    const jsArrayA = [1, 2, 3, 4, 5, 6, 7, 8, 8, 2, 1, 2, 7, 1, 3, 2];
    const jsArrayB = [1, 2, 3, 1, 5, 6, 7, 2, 8, 2, 1, 2, 9, 1, 3, 2];

    // Allocate memory for 4 32-bit integers
    // and return get starting address.
    const cArrayPointerA = wasmAPI.malloc(jsArrayA.length * 4);
    const cArrayPointerB = wasmAPI.malloc(jsArrayA.length * 4);
    const cArrayPointerOut = wasmAPI.malloc(jsArrayA.length * 4);

    // Turn that sequence of 32-bit integers
    // into a Float32Array, starting at that address.
    const cArrayA = new Float32Array(
      wasmAPI.memory.buffer,
      cArrayPointerA,
      jsArrayA.length,
    );
    const cArrayB = new Float32Array(
      wasmAPI.memory.buffer,
      cArrayPointerB,
      jsArrayA.length,
    );
    const cArrayOut = new Float32Array(
      wasmAPI.memory.buffer,
      cArrayPointerOut,
      jsArrayA.length,
    );

    // Copy the values from JS to C.
    cArrayA.set(jsArrayA);
    cArrayB.set(jsArrayB);

    return {
      cArrayPointerOut,
      cArrayPointerB,
      cArrayPointerA,
      cArrayOut,
      jsArrayA,
      jsArrayB,
    };
  }

  const state = init(wasmAPI);
  const stateSIMD = init(wasmSIMDAPI);

  // performance test
  test('matrix_multiply', () => {
    const m4JSA = new Matrix4().fromArray(state.jsArrayA);
    const m4JSB = new Matrix4().fromArray(state.jsArrayB);
    const glMatrixWasmMatrix4 = glMatrixWasm.Matrix4;
    m4JSA.multiplyMatrices(m4JSA, m4JSB);

    const mat4_v128_1 = wasmSIMDAPI.set_mat4_v128(1, stateSIMD.cArrayPointerA);
    const mat4_v128_2 = wasmSIMDAPI.set_mat4_v128(2, stateSIMD.cArrayPointerB);
    const mat4_v128_3 = wasmSIMDAPI.set_mat4_v128(
      3,
      stateSIMD.cArrayPointerOut,
    );

    wasmSIMDAPI.mat4_multiply_simd2(
      stateSIMD.cArrayPointerA,
      stateSIMD.cArrayPointerB,
      stateSIMD.cArrayPointerOut,
    );
    expect(stateSIMD.cArrayOut).toBe(m4JSA.elements);

    wasmSIMDAPI.mat4_multiply(
      stateSIMD.cArrayPointerA,
      stateSIMD.cArrayPointerB,
      stateSIMD.cArrayPointerOut,
    );
    expect(stateSIMD.cArrayOut).toBe(m4JSA.elements);

    wasmSIMDAPI.mat4_multiply_simd(
      stateSIMD.cArrayPointerA,
      stateSIMD.cArrayPointerB,
      stateSIMD.cArrayPointerOut,
    );
    expect(stateSIMD.cArrayOut).toBe(m4JSA.elements);

    wasmAPI.mat4_multiply(
      state.cArrayPointerA,
      state.cArrayPointerB,
      state.cArrayPointerOut,
    );
    expect(state.cArrayOut).toBe(m4JSA.elements);

    benchmark(
      {
        wasm_auto_simd() {
          wasmSIMDAPI.mat4_multiply(
            stateSIMD.cArrayPointerA,
            stateSIMD.cArrayPointerB,
            stateSIMD.cArrayPointerOut,
          );
        },
        wasm_hand_simd() {
          wasmSIMDAPI.mat4_multiply_simd(
            stateSIMD.cArrayPointerA,
            stateSIMD.cArrayPointerB,
            stateSIMD.cArrayPointerOut,
          );
        },
        wasm_hand_simd2() {
          wasmSIMDAPI.mat4_multiply_simd2(
            stateSIMD.cArrayPointerA,
            stateSIMD.cArrayPointerB,
            stateSIMD.cArrayPointerOut,
          );
        },
        wasm_glm_simd() {
          wasmSIMDAPI.mat4_multiply_simd_glm(
            mat4_v128_1,
            mat4_v128_2,
            mat4_v128_3,
          );
        },
        wasm_bench_simd() {
          wasmSIMDAPI.bench_simd(
            stateSIMD.cArrayPointerA,
            stateSIMD.cArrayPointerB,
            stateSIMD.cArrayPointerOut,
          );
        },
        wasm() {
          wasmAPI.mat4_multiply(
            state.cArrayPointerA,
            state.cArrayPointerB,
            state.cArrayPointerOut,
          );
        },
        rust_wasm() {
          glMatrixWasmMatrix4.multiply(ms[2], ms[0], ms[1]);
        },
        threejs() {
          m4JSA.multiplyMatrices(m4JSA, m4JSB);
        },
      },

      // 10_000,
      // 100_000,
      1_000_000,
      // 100_000,
      `_loop_in_js`,
    );

    const loopCount = 1_000_000;
    benchmark(
      {
        wasm_auto_simd() {
          wasmSIMDAPI.test(0, loopCount);
        },
        wasm_hand_simd() {
          wasmSIMDAPI.test(1, loopCount);
        },
        wasm_hand_simd2() {
          wasmSIMDAPI.test(3, loopCount);
        },
        wasm_glm_simd() {
          wasmSIMDAPI.test(4, loopCount);
        },
        wasm_bench_simd() {
          wasmSIMDAPI.test(2, loopCount);
        },
      },
      1,
      `_loop_in_wasm_${loopCount}`,
    );
  });

  test('matrix_determinant', () => {
    const m4JSA = new Matrix4().fromArray([
      2, 3, 4, 5, -1, -21, -3, -4, 6, 7, 8, 10, -8, -9, -10, -12,
    ]);

    benchmark(
      {
        wasm() {
          wasmAPI.mat4_determinant(state.cArrayPointerA);
        },
        wasm2() {
          wasmAPI.mat4_determinant2(state.cArrayPointerA);
        },
        wasm_hand_simd() {
          wasmSIMDAPI.mat4_determinant_simd(stateSIMD.cArrayPointerA);
        },
        wasm_hand_simd2() {
          wasmSIMDAPI.mat4_determinant_simd2(stateSIMD.cArrayPointerA);
        },
        wasm_hand_simd3() {
          // 更慢了...
          wasmSIMDAPI.mat4_determinant_simd3(stateSIMD.cArrayPointerA);
        },
        wasm_hand_simd4() {
          wasmSIMDAPI.mat4_determinant_simd4(stateSIMD.cArrayPointerA);
        },
        wasm_auto_simd() {
          wasmSIMDAPI.mat4_determinant(stateSIMD.cArrayPointerA);
        },
        threejs() {
          m4JSA.determinant();
        },
      },
      1000000,
    );

    const loopCount = 1_000_000;
    benchmark(
      {
        wasm() {
          wasmSIMDAPI.test(5, loopCount);
        },
        wasm_simd() {
          wasmSIMDAPI.test(6, loopCount);
        },
        wasm_simd2() {
          wasmSIMDAPI.test(7, loopCount);
        },
      },
      1,
      `_loop_in_wasm_${loopCount}`,
    );
  });
}
