#include "JsonFileClientRepository.h"
#include "../base/ClientDataException.hpp"
#include <fstream>
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

JsonFileClientRepository::~JsonFileClientRepository()
{
	if(save_on_destroy)
	{
		try
		{
			Serialize();
		}
		catch(std::exception &ex)
		{
			OATPP_LOGD("JsonFileClientRepository",
					   "Serialization from %s failed!\n Caused by: %s",
					   file_path.c_str(), ex.what());
			//supress exceptions
		}
	}
}

void JsonFileClientRepository::Open(const std::filesystem::path &_file_path, bool _save_on_destroy)
{
	file_path = _file_path;
	save_on_destroy = _save_on_destroy;
	Deserialize();
}

std::size_t JsonFileClientRepository::GetClientsCount() const noexcept
{
	return clients.size();
}

void JsonFileClientRepository::Serialize()
{
	std::ofstream ofs;
	ofs.open(file_path);
	if(!ofs.is_open())
		throw std::runtime_error("File not exists!");

	auto serizalizer_config = oatpp::parser::json::mapping::Serializer::Config::createShared();
	auto deserizalizer_config = oatpp::parser::json::mapping::Deserializer::Config::createShared();
	serizalizer_config->useBeautifier = true;

	auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared(serizalizer_config,
																		   deserizalizer_config);
	auto json =
		mapper->writeToString(GetClients(0, std::numeric_limits<std::size_t>::max()));
	ofs.write(json->c_str(), json->size());
}

void JsonFileClientRepository::Deserialize()
{
	clients.clear();
	std::ifstream ifs;
	ifs.open(file_path);
	if(!ifs.is_open())
		throw std::runtime_error("File not exists!");

	std::size_t data_size = std::filesystem::file_size(file_path);
	std::string str(data_size, '\0');
	ifs.read(str.data(), data_size);

	auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

	std::size_t max_id = 0;
	auto objects = mapper->readFromString<oatpp::Vector<oatpp::Object<Client>>>(str);
	for(auto &client : *objects)
	{
		if(max_id < client->id)
			max_id = client->id;

		clients.insert({*client->id.get(), client});
	}

	if(!clients.empty())
		max_id++;

	id_counter = max_id;
}

const std::filesystem::path & JsonFileClientRepository::GetFilePath() const noexcept
{
	return file_path;
}

oatpp::Vector<oatpp::Object<Client>>
JsonFileClientRepository::GetClients(const std::size_t offset,
									 const std::size_t count) const
{
	auto out_clients = oatpp::Vector<oatpp::Object<Client>>::createShared();
	if(offset >= clients.size() || count == 0)
		return out_clients;

	std::size_t elements_count = std::min(clients.size() - offset, count);
	out_clients->reserve(elements_count);

	for(auto it = std::next(clients.begin(), offset); it != clients.end(); it++)
		out_clients->push_back(it->second);

	return out_clients;
}

oatpp::Object<Client> JsonFileClientRepository::GetClientById(std::uint64_t id) const
{
	auto it = clients.find(id);
	if(it == clients.end())
		throw ClientDataException("Client not found!");

	return it->second;
}

std::uint64_t JsonFileClientRepository::AddClient(oatpp::Object<Client> client)
{
	CheckClient(client, false);
	std::uint64_t id = GenerateId();
	client->id = id;
	clients.insert({id, client});
	return id;
}

void JsonFileClientRepository::DeleteClient(std::uint64_t id)
{
	auto it = clients.find(id);
	if(it == clients.end())
		throw ClientDataException("Client not found!");

	clients.erase(it);
}

void JsonFileClientRepository::UpdateClient(oatpp::Object<Client> client)
{
	CheckClient(client, true);
	auto it = clients.find(*client->id.get());
	if(it == clients.end())
		return;

	it->second = client;
}

bool JsonFileClientRepository::IsEmpty() const noexcept
{
	return clients.empty();
}

std::uint64_t JsonFileClientRepository::GenerateId() noexcept
{
	return id_counter++;
}

void JsonFileClientRepository::RollbackGeneratedId() noexcept
{
	id_counter--;
}

void JsonFileClientRepository::CheckClient(oatpp::Object<Client> client, bool check_id) const
{
	if(check_id)
	{
		if(client->id.get() == nullptr)
			throw ClientDataException("Bad client id!");
	}

	if(client->name.get() == nullptr)
		throw ClientDataException("Bad client name!");
	else if(client->name.get()->empty())
		throw ClientDataException("Bad client name!");

	if(client->phone_number.get() == nullptr)
		throw ClientDataException("Bad client phone number!");

	if(client->address.get() == nullptr)
		throw ClientDataException("Bad client address!");
	else if(client->address.get()->empty())
		throw ClientDataException("Bad client address!");
}
