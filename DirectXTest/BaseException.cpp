#include "BaseException.h"

#include <sstream>

BaseException::BaseException(const int line, const char* file) noexcept
	: m_line(line), m_file(file)
{

}

std::string BaseException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << m_file << std::endl;
	oss << "[Line] " << m_line;

	return oss.str();
}

const char* BaseException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();

	m_whatBuffer = oss.str();

	return m_whatBuffer.c_str();
}
