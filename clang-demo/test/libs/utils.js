export async function load(name) {
  const [{ instance }, { instance: instanceSIMD }] = await Promise.all([
    WebAssembly.instantiateStreaming(fetch(`../dist/${name}.wasm`)),
    WebAssembly.instantiateStreaming(fetch(`../dist/${name}.simd.wasm`)),
  ]);
  const { exports: wasmAPI } = instance;
  const { exports: wasmSIMDAPI } = instanceSIMD;

  return { instance, instanceSIMD, wasmAPI, wasmSIMDAPI };
}
