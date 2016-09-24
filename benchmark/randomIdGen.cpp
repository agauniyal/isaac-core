#include <random>
#include <string>
#include <vector>


void genAndFill(std::vector<std::string> &v)
{
	const char alphanum[] = "0123456789"
	                        "abcdefghijklmnopqrstuvwxyz"
	                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char ss[8];
	std::random_device rd;
	std::mt19937 gen(rd());

	for (int i = 0; i < 8; i++) {
		std::uniform_int_distribution<> dis(0, 61);
		ss[i] = alphanum[dis(gen)];
	}
	v.push_back(ss);
}

int main()
{
	std::vector<std::string> v;
	for (auto l = 0; l < 500000; l++) {
		genAndFill(v);
	}
}
