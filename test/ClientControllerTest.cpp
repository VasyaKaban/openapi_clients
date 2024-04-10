#include <random>
#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "ClientControllerTest.h"
#include "App/TestComponent.hpp"
#include "App/ClientApiTestClient.hpp"
#include "../src/Controller/ClientController.hpp"
#include "../src/Controller/ClientControllerDTOs.h"
#include "../src/Repository/impl/JsonFileClientRepository.h"

ClientControllerTest::ClientControllerTest()
	: oatpp::test::UnitTest("TEST[ClientControllerTest]") {}

void ClientControllerTest::onRun()
{
	TestComponent components;
	oatpp::test::web::ClientServerTestRunner runner;

	std::unique_ptr<JsonFileClientRepository> repository(new JsonFileClientRepository);
	try
	{
		repository->Open("test_clients.json", false);
	}
	catch(const std::exception &ex)
	{
		OATPP_LOGD("ServerRun",
				   "Deserialization from %s failed!",
				   repository->GetFilePath().c_str());
	}

	runner.addController(std::make_shared<ClientController>(std::move(repository)));

	runner.run([this]()
	{
		OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>,
						client_connection_provider);
		OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, object_mapper);

		auto request_executor =
			oatpp::web::client::HttpRequestExecutor::createShared(client_connection_provider);

		auto api_client = ClientApiTestClient(request_executor, object_mapper);

		OATPP_LOGI("TEST_ENTRY", "TestGetAllClients");
		TestGetAllClients(api_client);

		OATPP_LOGI("TEST_ENTRY", "TestGetClientById");
		TestGetClientById(api_client);

		OATPP_LOGI("TEST_ENTRY", "TestCreateClient");
		TestCreateClient(api_client);

		OATPP_LOGI("TEST_ENTRY", "TestUpdateClient");
		TestUpdateClient(api_client);

		OATPP_LOGI("TEST_ENTRY", "TestDeleteClientById");
		TestDeleteClientById(api_client);

	}, std::chrono::minutes(10));

	//std::this_thread::sleep_for(std::chrono::seconds(5));//?????????????
}

template<std::ranges::input_range R>
constexpr static void AssertStatusCode(std::uint32_t code, const R &good_codes) noexcept
{
	auto it = std::ranges::find(good_codes, code);
	if(it == std::ranges::end(good_codes))
	{
		OATPP_LOGE("TEST",
				   "Server has returned bad response with satus code: %d",
				   code);
		exit(EXIT_FAILURE);
	}
}

static void AssertClientNotNull(const oatpp::Object<Client> &client)
{
	OATPP_ASSERT(client.get() != nullptr);
	OATPP_ASSERT(client->id != nullptr);
	OATPP_ASSERT(client->name != nullptr);
	OATPP_ASSERT(client->phone_number != nullptr);
	OATPP_ASSERT(client->address != nullptr);
}

oatpp::Vector<oatpp::Object<Client>> ClientControllerTest::TestGetAllClients(ClientApiTestClient &api_client)
{
	constexpr static std::array good_codes =
	{
		200
	};
	auto response = api_client.GetAllClients();
	AssertStatusCode(response->getStatusCode(), good_codes);

	auto clients = response->readBodyToDto<oatpp::Object<ResponseClients>>(api_client.getObjectMapper());
	OATPP_ASSERT(clients.get() != nullptr);
	OATPP_ASSERT(std::is_sorted(clients->data->begin(), clients->data->end(),
								[](const oatpp::Object<Client> cl0, const oatpp::Object<Client> cl1)
								{
									AssertClientNotNull(cl0);
									AssertClientNotNull(cl1);
									return cl0->id < cl0->id;
								}));

	return clients->data;
}

void ClientControllerTest::TestGetClientById(ClientApiTestClient &api_client)
{
	auto clients = TestGetAllClients(api_client);
	constexpr static std::array good_codes =
	{
		200
	};

	for(auto &cl : *clients.get())
	{
		auto response = api_client.GetClientById(cl->id);
		AssertStatusCode(response->getStatusCode(), good_codes);
		auto client = response->readBodyToDto<oatpp::Object<ResponseClient>>(api_client.getObjectMapper());
		OATPP_ASSERT(client.get() != nullptr);
		AssertClientNotNull(client->data);
		OATPP_ASSERT(cl == client->data);
	}
}

void ClientControllerTest::TestCreateClient(ClientApiTestClient &api_client)
{
	auto client = Client::createShared();
	client->id = std::uint64_t(0);
	client->name = "TEST_CLIENT_NAME";
	client->phone_number = 1234567890;
	client->address = "TEST_CLIENT_ADDRESS";

	constexpr static std::array good_codes =
	{
		200
	};
	auto response = api_client.CreateClient(client);
	AssertStatusCode(response->getStatusCode(), good_codes);

	auto client_id =
		response->readBodyToDto<oatpp::Object<ResponseCreatedClientId>>(api_client.getObjectMapper());
	OATPP_ASSERT(client_id.get() != nullptr);
	OATPP_ASSERT(client_id->data != nullptr);
	client->id = client_id->data;

	auto clients = TestGetAllClients(api_client);
	auto found_client = std::ranges::find_if(*clients.get(),
											 [&client](const oatpp::Object<Client> &cl) -> bool
											 {
												 return cl->id == client->id;
											 });

	OATPP_ASSERT(found_client != clients->end());
	OATPP_ASSERT(*found_client == client);
}

void ClientControllerTest::TestUpdateClient(ClientApiTestClient &api_client)
{
	auto pre_clients = TestGetAllClients(api_client);
	if(pre_clients->empty())
		return;

	std::random_device dev;
	std::mt19937_64 mt_random_gen(dev());
	std::uniform_int_distribution<std::size_t> dist(0, pre_clients->size() - 1);

	std::size_t update_index = dist(mt_random_gen);
	auto client = pre_clients[update_index];
	AssertClientNotNull(client);

	client->name = "CLIENT_TEST_NAME";
	client->address = "CLIENT_TEST_ADDRESS";
	client->phone_number = 1234567890;

	constexpr static std::array good_codes =
	{
		200
	};
	auto response = api_client.UpdateClient(client);
	AssertStatusCode(response->getStatusCode(), good_codes);

	auto post_clients = TestGetAllClients(api_client);
	OATPP_ASSERT(!post_clients->empty());

	auto found_client = std::ranges::find_if(*post_clients.get(),
											 [&client](const oatpp::Object<Client> &cl) -> bool
											 {
												 return cl->id == client->id;
											 });

	OATPP_ASSERT(found_client != post_clients->end());
	OATPP_ASSERT(*found_client == client);
}

void ClientControllerTest::TestDeleteClientById(ClientApiTestClient &api_client)
{
	auto pre_clients = TestGetAllClients(api_client);
	if(pre_clients->empty())
		return;

	std::random_device dev;
	std::mt19937_64 mt_random_gen(dev());
	std::uniform_int_distribution<std::size_t> dist(0, pre_clients->size() - 1);

	std::size_t delete_index = dist(mt_random_gen);
	auto client = pre_clients[delete_index];
	AssertClientNotNull(client);

	constexpr static std::array good_codes =
	{
		200
	};
	auto response = api_client.DeleteClientById(client->id);
	AssertStatusCode(response->getStatusCode(), good_codes);

	auto post_clients = TestGetAllClients(api_client);
	OATPP_ASSERT(pre_clients->size() == post_clients->size() + 1);
	auto found_client = std::ranges::find_if(*post_clients.get(),
											 [&client](const oatpp::Object<Client> &cl) -> bool
											 {
												 return cl->id == client->id;
											 });

	OATPP_ASSERT(found_client == post_clients->end());
}
