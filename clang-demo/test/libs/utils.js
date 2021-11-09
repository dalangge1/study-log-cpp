export async function load(name) {
  const [{ instance }, { instance: instanceSIMD }] = await Promise.all([
    WebAssembly.instantiateStreaming(
      fetch(`../dist/${name}.wasm?t=${Date.now()}`),
    ),
    WebAssembly.instantiateStreaming(
      fetch(`../dist/${name}.simd.wasm?t=${Date.now()}`),
    ),
  ]);
  const { exports: wasmAPI } = instance;
  const { exports: wasmSIMDAPI } = instanceSIMD;

  return { instance, instanceSIMD, wasmAPI, wasmSIMDAPI };
}
