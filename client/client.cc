/*
 *  Copyright (c) 2016, https://github.com/zhatalk
 *  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "client/client.h"

#include <iostream>

#include "proto/cc/seqsvr.pb.h"
#include "nebula/net/zproto/api_message_box.h"

#include "nebula/net/rpc/zrpc_service_util.h"
#include "nebula/net/net_engine_manager.h"
#include "nebula/base/readline_gets.h"

#include "client/client_command_handler.h"

bool Client::Initialize() {
  // 注册服务
  // RegisterService("seq_client", "rpc_client", "zrpc");
  // 注册服务
  RegisterService("alloc_client", "rpc_client", "zrpc");

  return BaseServer::Initialize();
}

bool Client::Run() {
  // auto net_engine_manager = nebula::NetEngineManager::GetInstance();
  // 启动成功
  try {
    nebula::NetEngineManager::GetInstance()->Start();
  } catch (std::exception& e) {
    LOG(ERROR) << "Run - catch exception: " << e.what();
    return false;
  } catch (...) {
    LOG(ERROR) << "Run - catch  a invalid exception";
    return false;
  }
  
  // GPerftoolsProfiler profiler;
  // profiler.ProfilerStart();
  
  //BaseDaemon::Run();
  DoCommandLineLoop();
  
  // profiler.ProfilerStop();
  
  nebula::NetEngineManager::GetInstance()->Stop();

  return true;
  // return BaseServer::Run();
}

void Client::DoCommandLineLoop() {
  
  try {
    while (true) {
      auto line = ReadlineGets("nebula-im> ");
      if (line.empty()) {
        continue;
      }
      
      std::vector<folly::StringPiece> cmds;
      folly::split(" ", line, cmds);
      
      if (-2 == DoClientCommand(cmds)) {
        break;
      }
    }
  } catch (const std::exception& e) {
    std::cout << "DoCommandLineLoop - " << folly::exceptionStr(e) << std::endl;
  }
}


int main(int argc, char* argv[]) {
  return nebula::DoMain<Client>(argc, argv);
}

void DebugTest() {
  zproto::GetRouteTableReq get_route_table_req;
  std::string o;
  get_route_table_req.SerializeToString(&o);
  std::cout << o.length() << ", " << get_route_table_req.Utf8DebugString() << std::endl;
  
  get_route_table_req.Clear();
  bool rv = get_route_table_req.ParseFromArray(o.c_str(), o.length());
  if (!rv) std::cout << "error!!!" << std::endl;
  std::cout << o.length() << ", " << get_route_table_req.Utf8DebugString() << std::endl;
}

