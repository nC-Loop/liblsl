# liblsl for LooPIT

This branch (`loopit`) is upstream [sccn/liblsl](https://github.com/sccn/liblsl)
at a release tag, plus the patches listed below and this directory with the
build for the LooPIT framework. Decision and measurements: loopit-framework
`roadmap/rt-stalling/liblsl-pipeline.md` (liblsl 1.17.7, upstream, 2026-10-06;
patched since `v1.17.7+nc2`).

```bash
make -C loopit armv7   # needs the Toradex SDK 5.7.2 under /opt/tdx-xwayland-rt/5.7.2
make -C loopit i686    # needs a multilib host compiler (-m32)
make -C loopit x86_64  # host 64-bit
make -C loopit portable  # i686 + x86_64 in Ubuntu 22.04 (podman): the release's host builds
```

`out/<target>/`: `liblsl.so` (stripped, with a debuglink), `liblsl.so.debug`,
`include/`, `BUILDINFO` (tag, commit, toolchain file and its hash, compiler,
flags) and `SHA256SUMS`. Releases are tagged `<liblsl tag>+nc<n>`
(`v1.17.7+nc1`), with the `out/` contents as assets.

Patches are commits on top of the tag with their rationale in the message;
the release tag's number then increases.

## Patches

Since `v1.17.7+nc2` (2026-10-14): the fix of upstream issue
[#312](https://github.com/sccn/liblsl/issues/312), which is on upstream `dev`
but in no release. Every destroyed outlet kept its data `io_context` (3 file
descriptors) and its sample buffers, and with a connected inlet also a
thread. A process that recreates outlets, as lsl-emitter does on every script
deploy, ran out of descriptors after about 160 recreations and crashed.

| Commit | Upstream | Note |
|---|---|---|
| Drain stranded IO handlers when destroying an outlet | a82ad8bc (#313) | as upstream |
| Keep the sample factory alive for client sessions | 744716ed (#313) | as upstream |
| Wait for transfer threads before stopping an outlet's IO | 59f32296 | backported by hand; the message lists the differences |
| Add the #312 outlet teardown tests | from a82ad8bc, 59f32296 | adapted to 1.17 |

The same commits on the bare tag are branch `backport/1.17-outlet-teardown`,
offered upstream for a 1.17 maintenance release.
