#pragma once

#include <chrono>
#include <format>
#include <string>

namespace Quokka::Time {
class DateTime {
  public:
	using TimePoint = std::chrono::time_point<std::chrono::system_clock,
											  std::chrono::seconds>;

  private:
	std::shared_ptr<const std::chrono::time_zone> tz_{
		std::chrono::current_zone()}; // 一旦あえて別に持つか。
	TimePoint tp_;

  public:
	DateTime() = default;
	DateTime(std::string_view tz,
			 std::chrono::local_time<std::chrono::seconds> local)
		: tz_{std::chrono::locate_zone(tz)}, tp_{tz_->to_sys(local)} {};
	DateTime(std::string_view tz, TimePoint tp)
		: tz_{std::chrono::locate_zone(tz)}, tp_{tp} {};

	[[nodiscard]] std::string to_iso_string() const {
		return std::format("{:%Y-%m-%dT%H:%M:%S %Z}",
						   std::chrono::zoned_time{tz_, tp_});
	}

	[[nodiscard]] std::chrono::local_time<std::chrono::seconds>
	local_time() const {
		return tz_->to_local(tp_);
	}

	[[nodiscard]] std::chrono::year_month_day year_month_day() const {
		return std::chrono::year_month_day(
			std::chrono::floor<std::chrono::days>(local_time()));
	}

	[[nodiscard]] TimePoint utc_time() const { return tp_; }

	[[nodiscard]] std::string_view time_zone() const { return tz_->name(); }

	friend auto operator<=>(const DateTime &l, const DateTime &r) {
		if (auto cmp = l.tp_ <=> r.tp_; cmp != 0)
			return cmp;
		return l.tz_->name() <=> r.tz_->name(); // optional
	}
};
} // namespace Quokka::Time
