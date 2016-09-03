#include "utils.hpp"
#include <mutex>
#include <string>

class Device {

	unsigned int powerPin;
	bool failState;
	bool exported;

	// 0 => 'in' | 1 => 'out'
	bool pinValue;
	std::mutex m_value;

	static const std::string GPIO_PATH;
	static bool occupied[gpio::NumPins];
	std::mutex m_occupied;

	std::string details;

	// 0 => 'in' | 1 => 'out'
	bool setDirection(bool);
	bool getDirection();

	Device(const Device &) = delete;
	Device &operator=(const Device &) = delete;

public:
	Device(const unsigned int, const std::string = "");

	bool write(bool);
	bool read();

	void mount();
	void unmount();
	bool hasFailed() { return failState; }
	bool getStatus() { return pinValue; }

	std::string getDetails();
	void setDetails(const std::string);

	~Device();
};
