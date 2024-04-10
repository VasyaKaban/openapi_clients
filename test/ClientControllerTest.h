#pragma once

#include "oatpp-test/UnitTest.hpp"
#include "../src/DTO/Client.h"

class ClientApiTestClient;

class ClientControllerTest : public oatpp::test::UnitTest
{
public:
	ClientControllerTest();
	virtual void onRun() override;

private:
	oatpp::Vector<oatpp::Object<Client>> TestGetAllClients(ClientApiTestClient &api_client);
	void TestGetClientById(ClientApiTestClient &api_client);
	void TestCreateClient(ClientApiTestClient &api_client);
	void TestUpdateClient(ClientApiTestClient &api_client);
	void TestDeleteClientById(ClientApiTestClient &api_client);
};
