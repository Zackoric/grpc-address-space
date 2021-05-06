//
// Created by zackoric on 4/2/21.
//
#include <iostream>
#include "kv_store.h"
#include "AddressSpace.h"

MemoryKVStore::MemoryKVStore()
    : _map() {}

void MemoryKVStore::Set(const std::string key, const std::string value)
{
  _map[key] = value;
}

bool MemoryKVStore::Get(const std::string &key, std::string &value)
{
  auto iter = _map.find(key);
  if (iter == _map.end()) {
    return false;
  } else {
    value = iter->second;
    return true;
  }
}

GRPCKVStore::GRPCKVStore(const std::string &address, const std::shared_ptr<::grpc::ChannelCredentials> &credentials)
    : _client(std::make_shared<stout::grpc::Client>(address, credentials)) {}

void GRPCKVStore::Set(const std::string key, const std::string value) {
  Notification<Status> done;
  kv_store::PutRequest req;
  req.set_key(key);
  req.set_data(value);
  std::cout << "Calling put with (" << key << ", " << value << ")" << std::endl;
  _client->Call<kv_store::KV_Store, kv_store::PutRequest, kv_store::PutResponse>(
      "Put",
      &req,
      [&](auto *call, std::shared_ptr<kv_store::PutResponse> response) {
          std::cout << "Put call returned with response code: " << response.get()->rescode() << std::endl;
        call->Finish();
      },
      [&](auto *call, const Status &status) {
        done.Notify(status);
      });
  Status status = done.Wait();
  if (!status.ok()) {
    std::cout << "Put rpc failed: " << status.error_message() << std::endl;
  }
}

bool GRPCKVStore::Get(const std::string &key, std::string &value) {
  Notification<Status> done;
  kv_store::GetRequest req;
  req.set_key(key);
    std::cout << "Calling Get with key: " << key << std::endl;
  _client->Call<kv_store::KV_Store, kv_store::GetRequest, kv_store::GetResponse>(
      "Get",
      &req,
      [&](auto *call, std::shared_ptr<kv_store::GetResponse> response) {
          std::cout << "Put call returned with response code: " << response.get()->rescode() << std::endl;
          std::cout << "Put call returned with response data: " << response.get()->data() << std::endl;
        call->Finish();
      },
      [&](auto *call, const Status &status) {
        done.Notify(status);
      });
  Status status = done.Wait();
  if (!status.ok()) {
    std::cout << "Get rpc failed: " << status.error_message() << std::endl;
    return false;
  }
  return true;
}

//int main() {
//  std::cout << "hi world." << std::endl;
//  AddressSpace space("localhost");
//  kv_store::MyStruct stru;
//  stru.set_astring("a string");
//  stru.set_anumber(42);
//  auto ref = space.Put(stru);
//  stru = space.Get<kv_store::MyStruct>(ref);
//  std::cout << stru.astring() << std::endl;
//}