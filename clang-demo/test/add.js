import { test, expect, benchmark } from './libs/test.js';
import { load } from './libs/utils.js';

export async function add() {
  const { wasmAPI } = await load('add');

  function add(a, b) {
    return a + b;
  }

  test('add', () => {
    expect(wasmAPI.add(4, 1)).toBe(5);

    benchmark(
      {
        js(i) {
          add(i, 1);
        },
        wasm(i) {
          wasmAPI.add(i, 1);
        },
      },
      100000,
    );
  });
}
