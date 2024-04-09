#include <thread>
#include <iostream>
#include "oatpp/network/Server.hpp"
#include "oatpp-swagger/Controller.hpp"
#include "Controller/ClientController.hpp"
#include "AppComponent.hpp"
#include "Repository/impl/JsonFileClientRepository.h"

void ServerInputHandling(const oatpp::network::Server &server)
{
	std::string input;
	while(true)
	{
		std::getline(std::cin, input);
		if(input == "shutdown")
		{
			OATPP_LOGD("Command", "Shutdown requested...");
			return;
		}
		else
			OATPP_LOGD("Command", "Unknown command: %s", input.c_str());
	}
}

void ServerRun()
{
	AppComponent components("localhost", 8000);

	OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

	std::unique_ptr<JsonFileClientRepository> repository(new JsonFileClientRepository());
	try
	{
		repository->Open("clients.json", true);
	}
	catch(const std::exception &ex)
	{
		OATPP_LOGD("ServerRun",
				   "Deserialization from %s failed!",
				   repository->GetFilePath().c_str());
	}

	auto client_controller = std::make_shared<ClientController>(std::move(repository));
	router->addController(client_controller);

	auto swagger_controller = oatpp::swagger::Controller::createShared(client_controller->getEndpoints());
	router->addController(swagger_controller);

	OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpConnectionHandler>, connection_handler);
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connection_provider);

	oatpp::network::Server server(connection_provider, connection_handler);

	OATPP_LOGD("Server",
			   "Server is working on port: %s",
			   connection_provider->getProperty("port").toString()->c_str());

	std::thread server_thread([&server]
	{
		server.run();
	});

	ServerInputHandling(server);

	connection_provider->stop();
	if(server.getStatus() == oatpp::network::Server::STATUS_RUNNING)
		server.stop();

	connection_handler->stop();
	server_thread.join();
}

int main(int argc, char **argv)
{
	oatpp::base::Environment::init();

	ServerRun();

	oatpp::base::Environment::destroy();
	return 0;
}
