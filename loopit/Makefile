# liblsl builds for the LooPIT framework (loopit-framework:
# roadmap/rt-stalling/liblsl-pipeline.md). This branch is upstream sccn/liblsl
# at a release tag plus this directory; patches, if ever needed, are commits on
# top of the tag, each with its rationale.
#
#   make -C loopit armv7      out/armv7/   (Colibri iMX6, Toradex SDK 5.7.2)
#   make -C loopit i686       out/i686/    (host -m32 builds and tests)
#   make -C loopit x86_64     out/x86_64/  (host 64-bit builds)
#   make -C loopit all
#   make -C loopit portable   i686 + x86_64 in Ubuntu 22.04 (podman): glibc 2.35 and
#                             GCC 11's libstdc++, so the host libraries load on
#                             CI runners and colleagues' machines; the releases use these
#
# Each out/<target>/ holds liblsl.so (stripped), liblsl.so.debug, include/,
# BUILDINFO and SHA256SUMS. Release tag: <liblsl tag>+nc<n>, e.g. v1.17.7+nc1.
SHELL := /bin/bash
TOP := $(abspath ..)
OUT := $(abspath out)
BUILD := $(abspath build)
TARGETS := armv7 i686 x86_64

.PHONY: all $(TARGETS) clean portable
all: $(TARGETS)

armv7: TOOLCHAIN = toolchains/armv7-tdx-5.7.2.cmake
armv7: STRIP = /opt/tdx-xwayland-rt/5.7.2/sysroots/x86_64-tdxsdk-linux/usr/bin/arm-tdx-linux-gnueabi/arm-tdx-linux-gnueabi-strip
armv7: OBJCOPY = /opt/tdx-xwayland-rt/5.7.2/sysroots/x86_64-tdxsdk-linux/usr/bin/arm-tdx-linux-gnueabi/arm-tdx-linux-gnueabi-objcopy
i686: TOOLCHAIN = toolchains/i686-host.cmake
i686: STRIP = strip
i686: OBJCOPY = objcopy
x86_64: TOOLCHAIN = toolchains/x86_64-host.cmake
x86_64: STRIP = strip
x86_64: OBJCOPY = objcopy

$(TARGETS):
	@set -euo pipefail; \
	b=$(BUILD)/$@; o=$(OUT)/$@; rm -rf $$o; mkdir -p $$o $(BUILD); \
	cmake -S $(TOP) -B $$b -DCMAKE_TOOLCHAIN_FILE=$(abspath $(TOOLCHAIN)) \
	  -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG -g" \
	  -DLSL_UNITTESTS=OFF -DLSL_BUILD_EXAMPLES=OFF \
	  -DCMAKE_INSTALL_PREFIX=$$b/install > $$b.configure.log; \
	cmake --build $$b -j > $$b.build.log; \
	cmake --install $$b > $$b.install.log; \
	lib=$$(readlink -f $$b/install/lib/liblsl.so); \
	cp $$lib $$o/liblsl.so; \
	$(OBJCOPY) --only-keep-debug $$o/liblsl.so $$o/liblsl.so.debug; \
	$(STRIP) --strip-unneeded $$o/liblsl.so; \
	$(OBJCOPY) --add-gnu-debuglink=$$o/liblsl.so.debug $$o/liblsl.so; \
	cp -r $$b/install/include $$o/include; \
	{ echo "liblsl: $$(git -C $(TOP) describe --tags --always --dirty)"; \
	  echo "commit: $$(git -C $(TOP) rev-parse HEAD)"; \
	  echo "upstream tag: $$(git -C $(TOP) describe --tags --abbrev=0)"; \
	  echo "target: $@"; \
	  echo "toolchain file: $(TOOLCHAIN) sha256 $$(sha256sum $(TOOLCHAIN) | cut -c1-64)"; \
	  cxx=$$(sed -n 's/^set(CMAKE_CXX_COMPILER "\(.*\)")$$/\1/p' $$b/CMakeFiles/*/CMakeCXXCompiler.cmake); \
	  echo "compiler: $$cxx ($$($$cxx --version | head -n 1))"; \
	  echo "cmake: $$(cmake --version | head -n 1)"; \
	  echo "build type: Release"; \
	  echo "cxx flags: $$(sed -n 's/^CMAKE_CXX_FLAGS:[A-Z]*=//p' $$b/CMakeCache.txt) $$(sed -n 's/^CMAKE_CXX_FLAGS_RELEASE:[A-Z]*=//p' $$b/CMakeCache.txt)"; \
	  echo "built: $$(date -u +%Y-%m-%dT%H:%M:%SZ) on $$(uname -srm)"; \
	} > $$o/BUILDINFO; \
	cd $$o && sha256sum liblsl.so liblsl.so.debug BUILDINFO > SHA256SUMS && cat BUILDINFO SHA256SUMS

# CMake from pip: liblsl needs 3.28, Ubuntu 22.04 has 3.22
PORTABLE_IMAGE := docker.io/library/ubuntu:22.04
portable:
	podman run --rm -v $(TOP):/src:Z -w /src $(PORTABLE_IMAGE) bash -c '\
	  set -e; export DEBIAN_FRONTEND=noninteractive; \
	  apt-get update -qq && apt-get install -y -qq g++ g++-multilib make git python3-pip >/dev/null; \
	  pip3 install -q cmake==3.30.5; \
	  git config --global --add safe.directory /src; \
	  make -C loopit i686 x86_64'

clean:
	rm -rf $(BUILD) $(OUT)
