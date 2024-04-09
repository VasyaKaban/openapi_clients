#pragma once

#include <format>

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include "../Repository/base/ClientRepository.h"
#include "../Repository/base/ClientDataException.hpp"
#include "ClientControllerDTOs.h"

#include OATPP_CODEGEN_BEGIN(ApiController)

class ClientController : public oatpp::web::server::api::ApiController
{
public:
	ClientController(std::unique_ptr<ClientRepository> _client_repository,
					 OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, object_mapper))
		: oatpp::web::server::api::ApiController(object_mapper),
		  client_repository(std::move(_client_repository)) {}

	~ClientController() {}

	ENDPOINT("GET", "/api/clients", GetAllClients)
	{
		try
		{
			auto clients = client_repository->GetClients(0, std::numeric_limits<std::size_t>::max());
			return CreateJSONDataResponse<ResponseClients>(Status::CODE_200, clients);
		}
		catch(const std::exception &ex)
		{
			OATPP_LOGD("Client controller", "Exception: %s", ex.what());
			return CreateInnerServerErrorResponse();
		}
	}

	ENDPOINT("GET", "/api/clients/{clientId}", GetClientById,
			 PATH(UInt64, client_id, "clientId"))
	{
		try
		{
			auto client = client_repository->GetClientById(client_id);
			return CreateJSONDataResponse<ResponseClient>(Status::CODE_200, client);
		}
		catch(const ClientDataException &ex)
		{
			return CreateJSONStatusResponse(Status::CODE_400, ex.what());
		}
		catch(const std::exception &ex)
		{
			OATPP_LOGD("Client controller", "Exception: %s", ex.what());
			return CreateInnerServerErrorResponse();
		}
	}

	ENDPOINT("POST", "/api/clients", CreateClient,
			 REQUEST(std::shared_ptr<IncomingRequest>, request))
	{
		try
		{
			auto client = request->readBodyToDto<oatpp::Object<Client>>(getDefaultObjectMapper());
			std::uint64_t id = client_repository->AddClient(client);
			return CreateJSONDataResponse<ResponseCreatedClientId>(Status::CODE_200, id);
			return CreateSuccessResponse();
		}
		catch(const oatpp::parser::ParsingError &ex)
		{
			return CreateJSONStatusResponse(Status::CODE_400, "Bad client data!");
		}
		catch(const ClientDataException &ex)
		{
			return CreateJSONStatusResponse(Status::CODE_400, ex.what());
		}
		catch(const std::exception &ex)
		{
			OATPP_LOGD("Client controller", "Exception: %s", ex.what());
			return CreateInnerServerErrorResponse();
		}
	}

	ENDPOINT("PUT", "/api/clients", UpdateClient,
			 REQUEST(std::shared_ptr<IncomingRequest>, request))
	{
		try
		{
			auto client = request->readBodyToDto<oatpp::Object<Client>>(getDefaultObjectMapper());
			client_repository->UpdateClient(client);
			return CreateSuccessResponse();
		}
		catch(const oatpp::parser::ParsingError &ex)
		{
			return CreateJSONStatusResponse(Status::CODE_400, "Bad client data!");
		}
		catch(const ClientDataException &ex)
		{
			return CreateJSONStatusResponse(Status::CODE_400, ex.what());
		}
		catch(const std::exception &ex)
		{
			OATPP_LOGD("Client controller", "Exception: %s", ex.what());
			return CreateInnerServerErrorResponse();
		}
	}

	ENDPOINT("DELETE", "/api/clients/{clientId}", DeleteClientById,
			 PATH(UInt64, client_id, "clientId"))
	{
		try
		{
			client_repository->DeleteClient(client_id);
			return CreateSuccessResponse();
		}
		catch(const ClientDataException &ex)
		{
			return CreateJSONStatusResponse(Status::CODE_400, ex.what());
		}
		catch(const std::exception &ex)
		{
			OATPP_LOGD("Client controller", "Exception: %s", ex.what());
			return CreateInnerServerErrorResponse();
		}
	}

private:

	template<typename R, typename T>
	std::shared_ptr<OutgoingResponse>
	CreateJSONDataResponse(Status status, T &&data, const std::string &message = "Success")
	{
		auto resp = R::createShared();
		resp->status = message;
		resp->data = std::forward<T>(data);
		return createDtoResponse(status, resp);
	}

	std::shared_ptr<OutgoingResponse>
	CreateJSONStatusResponse(Status status, const std::string &status_message)
	{
		auto resp = createResponse(Status::CODE_200, std::format("{{\"status\": \"{}\"}}", status_message));
		resp->putHeader("Content-Type", "application/json");
		return resp;
	}


	std::shared_ptr<OutgoingResponse> CreateSuccessResponse()
	{
		return CreateJSONStatusResponse(Status::CODE_200, "Success");
	}

	std::shared_ptr<OutgoingResponse>
	CreateInnerServerErrorResponse(const oatpp::String &error_message = "Inner server error!")
	{
		return CreateJSONStatusResponse(Status::CODE_500, error_message);
	}

private:
	std::unique_ptr<ClientRepository> client_repository;
};

#include OATPP_CODEGEN_END(ApiController)
