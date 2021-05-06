//
// Created by zackoric on 4/18/21.
//

#ifndef GRPC_ADDRESS_SPACE_ADDRESSSPACETESTS_H
#define GRPC_ADDRESS_SPACE_ADDRESSSPACE_H

#include <string>
#include "kv_store.h"

#include "stout/grpc/server.h"
#include "protos/kv_store.grpc.pb.h"

typedef std::string HostAddress;
typedef std::string ResourceUUID;

using grpc::Status;
using std::chrono::system_clock;
using stout::grpc::Server;
using stout::grpc::ServerBuilder;
using stout::grpc::Stream;

template <typename T>
struct Reference {
public:
    Reference(HostAddress host, ResourceUUID uuid)
            : _host(host), _uuid(uuid) {}

    const std::string _uuid;
    const HostAddress _host;
};

struct AddressSpace {
    AddressSpace(std::string port)
    : _address("0.0.0.0:" + port) {
        _local_kv = std::make_shared<MemoryKVStore>();
    }

    template <typename T>
    std::enable_if_t<stout::grpc::IsMessage<T>::value, T>
    Get(Reference<T> reference) {
        std::string data;
        T ret;
        T* rett = &ret;
        if (!_local_kv->Get(reference._uuid, data)) {
            GRPCKVStore remoteKV(reference._host);
            remoteKV.Get(reference._uuid, data);
        }
        dynamic_cast<google::protobuf::MessageLite*>(rett)->ParseFromString(data);
        return ret;
    }

    template <typename T>
    std::enable_if_t<stout::grpc::IsMessage<T>::value, Reference<T>>
    Put(T message) {
        T* rett = &message;
        std::string data = dynamic_cast<google::protobuf::Message*>(rett)->SerializeAsString();
        std::string key = std::to_string(rand());
        _local_kv->Set(key, data);
        return Reference<T>(_address, key);
    }

    stout::grpc::ServerStatus Serve(stout::grpc::Server *server);
    void StartServer();

private:
    const std::string _address;
    std::shared_ptr<KVInterface> _local_kv;
    std::shared_ptr<GRPCKVStore> _grpc_kv;
};

#endif //GRPC_ADDRESS_SPACE_ADDRESSSPACETESTS_H
