//
// Created by zackoric on 4/2/21.
//
#include <iostream>
#include "kv_store.h"

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
    : _client(make_shared<stout::grpc::Client>(address, credentials)) {}

void GRPCKVStore::Set(const std::string key, const std::string value) {
  Notification<Status> done;
  PutRequest req;
  req.set_key(key);
  req.set_value(value);
  cout << "Calling put with (" << key << ", " << value << ")" << endl;
  _client->Call<KV_Store, PutRequest, PutResponse>(
      "Put",
      &req,
      [&](auto *call, shared_ptr<PutResponse> response) {
        cout << "Put call returned with response code: " << response.get()->rescode() << endl;
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
  GetRequest req;
  req.set_key(key);
  cout << "Calling Get with key: " << key << endl;
  _client->Call<KV_Store, GetRequest, GetResponse>(
      "Get",
      &req,
      [&](auto *call, shared_ptr<GetResponse> response) {
        cout << "Put call returned with response code: " << response.get()->rescode() << endl;
        cout << "Put call returned with response value: " << response.get()->value() << endl;
        call->Finish();
      },
      [&](auto *call, const Status &status) {
        done.Notify(status);
      });
  Status status = done.Wait();
  if (!status.ok()) {
    std::cout << "Get rpc failed: " << status.error_message() << std::endl;
  }
}

int main() {
  std::cout << "hi world." << std::endl;
}