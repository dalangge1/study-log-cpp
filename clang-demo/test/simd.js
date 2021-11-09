import { test, expect, benchmark } from './libs/test.js';
import { load } from './libs/utils.js';

export async function simd() {
  const { instance, instanceSIMD, wasmAPI, wasmSIMDAPI } = await load('simd');

  const jsArrayExpect = new Array(4 * 10).fill(6);

  /**
   * @param {WebAssembly.Instance} instance
   */
  function init(wasmAPI) {
    const jsArrayA = new Array(4 * 10).fill(2);
    const jsArrayB = new Array(4 * 10).fill(3);

    // Allocate memory for 4 32-bit integers
    // and return get starting address.
    const cArrayPointerA = wasmAPI.malloc(jsArrayA.length * 4);
    const cArrayPointerB = wasmAPI.malloc(jsArrayA.length * 4);
    const cArrayPointerOut = wasmAPI.malloc(jsArrayA.length * 4);

    // Turn that sequence of 32-bit integers
    // into a Int32Array, starting at that address.
    const cArrayA = new Int32Array(
      wasmAPI.memory.buffer,
      cArrayPointerA,
      jsArrayA.length,
    );
    const cArrayB = new Int32Array(
      wasmAPI.memory.buffer,
      cArrayPointerB,
      jsArrayA.length,
    );
    const cArrayOut = new Int32Array(
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
    };
  }

  const state = init(wasmAPI);
  const stateSIMD = init(wasmSIMDAPI);

  // performance test
  test('multiply array', () => {
    // Run the function, passing the starting address and length.
    wasmSIMDAPI.multiply_arrays(
      stateSIMD.cArrayPointerOut,
      stateSIMD.cArrayPointerA,
      stateSIMD.cArrayPointerB,
      stateSIMD.jsArrayA.length,
    );
    expect(stateSIMD.cArrayOut).toBe(jsArrayExpect);

    wasmAPI.multiply_arrays_no_simd(
      state.cArrayPointerOut,
      state.cArrayPointerA,
      state.cArrayPointerB,
      state.jsArrayA.length,
    );
    expect(state.cArrayOut).toBe(jsArrayExpect);

    benchmark(
      {
        wasm_simd() {
          wasmSIMDAPI.multiply_arrays(
            stateSIMD.cArrayPointerOut,
            stateSIMD.cArrayPointerA,
            stateSIMD.cArrayPointerB,
            stateSIMD.jsArrayA.length,
          );
        },
        wasm() {
          wasmAPI.multiply_arrays_no_simd(
            state.cArrayPointerOut,
            state.cArrayPointerA,
            state.cArrayPointerB,
            state.jsArrayA.length,
          );
        },
      },
      100000,
    );
  });

  test('void fn', () => {
    const { exports } = instance;
    benchmark(
      {
        wasm_void_fn_call_with_cached_exports() {
          exports.void_fn();
        },
        // 不缓存exports会导致非常可观的额外耗时
        wasm_void_fn_call_with_instance_exports() {
          instance.exports.void_fn();
        },
      },
      100000,
    );
  });

  test('f32x4_make', () => {
    expect(wasmSIMDAPI.test_f32x4_make(1, 2, 3, 4)).toBe(20);
  });
  test('v32x4_shuffle', () => {
    expect(wasmSIMDAPI.test_f32x4_shuffle(1, 2, 3, 4)).toBe(10);
  });
  test('benchmark', () => {
    benchmark(
      {
        make() {
          wasmSIMDAPI.test_v128_load_by_make(1, 2, 3, 4);
        },
        load() {
          wasmSIMDAPI.test_v128_load(1, 2, 3, 4);
        },
        shuffle() {
          wasmSIMDAPI.test_v128_load_by_shuffle(1, 2, 3, 4);
        },
      },
      1000_000,
    );
  });
}
