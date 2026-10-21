// src/core/nv_defaults.cpp - see include/strata/core/nv_defaults.hpp.
#include "strata/core/nv_defaults.hpp"

#include "strata/core/emulate.hpp"

#include <cstdio>
#include <cstdlib>

namespace strata::core {

namespace {
bool off(const char* name) {
    const char* v = std::getenv(name);
    return v != nullptr && v[0] == '0';
}
}  // namespace

std::vector<std::pair<std::string, std::string>> nv_default_env(int major, int minor) {
    std::vector<std::pair<std::string, std::string>> t;
    const int mj = strata::cc_major_of(major), mn = strata::cc_minor_of(minor);
    if (mj == 8 && mn == 6) {
        if (off("STRATA_SM86_PERF")) return t;
        t = {
            // 148-nv, RTX 3060 + RTX A4000 (IQ3_XXS, default flags, 8 pairs): the stack +9.6% / +14.6% (8/8 each), identity gate IDENTICAL.
            // Each is in because removing it from the full stack cost speed on a card (drop-one, 4 rounds) and saved none on the other.
            {"STRATA_ADAPT_NOWAIT", "1"},        // A4000 -6.3 points when removed (with ADAPT_LAG), 3060 -0.8
            {"STRATA_ADAPT_LAG", "2"},
            {"STRATA_CPU_IQ_ALL_EXACT1", "1"},   // A4000 -3.3, 3060 -0.6
            {"STRATA_SIDE_MIXER", "2"},          // A4000 -1.2, 3060 -2.3
            {"STRATA_ATTN_DAILY", "1"},          // -1.2, -1.3
            {"STRATA_GR_UP75", "1"},             // -0.9, -1.8
            {"STRATA_GU_SWIGLU", "1"},           // -0.8, -1.4
            {"STRATA_QSA_ROWS", "1"},            // -0.5, -1.2
            {"STRATA_B6_MMVQ_ROWS", "1"},        // A4000 0, 3060 -1.7
        };
    } else if (mj == 8 && mn == 9) {
        if (off("STRATA_SM89_PERF")) return t;
        t = {
            // 2026-10-21, RTX 4000 SFF Ada 20 GB (sm_89, 70 W power cap, i5-13500 host), IQ3_XXS GSQ-RCO, standard-v1
            // (docs/BENCHMARK_PROTOCOL.md, draft.1), fresh engine per run, 3 repeats per case, medians:
            // decode 52.76 -> 60.81 at 4K (+15%), 48.80 -> 55.46 at 16K (+14%), 61.95 -> 66.33 short (+7%);
            // prompt rates unchanged; 12/12 sanity each run; the answers are bitwise, per the switches' identity gate.
            // The nine are the sm_86 ones unchanged: the set as a whole was measured here, not switch by switch.
            // No other sm_89 measurement of the set exists yet.
            {"STRATA_ADAPT_NOWAIT", "1"},
            {"STRATA_ADAPT_LAG", "2"},
            {"STRATA_CPU_IQ_ALL_EXACT1", "1"},
            {"STRATA_SIDE_MIXER", "2"},
            {"STRATA_ATTN_DAILY", "1"},
            {"STRATA_GR_UP75", "1"},
            {"STRATA_GU_SWIGLU", "1"},
            {"STRATA_QSA_ROWS", "1"},
            {"STRATA_B6_MMVQ_ROWS", "1"},
        };
    } else if (mj == 6 && mn == 0) {
        if (off("STRATA_SM60_PERF")) return t;
        t = {
            // 148-nv, Tesla P100 (IQ3_XXS, default flags, 8 pairs): the stack +17.8% (8/8), identity gate IDENTICAL.
            {"STRATA_ADAPT_NOWAIT", "1"},        // -10 points when removed (with ADAPT_LAG)
            {"STRATA_ADAPT_LAG", "2"},
            {"STRATA_SIDE_MIXER", "2"},          // -1.8
            {"STRATA_FUSE_WAIT", "2"},           // -1.6
            {"STRATA_B6_MMVQ_ROWS", "1"},        // -1.3
            {"STRATA_FUSE_DECODE", "1"},         // -1.3
            {"STRATA_GR_UP75", "1"},             // -1.2
        };
    }
    return t;
}

std::vector<std::string> apply_nv_defaults(int major, int minor) {
    std::vector<std::string> set;
    for (const auto& kv : nv_default_env(major, minor)) {
        if (std::getenv(kv.first.c_str()) != nullptr) continue;   // the user's setting wins, whatever it is
#if defined(_WIN32)
        _putenv_s(kv.first.c_str(), kv.second.c_str());
#else
        setenv(kv.first.c_str(), kv.second.c_str(), 0);
#endif
        set.push_back(kv.first);
    }
    if (!set.empty()) {
        const int mj = strata::cc_major_of(major), mn = strata::cc_minor_of(minor);
        const char* what = mj == 6 ? "sm_60 (Pascal)" : mn == 9 ? "sm_89 (Ada)" : "sm_86 (Ampere)";
        const char* gate = mj == 6 ? "STRATA_SM60_PERF" : mn == 9 ? "STRATA_SM89_PERF" : "STRATA_SM86_PERF";
        std::fprintf(stderr, "strata: %s: %zu exact speed switches on by default (%s=0 turns them off; a switch you set is kept): ",
                     what, set.size(), gate);
        for (size_t i = 0; i < set.size(); ++i) std::fprintf(stderr, "%s%s", i ? " " : "", set[i].c_str() + 7);
        std::fprintf(stderr, "\n");
    }
    return set;
}

}  // namespace strata::core
