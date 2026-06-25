---
name: alveo-deprecated
description: >-
  Treat Alveo as a deprecated device for this project. Ignore Alveo-only xrt-smi
  commands, reports, and tests; keep work focused on Ryzen AI NPUs. Use whenever
  designing, documenting, or generating xrt-smi / xrt-smi-MCP tools, examples,
  reports, or use cases, or when the user mentions Alveo, NPU, Ryzen, thermal,
  flash/program, reset, or PCIe in an xrt-smi context.
---

# Alveo is deprecated — Ryzen AI NPU only

For this project, **Alveo is a deprecated device**. Anything in `xrt-smi` that
applies *only* to Alveo should be ignored for now. Focus all tools, reports,
examples, and use cases on **Ryzen AI NPUs** (Phoenix, Strix, NPU3). By design,
**one NPU device per host** (may change in future).

## Ignore (Alveo-only — out of scope)
- **State-changing:** `program` / flash xclbin, device `reset`.
- **Reports:** `pcie-info`, `thermal`, `mechanical`, `firewall`, `mailbox`,
  `qspi-status`, `cmc`, `vmr`, `dynamic-regions`.
- **Fleet framing:** no "across all cards on the host" — a host has one NPU.
- **BDF addressing:** there is **no BDF on Ryzen**. Do not use `-d <bdf>` and do
  not give tools a `device` argument.  One NPU per host → nothing to select; the
  CLI runs without `-d` and the server only confirms the NPU is present. Avoid
  example device strings like `0000:c4:00.1` / `c4:00.1`.

## Use instead (Ryzen NPU)
- **Power performance:** there is no `thermal` report on NPU. Get power from
  `xrt-smi examine --report platform` (use `get_power_report` backed by the
  `platform` report).
- **NPU reports:** `platform`, `aie`, `aieshim`, `aiemem`, `aie-partitions`,
  `telemetry`, `preemption`, `memory`, `host`, `error`.
- **Benchmarks/validate:** `gemm` (TOPS), `latency`, `throughput`,
  `runlist-latency/throughput`, `df-bw`, `shim-dma-bw`, `preemption-overhead`,
  `aie-reconfig-overhead`, `temporal-sharing-overhead`, `verify`, `sc-version`.

## When generating tools/examples
- Drop Alveo-only tools (e.g. `get_thermal_report`, `get_pcie_report`,
  `program`, `reset`) and Alveo-centric root-cause stories (e.g. "firewall
  tripped", "88 °C throttling").
- Prefer NPU-relevant narratives: power draw, clock throttling, AIE tile errors,
  preemption/telemetry counters, GEMM TOPS regressions.
- If a feature genuinely applies to both, keep it; only exclude Alveo-*only* items.
