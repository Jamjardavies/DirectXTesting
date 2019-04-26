#pragma once

#include <exception>
#include <string>

class BaseException : public std::exception
{
private:
	int m_line;
	std::string m_file;

protected:
	mutable std::string m_whatBuffer;

public:
	BaseException(int line, const char* file) noexcept;

	std::string GetOriginString() const noexcept;

	virtual const char* what() const noexcept override;

	virtual const char* GetType() const noexcept { return "Base Exception"; }
	int GetLine() const noexcept { return m_line; }
	const std::string& GetFile() const noexcept { return m_file; }
};

