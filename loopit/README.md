# liblsl for LooPIT

This branch (`loopit`) is upstream [sccn/liblsl](https://github.com/sccn/liblsl)
at a release tag, unpatched, plus this directory with the build for the LooPIT
framework. Decision and measurements: loopit-framework
`roadmap/rt-stalling/liblsl-pipeline.md` (liblsl 1.17.7, upstream, unpatched,
2026-10-06).

```bash
make -C loopit armv7   # needs the Toradex SDK 5.7.2 under /opt/tdx-xwayland-rt/5.7.2
make -C loopit i686    # needs a multilib host compiler (-m32)
```

`out/<target>/`: `liblsl.so` (stripped, with a debuglink), `liblsl.so.debug`,
`include/`, `BUILDINFO` (tag, commit, toolchain file and its hash, compiler,
flags) and `SHA256SUMS`. Releases are tagged `<liblsl tag>+nc<n>`
(`v1.17.7+nc1`), with the `out/` contents as assets.

Patches, if one ever becomes necessary, are commits on top of the tag with
their rationale in the message; the release tag's number then increases.
