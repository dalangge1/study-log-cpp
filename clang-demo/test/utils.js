export async function load(name) {
  const [{ instance }, { instance: instanceSIMD }] = await Promise.all([
    WebAssembly.instantiateStreaming(fetch(`../dist/${name}.wasm`)),
    WebAssembly.instantiateStreaming(fetch(`../dist/${name}.simd.wasm`)),
  ]);

  return { instance, instanceSIMD };
}
