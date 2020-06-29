/**
 * This file is generated by jsonrpcstub, DO NOT CHANGE IT MANUALLY!
 */

#ifndef JSONRPC_CPP_STUB_LEADERBOARD_SUBMIT_H_
#define JSONRPC_CPP_STUB_LEADERBOARD_SUBMIT_H_

#include <jsonrpccpp/client.h>

class leaderboard_submit : public jsonrpc::Client
{
    public:
        leaderboard_submit(jsonrpc::IClientConnector &conn, jsonrpc::clientVersion_t type = jsonrpc::JSONRPC_CLIENT_V2) : jsonrpc::Client(conn, type) {}

        Json::Value submit(const std::string& submission) throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p["submission"] = submission;
            Json::Value result = this->CallMethod("submit",p);
            if (result.isObject())
                return result;
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
};

#endif //JSONRPC_CPP_STUB_LEADERBOARD_SUBMIT_H_