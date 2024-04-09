#pragma once

#include <stdexcept>

class ClientDataException : public std::runtime_error
{
public:
	ClientDataException(const char *_message)
		: std::runtime_error(_message) {}

	ClientDataException(const std::string &_message)
		: std::runtime_error(_message) {}

	~ClientDataException() = default;

	ClientDataException(const ClientDataException &) = default;
	ClientDataException(ClientDataException &&) = default;
	ClientDataException & operator=(const ClientDataException &) = default;
	ClientDataException & operator=(ClientDataException &&) = default;
};
