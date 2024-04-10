#pragma once

#include "oatpp/network/virtual_/Interface.hpp"
#include "oatpp/network/virtual_/server/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/client/ConnectionProvider.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/component.hpp"

class TestComponent
{
public:
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, virtual_interface)
	([]
	{
		return oatpp::network::virtual_::Interface::obtainShared("virtualhost");
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
						   server_connection_provider)
	([]
	{
		OATPP_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, virtual_interface);
		return oatpp::network::virtual_::server::ConnectionProvider::createShared(virtual_interface);
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>,
						   client_connection_provider)
	([]
	{
		OATPP_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, virtual_interface);
		return oatpp::network::virtual_::client::ConnectionProvider::createShared(virtual_interface);
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
						   router)
	([]
	{
		return oatpp::web::server::HttpRouter::createShared();
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
						   server_connection_handler)
	([]
	{
		OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
		return oatpp::web::server::HttpConnectionHandler::createShared(router);
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
						   object_mapper)
	([]
	{
		return oatpp::parser::json::mapping::ObjectMapper::createShared();
	}());
};
