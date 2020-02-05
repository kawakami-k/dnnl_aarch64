/*******************************************************************************
* Copyright 2019-2020 FUJITSU LIMITED
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/*******************************************************************************
* Copyright 2016-2018 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#ifndef CPU_JIT_AVX2_GENERATOR_HPP_AARCH64
#define CPU_JIT_AVX2_GENERATOR_HPP_AARCH64

#define XBYAK_CODE_PTR uint32

#include "cpu_isa_traits.hpp"
#include <limits.h>

#include "mkldnn_thread.hpp"
#include "utils.hpp"

#ifdef JIT_PROFILING_VTUNE
#include "jitprofiling.h"
#endif

#if defined(_WIN32) && !defined(__GNUC__)
#define STRUCT_ALIGN(al, ...) __declspec(align(al)) __VA_ARGS__
#else
#define STRUCT_ALIGN(al, ...) __VA_ARGS__ __attribute__((__aligned__(al)))
#endif

#if defined(_WIN32)
#define OFFSET_SHADOWSPACE 0x28
#endif

#define DECLARE_CPU_JIT_AUX_FUNCTIONS(jit_name)                       \
    const char *name() const override { return STRINGIFY(jit_name); } \
    const char *source_file() const override { return __FILE__; }

namespace mkldnn {
namespace impl {
namespace cpu {

// TODO: move this to jit_generator class?
#if 1
namespace {

/* following code is implemented in jit_generator.hpp */
#if 0
typedef enum {
    PAGE_4K = 4096,
    PAGE_2M = 2097152,
} cpu_page_size_t;

// TODO: move this somewhere else? Although this is only used by jit kernels
// (Roma)
static inline int float2int(float x) {
    union {
        float vfloat;
        int vint;
    } cvt;
    cvt.vfloat = x;
    return cvt.vint;
}
#endif


// TODO: A GPR class that hides ABI details from the JIT kernels and allows
// numbering registers from 0 to 14 (x86_64) / 6 (x32) (gpr0, gpr1, ...) and
// stack register (sr).
//
// This will allow using syntax like this:
//
// param = gpr0;
// reg_input = gpr0;
// reg_output =  gpr1;
// ...
//
// #ifndef XBYAK64
// mov(param, ptr[sr])
// #endif
//
// (Roma)

#if XBYAK_TRANSLATE_AARCH64
#ifndef CPU_JIT_AVX2_GENERATOR_HPP
#define CPU_JIT_AVX2_GENERATOR_HPP
// Callee-saved registers
  constexpr Xbyak::Xbyak_aarch64::Operand::Code abi_save_gpr_regs_aarch64[] = { Xbyak::Xbyak_aarch64::Operand::X19,
    Xbyak::Xbyak_aarch64::Operand::X20, Xbyak::Xbyak_aarch64::Operand::X21, Xbyak::Xbyak_aarch64::Operand::X22,
    Xbyak::Xbyak_aarch64::Operand::X23, Xbyak::Xbyak_aarch64::Operand::X24, Xbyak::Xbyak_aarch64::Operand::X25,
    Xbyak::Xbyak_aarch64::Operand::X26, Xbyak::Xbyak_aarch64::Operand::X27, Xbyak::Xbyak_aarch64::Operand::X28 };

// See "Procedure Call Standsard for the ARM 64-bit Architecture (AArch64)"
static const Xbyak::Xbyak_aarch64::XReg abi_param1_aarch64(Xbyak::Xbyak_aarch64::Operand::X0),
        abi_param2_aarch64(Xbyak::Xbyak_aarch64::Operand::X1),
        abi_param3_aarch64(Xbyak::Xbyak_aarch64::Operand::X2),
        abi_param4_aarch64(Xbyak::Xbyak_aarch64::Operand::X3),
        abi_param5_aarch64(Xbyak::Xbyak_aarch64::Operand::X4),
        abi_param6_aarch64(Xbyak::Xbyak_aarch64::Operand::X5),
        abi_param7_aarch64(Xbyak::Xbyak_aarch64::Operand::X6),
        abi_param8_aarch64(Xbyak::Xbyak_aarch64::Operand::X7),
        abi_not_param1_aarch64(Xbyak::Xbyak_aarch64::Operand::X15); // Fujitsu uses X15 on A64FX as
                                             // abi_not_param1 on x64.
#endif //#ifndef CPU_JIT_AVX2_GENERATOR_HPP

#else // __ARM_ARCH
constexpr Xbyak::Operand::Code abi_save_gpr_regs[] = {
    Xbyak::Operand::RBX,
    Xbyak::Operand::RBP,
    Xbyak::Operand::R12,
    Xbyak::Operand::R13,
    Xbyak::Operand::R14,
    Xbyak::Operand::R15,
#ifdef _WIN32
    Xbyak::Operand::RDI,
    Xbyak::Operand::RSI,
#endif
};

#ifdef _WIN32
static const Xbyak::Reg64 abi_param1(Xbyak::Operand::RCX),
        abi_param2(Xbyak::Operand::RDX), abi_param3(Xbyak::Operand::R8),
        abi_param4(Xbyak::Operand::R9), abi_not_param1(Xbyak::Operand::RDI);
#else
static const Xbyak::Reg64 abi_param1(Xbyak::Operand::RDI),
        abi_param2(Xbyak::Operand::RSI), abi_param3(Xbyak::Operand::RDX),
        abi_param4(Xbyak::Operand::RCX), abi_param5(Xbyak::Operand::R8),
        abi_param6(Xbyak::Operand::R9), abi_not_param1(Xbyak::Operand::RCX);
#endif // #ifdef _WIN32
#endif // __ARM_ARCH

#if 0
inline unsigned int get_cache_size(int level, bool per_core = true) {
    unsigned int l = level - 1;
    // Currently, if XByak is not able to fetch the cache topology
    // we default to 32KB of L1, 512KB of L2 and 1MB of L3 per core.
    if (cpu.getDataCacheLevels() == 0) {
        const int L1_cache_per_core = 32000;
        const int L2_cache_per_core = 512000;
        const int L3_cache_per_core = 1024000;
        int num_cores = per_core ? 1 : mkldnn_get_max_threads();
        switch (l) {
        case (0): return L1_cache_per_core * num_cores;
        case (1): return L2_cache_per_core * num_cores;
        case (2): return L3_cache_per_core * num_cores;
        default: return 0;
        }
    }
    if (l < cpu.getDataCacheLevels()) {
        return cpu.getDataCacheSize(l)
                / (per_core ? cpu.getCoresSharingDataCache(l) : 1);
    } else
        return 0;
}
#endif

} // namespace
#endif //#if 1


class jit_generator_aarch64 : public Xbyak::Xbyak_aarch64::CodeGeneratorAArch64
{
private:
    const size_t xmm_len = 16;
#ifdef _WIN32
    const size_t xmm_to_preserve_start = 6;
    const size_t xmm_to_preserve = 10;
#else
    const size_t xmm_to_preserve_start = 0;
    const size_t xmm_to_preserve = 0;
#endif

#ifdef XBYAK_TRANSLATE_AARCH64
    const size_t xreg_len = 8;

    const size_t vreg_len_preserve = 8; // Only bottom 8byte must be preserved.
    const size_t vreg_to_preserve_start = 8;
    const size_t vreg_to_preserve = 8; // VREG8 - VREG15

    const size_t gpr_to_preserve_start = 19;
    const size_t gpr_to_preserve = 19;
#endif

    const size_t num_abi_save_gpr_regs
            = sizeof(abi_save_gpr_regs_aarch64) / sizeof(abi_save_gpr_regs_aarch64[0]);

#ifdef XBYAK_TRANSLATE_AARCH64
    const size_t size_of_abi_save_regs = num_abi_save_gpr_regs * x0.getBit() / 8
            + vreg_to_preserve * vreg_len_preserve;

	const size_t preserved_stack_size = xreg_len * (2 + num_abi_save_gpr_regs) + vreg_len_preserve * vreg_to_preserve;
#else
    const size_t size_of_abi_save_regs
            = num_abi_save_gpr_regs * rax.getBit() / 8
            + xmm_to_preserve * xmm_len;
#endif

public:
    enum {
        _cmp_eq_oq = 0u,
        _cmp_lt_os = 1u,
        _cmp_le_os = 2u,
        _cmp_neq_uq = 4u,
        _cmp_nlt_us = 5u,
        _cmp_nle_us = 6u,

        _op_floor = 1u,
    };

#if defined(XBYAK_TRANSLATE_AARCH64)
    Xbyak::Xbyak_aarch64::XReg param1 = abi_param1_aarch64;
    const int EVEX_max_8b_offt = 0x200;
//    const Xbyak::Reg64 reg_EVEX_max_8b_offt = x5;

    class XRegValue : public Xbyak::Xbyak_aarch64::XReg {
public:
    int64_t value_;
    explicit XRegValue(uint32_t idx, int64_t value) : XReg(idx), value_(value) {}
    explicit XRegValue(uint32_t idx) : XReg(idx), value_(0xFFFFFFFFFFFFFFFF) {}
  };

    inline size_t get_size_of_abi_save_regs() { return size_of_abi_save_regs; }

    void preamble() {
      stp(x29, x30, pre_ptr(sp, -(static_cast<int64_t>(preserved_stack_size))));
	  add(x29, sp, xreg_len * 2);
        if (vreg_to_preserve) {
            st4((v8.d - v11.d)[0], post_ptr(x29, vreg_len_preserve*4));
            st4((v12.d - v15.d)[0], post_ptr(x29, vreg_len_preserve*4));
        }
        for (size_t i = 0; i < num_abi_save_gpr_regs; i += 2) {
            stp(Xbyak::Xbyak_aarch64::XReg(abi_save_gpr_regs_aarch64[i]),
                    Xbyak::Xbyak_aarch64::XReg(abi_save_gpr_regs_aarch64[i + 1]), post_ptr(x29, xreg_len*2));
	}
    }
#else
    Xbyak::Reg64 param1 = abi_param1;
    const int EVEX_max_8b_offt = 0x200;
    const Xbyak::Reg64 reg_EVEX_max_8b_offt = rbp;

    inline size_t get_size_of_abi_save_regs() { return size_of_abi_save_regs; }

    void preamble() {

      if (xmm_to_preserve) {
            sub(rsp, xmm_to_preserve * xmm_len); // imm減算
            for (size_t i = 0; i < xmm_to_preserve; ++i)
                movdqu(ptr[rsp + i * xmm_len],
                        Xbyak::Xmm(xmm_to_preserve_start
                                + i)); // xmmレジスタをメモリに待避
        }
        for (size_t i = 0; i < num_abi_save_gpr_regs; ++i)
            push(Xbyak::Reg64(abi_save_gpr_regs[i]));
        if (mayiuse(avx512_common)) {
            mov(reg_EVEX_max_8b_offt, 2 * EVEX_max_8b_offt);
        }
    }
#endif

#ifndef XBYAK_TRANSLATE_AARCH64
    void mic_prefetcht0(Xbyak::Address a) {
        if (mayiuse(avx512_mic))
            prefetcht0(a);
    }

    void mic_prefetcht1(Xbyak::Address a) {
        if (mayiuse(avx512_mic))
            prefetcht1(a);
    }

    void mic_prefetcht2(Xbyak::Address a) {
        if (mayiuse(avx512_mic))
            prefetcht2(a);
    }
#endif // #ifndef XBYAK_TRANSLATE_AARCH64

#ifdef XBYAK_TRANSLATE_AARCH64
#if 0
    template<class T> void uni_reg_clear(const T& r) {

#ifdef XBYAK_AARCH64_CHECKER
    Xbyak::ElementWidth ew = Xbyak::undefElement;
    setWriteFlag(r, true);

    switch(r.getBit()) {
    case 8: ew = Xbyak::byteElement; break;
    case 16: ew = Xbyak::halfElement; break;
    case 32: ew = Xbyak::singleElement; break;
    case 64: ew = Xbyak::doubleElement; break;
    case 128: ew = Xbyak::quadElement; break;
    default: assert(!"no reach here"); break;
    }

    setElementWidth(r, ew);
#endif //#ifdef XBYAK_AARCH64_CHECKER
    eor(r, r, r);
  }
#endif

    void uni_vzeroupper() {
        assert(NULL);
    }

    void postamble() {
      add(x29, sp, xreg_len * 2);

        if (vreg_to_preserve) {
            ld4((v8.d - v11.d)[0], post_ptr(x29, vreg_len_preserve*4));
	  ld4((v12.d - v15.d)[0], post_ptr(x29, vreg_len_preserve*4));
        }

        for (size_t i = 0; i < num_abi_save_gpr_regs; i += 2) {
            ldp(Xbyak::Xbyak_aarch64::XReg(abi_save_gpr_regs_aarch64[i]),
                    Xbyak::Xbyak_aarch64::XReg(abi_save_gpr_regs_aarch64[i + 1]), post_ptr(x29, xreg_len*2));
	}

	ldp(x29, x30, post_ptr(sp, static_cast<int64_t>(preserved_stack_size)));
        ret();
    }
#else
    void uni_vzeroupper() {
        if (mayiuse(avx) && !mayiuse(avx512_mic))
            vzeroupper();
    }

    void postamble() {
        for (size_t i = 0; i < num_abi_save_gpr_regs; ++i)
            pop(Xbyak::Reg64(abi_save_gpr_regs[num_abi_save_gpr_regs - 1 - i]));
        if (xmm_to_preserve) {
            for (size_t i = 0; i < xmm_to_preserve; ++i)
                movdqu(Xbyak::Xmm(xmm_to_preserve_start + i),
                        ptr[rsp + i * xmm_len]);
            add(rsp, xmm_to_preserve * xmm_len);
        }
        uni_vzeroupper();
        ret();
    }
#endif

#ifdef XBYAK_TRANSLATE_AARCH64
    /*
      template <typename T>
      Xbyak::Xbyak_aarch64::Adr EVEX_compress_addr(Xbyak::Xbyak_aarch64::XReg base,
          T raw_offt, bool bcast = false)
      {
        return Adr(BASE_ONLY);
      }
    */

    Xbyak::Xbyak_aarch64::Adr make_safe_addr(const Xbyak::Xbyak_aarch64::XReg &reg_out, size_t offt,
            const Xbyak::Xbyak_aarch64::XReg &tmp_reg, bool bcast = false) {

        assert(NULL);
        return ptr(x0);
    }

    Xbyak::Xbyak_aarch64::Adr EVEX_compress_addr_safe(const Xbyak::Xbyak_aarch64::XReg &base, size_t raw_offt,
            const Xbyak::Xbyak_aarch64::XReg &reg_offt, bool bcast = false) {

        assert(NULL);
        return ptr(x0);
    }

#else // #ifdef 1
    template <typename T>
    Xbyak::Address EVEX_compress_addr(
            Xbyak::Reg64 base, T raw_offt, bool bcast = false) {
        using Xbyak::Address;
        using Xbyak::Reg64;
        using Xbyak::RegExp;
        using Xbyak::Zmm;

        assert(raw_offt <= INT_MAX);
        auto offt = static_cast<int>(raw_offt);

        int scale = 0;

        if (EVEX_max_8b_offt <= offt && offt < 3 * EVEX_max_8b_offt) {
            offt = offt - 2 * EVEX_max_8b_offt;
            scale = 1;
        } else if (3 * EVEX_max_8b_offt <= offt
                && offt < 5 * EVEX_max_8b_offt) {
            offt = offt - 4 * EVEX_max_8b_offt;
            scale = 2;
        }

        auto re = RegExp() + base + offt;
        if (scale)
            re = re + reg_EVEX_max_8b_offt * scale;

        if (bcast)
            return zword_b[re];
        else
            return zword[re];
    }

    Xbyak::Address make_safe_addr(const Xbyak::Reg64 &reg_out, size_t offt,
            const Xbyak::Reg64 &tmp_reg, bool bcast = false) {
        if (offt > INT_MAX) {
            mov(tmp_reg, offt);
            return bcast ? ptr_b[reg_out + tmp_reg] : ptr[reg_out + tmp_reg];
        } else {
            return bcast ? ptr_b[reg_out + offt] : ptr[reg_out + offt];
        }
    }

    Xbyak::Address EVEX_compress_addr_safe(const Xbyak::Reg64 &base,
            size_t raw_offt, const Xbyak::Reg64 &reg_offt, bool bcast = false) {
        if (raw_offt > INT_MAX) {
            return make_safe_addr(base, raw_offt, reg_offt, bcast);
        } else {
            return EVEX_compress_addr(base, raw_offt, bcast);
        }
    }
#endif // #ifdef 1

    // Disallow char-based labels completely
    void L(const char *label) = delete;
    void L(Xbyak::Xbyak_aarch64::LabelAArch64 &label) { Xbyak::Xbyak_aarch64::CodeGeneratorAArch64::L_aarch64(label); }

    void L_aligned(Xbyak::Xbyak_aarch64::LabelAArch64&label, int alignment = 16) {
        align(alignment);
        L(label);
    }

#ifdef XBYAK_TRANSLATE_AARCH64
    // WREG, XREG, VREG, ZREG
#if 0  
  void uni_vpxor(const Xbyak::VRreg &x1, const Xbyak::Xbyak_aarch64::VReg &x2, const Xbyak::VRreg &op) {
    assert(x1.getIdx() == x2.getIdx());
    xor(x2, x2, op);
  }

  void uni_vpxor(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::ZReg &x2, const Xbyak::Xbyak_aarch64::ZReg &op) {
    assert(x1.getIdx() == x2.getIdx());
    xor(x2, x2, op);
  }

  void uni_vmovss(const Xbyak::Xbyak_aarch64::Adr& addr, const Xbyak::Xbyak_aarch64::QReg& x) {
    str(x, addr);
  }

  void uni_vmovss(const Xbyak::Xbyak_aarch64::Adr& addr, const Xbyak::Xbyak_aarch64::ZReg& x) {
    str(x, addr);
  }

  void uni_vmovss(const Xbyak::Xbyak_aarch64::Adr& addr, const Xbyak::Xbyak_aarch64::QReg& x) {
    str(x, addr);
  }

  void uni_vmovss(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Xbyak_aarch64::Adr& addr) {
    ldr(x, addr);
  }
#endif // #if 0
    void uni_vpxor(const Xbyak::Xbyak_aarch64::QReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2,
            const Xbyak::Xbyak_aarch64::QReg &op) {
        assert(NULL);
    }
    void uni_vpxor(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::ZReg &x2,
            const Xbyak::Xbyak_aarch64::ZReg &op) {
        assert(NULL);
    }

    void uni_vmovss(const Xbyak::Xbyak_aarch64::Adr &addr, const Xbyak::Xbyak_aarch64::QReg &x) {
        assert(NULL);
    }
    void uni_vmovss(const Xbyak::Xbyak_aarch64::Adr &addr, const Xbyak::Xbyak_aarch64::ZReg &x) {
        assert(NULL);
    }
    void uni_vmovss(const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Xbyak_aarch64::Adr &addr) {
        assert(NULL);
    }
    void uni_vmovss(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Xbyak_aarch64::Adr &addr) {
        assert(NULL);
    }

    void uni_vmovsd(const Xbyak::Xbyak_aarch64::Adr &addr, const Xbyak::Xbyak_aarch64::QReg &x) {
        assert(NULL);
    }
    void uni_vmovsd(const Xbyak::Xbyak_aarch64::Adr &addr, const Xbyak::Xbyak_aarch64::ZReg &x) {
        assert(NULL);
    }
    void uni_vmovsd(const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Xbyak_aarch64::Adr &addr) {
        assert(NULL);
    }
    void uni_vmovsd(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Xbyak_aarch64::Adr &addr) {
        assert(NULL);
    }

    void uni_vmovdqu(const Xbyak::Xbyak_aarch64::Adr &addr, const Xbyak::Xbyak_aarch64::QReg &x) {
        assert(NULL);
    }
    void uni_vmovdqu(const Xbyak::Xbyak_aarch64::Adr &addr, const Xbyak::Xbyak_aarch64::ZReg &x) {
        assert(NULL);
    }

    void uni_vmovdqu(const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Xbyak_aarch64::Adr &addr) {
        assert(NULL);
    }
    void uni_vmovdqu(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Xbyak_aarch64::Adr &addr) {
        assert(NULL);
    }

    void uni_vmovups(const Xbyak::Xbyak_aarch64::Adr &addr, const Xbyak::Xbyak_aarch64::QReg &x) {
        assert(NULL);
    }
    void uni_vmovups(const Xbyak::Xbyak_aarch64::Adr &addr, const Xbyak::Xbyak_aarch64::ZReg &x) {
        assert(NULL);
    }

    void uni_vmovups(const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Operand &op) {
        assert(NULL);
    }
    void uni_vmovups(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Operand &op) {
        assert(NULL);
    }

    void uni_vmovntps(const Xbyak::Xbyak_aarch64::Adr &addr, const Xbyak::Xbyak_aarch64::QReg &x) {
        assert(NULL);
    }
    void uni_vmovntps(const Xbyak::Xbyak_aarch64::Adr &addr, const Xbyak::Xbyak_aarch64::ZReg &x) {
        assert(NULL);
    }

    void uni_vbroadcastss(const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Operand &op) {
        assert(NULL);
    }
    void uni_vbroadcastss(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Operand &op) {
        assert(NULL);
    }

    void uni_vpbroadcastd(const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Operand &op) {
        assert(NULL);
    }
    void uni_vpbroadcastd(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Operand &op) {
        assert(NULL);
    }

    void uni_vrcpss(const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Operand &op) {
        assert(NULL);
    }
    void uni_vrcpss(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2) {
        assert(NULL);
    }
    void uni_vrcpss(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Xbyak_aarch64::Adr &op) {
        assert(NULL);
    }

    void uni_vrcpps(const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Operand &op) {
        assert(NULL);
    }
    void uni_vrcpps(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Operand &op) {
        assert(NULL);
    }

    void uni_vdivps() {
        assert(NULL);
    }

    void uni_vdivps(const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2, const Xbyak::Xbyak_aarch64::QReg &buf) {
        assert(NULL);
    }

    void uni_vdivps(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2, const Xbyak::Xbyak_aarch64::ZReg &buf) {
        assert(NULL);
    }

    void uni_vaddps() {
        assert(NULL);
    }

    void uni_vaddss() {
        assert(NULL);
    }

    void uni_vpsignd(const Xbyak::Xbyak_aarch64::QReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2,
            const Xbyak::Operand &op) {
        assert(NULL);
    }
    void uni_vpsignd(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::ZReg &x2,
            const Xbyak::Operand &op) {
        assert(NULL);
    }

    void uni_vsubss() {
        assert(NULL);
    }

    void uni_vsubps() {
        assert(NULL);
    }

    void uni_vsubps(const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2, const Xbyak::Xbyak_aarch64::QReg &buf) {
        assert(NULL);
    }

    void uni_vsubps(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2, const Xbyak::Xbyak_aarch64::ZReg &buf) {
        assert(NULL);
    }

    void uni_vmulps() {
        assert(NULL);
    }

    void uni_vmulss() {
        assert(NULL);
    }
    void uni_vmulss(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Operand &op1,
            const Xbyak::Xbyak_aarch64::Adr &op2) {
        assert(NULL);
    }
    void uni_vmulss(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Operand &op1,
            const Xbyak::Xbyak_aarch64::ZReg &op2) {
        assert(NULL);
    }

    void uni_vfmadd213ps(const Xbyak::Xbyak_aarch64::QReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2,
            const Xbyak::Operand &op) {
        assert(NULL);
    }
    void uni_vfmadd213ps(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::ZReg &x2,
            const Xbyak::Operand &op) {
        assert(NULL);
    }

    void uni_vfmadd213ss(const Xbyak::Xbyak_aarch64::QReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2,
            const Xbyak::Operand &op) {
        assert(NULL);
    }
    void uni_vfmadd213ss(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::ZReg &x2,
            const Xbyak::Operand &op) {
        assert(NULL);
    }

    void uni_vfmadd231ps(const Xbyak::Xbyak_aarch64::QReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2,
            const Xbyak::Operand &op) {
        assert(NULL);
    }
    void uni_vfmadd231ps(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::ZReg &x2,
            const Xbyak::Operand &op) {
        assert(NULL);
    }
    void uni_vfmadd231ss(const Xbyak::Xbyak_aarch64::QReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2,
            const Xbyak::Operand &op) {
        assert(NULL);
    }
    void uni_vfmadd231ss(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::ZReg &x2,
            const Xbyak::Operand &op) {
        assert(NULL);
    }

    void uni_vfnmadd231ps(const Xbyak::Xbyak_aarch64::QReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2,
            const Xbyak::Operand &op) {

        assert(NULL);
    }

    void uni_vfnmadd231ps(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::ZReg &x2,
            const Xbyak::Operand &op) {
        assert(NULL);
    }

    void uni_vsqrtps(const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Operand &op) {
        assert(NULL);
    }
    void uni_vsqrtps(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Operand &op) {
        assert(NULL);
    }

    void uni_vpaddd(const Xbyak::Xbyak_aarch64::QReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2,
            const Xbyak::Operand &op) {
        assert(NULL);
    }
    void uni_vpaddd(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2,
            const Xbyak::Operand &op) {
        assert(NULL);
    }

    void uni_vandps() {
        assert(NULL);
    }

    void uni_vorps() {
        assert(NULL);
    }

    void uni_vpslld(
            const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Operand &op, const int imm) {
        assert(NULL);
    }
    void uni_vpslld(
            const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Operand &op, const int imm) {
        assert(NULL);
    }

    void uni_vpsrld(
            const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Operand &op, const int imm) {
        assert(NULL);
    }
    void uni_vpsrld(
            const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Operand &op, const int imm) {

        assert(NULL);
    }

    void uni_vmaxps() {

        assert(NULL);
    }

    void uni_vminps() {

        assert(NULL);
    }

    void uni_vcmpgtps(const Xbyak::Xbyak_aarch64::QReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2,
            const Xbyak::Operand &op) {


        assert(NULL);
    }

    void uni_vcmpgtps(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::ZReg &x2,
            const Xbyak::Operand &op) {


        assert(NULL);
    }

    void uni_vcmpgeps(const Xbyak::Xbyak_aarch64::QReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2,
            const Xbyak::Operand &op) {


        assert(NULL);
    }

    void uni_vcmpgeps(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::ZReg &x2,
            const Xbyak::Operand &op) {

        assert(NULL);
    }

    void uni_vtestps(const Xbyak::Xbyak_aarch64::QReg &x1, const Xbyak::Operand &op) {

        assert(NULL);
    }

    void uni_vtestps(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Operand &op) {

        assert(NULL);
    }

    void uni_vblendvps(const Xbyak::Xbyak_aarch64::QReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2,
            const Xbyak::Operand &op, const Xbyak::Xbyak_aarch64::QReg &msk) {

        assert(NULL);
    }
    void uni_vblendvps(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::ZReg &x2,
            const Xbyak::Operand &op, const Xbyak::Xbyak_aarch64::ZReg &msk) {

        assert(NULL);
    }

    void uni_vroundps(
            const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Operand &op, const int imm) {


        assert(NULL);
    }
    void uni_vroundps(
            const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Operand &op, const int imm) {

        assert(NULL);
    }

    void uni_vcvtps2dq(const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Operand &op) {

        assert(NULL);
    }
    void uni_vcvtps2dq(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Operand &op) {

        assert(NULL);
    }

    void uni_vcvtdq2ps(const Xbyak::Xbyak_aarch64::QReg &x, const Xbyak::Operand &op) {

        assert(NULL);
    }
    void uni_vcvtdq2ps(const Xbyak::Xbyak_aarch64::ZReg &x, const Xbyak::Operand &op) {

        assert(NULL);
    }

    void uni_vmovmskps(const Xbyak::Reg &x1, const Xbyak::Xbyak_aarch64::QReg &x2) {

        assert(NULL);
    }
    void uni_vmovmskps(const Xbyak::Reg &x1, const Xbyak::Xbyak_aarch64::ZReg &x2) {

        assert(NULL);
    }

    void uni_vpackssdw(const Xbyak::Xbyak_aarch64::QReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2,
            const Xbyak::Operand &op) {


        assert(NULL);
    }
    void uni_vpackssdw(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::ZReg &x2,
            const Xbyak::Operand &op) {

        assert(NULL);
    }

    void uni_vpackuswb(const Xbyak::Xbyak_aarch64::QReg &x1, const Xbyak::Xbyak_aarch64::QReg &x2,
            const Xbyak::Operand &op) {

        assert(NULL);
    }
    void uni_vpackuswb(const Xbyak::Xbyak_aarch64::ZReg &x1, const Xbyak::Xbyak_aarch64::ZReg &x2,
            const Xbyak::Operand &op) {

        assert(NULL);
    }

	void uni_ld(const Xbyak::Xbyak_aarch64::ZRegS &z1s, const Xbyak::Xbyak_aarch64::AdrScImm &addr) {
		assert(NULL);
	}


	void uni_ld(const Xbyak::Xbyak_aarch64::VReg4S &v1s4, const Xbyak::Xbyak_aarch64::AdrScImm &addr) {
		assert(NULL);
	}

	void uni_st(const Xbyak::Xbyak_aarch64::ZRegS &z1s, const Xbyak::Xbyak_aarch64::AdrScImm &addr) {
		assert(NULL);
	}

	void uni_st(const Xbyak::Xbyak_aarch64::VReg4S &v1s4, const Xbyak::Xbyak_aarch64::AdrScImm &addr) {
		assert(NULL);
	}

	void uni_vcvts2f(const Xbyak::Xbyak_aarch64::ZRegS &z1s, const Xbyak::Xbyak_aarch64::ZRegS &z2s,
            const Xbyak::Xbyak_aarch64::_PReg &p1) {
		assert(NULL);
	}

	void uni_vcvts2f(const Xbyak::Xbyak_aarch64::VReg4S &v1, const Xbyak::Xbyak_aarch64::VReg4S &v2,
            const Xbyak::Xbyak_aarch64::_PReg &p1) {
		assert(NULL);
	}

	void uni_vcvtf2s(
            const Xbyak::Xbyak_aarch64::ZRegS &z1s, const Xbyak::Xbyak_aarch64::ZRegS &z2s, const Xbyak::Xbyak_aarch64::_PReg &p1) {
	}

	void uni_vcvtf2s(const Xbyak::Xbyak_aarch64::VReg4S &v1, const Xbyak::Xbyak_aarch64::VReg4S &v2,
            const Xbyak::Xbyak_aarch64::PReg &p1) {}


            void mul_by_const(const Xbyak::Xbyak_aarch64::XReg &out, const Xbyak::Reg64 &tmp,
                    int value) {
        // Generates a shift + add sequence for multiplicating contents of the
        // out register by a known JIT-time value. Clobbers the tmp register.
        //
        // Pros compared to mul/imul:
        // - does not require using known registers
        // - not microcoded on Intel(R) Xeon Phi(TM) processors
        // Still, there are probably a lot of cases when mul/imul is faster on
        // Intel(R) Core(TM) processors. Not intended for critical path.

        // TODO: detect when overflow is emminent (Roma)
        // TODO: detect when using mul/imul is a better option (Roma)

        int p = 0; // the current power of 2
        int old_p = 0; // the last seen power of 2 such that value[old_p] != 0

//        eor(tmp, tmp, tmp);
        assert(NULL);
        while (value) {
            if (value & 1) {
                int shift = p - old_p;
                if (shift) {
                    lsl(out, out, shift); // 1 <= shift <= 63
                    old_p = p;
                }
//                add(tmp, tmp, out);
                assert(NULL);
            }
            value >>= 1;
            p++;
        }
//        mov(out, tmp);
        assert(NULL);
            }
#else // #ifdef 1
    void uni_vpxor(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op) {
        assert(x1.getIdx() == x2.getIdx());
        pxor(x2, op);
    }
    void uni_vpxor(const Xbyak::Ymm &x1, const Xbyak::Ymm &x2,
            const Xbyak::Operand &op) {
        if (mayiuse(avx2)) {
            vpxor(x1, x2, op);
        } else {
            vxorps(x1, x2, op);
        }
    }
    void uni_vpxor(const Xbyak::Zmm &x1, const Xbyak::Zmm &x2,
            const Xbyak::Operand &op) {
        vpxord(x1, x2, op);
    }

    void uni_vmovss(const Xbyak::Address &addr, const Xbyak::Xmm &x) {
        movss(addr, x);
    }
    void uni_vmovss(const Xbyak::Address &addr, const Xbyak::Ymm &x) {
        vmovss(addr, x);
    }
    void uni_vmovss(const Xbyak::Xmm &x, const Xbyak::Address &addr) {
        movss(x, addr);
    }
    void uni_vmovss(const Xbyak::Ymm &x, const Xbyak::Address &addr) {
        vmovss(x, addr);
    }

    void uni_vmovsd(const Xbyak::Address &addr, const Xbyak::Xmm &x) {
        movsd(addr, x);
    }
    void uni_vmovsd(const Xbyak::Address &addr, const Xbyak::Ymm &x) {
        vmovsd(addr, x);
    }
    void uni_vmovsd(const Xbyak::Xmm &x, const Xbyak::Address &addr) {
        movsd(x, addr);
    }
    void uni_vmovsd(const Xbyak::Ymm &x, const Xbyak::Address &addr) {
        vmovsd(x, addr);
    }

    void uni_vmovdqu(const Xbyak::Address &addr, const Xbyak::Xmm &x) {
        movdqu(addr, x);
    }
    void uni_vmovdqu(const Xbyak::Address &addr, const Xbyak::Ymm &x) {
        vmovdqu(addr, x);
    }
    void uni_vmovdqu(const Xbyak::Address &addr, const Xbyak::Zmm &x) {
        vmovdqu32(addr, x);
    }

    void uni_vmovdqu(const Xbyak::Xmm &x, const Xbyak::Address &addr) {
        movdqu(x, addr);
    }
    void uni_vmovdqu(const Xbyak::Ymm &x, const Xbyak::Address &addr) {
        vmovdqu(x, addr);
    }
    void uni_vmovdqu(const Xbyak::Zmm &x, const Xbyak::Address &addr) {
        vmovdqu32(x, addr);
    }

    void uni_vmovups(const Xbyak::Address &addr, const Xbyak::Xmm &x) {
        movups(addr, x);
    }
    void uni_vmovups(const Xbyak::Address &addr, const Xbyak::Ymm &x) {
        vmovups(addr, x);
    }

    void uni_vmovups(const Xbyak::Xmm &x, const Xbyak::Operand &op) {
        movups(x, op);
    }
    void uni_vmovups(const Xbyak::Ymm &x, const Xbyak::Operand &op) {
        vmovups(x, op);
    }

    void uni_vmovntps(const Xbyak::Address &addr, const Xbyak::Xmm &x) {
        movntps(addr, x);
    }
    void uni_vmovntps(const Xbyak::Address &addr, const Xbyak::Ymm &x) {
        vmovntps(addr, x);
    }

    void uni_vbroadcastss(const Xbyak::Xmm &x, const Xbyak::Operand &op) {
        movss(x, op);
        shufps(x, x, 0x0);
    }
    void uni_vbroadcastss(const Xbyak::Ymm &x, const Xbyak::Operand &op) {
        if (op.isMEM() || mayiuse(avx2)) {
            vbroadcastss(x, op);
        } else {
            Xbyak::Xmm t(x.getIdx());
            if (t.getIdx() != op.getIdx())
                movss(t, op);
            vinsertf128(x, x, t, 1);
            vshufps(x, x, x, 0);
        }
    }

    void uni_vpbroadcastd(const Xbyak::Xmm &x, const Xbyak::Operand &op) {
        movsd(x, op);
        pshufd(x, x, 0x0);
    }
    void uni_vpbroadcastd(const Xbyak::Ymm &x, const Xbyak::Operand &op) {
        if (mayiuse(avx2)) {
            vpbroadcastd(x, op);
        } else {
            Xbyak::Xmm t(x.getIdx());
            if (t.getIdx() != op.getIdx())
                movsd(t, op);
            vinsertf128(x, x, t, 1);
            vshufps(x, x, x, 0);
        }
    }

    void uni_vrcpss(const Xbyak::Xmm &x, const Xbyak::Operand &op) {
        rcpss(x, op);
    }
    void uni_vrcpss(const Xbyak::Ymm &x1, const Xbyak::Xmm &x2) {
        Xbyak::Xmm x1_(x1.getIdx());
        Xbyak::Xmm x2_(x2.getIdx());
        vrcpss(x1_, x1_, x2_);
    }
    void uni_vrcpss(const Xbyak::Ymm &x, const Xbyak::Address &op) {
        Xbyak::Xmm x_(x.getIdx());
        vrcpss(x_, x_, op);
    }

    void uni_vrcpps(const Xbyak::Xmm &x, const Xbyak::Operand &op) {
        rcpps(x, op);
    }
    void uni_vrcpps(const Xbyak::Ymm &x, const Xbyak::Operand &op) {
        vrcpps(x, op);
    }
    void uni_vrcpps(const Xbyak::Zmm &x, const Xbyak::Operand &op) {
        vrcp14ps(x, op);
    }

    void uni_vdivps(const Xbyak::Xmm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        assert(x.getIdx() == op1.getIdx());
        divps(x, op2);
    }
    void uni_vdivps(const Xbyak::Ymm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        vdivps(x, op1, op2);
    }

    void uni_vdivps(const Xbyak::Xmm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2, const Xbyak::Xmm &buf) {
        movups(buf, op1);
        divps(buf, op2);
        if (x.getIdx() != buf.getIdx()) {
            movups(x, buf);
        }
    }

    void uni_vdivps(const Xbyak::Ymm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2, const Xbyak::Ymm &buf) {
        vdivps(x, op1, op2);
    }

    void uni_vaddps(const Xbyak::Xmm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        assert(x.getIdx() == op1.getIdx());
        addps(x, op2);
    }
    void uni_vaddps(const Xbyak::Ymm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        vaddps(x, op1, op2);
    }
    void uni_vaddss(const Xbyak::Xmm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        assert(x.getIdx() == op1.getIdx());
        addss(x, op2);
    }
    void uni_vaddss(const Xbyak::Ymm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        vaddss(x, op1, op2);
    }

    void uni_vpsignd(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op) {
        assert(x1.getIdx() == x2.getIdx());
        psignd(x1, op);
    }
    void uni_vpsignd(const Xbyak::Ymm &x1, const Xbyak::Ymm &x2,
            const Xbyak::Operand &op) {
        vpsignd(x1, x2, op);
    }

    void uni_vsubss(const Xbyak::Xmm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        assert(x.getIdx() == op1.getIdx());
        subps(x, op2);
    }
    void uni_vsubss(const Xbyak::Ymm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        vsubss(x, Xbyak::Xmm(op1.getIdx()), Xbyak::Xmm(op2.getIdx()));
    }

    void uni_vsubps(const Xbyak::Xmm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        assert(x.getIdx() == op1.getIdx());
        subps(x, op2);
    }
    void uni_vsubps(const Xbyak::Ymm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        vsubps(x, op1, op2);
    }

    void uni_vsubps(const Xbyak::Xmm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2, const Xbyak::Xmm &buf) {
        movups(buf, op1);
        subps(buf, op2);
        if (x.getIdx() != buf.getIdx()) {
            movups(x, buf);
        }
    }

    void uni_vsubps(const Xbyak::Ymm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2, const Xbyak::Ymm &buf) {
        vsubps(x, op1, op2);
    }

    void uni_vmulps(const Xbyak::Xmm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        assert(x.getIdx() == op1.getIdx());
        mulps(x, op2);
    }
    void uni_vmulps(const Xbyak::Ymm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        vmulps(x, op1, op2);
    }

    void uni_vmulss(const Xbyak::Xmm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        assert(x.getIdx() == op1.getIdx());
        mulss(x, op2);
    }
    void uni_vmulss(const Xbyak::Ymm &x, const Xbyak::Operand &op1,
            const Xbyak::Address &op2) {
        vmulss(x, Xbyak::Xmm(op1.getIdx()), op2);
    }
    void uni_vmulss(const Xbyak::Ymm &x, const Xbyak::Operand &op1,
            const Xbyak::Ymm &op2) {
        vmulss(x, Xbyak::Xmm(op1.getIdx()), Xbyak::Xmm(op2.getIdx()));
    }

    void uni_vfmadd213ps(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op) {
        mulps(x1, x2);
        addps(x1, op);
    }
    void uni_vfmadd213ps(const Xbyak::Ymm &x1, const Xbyak::Ymm &x2,
            const Xbyak::Operand &op) {
        vfmadd213ps(x1, x2, op);
    }

    void uni_vfmadd213ss(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op) {
        mulss(x1, x2);
        addss(x1, op);
    }
    void uni_vfmadd213ss(const Xbyak::Ymm &x1, const Xbyak::Ymm &x2,
            const Xbyak::Operand &op) {
        vfmadd213ss(x1, x2, op);
    }

    void uni_vfmadd231ps(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op) {
        mulps(x2, op);
        addps(x1, x2);
    }
    void uni_vfmadd231ps(const Xbyak::Ymm &x1, const Xbyak::Ymm &x2,
            const Xbyak::Operand &op) {
        vfmadd231ps(x1, x2, op);
    }
    void uni_vfmadd231ss(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op) {
        mulss(x2, op);
        addss(x1, x2);
    }
    void uni_vfmadd231ss(const Xbyak::Ymm &x1, const Xbyak::Ymm &x2,
            const Xbyak::Operand &op) {
        vfmadd231ss(Xbyak::Xmm(x1.getIdx()), Xbyak::Xmm(x2.getIdx()), op);
    }

    void uni_vfnmadd231ps(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op) {
        mulps(x2, op);
        subps(x1, x2);
    }

    void uni_vfnmadd231ps(const Xbyak::Ymm &x1, const Xbyak::Ymm &x2,
            const Xbyak::Operand &op) {
        vfnmadd231ps(x1, x2, op);
    }

    void uni_vsqrtps(const Xbyak::Xmm &x, const Xbyak::Operand &op) {
        sqrtps(x, op);
    }
    void uni_vsqrtps(const Xbyak::Ymm &x, const Xbyak::Operand &op) {
        vsqrtps(x, op);
    }

    void uni_vpaddd(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op) {
        assert(x1.getIdx() == x2.getIdx());
        paddd(x2, op);
    }
    void uni_vpaddd(const Xbyak::Ymm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op) {
        vpaddd(x1, x2, op);
    }

    void uni_vandps(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op = Xbyak::Operand()) {
        assert(x1.getIdx() == x2.getIdx());
        andps(x1, op);
    }
    void uni_vandps(const Xbyak::Ymm &x1, const Xbyak::Ymm &x2,
            const Xbyak::Operand &op = Xbyak::Operand()) {
        if (!mayiuse(avx512_common) || x1.getBit() < 512)
            vandps(x1, x2, op);
        else
            vpandd(x1, x2, op);
    }

    void uni_vorps(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op = Xbyak::Operand()) {
        assert(x1.getIdx() == x2.getIdx());
        orps(x1, op);
    }
    void uni_vorps(const Xbyak::Ymm &x1, const Xbyak::Ymm &x2,
            const Xbyak::Operand &op = Xbyak::Operand()) {
        if (!mayiuse(avx512_common) || x1.getBit() < 512)
            vorps(x1, x2, op);
        else
            vpord(x1, x2, op);
    }

    void uni_vpslld(
            const Xbyak::Xmm &x, const Xbyak::Operand &op, const int imm) {
        assert(x.getIdx() == op.getIdx());
        pslld(x, imm);
    }
    void uni_vpslld(
            const Xbyak::Ymm &x, const Xbyak::Operand &op, const int imm) {
        vpslld(x, op, imm);
    }

    void uni_vpsrld(
            const Xbyak::Xmm &x, const Xbyak::Operand &op, const int imm) {
        assert(x.getIdx() == op.getIdx());
        psrld(x, imm);
    }
    void uni_vpsrld(
            const Xbyak::Ymm &x, const Xbyak::Operand &op, const int imm) {
        vpsrld(x, op, imm);
    }

    void uni_vmaxps(const Xbyak::Xmm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        assert(x.getIdx() == op1.getIdx());
        maxps(x, op2);
    }
    void uni_vmaxps(const Xbyak::Ymm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        vmaxps(x, op1, op2);
    }

    void uni_vminps(const Xbyak::Xmm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        assert(x.getIdx() == op1.getIdx());
        minps(x, op2);
    }
    void uni_vminps(const Xbyak::Ymm &x, const Xbyak::Operand &op1,
            const Xbyak::Operand &op2 = Xbyak::Operand()) {
        vminps(x, op1, op2);
    }

    void uni_vcmpgtps(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op) {
        assert(x1.getIdx() == x2.getIdx());
        cmpps(x1, op, _cmp_nle_us);
    }

    void uni_vcmpgtps(const Xbyak::Ymm &x1, const Xbyak::Ymm &x2,
            const Xbyak::Operand &op) {
        vcmpgtps(x1, x2, op);
    }

    void uni_vcmpgeps(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op) {
        assert(x1.getIdx() == x2.getIdx());
        cmpps(x1, op, _cmp_nlt_us);
    }

    void uni_vcmpgeps(const Xbyak::Ymm &x1, const Xbyak::Ymm &x2,
            const Xbyak::Operand &op) {
        vcmpps(x1, x2, op, _cmp_nlt_us);
    }

    void uni_vtestps(const Xbyak::Xmm &x1, const Xbyak::Operand &op) {
        ptest(x1, op);
    }

    void uni_vtestps(const Xbyak::Ymm &x1, const Xbyak::Operand &op) {
        assert(!(x1.isZMM() || op.isZMM()));
        vtestps(x1, op);
    }

    void uni_vblendvps(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op, const Xbyak::Xmm &msk) {
        assert(x1.getIdx() == x2.getIdx());
        assert(msk.getIdx() == 0);
        blendvps(x1, op);
    }
    void uni_vblendvps(const Xbyak::Ymm &x1, const Xbyak::Ymm &x2,
            const Xbyak::Operand &op, const Xbyak::Ymm &msk) {
        vblendvps(x1, x2, op, msk);
    }

    void uni_vroundps(
            const Xbyak::Xmm &x, const Xbyak::Operand &op, const int imm) {
        roundps(x, op, imm);
    }
    void uni_vroundps(
            const Xbyak::Ymm &x, const Xbyak::Operand &op, const int imm) {
        vroundps(x, op, imm);
    }
    void uni_vroundps(
            const Xbyak::Zmm &x, const Xbyak::Operand &op, const int imm) {
        vrndscaleps(x, op, imm & 0x3);
    }

    void uni_vcvtps2dq(const Xbyak::Xmm &x, const Xbyak::Operand &op) {
        cvtps2dq(x, op);
    }
    void uni_vcvtps2dq(const Xbyak::Ymm &x, const Xbyak::Operand &op) {
        vcvtps2dq(x, op);
    }

    void uni_vcvtdq2ps(const Xbyak::Xmm &x, const Xbyak::Operand &op) {
        cvtdq2ps(x, op);
    }
    void uni_vcvtdq2ps(const Xbyak::Ymm &x, const Xbyak::Operand &op) {
        vcvtdq2ps(x, op);
    }

    void uni_vmovmskps(const Xbyak::Reg &x1, const Xbyak::Xmm &x2) {
        movmskps(x1.cvt64(), x2);
    }
    void uni_vmovmskps(const Xbyak::Reg &x1, const Xbyak::Ymm &x2) {
        vmovmskps(x1, x2);
    }

    void uni_vpackssdw(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op) {
        assert(x1.getIdx() == x1.getIdx());
        packssdw(x1, op);
    }
    void uni_vpackssdw(const Xbyak::Ymm &x1, const Xbyak::Ymm &x2,
            const Xbyak::Operand &op) {
        vpackssdw(x1, x2, op);
    }

    void uni_vpackuswb(const Xbyak::Xmm &x1, const Xbyak::Xmm &x2,
            const Xbyak::Operand &op) {
        assert(x1.getIdx() == x1.getIdx());
        packuswb(x1, op);
    }
    void uni_vpackuswb(const Xbyak::Ymm &x1, const Xbyak::Ymm &x2,
            const Xbyak::Operand &op) {
        vpackuswb(x1, x2, op);
    }

    void mul_by_const(
            const Xbyak::Reg &out, const Xbyak::Reg64 &tmp, int value) {
        // Generates a shift + add sequence for multiplicating contents of the
        // out register by a known JIT-time value. Clobbers the tmp register.
        //
        // Pros compared to mul/imul:
        // - does not require using known registers
        // - not microcoded on Intel(R) Xeon Phi(TM) processors
        // Still, there are probably a lot of cases when mul/imul is faster on
        // Intel(R) Core(TM) processors. Not intended for critical path.

        // TODO: detect when overflow is emminent (Roma)
        // TODO: detect when using mul/imul is a better option (Roma)

        int p = 0; // the current power of 2
        int old_p = 0; // the last seen power of 2 such that value[old_p] != 0

        xor_(tmp, tmp);
        while (value) {
            if (value & 1) {
                int shift = p - old_p;
                if (shift) {
                    shl(out, shift);
                    old_p = p;
                }
                add(tmp, out);
            }
            value >>= 1;
            p++;
        }
        mov(out, tmp);
    }
#endif // #ifdef 1

    void dump_code(const Xbyak::XBYAK_CODE_PTR *code) const {
        if (code) {
            static int counter = 0;
#define MAX_FNAME_LEN 256
            char fname[MAX_FNAME_LEN + 1];
            snprintf(fname, MAX_FNAME_LEN, "mkldnn_dump_%s.%d.bin", name(),
                    counter);
            counter++;

            FILE *fp = mkldnn_fopen(fname, "w+");
            // Failure to dump code is not fatal
            if (fp) {
#ifdef XBYAK_TRANSLATE_AARCH64
                size_t unused = fwrite(code, getSize() * 4, 1, fp);
#else
                size_t unused = fwrite(code, getSize(), 1, fp);
#endif
                UNUSED(unused);
                fclose(fp);
            }
        }
#undef MAX_FNAME_LEN
    }

    void register_code(const Xbyak::XBYAK_CODE_PTR *code) const {
#ifdef JIT_PROFILING_VTUNE
        if (iJIT_IsProfilingActive() == iJIT_SAMPLING_ON) {
            auto jmethod = iJIT_Method_Load();
            jmethod.method_id = iJIT_GetNewMethodID();
            jmethod.method_name = (char *)name();
            jmethod.class_file_name = NULL;
            jmethod.source_file_name = (char *)source_file();
            jmethod.method_load_address = (void *)code;
            jmethod.method_size = getSize();

            iJIT_NotifyEvent(
                    iJVM_EVENT_TYPE_METHOD_LOAD_FINISHED, (void *)&jmethod);
        }
#endif
    }

public:
    jit_generator_aarch64(void *code_ptr = nullptr, size_t code_size = 256 * 1024)
        : Xbyak::Xbyak_aarch64::CodeGeneratorAArch64(code_size, code_ptr) {
#ifdef XBYAK_TRANSLATE_AARCH64
        assert(!(num_abi_save_gpr_regs % 2));
#endif
    }
    virtual ~jit_generator_aarch64() {}

    virtual const char *name() const = 0;
    virtual const char *source_file() const = 0;

    // XXX: use normal_case name and update all callees (?)

    const uint32_t *getCode32() {
        const uint32_t *code = CodeGeneratorAArch64::getCode32();
        register_code(code);

	if (mkldnn_jit_dump())
            dump_code(code);

        return code;
    }
#undef XBYAK_CODE_PTR

    template <typename F>
    const F getCode() {
        // XXX (Roma): Xbyak code probably has a bug here
        return (const F)getCode32();
    }
};

} // namespace cpu
} // namespace impl
} // namespace mkldnn

#endif
