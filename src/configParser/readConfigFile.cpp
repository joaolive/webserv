#include <fstream>
#include <sstream>
#include <cstring>
#include <cerrno>
#include <sys/stat.h>

std::string readConfigFile(const std::string& filename) {
	std::ifstream file(filename.c_str());
	validateFile(filename, file);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return (buffer.str());
}

void validateFile(const std::string& filename, std::ifstream& file)
{
	struct stat pathStat;

	if (stat(filename.c_str(), &pathStat) != 0)
		throw std::runtime_error("Error stat: " + std::string(std::strerror(errno)));
	if (!S_ISREG(pathStat.st_mode))
		throw std::runtime_error("Error: Path is a directory, not a regular file");
	if (!file.is_open())
		throw std::runtime_error("Error: " + std::string(std::strerror(errno)));
	if (file.peek() == std::ifstream::traits_type::eof())
		throw std::runtime_error("Error: Empty file");
}