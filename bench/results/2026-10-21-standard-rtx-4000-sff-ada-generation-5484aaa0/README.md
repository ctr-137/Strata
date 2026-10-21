# Strata standard benchmark: NVIDIA RTX 4000 SFF Ada Generation

Protocol `strata-qwen38-thinking-v1-draft.1` (draft), run `20261021T185126Z-5484aaa0`, started 2026-10-21T18:51:26Z. **Status: complete.**

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
- Track: **custom** (same serve config plus sm_86 perf stack exported in start script (ADAPT_NOWAIT, ADAPT_LAG, CPU_IQ_ALL_EXACT1, SIDE_MIXER, ATTN_DAILY, GR_UP75, GU_SWIGLU, QSA_ROWS, B6_MMVQ_ROWS))
- Context 262144 tokens; server capacity 1 slot(s)
- Sampling: temperature 1.0, top_p 0.95, top_k 20, min_p 0.0, presence_penalty 0.0, repetition_penalty 1.0, reasoning_effort xhigh; thinking on, preserved
- Engine options: `config.json`; chat template: `provenance/chat_template.jinja`

## Method

The frozen requests of `bench/prompts/standard-v1` (corpus `e75eff8b9feea04f...`) were sent over loopback HTTP, streaming, one at a time, in this order: 1 warm-up (excluded), 3 repetitions of the short, 4K and 16K prompts, and the twelve sanity checks. Prompt and decode speed are the engine's own `timings`; time to first token is the client's monotonic clock from dispatch to the first nonempty reasoning, answer or tool-call delta. No request was retried. Time after the model loaded: 4 min 35 s.

## Results

| Case | Prompt tok/s | Decode tok/s | TTFT s | First answer s | Total s |
| --- | --- | --- | --- | --- | --- |
| short | 140.8 (137.5-147.8) n=3 | 63.7 (63.1-68.5) n=3 | 0.58 (0.55-0.59) n=3 | 2.26 (2.10-5.68) n=3 | 5.7 (5.5-8.6) n=3 |
| prompt-4k | 1157.0 (1147.4-1183.0) n=3 | 61.2 (55.5-66.8) n=3 | 3.57 (3.49-3.60) n=3 | 8.31 (6.64-9.97) n=2 | 11.9 (11.1-12.8) n=3 |
| prompt-16k | 1341.0 (1327.5-1379.3) n=3 | 53.9 (52.1-58.9) n=3 | 12.26 (11.92-12.38) n=3 | not measured | 21.7 (20.9-21.8) n=3 |

Cells are median (min-max) n=samples.

| Phase | Planned | Completed | Truncated | Failed | Skipped |
| --- | ---: | ---: | ---: | ---: | ---: |
| warmup | 1 | 0 | 1 | 0 | 0 |
| performance | 9 | 2 | 7 | 0 | 0 |
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
- Excluded from speed summaries: performance-prompt-16k-r2: first_answer_s is not available for this request
- Excluded from speed summaries: performance-prompt-16k-r3: first_answer_s is not available for this request
- Excluded from speed summaries: performance-prompt-4k-r3: first_answer_s is not available for this request
- Prompt-prefix reuse is the shared template prefix only; the cache was not emptied between requests, and the expert cache stays warm and evolves. These are not cold-model measurements.
- Output caps include reasoning; a request that reaches its cap is `truncated` and fails its sanity check.
- The runner cannot see other programs on the machine, the repeat-stop and loop-recovery switches of the server, or the quantization repository's revision; see `missing` in results.json for every value it could not read.
- The server had answered 3 request(s) before this run.
