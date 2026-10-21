# Strata standard benchmark: NVIDIA RTX 4000 SFF Ada Generation

Protocol `strata-qwen38-thinking-v1-draft.1` (draft), run `20261021T213129Z-d06d6921`, started 2026-10-21T21:31:29Z. **Status: complete.**

Written by `strata bench run`. The machine-readable result is [results.json](results.json); every request, its raw usage and timings and the client event offsets are in [outputs.jsonl](outputs.jsonl).

## Hardware and software

- GPU: NVIDIA RTX 4000 SFF Ada Generation (20.0 GiB dedicated)
- CPU: 13th Gen Intel(R) Core(TM) i5-13500; RAM: 62.6 GiB
- Storage of the model files: not measured
- OS: Ubuntu 26.04.1 LTS 7.0.0-38-generic; drivers: gpu-driver 595.91.07; runtime: cuda 13.2
- Strata commit: `cbfd5b926b794b356195d3cb600191b0c3567540`; engine 0.1.46.2; source build
- Background work: strata.service is the only GPU workload; 70W power cap active during inference

## Model and configuration

- Model: qwen3.8-flash-next-iq3_xxs; quantization IQ3_XXS; revision not measured; source revision not measured
- Files: `Qwen3.8-Flash-Next-GSQ-RCO-IQ3_XXS-00001-of-00002.gguf` `219ea929900dfa9e...`; `Qwen3.8-Flash-Next-GSQ-RCO-IQ3_XXS-00002-of-00002.gguf` `316b46f3a2dbd68c...`
- Track: **custom** (same flags; switches applied by patched nv_defaults.cpp (sm_89 branch) inside engine <strata>/build/strata instead of start-script exports; no export lines in start script)
- Context 262144 tokens; server capacity 1 slot(s)
- Sampling: temperature 1.0, top_p 0.95, top_k 20, min_p 0.0, presence_penalty 0.0, repetition_penalty 1.0, reasoning_effort xhigh; thinking on, preserved
- Engine options: `config.json`; chat template: `provenance/chat_template.jinja`

## Method

The frozen requests of `bench/prompts/standard-v1` (corpus `e75eff8b9feea04f...`) were sent over loopback HTTP, streaming, one at a time, in this order: 1 warm-up (excluded), 3 repetitions of the short, 4K and 16K prompts, and the twelve sanity checks. Prompt and decode speed are the engine's own `timings`; time to first token is the client's monotonic clock from dispatch to the first nonempty reasoning, answer or tool-call delta. No request was retried. Time after the model loaded: 2 min 59 s.

## Results

| Case | Prompt tok/s | Decode tok/s | TTFT s | First answer s | Total s |
| --- | --- | --- | --- | --- | --- |
| short | 137.4 (136.7-138.6) n=3 | 66.3 (63.7-68.9) n=3 | 0.59 (0.58-0.59) n=3 | 2.59 (1.90-5.72) n=3 | 6.3 (4.7-8.6) n=3 |
| prompt-4k | 1145.7 (1129.0-1163.9) n=3 | 60.8 (54.2-64.1) n=3 | 3.60 (3.55-3.66) n=3 | 8.91 (7.50-12.41) n=3 | 11.9 (11.5-13.0) n=3 |
| prompt-16k | 1325.2 (1314.3-1343.3) n=3 | 55.5 (54.9-65.4) n=3 | 12.40 (12.24-12.51) n=3 | 18.30 (16.40-20.20) n=2 | 21.4 (20.3-21.7) n=3 |

Cells are median (min-max) n=samples.

| Phase | Planned | Completed | Truncated | Failed | Skipped |
| --- | ---: | ---: | ---: | ---: | ---: |
| warmup | 1 | 0 | 1 | 0 | 0 |
| performance | 9 | 3 | 6 | 0 | 0 |
| sanity | 12 | 12 | 0 | 0 | 0 |
| concurrency | 0 | 0 | 0 | 0 | 0 |

Sanity checks: **12 of 12**.

| Case | Result | Reason |
| --- | --- | --- |
| I1 | pass | the expected JSON value |
| I2 | pass | the expected JSON value |
| I3 | pass | the expected JSON value |
| I4 | pass | the exact expected text |
| R1 | pass | the exact expected text |
| R2 | pass | the exact expected text |
| R3 | pass | the exact expected text |
| R4 | pass | the expected JSON value |
| T1 | pass | the expected tool call |
| T2 | pass | the expected tool call |
| T3 | pass | the expected tool call |
| T4 | pass | the expected tool call |

## Failures, exclusions and limitations

- No request failed.
- Excluded from speed summaries: performance-prompt-16k-r1: first_answer_s is not available for this request
- Prompt-prefix reuse is the shared template prefix only; the cache was not emptied between requests, and the expert cache stays warm and evolves. These are not cold-model measurements.
- Output caps include reasoning; a request that reaches its cap is `truncated` and fails its sanity check.
- The runner cannot see other programs on the machine, the repeat-stop and loop-recovery switches of the server, or the quantization repository's revision; see `missing` in results.json for every value it could not read.
- The server had answered 4 request(s) before this run.
