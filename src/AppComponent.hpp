#pragma once

#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/component.hpp"

#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"

class AppComponent
{
private:
	std::string host;
	std::uint16_t port;
public:
	AppComponent(const std::string &_host, std::uint16_t _port)
		: host(_host),
		  port(_port) {}

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
						   connection_provider)([this]
	{
		const oatpp::network::Address addr(host, port, oatpp::network::Address::IP_4);
		return oatpp::network::tcp::server::ConnectionProvider::createShared(addr);
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
						   router)([]
	{
		return oatpp::web::server::HttpRouter::createShared();
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpConnectionHandler>,
						   connection_handler)([]
	{
		OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
		return oatpp::web::server::HttpConnectionHandler::createShared(router);
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>,
						   api_object_mapper)([]
	{
		return oatpp::parser::json::mapping::ObjectMapper::createShared();
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swagger_document_info)
	([]
	{
		oatpp::swagger::DocumentInfo::Builder builder;
		builder
			.setTitle("Clients service")
			.setDescription("Simple CRUD api")
			.setContactEmail("kirill.balashov03@gmail.com")
			.setContactName("Kirill Balashov")
			.setVersion("1.0");

		return builder.build();
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swagger_resources)
	([]
	{
		return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);
	}());
};
