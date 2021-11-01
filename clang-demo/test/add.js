import { test, expect, benchmark } from './test.js';

export async function add() {
  const { instance } = await WebAssembly.instantiateStreaming(
    fetch('../dist/addc.wasm'),
  );

  function add(a, b) {
    return a + b;
  }

  test('add', () => {
    expect(instance.exports.add(4, 1)).toBe(5);

    benchmark({
      js(i) {
        add(i, 1);
      },
      wasm(i) {
        instance.exports.add(i, 1);
      },
    }, 100000);
  });
}
