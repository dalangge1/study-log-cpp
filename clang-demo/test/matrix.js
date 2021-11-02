import { test, expect, benchmark } from './libs/test.js';
import { load } from './libs/utils.js';
import { Matrix4 } from '../node_modules/three/src/math/Matrix4.js';

export async function matrix() {
  const { wasmAPI, wasmSIMDAPI } = await load('matrix');
  const glMatrixWasm = window.glMatrixWasm;

  await glMatrixWasm.init();

  // prettier-ignore
  const ms = [
    glMatrixWasm.Matrix4.fromValues(2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2),
    glMatrixWasm.Matrix4.fromValues(3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3),
    glMatrixWasm.Matrix4.fromValues(2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2),
  ];

  const jsArrayExpect = new Array(4 * 4).fill(24);

  /**
   * @param {WebAssembly.Exports} wasmAPI
   */
  function init(wasmAPI) {
    const jsArrayA = new Array(4 * 4).fill(2);
    const jsArrayB = new Array(4 * 4).fill(3);

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
  test('multiply matrix', () => {
    const m4JSA = new Matrix4().fromArray(state.jsArrayA);
    const m4JSB = new Matrix4().fromArray(state.jsArrayB);
    const glMatrixWasmMatrix4 = glMatrixWasm.Matrix4;

    // Run the function, passing the starting address and length.
    wasmSIMDAPI.mat4_multiply(
      stateSIMD.cArrayPointerA,
      stateSIMD.cArrayPointerB,
      stateSIMD.cArrayPointerOut,
    );
    expect(stateSIMD.cArrayOut).toBe(jsArrayExpect);

    wasmAPI.mat4_multiply(
      state.cArrayPointerA,
      state.cArrayPointerB,
      state.cArrayPointerOut,
    );
    expect(state.cArrayOut).toBe(jsArrayExpect);

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
        wasm_bench_simd() {
          wasmSIMDAPI.test(2, loopCount);
        },
      },
      1,
    );
  });
}
