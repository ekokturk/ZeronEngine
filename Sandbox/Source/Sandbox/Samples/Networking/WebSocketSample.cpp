// Copyright (C) Eser Kokturk. All Rights Reserved.


#include <Sandbox/Samples/Networking/WebSocketSample.h>

#include <imgui/imgui.h>
#include <Net/Http/HttpUrl.h>
#include <Net/WebSocket/WebSocketClient.h>
#include <Net/WebSocket/WebSocketServer.h>

using namespace Zeron;

namespace Sandbox
{
	WebSocketSample::WebSocketSample(IWindowSampleContext& ctx)
		: IWindowSample(ctx)
	{}

	WebSocketSample::~WebSocketSample() {}

	void WebSocketSample::Init() {}

	void WebSocketSample::Update()
	{

		for (auto&& [_, client] : mClients) {
			if (client.mSession) {
				client.mSession->Update();
			}
		}

		for (auto&& [_, server] : mServers) {
			if (server.mSession) {
				server.mSession->Update();
			}
		}
	}

	void WebSocketSample::OnGui()
	{
		if (ImGui::Begin("Websocket", &mShowingGui)) {
			static int serverPort = 3136;

			if (ImGui::Button("Create Server") && !mServers.contains(serverPort)) {
				auto server = Net::WebSocketServer::Create();
				server->Start(serverPort, [this, srv = server.get()](const Net::WebSocketMessage& msg) {
					switch (msg.mType) {
						case Net::WebSocketMessage::Open: {
							if (msg.mConnId != 0) {
								ZE_LOG("WebSocketSample Server: Client Connected {}", msg.mConnId);
								srv->SendText("Welcome", msg.mConnId);
							}
						} break;
						case Net::WebSocketMessage::Close: {
							if (msg.mConnId == 0) {
								mServers.erase(serverPort);
							}
							else {
								ZE_LOG("WebSocketSample Server: Client Disconnected {}", msg.mConnId);
							}
						} break;
						case Net::WebSocketMessage::Error: {
							ZE_LOGE("WebSocketSample Server: {}", msg.mPayload);
						} break;
						case Net::WebSocketMessage::Text: {
							mServers[serverPort].mMessages.push_back(msg.mPayload);
						} break;
						case Net::WebSocketMessage::Binary: break;
						case Net::WebSocketMessage::Ping: break;
						case Net::WebSocketMessage::Pong: break;
						default:;
					}
				});
				mServers.emplace(serverPort, Server{ std::move(server) });
			}
			ImGui::SameLine();
			ImGui::InputInt("Port", &serverPort);
			if (ImGui::Button("Create Client")) {
				Random rand;
				int id = rand.GetInt(0, 100000);
				auto client = Net::WebSocketClient::Create();
				mClients.emplace(id, Client{ std::move(client) });
			}

			ImGui::Separator();

			for (auto it = mServers.begin(); it != mServers.end();) {
				const int id = it->first;
				auto& server = it->second;
				auto& session = server.mSession;
				if (ImGui::TreeNode(Util::Format("Server {}", id).c_str())) {
					if (session->IsRunning()) {
						ImGui::BeginChild("Messages", ImVec2(0, 100), true, ImGuiWindowFlags_HorizontalScrollbar);
						const auto& messages = server.mMessages;
						for (size_t i = 0; i < server.mMessages.size(); ++i) {
							ImGui::Text("%s", messages[i].c_str());
						}
						ImGui::EndChild();

						ImGui::InputText("Text", server.mStagedMessage, 256);
						ImGui::SameLine();
						if (ImGui::Button("Send")) {
							session->SendText(server.mStagedMessage);
							std::memset(server.mStagedMessage, 0, sizeof(server.mStagedMessage));
						}

						if (ImGui::Button("Stop")) {
							session->Stop();
						}
					}

					if (ImGui::Button("Remove")) {
						it = mServers.erase(it);
						ImGui::TreePop();
						continue;
					}
					ImGui::TreePop();
				}
				++it;
			}

			ImGui::Separator();

			for (auto it = mClients.begin(); it != mClients.end();) {
				const int id = it->first;
				auto& client = it->second;
				auto& session = client.mSession;
				if (ImGui::TreeNode(Util::Format("Client {}", id).c_str())) {
					if (session->GetConnectionState() == Net::WebSocketConnectionState::Connected) {
						ImGui::BeginChild("Messages", ImVec2(0, 100), true, ImGuiWindowFlags_HorizontalScrollbar);
						const auto& messages = client.mMessages;
						for (size_t i = 0; i < client.mMessages.size(); ++i) {
							ImGui::Text("%s", messages[i].c_str());
						}
						ImGui::EndChild();

						ImGui::InputText("Text", client.mStagedMessage, 256);
						ImGui::SameLine();
						if (ImGui::Button("Send")) {
							session->SendText(client.mStagedMessage);
							std::memset(client.mStagedMessage, 0, sizeof(client.mStagedMessage));
						}

						if (ImGui::Button("Disconnect")) {
							session->End();
						}
					}
					else if (session->GetConnectionState() == Net::WebSocketConnectionState::Idle) {
						static char clientConnIp[64] = "127.0.0.1:3136";
						ImGui::InputText("IP", clientConnIp, 64);
						ImGui::SameLine();
						if (ImGui::Button("Connect")) {
							session->Connect(Net::HttpUrl(clientConnIp), [this, &client, id, me = session.get()](const Net::WebSocketMessage& message) {
								switch (message.mType) {
									case Net::WebSocketMessage::Open: {
										ZE_LOG("WebSocketSample Client: Opened Connection.");
										me->SendText("Hello");
									} break;
									case Net::WebSocketMessage::Close: {
										ZE_LOG("WebSocketSample Client: Closed Connection.");
									} break;
									case Net::WebSocketMessage::Error: {
										ZE_LOGE("WebSocketSample Client: {}", message.mPayload);
									} break;
									case Net::WebSocketMessage::Text: {
										client.mMessages.push_back(message.mPayload);
									} break;
									case Net::WebSocketMessage::Binary: break;
									case Net::WebSocketMessage::Ping: break;
									case Net::WebSocketMessage::Pong: break;
									default:;
								}
							});
						}
					}

					if (ImGui::Button("Remove")) {
						it = mClients.erase(it);
						ImGui::TreePop();
						continue;
					}
					ImGui::TreePop();
				}
				++it;
			}
		}
		ImGui::End();
	}
}