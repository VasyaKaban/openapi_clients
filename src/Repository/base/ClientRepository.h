#pragma once

#include "oatpp/core/Types.hpp"
#include "../../DTO/Client.h"

class ClientRepository
{
public:
	virtual ~ClientRepository() {}
	virtual oatpp::Vector<oatpp::Object<Client>> GetClients(const std::size_t offset,
															const std::size_t count) const = 0;

	virtual oatpp::Object<Client> GetClientById(std::uint64_t id) const = 0;
	virtual std::uint64_t AddClient(oatpp::Object<Client> client) = 0;
	virtual void DeleteClient(std::uint64_t id) = 0;
	virtual void UpdateClient(oatpp::Object<Client> client) = 0;
	virtual bool IsEmpty() const noexcept = 0;
};
