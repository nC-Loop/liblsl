#include <catch2/catch_all.hpp>
#include <lsl_cpp.h>
#include <memory>

#ifndef _WIN32
#include <fcntl.h>
#endif

// clazy:excludeall=non-pod-global-static

namespace {

#ifndef _WIN32
int open_fds() {
	int n = 0;
	for (int fd = 0; fd < 4096; ++fd) n += fcntl(fd, F_GETFD) != -1;
	return n;
}

TEST_CASE("destroying an outlet releases its io_context (#312)", "[outlet][basic]") {
	lsl::stream_info info("teardown_leak", "EEG", 1, 100., lsl::cf_float32, "teardown_leak");
	const auto cycle = [&info]() { lsl::stream_outlet outlet(info); };
	cycle(); // warm up lazily created singletons
	const int before = open_fds();
	for (int i = 0; i < 10; ++i) cycle();
	// each leaked data io_context keeps ~3 fds (reactor + interrupter) open
	CHECK(open_fds() < before + 10);
}

TEST_CASE("destroying an outlet with a connected inlet releases its io_context (#312)",
	"[outlet][basic]") {
	lsl::stream_info info(
		"teardown_leak_connected", "EEG", 1, 100., lsl::cf_float32, "teardown_leak_connected");
	const auto cycle = [&info]() {
		auto outlet = std::make_unique<lsl::stream_outlet>(info);
		auto found = lsl::resolve_stream("source_id", info.source_id(), 1, 2.0);
		REQUIRE(found.size() == 1);
		lsl::stream_inlet inlet(found[0]);
		inlet.open_stream(2);
		// 1.17 registers the consumer after the handshake (b3cb1e08 on dev changed that), so a
		// sample pushed before the registration would never reach the inlet
		REQUIRE(outlet->wait_for_consumers(2.0));
		float sample = 1.f;
		outlet->push_sample(&sample);
		REQUIRE(inlet.pull_sample(&sample, 1, 2.0) != 0.0);
		// destroy the outlet while its transfer thread is still serving the inlet
		outlet.reset();
	};
	cycle(); // warm up lazily created singletons
	const int before = open_fds();
	for (int i = 0; i < 10; ++i) cycle();
	CHECK(open_fds() < before + 10);
}
#endif

} // namespace
