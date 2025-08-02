#pragma once
#include "datetime.h"

#include <chrono>
#include <unordered_set>

namespace Quokka::Time {
class Calendar {
  public:
	using Holidays = std::unordered_set<std::chrono::local_days>;
	using Weekends = std::unordered_set<std::chrono::weekday>;

  private:
	std::shared_ptr<const std::chrono::time_zone> tz_{
		std::chrono::current_zone()};
	Holidays holidays_;
	Weekends weekends_;

  public:
	Calendar(std::string_view tz, Holidays holidays)
		: tz_{std::chrono::locate_zone(tz)}, holidays_{std::move(holidays)} {};

	[[nodiscard]] bool is_holiday(const DateTime &dt) const {
		auto local_day =
			std::chrono::floor<std::chrono::days>(tz_->to_local(dt.utc_time()));
		return holidays_.contains(local_day);
	}

	[[nodiscard]] bool is_weekend(const DateTime &dt) const {
		auto wd = std::chrono::weekday(dt.year_month_day());
		return weekends_.contains(wd);
	}

	[[nodiscard]] bool is_business_day(const DateTime &dt) const {
		return !is_weekend(dt) && !is_holiday(dt);
	}

	[[nodiscard]] std::string_view time_zone() const {
		return tz_->name();
	}

	Calendar(const Calendar &) = delete;
	Calendar &operator=(const Calendar &) = delete;
};
} // namespace Quokka::Time
