#pragma once

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "../../src/DTO/Client.h"

#include OATPP_CODEGEN_BEGIN(ApiClient)

class ClientApiTestClient : public oatpp::web::client::ApiClient
{
	API_CLIENT_INIT(ClientApiTestClient)

	API_CALL("GET", "api/clients", GetAllClients)
	API_CALL("GET", "api/clients/{clientId}", GetClientById, PATH(UInt64, client_id, "clientId"))
	API_CALL("POST", "api/clients", CreateClient, BODY_DTO(oatpp::Object<Client>, client));
	API_CALL("PUT", "api/clients", UpdateClient, BODY_DTO(oatpp::Object<Client>, client));
	API_CALL("DELETE", "api/clients/{clientId}", DeleteClientById, PATH(UInt64, client_id, "clientId"));
};

#include OATPP_CODEGEN_END(ApiClient)
