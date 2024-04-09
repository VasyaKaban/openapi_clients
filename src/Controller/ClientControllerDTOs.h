#pragma once

#include "oatpp/core/macro/codegen.hpp"
#include "../DTO/Response.h"
#include "../DTO/Client.h"

#include OATPP_CODEGEN_BEGIN(DTO)

RESPONSE(oatpp::Vector<oatpp::Object<Client>>, "Array of clients", Clients);
RESPONSE(oatpp::Object<Client>, "Requested client", Client);
RESPONSE(oatpp::UInt64, "Newly created client's ID", CreatedClientId);

#include OATPP_CODEGEN_END(DTO)

