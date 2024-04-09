#pragma once

#include <map>
#include <filesystem>
#include "../base/ClientRepository.h"

class JsonFileClientRepository : public ClientRepository
{
public:
	JsonFileClientRepository() = default;
	~JsonFileClientRepository();
	JsonFileClientRepository(const JsonFileClientRepository &) = default;
	JsonFileClientRepository(JsonFileClientRepository &&) = default;
	JsonFileClientRepository & operator=(const JsonFileClientRepository &) = default;
	JsonFileClientRepository & operator=(JsonFileClientRepository &&) = default;

	void Open(const std::filesystem::path &_file_path, bool _save_on_destroy);

	std::size_t GetClientsCount() const noexcept;

	void Serialize();
	void Deserialize();

	const std::filesystem::path & GetFilePath() const noexcept;

	virtual oatpp::Vector<oatpp::Object<Client>> GetClients(const std::size_t offset,
															const std::size_t count) const override final;

	virtual oatpp::Object<Client> GetClientById(std::uint64_t id) const override final;
	virtual std::uint64_t AddClient(oatpp::Object<Client> client) override final;
	virtual void DeleteClient(std::uint64_t id) override final;
	virtual void UpdateClient(oatpp::Object<Client> client) override final;
	virtual bool IsEmpty() const noexcept override final;

private:

	std::uint64_t GenerateId() noexcept;
	void RollbackGeneratedId() noexcept;

	void CheckClient(oatpp::Object<Client> client, bool check_id) const;

private:
	std::uint64_t id_counter;
	std::filesystem::path file_path;
	bool save_on_destroy;
	std::map<std::uint64_t, oatpp::Object<Client>> clients;
};
