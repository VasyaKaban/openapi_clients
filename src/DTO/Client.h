#pragma once

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class Client : public oatpp::DTO
{
public:
	DTO_INIT(Client, DTO)

	DTO_FIELD_INFO(id)
	{
		info->description = "Client's identificator";
	}
	DTO_FIELD(UInt64, id);

	DTO_FIELD_INFO(name)
	{
		info->description = "Client's name";
	}
	DTO_FIELD(String, name);

	DTO_FIELD_INFO(phone_number)
	{
		info->description = "Client's phone number";
	}
	DTO_FIELD(UInt64, phone_number);

	DTO_FIELD_INFO(address)
	{
		info->description = "Client's residential address";
	}
	DTO_FIELD(String, address);

	//we need this because DTO's defaultEquals uses plain pointer comparison!
	bool operator==(const Client &cl) const noexcept
	{
		return
			id == cl.id &&
			name == cl.name &&
			phone_number == cl.phone_number &&
			address == cl.address;

	}

};

#include OATPP_CODEGEN_END(DTO)
