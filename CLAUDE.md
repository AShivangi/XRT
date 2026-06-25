# CLAUDE.md — XRT Project Knowledge Base

> Living context for AI agents. Auto-loaded into every conversation.
> Last updated: 2026-06-25

## What is XRT
- XRT (Xilinx/AMD Runtime) is the open-source runtime stack that lets host software
  load, run, and manage workloads on AMD adaptive/accelerator devices.
- Target hardware: Alveo datacenter accelerator cards and Ryzen AI NPUs (Phoenix,
  Strix, NPU3, etc.); plus embedded/edge (Kria) via JSON-injected subcommands.
- Repo root layout of interest: most tooling lives under `src/runtime_src/core/`.

## Who uses it
- Customers/public, FAEs (field application engineers), driver/firmware/silicon
  developers, and validation/QA/CI — each surfaced different command tiers via
  public vs hidden vs `--advanced` gating.

## Architecture & components
- `src/runtime_src/core/tools/` — command-line tooling.
- `src/runtime_src/core/common/smi/` — per-device SMI config (which subcommands,
  reports, tests are visible per device class/SKU).

## Build, test, run
- Build is CMake-based. `xrt-smi` executable name is set in
  `src/runtime_src/core/tools/xbutil2/CMakeLists.txt` (`XBUTIL2_NAME = "xrt-smi"`).
- Build feature flags: `ENABLE_NATIVE_SUBCMDS_AND_REPORTS` (enables native
  `validate` + Alveo-only reports), `ENABLE_DEFAULT_ONE_DEVICE_OPTION` (edge).
- On-device validation/benchmarks run via `xrt-smi validate` (see tests below).
- Build script: `build/build.sh` (run from `build/`).
  - DEFAULT for agents: `./build.sh -dbg -nocmake -noctest` — debug-only, skips
    the CMake re-config step (`-nocmake`) and the unit-test run (`-noctest`);
    fastest for iterating on source changes (ctest alone adds ~6 min).
  - FULL build (only when the user explicitly says "full build"):
    `./build.sh -clean` followed by `./build.sh`.
  - Build runs outside the sandbox (symlinked repo path + toolchain); use
    elevated permissions.

## xrt-smi (CLI tool) — detailed
`xrt-smi` is the user-facing name of the internal `xbutil2` tool. It is a
device-tailored CLI: at startup it loads an SMI config from the device shim and
filters which subcommands/reports/tests are visible per device class (Alveo vs
Ryzen/NPU) and SKU.

- Entry / registration: `src/runtime_src/core/tools/xbutil2/xbutil.cpp`
- Main loop / global flags: `src/runtime_src/core/tools/common/XBMain.cpp`
- Wrapper script: `src/runtime_src/core/tools/xbutil2/xrt-smi`

### Subcommands
- `examine` (`SubCmdExamine.cpp`) — read-only reports (text or JSON).
- `program` (`SubCmdProgram.cpp`) — load xclbin.
- `reset` (`SubCmdReset.cpp`) — user reset.
- `configure` (`SubCmdConfigure.cpp`) — device/host config (pmode, host-mem, p2p, +hidden dev toggles).
- `validate` (`SubCmdValidate.cpp`) — run on-device test/benchmark suite (native builds).
- `advanced` (`SubCmdAdvanced.cpp`) — hidden; low-level mem/register/AIE ops.
- JSON-injected subcommands via `XRT_SUBCOMMANDS_JSON` (`common/SubCmdJSON.cpp`, e.g. Kria).

### Reports (`src/runtime_src/core/tools/common/reports/`)
- Sensors: `thermal`, `electrical` (power), `mechanical`.
- Inventory/state: `memory`, `pcie-info`, `clocks`, `platform`, `host`, `dynamic-regions`, `debug-ip-status`.
- Health/errors: `error`, `firewall`, `mailbox`, `qspi-status` (+ `cmc`, `vmr`).
- NPU/AIE: `aie`, `aieshim`, `aiemem`, `aie-partitions`, `telemetry`, `preemption`.
- Hidden/dev: `bo-stats`, `ps-kernels`.

### Validate tests (`src/runtime_src/core/tools/common/tests/`)
- Bandwidth: `mem-bw`, `df-bw`, `shim-dma-bw`, `dma`, `hostmem-bw`.
- Latency/throughput: `latency`, `throughput`, `runlist-latency`, `runlist-throughput`.
- Compute: `gemm` (INT8 TOPS; branches Strix vs NPU3).
- Overheads: `preemption-overhead`, `aie-reconfig-overhead`, `temporal-sharing-overhead`.
- Functional/health: `verify`, `sc-version`, `pcie-link`, `aux-connection`, `sanity`.

### Output formats
- Human-readable text to stdout (each report's `writeReport()`).
- JSON via `--format JSON` (schema `json_20202`) to a `--output` file; hidden
  `JSON-internal` schema.
- NOTE: `--element` is accepted on the command line but is NOT implemented
  (no-op); do not rely on it for JSON node filtering.
- `examine --watch` (Ryzen) = text-only refreshing view.
- Output plumbing: `src/runtime_src/core/tools/common/XBHelpMenus.cpp` (`produce_reports`).

### Visibility tiering
- Global flags: `--show-hidden` (show hidden subcommands), `--advanced` (unlock
  hidden reports/tests/options marked `"hidden"` in SMI JSON config).
- Per-device gating: `core/common/smi/smi_alveo.cpp`, `smi_ryzen.cpp`, `smi.h`;
  fallback when no device: `tools/common/SmiDefault.cpp`.

### Known limitations (today)
- Pull-based, point-in-time snapshots; no built-in history/trends/alerting.
- Single-device per invocation; fleet aggregation needs external orchestration.
- Consumers must parse tool-specific JSON; no first-class library/daemon API.
- Available commands vary by device/SKU; portable automation must handle gaps.

## Conventions & gotchas
- Git: NEVER stage, commit, or update git submodules (e.g. `aiebu`) unless the
  user explicitly asks. Exclude submodule changes from commits by default.
- xrt-smi == xbutil2 internally (name set in CMake, not a separate codebase).
- Device-tailoring happens at runtime via SMI config; a command "missing" on one
  device may simply be filtered out for that device class/SKU.

## Glossary
- SMI — System Management Interface (the xrt-smi tooling layer).
- NPU — Neural Processing Unit (Ryzen AI); SKUs: Phoenix, Strix, NPU3.
- Alveo — AMD datacenter accelerator cards (U200/U250/U280, etc.).
- AIE — AI Engine array on adaptive devices.

## Open questions
- (none recorded yet)
