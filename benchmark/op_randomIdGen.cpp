#include <random>
#include <string>
#include <vector>


void genAndFill(std::vector<std::string> &v)
{
	std::string ss;
	ss.resize(8, ' ');

	constexpr char alphanum[] = "0123456789"
	                            "abcdefghijklmnopqrstuvwxyz"
	                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	static const auto seed = std::random_device()();
	static std::mt19937 gen(seed);
	std::uniform_int_distribution<> dis(0, 61);

	for (int i = 0; i < 8; ++i) {
		ss[i] = alphanum[dis(gen)];
	}
	v.push_back(ss);
}

int main()
{
	std::vector<std::string> v;
	v.reserve(500000);

	for (auto l = 0; l < 500000; ++l) {
		genAndFill(v);
	}
}
