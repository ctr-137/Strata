# sm_89 IL-table measurement: RTX 4000 SFF Ada 20 GB (2026-10-21)

Three runs of `mmvq_il_parity --bench --emit-table` (`docs/MMVQ_IL_TABLE.md`) on one RTX 4000 SFF Ada Generation
(20 GB, sm_89, 70 W power cap, PCIe gen4 x16, driver 595.91.07), an Ubuntu host with an i5-13500. The tool was built
with `-DSTRATA_BUILD_TESTS=ON` from the v0.2.1.3 checkout (`cbfd5b92`), the same tree the engine binary in use was
built from; the GPU was dedicated to each run (nothing else held the card). Raw output: [run1.txt](run1.txt),
[run2.txt](run2.txt), [run3.txt](run3.txt); every run ends `mmvq_il_parity: OK (every case, T 1-8, rows 1/2/4 and
the table, bitwise)`.

## What the three runs agree on

The emitted tables differ from run to run; only these cells of the 2-4 column table were unanimous across all three
runs, and all four differ from `kIlRowsShared`, which is what an unmeasured sm_89 card currently gets:

| Type | Columns | `kIlRowsShared` | unanimous on this card |
| --- | --- | --- | --- |
| Q3_K | 3 | no Q3_K entry (plain kernel) | `1, 1, 1, 1, 1` |
| Q3_K | 4 | no Q3_K entry (plain kernel) | `1, 1, 1, 1, 1` |
| Q6_K | 2 | `0, 0, 1, 1, 0` | `0, 0, 1, 1, 1` |
| IQ4_XS | 3 | `0, 0, 2, 4, 4` | `0, 1, 1, 1, 1` |

Every other 2-4 column cell wobbled between runs (e.g. Q5_K T4 measured `1, 0, 2, 2, 2` / `0, 2, 2, 2, 2` /
`1, 2, 2, 2, 2`): borderline against the tool's 3% rule, so no claim there. The one-column table is stable for
IQ4_XS/Q4_K/Q5_K/Q6_K (all zero, as in the shared table); its Q3_K row disagrees once across the three runs
(`0, 1, 1, 1, 1` twice, `1, 1, 1, 1, 1` once). The 5-8 column table disagreed the most and no cell is claimed.

## End-to-end A/B: applying the four cells changed nothing measurable

The four unanimous cells only, set through `STRATA_MMVQ_IL_ROWS` (`11:3:1,1,1,1,1;11:4:1,1,1,1,1;14:2:0,0,1,1,1;
23:3:0,1,1,1,1`), were A/B-ed with the standard benchmark protocol on the same machine, build and day, fresh engine
per run (the sibling `2026-10-21-standard-rtx-4000-sff-ada-generation-*` folders: 5484aaa0 and 85932550 without the
override, 0ec6c4e4 with it): decode medians −0.7% (short), −2.0% (4K), +3.8% (16K) against the mean of the two
without-override runs - inside the run-to-run spread. The override was not kept on this machine.

## Against the RTX 4070 table (PR #3453)

The RTX 4070 table proposed in PR #3453 corroborates the new Q3_K rows on this second sm_89 card: Q3_K at 3 columns
is identical (`1, 1, 1, 1, 1`), at 4 columns nearly (`2, 1, 1, 1, 1` vs `1, 1, 1, 1, 1`), and IQ4_XS at 3 columns
moves in the same direction (rows of 1, not the shared 2/4; they differ only in the below-2048 class). It differs
for Q6_K at 2 columns, where the 4070 turned the whole group off and this card measured 1 row for three classes.
Independently, both cards report the same end-to-end outcome: the tables are bitwise safe but speed-neutral here,
where the decode is gated by the 70 W cap and the expert path, not by the dense GEMVs.
