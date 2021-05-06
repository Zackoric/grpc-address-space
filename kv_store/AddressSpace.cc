//
// Created by zackoric on 4/18/21.
//

#include "AddressSpace.h"

void AddressSpace::StartServer() {
    ServerBuilder builder;
    builder.AddListeningPort(_address, grpc::InsecureServerCredentials());

    auto build = builder.BuildAndStart();

    if (!build.status.ok())
    {
        std::cerr << "Failed to build and start server: "
                  << build.status.error() << std::endl;
        return;
    }
    std::unique_ptr<Server> server(std::move(build.server));
    std::cout << "Server listening on " << _address << std::endl;

    auto status = Serve(server.get());

    if (!status.ok())
    {
        std::cerr << "Failed to serve: " << status.error() << std::endl;
        return;
    }
}

stout::grpc::ServerStatus AddressSpace::Serve(stout::grpc::Server *server) {

    auto status = server->Serve<kv_store::KV_Store, kv_store::GetRequest, kv_store::GetResponse>(
            "Get",
            [this](auto *call, auto &&getRequest)
            {
                auto key = getRequest->key();
                kv_store::GetResponse getResponse;
                std::string value;
                if (_local_kv->Get(key, value)) {
                    getResponse.set_rescode(200);
                    getResponse.set_data(value);
                    call->WriteAndFinish(getResponse, Status::OK);
                }
                getResponse.set_rescode(404);
                call->WriteAndFinish(getResponse, Status::CANCELLED);
            },
            [](auto *, bool)
            {});

    if (!status.ok())
    {
        std::cout << "Failed instantiating Get service;" << std::endl;
        return status;
    }

    status = server->Serve<kv_store::KV_Store, kv_store::PutRequest, kv_store::PutResponse>(
            "Put",
            [this](auto *call, auto &&putRequest)
            {
                auto key = putRequest->key();
                kv_store::PutResponse putResponse;
                _local_kv->Set(putRequest->key(), putRequest->data());
                putResponse.set_rescode(200);
                call->WriteAndFinish(putResponse, Status::OK);
            },
            [](auto *, bool)
            {});

    if (!status.ok())
    {
        std::cout << "Failed instantiating Get service;" << std::endl;
        return status;
    }
    return status;
}
