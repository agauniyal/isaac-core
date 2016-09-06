#include "utils.hpp"
#include <atomic>
#include <mutex>
#include <string>


class Device {

	unsigned int powerPin;
	std::atomic<bool> failState;
	std::atomic<bool> exported;
	std::string id;

	// 0 => 'in' | 1 => 'out'
	std::atomic<bool> pinValue;

	static const std::string GPIO_PATH;

	// make sure to guard against n > gpio::NumPins
	static bool occupied[gpio::NumPins];
	std::mutex m_occupied;

	// 0 => 'in' | 1 => 'out'
	bool setDirection(bool);
	bool getDirection();

	std::string name;
	std::string info;
	std::mutex m_meta;

	Device(const Device &) = delete;
	Device &operator=(const Device &) = delete;

public:
	Device(const unsigned int, const std::string = "");

	bool write(bool);
	bool read();

	std::string getId() const { return id; }
	bool hasFailed() const { return failState; }
	bool pinStatus() const { return pinValue; }

	void mount();
	void unmount();

	void setName(const std::string);
	void setInfo(const std::string);
	std::string getName() const { return name; }
	std::string getInfo() const { return info; }

	virtual bool on() { return write(1); }
	virtual bool off() { return write(0); }

	virtual bool execute() { return true; }
	virtual void process() {}

	virtual ~Device();
};
