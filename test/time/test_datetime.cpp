#include "time/datetime.h"
#include <gtest/gtest.h>

using namespace Quokka::Time;
using namespace std::chrono;

TEST(DateTimeTest, CompareByUtcEqual) {
	const DateTime a("UTC", sys_seconds{seconds{123456}});
	const DateTime b("Asia/Tokyo", sys_seconds{seconds{123456}});

	EXPECT_EQ(DateTime::compare_by_utc(a, b), std::strong_ordering::equal);
}

TEST(DateTimeTest, CompareByUtcLess) {
	const DateTime earlier("UTC", sys_seconds{seconds{1000}});
	const DateTime later("UTC", sys_seconds{seconds{2000}});

	EXPECT_EQ(DateTime::compare_by_utc(earlier, later),
			  std::strong_ordering::less);
}

TEST(DateTimeTest, OperatorThreeWayThrowsOnDifferentTimezone) {
	const DateTime a("UTC", sys_seconds{seconds{1000}});
	const DateTime b("Asia/Tokyo", sys_seconds{seconds{1000}});

	EXPECT_THROW({ auto _ = a <=> b; }, std::logic_error);
}

TEST(DateTimeTest, LocalTimeConversionIsCorrect) {
	const DateTime tokyo("Asia/Tokyo", sys_seconds{seconds{0}});
	const auto local = tokyo.local_time();

	// 1970-01-01T00:00:00 UTC + 9h = 09:00:00 JST
	auto t = floor<hours>(local.time_since_epoch());
	EXPECT_EQ(t.count(), 9);
}

TEST(DateTimeTest, ToIsoStringProducesExpectedFormat) {
	const DateTime dt("UTC", sys_seconds{seconds{0}});
	const std::string iso = dt.to_iso_string();

	EXPECT_EQ(iso, "1970-01-01T00:00:00 UTC");
}

TEST(DateTimeTest, TimeZoneNameIsCorrect) {
	const DateTime dt("Asia/Tokyo", sys_seconds{seconds{0}});
	EXPECT_EQ(dt.time_zone(), "Asia/Tokyo");
}
