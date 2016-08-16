#include "utils/timer.h"

namespace f3d {
	namespace utils {
		Timer::Timer() : _running(false) {
			_start_clock = std::chrono::steady_clock::now();
			_stop_clock = std::chrono::steady_clock::now();
			_duration = std::chrono::duration<uint64_t>(0);
		}

		Timer::~Timer() {
		}

		void		Timer::start() {
			_running = true;
			_start_clock = std::chrono::steady_clock::now();
			_stop_clock = std::chrono::steady_clock::now();
		}

		void		Timer::stop() {
			_running = false;
			_stop_clock = std::chrono::steady_clock::now();
			_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(_stop_clock - _start_clock);
		}

		void		Timer::restart() {
			_running = true;
			_start_clock = std::chrono::steady_clock::now();
			_stop_clock = std::chrono::steady_clock::now();
			_duration = std::chrono::duration<uint64_t>(0);
		}

		uint64_t	Timer::ticks() const {
			std::chrono::nanoseconds	tmp(_duration);
			if (_running == true)
				tmp += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - _start_clock);
			return tmp.count();
		}

		uint64_t						Timer::seconds() const {
			std::chrono::nanoseconds	tmp(_duration);
			if (_running == true)
				tmp += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - _start_clock);

			return (std::chrono::duration_cast<std::chrono::seconds>(tmp)).count();
		}

		uint64_t						Timer::millis() const {
			std::chrono::nanoseconds	tmp(_duration);
			if (_running == true)
				tmp += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - _start_clock);

			return (std::chrono::duration_cast<std::chrono::milliseconds>(tmp)).count() % (uint64_t)1e3;
		}

		uint64_t						Timer::micros() const {
			std::chrono::nanoseconds	tmp(_duration);

			if (_running == true)
				tmp += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - _start_clock);

			return (std::chrono::duration_cast<std::chrono::microseconds>(tmp)).count() % (uint64_t)1e3;
		}

		uint64_t						Timer::nanos() const {
			std::chrono::nanoseconds	tmp(_duration);

			if (_running == true)
				tmp += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - _start_clock);

			return  (std::chrono::duration_cast<std::chrono::nanoseconds>(tmp)).count() % (uint64_t)1e3;
		}

		std::ostream&	operator<<(std::ostream& os, const Timer& t) {
			os  << t.seconds() << "s - " 
				<< t.millis() << "ms - "
				<< t.micros() << "us - "
				<< t.nanos() << "ns";
			return os;
		}
	}
}