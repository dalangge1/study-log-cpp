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
      cArrayA,
      cArrayB,
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
    const m4JSC = new Matrix4();
    const m4JSD = new Matrix4();
    m4JSC.elements = new Float32Array(state.jsArrayA);
    m4JSD.elements = new Float32Array(state.jsArrayB);
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
        threejs_f32_array() {
          m4JSC.multiplyMatrices(m4JSC, m4JSD);
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
    const m4JSB = new Matrix4().fromArray([
      1, 2, 3, 4, 5, 6, 7, 8, 8, 2, 1, 2, 7, 1, 3, 2,
    ]);
    const m4JSC = { elements: new Float32Array(m4JSA.elements) };

    // prettier-ignore three.js invert det
    function invert_det(m4) {
      const te = m4.elements,
        n11 = te[0],
        n21 = te[1],
        n31 = te[2],
        n41 = te[3],
        n12 = te[4],
        n22 = te[5],
        n32 = te[6],
        n42 = te[7],
        n13 = te[8],
        n23 = te[9],
        n33 = te[10],
        n43 = te[11],
        n14 = te[12],
        n24 = te[13],
        n34 = te[14],
        n44 = te[15],
        t11 =
          n23 * n34 * n42 -
          n24 * n33 * n42 +
          n24 * n32 * n43 -
          n22 * n34 * n43 -
          n23 * n32 * n44 +
          n22 * n33 * n44,
        t12 =
          n14 * n33 * n42 -
          n13 * n34 * n42 -
          n14 * n32 * n43 +
          n12 * n34 * n43 +
          n13 * n32 * n44 -
          n12 * n33 * n44,
        t13 =
          n13 * n24 * n42 -
          n14 * n23 * n42 +
          n14 * n22 * n43 -
          n12 * n24 * n43 -
          n13 * n22 * n44 +
          n12 * n23 * n44,
        t14 =
          n14 * n23 * n32 -
          n13 * n24 * n32 -
          n14 * n22 * n33 +
          n12 * n24 * n33 +
          n13 * n22 * n34 -
          n12 * n23 * n34;

      const det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
      return det;
    }

    // prettier-ignore
    function gl_matrix_det( te) {
      let a00 = te[0], a10 = te[1], a20 = te[2], a30 = te[3], a01 = te[4],
            a11 = te[5], a21 = te[6], a31 = te[7], a02 = te[8], a12 = te[9],
            a22 = te[10], a32 = te[11], a03 = te[12], a13 = te[13], a23 = te[14],
            a33 = te[15];
    
      let b00 = a00 * a11 - a01 * a10;
      let b01 = a00 * a12 - a02 * a10;
      let b02 = a00 * a13 - a03 * a10;
      let b03 = a01 * a12 - a02 * a11;
      let b04 = a01 * a13 - a03 * a11;
      let b05 = a02 * a13 - a03 * a12;
      let b06 = a20 * a31 - a21 * a30;
      let b07 = a20 * a32 - a22 * a30;
      let b08 = a20 * a33 - a23 * a30;
      let b09 = a21 * a32 - a22 * a31;
      let b10 = a21 * a33 - a23 * a31;
      let b11 = a22 * a33 - a23 * a32;
    
      // Calculate the determinant
      return b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;
    }

    // prettier-ignore 居然能比three原本的快66%
    function determinant2() {
      const te = this.elements;
      const n11 = te[0],
        n12 = te[4],
        n13 = te[8],
        n14 = te[12];
      const n21 = te[1],
        n22 = te[5],
        n23 = te[9],
        n24 = te[13];
      const n31 = te[2],
        n32 = te[6],
        n33 = te[10],
        n34 = te[14];
      const n41 = te[3],
        n42 = te[7],
        n43 = te[11],
        n44 = te[15];

      // s1
      const n14n23n13n24 = n14 * n23 - n13 * n24;
      const n14n21n11n24 = n14 * n21 - n11 * n24;
      const n12n21n11n22 = n12 * n21 - n11 * n22;
      const n13n22n12n23 = n12 * n23 - n13 * n22;
      // s1
      const n11n23n13n21 = n13 * n21 - n11 * n23;
      const n12n24n14n22 = n12 * n24 - n14 * n22;

      // const n14n23n13n24n32 = n14n23n13n24 * n32;
      // const n14n21n11n24n32 = n14n21n11n24 * n32;
      // const n12n21n11n22n34 = n12n21n11n22 * n34;
      // const n13n22n12n23n34 = n13n22n12n23 * n34;

      // const n14n23n13n24n31 = n14n23n13n24 * n31;
      // const n14n21n11n24n33 = n14n21n11n24 * n33;
      // const n12n21n11n22n33 = n12n21n11n22 * n33;
      // const n13n22n12n23n31 = n13n22n12n23 * n31;

      // const n11n23n13n21n34 = n11n23n13n21 * n34;
      // const n11n23n13n21n32 = n11n23n13n21 * n32;
      // const n12n24n14n22n31 = n12n24n14n22 * n31;
      // const n12n24n14n22n33 = n12n24n14n22 * n33;

      const t0 = n14n23n13n24 * n32 + n12n24n14n22 * n33 - n13n22n12n23 * n34;
      const t1 = -n14n23n13n24 * n31 + n14n21n11n24 * n33 - n11n23n13n21 * n34;
      const t2 = -n14n21n11n24 * n32 + n12n21n11n22 * n34 - n12n24n14n22 * n31;
      const t3 = n11n23n13n21 * n32 + n13n22n12n23 * n31 - n12n21n11n22 * n33;

      return n41 * t0 + n42 * t1 + n43 * t2 + n44 * t3;

      // return (
      //   n41 * (n14n23n13n24 * n32 + n12n24n14n22 * n33 - n13n22n12n23 * n34) +
      //   n42 * (-n14n23n13n24 * n31 + n14n21n11n24 * n33 - n11n23n13n21 * n34) +
      //   n43 * (-n14n21n11n24 * n32 + n12n21n11n22 * n34 - n12n24n14n22 * n31) +
      //   n44 * (n11n23n13n21 * n32 + n13n22n12n23 * n31 - n12n21n11n22 * n33)
      // );
    }
    m4JSA.__proto__.determinant2 = determinant2;
    m4JSC.determinant2 = determinant2;

    // prettier-ignore
    function determinant3() {
      const te = this.elements;
      const n11 = te[0], n12 = te[4], n13 = te[8], n14 = te[12];
      const n21 = te[1], n22 = te[5], n23 = te[9], n24 = te[13];
      const n31 = te[2], n32 = te[6], n33 = te[10], n34 = te[14];
      const n41 = te[3], n42 = te[7], n43 = te[11], n44 = te[15];

      // s0
      const n14n23n13n24 = n14 * n23 - n13 * n24;
      const n12n24n14n22 = n12 * n24 - n14 * n22;
      const n13n22n12n23 = n13 * n22 - n12 * n23;
      const n11n23n13n21 = n11 * n23 - n13 * n21;
      // s1
      const n14n21n11n24 = n14 * n21 - n11 * n24;
      const n12n21n11n22 = n12 * n21 - n11 * n22;
      // m4
      const n14n23n13n24n32 = n14n23n13n24 * n32;
      const n12n24n14n22n33 = n12n24n14n22 * n33;
      const n13n22n12n23n34 = n13n22n12n23 * n34;
      const n11n23n13n21n34 = n11n23n13n21 * n34;
      // m5
      const n14n23n13n24n31 = n14n23n13n24 * n31;
      const n12n24n14n22n31 = n12n24n14n22 * n31;
      const n13n22n12n23n31 = n13n22n12n23 * n31;
      const n11n23n13n21n32 = n11n23n13n21 * n32;
      // m6
      const n14n21n11n24n33 = n14n21n11n24 * n33;
      const n12n21n11n22n34 = n12n21n11n22 * n34;
      // m7
      const n14n21n11n24n32 = n14n21n11n24 * n32;
      const n12n21n11n22n33 = n12n21n11n22 * n33;

      return (n41 * (n14n23n13n24n32 + n12n24n14n22n33 + n13n22n12n23n34) +
              n42 * (n11n23n13n21n34 + n14n21n11n24n33 - n14n23n13n24n31) +
              n43 * (-n14n21n11n24n32 + n12n21n11n22n34 - n12n24n14n22n31) +
              n44 * (-n13n22n12n23n31 - n11n23n13n21n32 - n12n21n11n22n33));
    }
    m4JSA.__proto__.determinant3 = determinant3;
    m4JSC.determinant3 = determinant3;

    // prettier-ignore babylon.js 这个determinant2快了2ms, 明明和determinant2一样的运算, 但是就是快了2ms...
    function babylon_determinant(m) {
      const m00 = m[0],
        m01 = m[1],
        m02 = m[2],
        m03 = m[3];
      const m10 = m[4],
        m11 = m[5],
        m12 = m[6],
        m13 = m[7];
      const m20 = m[8],
        m21 = m[9],
        m22 = m[10],
        m23 = m[11];
      const m30 = m[12],
        m31 = m[13],
        m32 = m[14],
        m33 = m[15];
      // https://en.wikipedia.org/wiki/Laplace_expansion
      // to compute the deterrminant of a 4x4 Matrix we compute the cofactors of any row or column,
      // then we multiply each Cofactor by its corresponding matrix value and sum them all to get the determinant
      // Cofactor(i, j) = sign(i,j) * det(Minor(i, j))
      // where
      //  - sign(i,j) = (i+j) % 2 === 0 ? 1 : -1
      //  - Minor(i, j) is the 3x3 matrix we get by removing row i and column j from current Matrix
      //
      // Here we do that for the 1st row.

      const det_22_33 = m22 * m33 - m32 * m23;
      const det_21_33 = m21 * m33 - m31 * m23;
      const det_21_32 = m21 * m32 - m31 * m22;
      const det_20_33 = m20 * m33 - m30 * m23;
      const det_20_32 = m20 * m32 - m22 * m30;
      const det_20_31 = m20 * m31 - m30 * m21;
      const cofact_00 = +(m11 * det_22_33 - m12 * det_21_33 + m13 * det_21_32);
      const cofact_01 = -(m10 * det_22_33 - m12 * det_20_33 + m13 * det_20_32);
      const cofact_02 = +(m10 * det_21_33 - m11 * det_20_33 + m13 * det_20_31);
      const cofact_03 = -(m10 * det_21_32 - m11 * det_20_32 + m12 * det_20_31);
      return (
        m00 * cofact_00 + m01 * cofact_01 + m02 * cofact_02 + m03 * cofact_03
      );
    }

    // 和gl-matrix-wasm一样的算法, 应该出自同一人之手, 不过还是babylon的快一点, 应该最原始出处时https://github.com/toji/gl-matrix
    function oasis_determinant() {
      const e = this.elements;

      const a11 = e[0],
        a12 = e[1],
        a13 = e[2],
        a14 = e[3];
      const a21 = e[4],
        a22 = e[5],
        a23 = e[6],
        a24 = e[7];
      const a31 = e[8],
        a32 = e[9],
        a33 = e[10],
        a34 = e[11];
      const a41 = e[12],
        a42 = e[13],
        a43 = e[14],
        a44 = e[15];

      const b00 = a11 * a22 - a12 * a21;
      const b01 = a11 * a23 - a13 * a21;
      const b02 = a11 * a24 - a14 * a21;
      const b03 = a12 * a23 - a13 * a22;
      const b04 = a12 * a24 - a14 * a22;
      const b05 = a13 * a24 - a14 * a23;
      const b06 = a31 * a42 - a32 * a41;
      const b07 = a31 * a43 - a33 * a41;
      const b08 = a31 * a44 - a34 * a41;
      const b09 = a32 * a43 - a33 * a42;
      const b10 = a32 * a44 - a34 * a42;
      const b11 = a33 * a44 - a34 * a43;

      // Calculate the determinant
      return (
        b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06
      );
    }
    m4JSA.__proto__.oasis_determinant = oasis_determinant;
    m4JSC.oasis_determinant = oasis_determinant;

    expect(m4JSA.determinant()).toBe(76);
    // 和three.js计算结果不一样...
    // expect(gl_matrix_det(m4JSA)).toBe(76);
    expect(invert_det(m4JSA)).toBe(76);
    expect(babylon_determinant(m4JSA.elements)).toBe(76);
    // transpose前后的determinant是相同的
    expect(m4JSA.transpose().determinant()).toBe(76);
    expect(wasmSIMDAPI.mat4_determinant_simd5(stateSIMD.cArrayPointerA)).toBe(
      m4JSB.determinant(),
    );

    const run = (loopCount = 50000) =>
      benchmark(
        {
          wasm() {
            wasmAPI.mat4_determinant(state.cArrayPointerA);
          },
          wasm2() {
            wasmAPI.mat4_determinant2(state.cArrayPointerA);
          },
          // wasm_determinant_babylon() {
          //   wasmAPI.mat4_determinant_babylon(state.cArrayPointerA);
          // },
          // mat4_invert_det() {
          //   wasmAPI.mat4_invert_det(state.cArrayPointerA);
          // },
          // mat4_det_glmatrix() {
          //   wasmAPI.mat4_det_glmatrix(state.cArrayPointerA);
          // },
          // wasm_hand_simd() {
          //   wasmSIMDAPI.mat4_determinant_simd(stateSIMD.cArrayPointerA);
          // },
          // wasm_hand_simd2() {
          //   wasmSIMDAPI.mat4_determinant_simd2(stateSIMD.cArrayPointerA);
          // },
          // wasm_hand_simd3() {
          //   // 更慢了...
          //   wasmSIMDAPI.mat4_determinant_simd3(stateSIMD.cArrayPointerA);
          // },
          // wasm_hand_simd4() {
          //   wasmSIMDAPI.mat4_determinant_simd4(stateSIMD.cArrayPointerA);
          // },
          wasm_hand_simd5() {
            wasmSIMDAPI.mat4_determinant_simd5(stateSIMD.cArrayPointerA);
          },
          // wasm_auto_simd() {
          //   wasmSIMDAPI.mat4_determinant(stateSIMD.cArrayPointerA);
          // },
          // wasm_auto_simd2() {
          //   wasmSIMDAPI.mat4_determinant2(stateSIMD.cArrayPointerA);
          // },
          threejs_determinant() {
            m4JSA.determinant();
          },
          threejs_determinant2() {
            m4JSA.determinant2();
          },
          threejs_determinant2_f32_array(i) {
            m4JSC.determinant2();
            m4JSC.elements[1] = i;
          },
          // threejs_determinant3(i) {
          //   m4JSA.determinant3();
          //   m4JSA.elements[1] = i;
          // },
          babylon_determinant() {
            babylon_determinant(m4JSA.elements);
          },
          babylon_determinant_f32_array() {
            babylon_determinant(m4JSC.elements);
          },
          oasis_determinant() {
            m4JSA.oasis_determinant();
          },
          oasis_determinant_f32_array() {
            m4JSC.oasis_determinant();
          },
          // threejs_invert_det() {
          //   invert_det(m4JSA);
          //   // 和threejs_determinant没明显性能差距
          // },
        },
        loopCount,
      );

    // run(10000);
    // run();
    // run();
    // run(10000);
    run(1000000);

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
        wasm_simd5() {
          wasmSIMDAPI.test(8, loopCount);
        },
      },
      1,
      `_loop_in_wasm_${loopCount}`,
    );
  });

  test('matrix_invert', () => {
    const m4JSA = new Matrix4().fromArray(state.jsArrayA);

    state.cArrayA.set(state.jsArrayA);
    m4JSA.invert();
    wasmAPI.mat4_invert(state.cArrayPointerA);
    expect(state.cArrayA).toBe(m4JSA.elements);

    benchmark(
      {
        wasm() {
          wasmAPI.mat4_invert(state.cArrayPointerA);
        },
        wasm2() {
          wasmAPI.mat4_invert2(state.cArrayPointerA);
        },
        wasm_simd() {
          wasmSIMDAPI.mat4_invert_simd(stateSIMD.cArrayPointerA);
        },
        wasm_simd2() {
          wasmSIMDAPI.mat4_invert_simd2(stateSIMD.cArrayPointerA);
        },
        threejs() {
          m4JSA.invert();
        },
      },
      1000_000,
    );

    const loopCount = 1_000_000;
    benchmark(
      {
        wasm() {
          wasmSIMDAPI.test(9, loopCount);
        },
        wasm_simd() {
          wasmSIMDAPI.test(10, loopCount);
        },
      },
      1,
      `_loop_in_wasm_${loopCount}`,
    );
  });
}
