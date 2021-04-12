//
// Created by zackoric on 4/2/21.
//

#ifndef GRPC_ADDRESS_SPACE_KV_STORE_H
#define GRPC_ADDRESS_SPACE_KV_STORE_H

#include <string>
#include <map>
#include <memory>

#include "stout/grpc/client.h"
#include "stout/notification.h"
//#include "stout/credentials.h"
#include "protos/kv_store.grpc.pb.h"

using grpc::Status;
using stout::Notification;

using namespace std;
using namespace stout;
using namespace kv_store;

struct KVInterface {
 public:
  virtual void Set(const std::string key, const std::string value) = 0;
  virtual bool Get(const std::string &key, std::string &value) = 0;

};

struct GRPCKVStore: KVInterface {
 public:
  GRPCKVStore(const std::string& , const std::shared_ptr<::grpc::ChannelCredentials>&);
  void Set(const std::string key, const std::string value);
  bool Get(const std::string &key, std::string &value);
 private:
  shared_ptr<stout::grpc::Client> _client;
};

struct MemoryKVStore: KVInterface {
 public:
  MemoryKVStore();
  void Set(const std::string key, const std::string value);
  bool Get(const std::string &key, std::string &value);
 private:
  std::map<std::string, std::string> _map;
};
#endif //GRPC_ADDRESS_SPACE_KV_STORE_H
