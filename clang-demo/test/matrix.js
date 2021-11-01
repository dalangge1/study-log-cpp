import { test, expect, benchmark } from './test.js';
import { load } from './utils.js';

export async function matrix() {
  const { instance, instanceSIMD } = await load('matrix');
  const glMatrixWasm = window.glMatrixWasm;

  await glMatrixWasm.init();

  // prettier-ignore
  const ms = [
    glMatrixWasm.Matrix4.fromValues(7, 3, 6, 9, 2, 3, 2, 5, 1, 9, 5, 8, 3, 7, 2, 2),
    glMatrixWasm.Matrix4.fromValues(2, 5, 7, 8, 4, 8, 3, 9, 2, 5, 4, 9, 5, 6, 3, 1),
    glMatrixWasm.Matrix4.fromValues(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1),
  ];

  const jsArrayExpect = new Array(4 * 4).fill(6);

  /**
   * @param {WebAssembly.Instance} instance
   */
  function init(instance) {
    const jsArrayA = new Array(4 * 4).fill(2);
    const jsArrayB = new Array(4 * 4).fill(3);

    // Allocate memory for 4 32-bit integers
    // and return get starting address.
    const cArrayPointerA = instance.exports.malloc(jsArrayA.length * 4);
    const cArrayPointerB = instance.exports.malloc(jsArrayA.length * 4);
    const cArrayPointerOut = instance.exports.malloc(jsArrayA.length * 4);

    // Turn that sequence of 32-bit integers
    // into a Float32Array, starting at that address.
    const cArrayA = new Float32Array(
      instance.exports.memory.buffer,
      cArrayPointerA,
      jsArrayA.length,
    );
    const cArrayB = new Float32Array(
      instance.exports.memory.buffer,
      cArrayPointerB,
      jsArrayA.length,
    );
    const cArrayOut = new Float32Array(
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
    instanceSIMD.exports.mat4_multiply(
      stateSIMD.cArrayPointerOut,
      stateSIMD.cArrayPointerA,
      stateSIMD.cArrayPointerB,
      stateSIMD.jsArrayA.length,
    );
    expect(stateSIMD.cArrayOut).toBe(jsArrayExpect);

    instance.exports.mat4_multiply(
      state.cArrayPointerA,
      state.cArrayPointerB,
      state.cArrayPointerOut,
    );
    expect(state.cArrayOut).toBe(jsArrayExpect);

    benchmark(
      {
        wasm_simd() {
          instanceSIMD.exports.mat4_multiply(
            stateSIMD.cArrayPointerA,
            stateSIMD.cArrayPointerB,
            stateSIMD.cArrayPointerOut,
          );
        },
        wasm() {
          instance.exports.mat4_multiply(
            state.cArrayPointerA,
            state.cArrayPointerB,
            state.cArrayPointerOut,
          );
        },
        rust_wasm() {
          glMatrixWasm.Matrix4.multiply(ms[2], ms[0], ms[1]);
        },
      },
      100000,
    );
  });
}
