import { test, expect, benchmark } from './test.js';
import { load } from './utils.js';

export async function simd() {
  const { instance, instanceSIMD } = await load('simd');

  const jsArrayExpect = new Array(4 * 10).fill(6);

  /**
   * @param {WebAssembly.Instance} instance
   */
  function init(instance) {
    const jsArrayA = new Array(4 * 10).fill(2);
    const jsArrayB = new Array(4 * 10).fill(3);

    // Allocate memory for 4 32-bit integers
    // and return get starting address.
    const cArrayPointerA = instance.exports.malloc(jsArrayA.length * 4);
    const cArrayPointerB = instance.exports.malloc(jsArrayA.length * 4);
    const cArrayPointerOut = instance.exports.malloc(jsArrayA.length * 4);

    // Turn that sequence of 32-bit integers
    // into a Int32Array, starting at that address.
    const cArrayA = new Int32Array(
      instance.exports.memory.buffer,
      cArrayPointerA,
      jsArrayA.length,
    );
    const cArrayB = new Int32Array(
      instance.exports.memory.buffer,
      cArrayPointerB,
      jsArrayA.length,
    );
    const cArrayOut = new Int32Array(
      instance.exports.memory.buffer,
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

  const state = init(instance);
  const stateSIMD = init(instanceSIMD);

  // performance test
  test('multiply array', () => {
    // Run the function, passing the starting address and length.
    instanceSIMD.exports.multiply_arrays(
      stateSIMD.cArrayPointerOut,
      stateSIMD.cArrayPointerA,
      stateSIMD.cArrayPointerB,
      stateSIMD.jsArrayA.length,
    );
    expect(stateSIMD.cArrayOut).toBe(jsArrayExpect);

    instance.exports.multiply_arrays_no_simd(
      state.cArrayPointerOut,
      state.cArrayPointerA,
      state.cArrayPointerB,
      state.jsArrayA.length,
    );
    expect(state.cArrayOut).toBe(jsArrayExpect);

    benchmark(
      {
        wasm_simd() {
          instanceSIMD.exports.multiply_arrays(
            stateSIMD.cArrayPointerOut,
            stateSIMD.cArrayPointerA,
            stateSIMD.cArrayPointerB,
            stateSIMD.jsArrayA.length,
          );
        },
        wasm() {
          instance.exports.multiply_arrays_no_simd(
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
}
