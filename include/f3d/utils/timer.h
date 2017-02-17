#pragma once

#ifndef _F3D_TIMER_H
#define _F3D_TIMER_H

#include <chrono>
#include <memory>
#include <iostream>
#include <ostream>

#include "core/plateform.h"

namespace f3d {
	namespace utils {
		class F3D_API Timer {
		public:
			Timer();
			~Timer();

			void		start();
			void		stop();
			void		restart();

			uint64_t	ticks() const;
			uint64_t	seconds() const;
			uint64_t	millis() const;
			uint64_t	micros() const;
			uint64_t	nanos() const;

		private:
			bool									_running;
			std::chrono::steady_clock::time_point	_start_clock;
			std::chrono::steady_clock::time_point	_stop_clock;
			std::chrono::nanoseconds				_duration;
		};
		std::ostream&	operator<<(std::ostream& os, const Timer& t);
	}
}

#endif