// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/API/asio/AsioUtils.h>

#include <asio/connect.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/read.hpp>
#include <asio/read_until.hpp>
#include <asio/write.hpp>

namespace Zeron::Net::AsioUtils
{
	namespace Http
	{
		namespace
		{
			void _readHeader(std::shared_ptr<PayloadReader> reader, std::function<void(const std::error_code&)> callback)
			{
				asio::async_read_until(
					reader->mSocket.get(),
					asio::dynamic_buffer(reader->mData),  // Fill the body until we construct the header
					"\r\n\r\n",
					[reader, fn = std::move(callback)](std::error_code ec, std::size_t headerEnd) {
						if (ec) {
							fn(ec);
							return;
						}

						std::string& data = reader->mData;
						const std::string headers = data.substr(0, headerEnd);
						if (!reader->mPayload->ParseHeader(headers)) {
							fn(std::make_error_code(std::errc::protocol_error));
							return;
						}
						data.erase(0, headerEnd);

						fn(ec);
					}
				);
			}

			void _readPayloadBody(
				std::shared_ptr<PayloadReader> reader, std::size_t remaining, std::size_t totalLength,
				std::function<void(std::shared_ptr<HttpPayload>, const std::error_code&)> callback
			)
			{
				asio::async_read(
					reader->mSocket.get(),
					asio::dynamic_buffer(reader->mData),
					asio::transfer_exactly(remaining),
					[totalLength, reader, fn = std::move(callback)](std::error_code ec, std::size_t /*read*/) {
						if (ec) {
							fn(nullptr, ec);
							return;
						}

						reader->mPayload->SetBody(reader->mData.substr(0, totalLength));
						fn(reader->mPayload, ec);
					}
				);
			}
		}

		void readPayload(std::shared_ptr<PayloadReader> reader, std::function<void(std::shared_ptr<HttpPayload>, const std::error_code&)> callback)
		{
			_readHeader(reader, [reader, fn = std::move(callback)](const std::error_code& ec) {
				if (ec) {
					fn(nullptr, ec);
					return;
				}

				const std::string& data = reader->mData;
				const size_t contentLength = reader->mPayload->GetHeader().GetContentLength();

				// Check how much of the body is already in buffer
				const std::size_t currentBodySize = data.size();
				if (currentBodySize >= contentLength) {
					reader->mPayload->SetBody(data.substr(0, contentLength));
					fn(reader->mPayload, ec);
				}
				else {
					const std::size_t remaining = contentLength - currentBodySize;
					_readPayloadBody(reader, remaining, contentLength, fn);
				}
			});
		}

		void readServerEventStream(std::shared_ptr<PayloadReader> reader, HttpServerSentEventCallback callback)
		{
			asio::async_read_until(
				reader->mSocket.get(),
				asio::dynamic_buffer(reader->mData),
				"\n\n",
				[reader, fn = std::move(callback)](std::error_code ec, std::size_t length) {
					if (ec) {
						fn(nullptr, ec);
						return;
					}

					std::string& buffer = reader->mData;
					// std::string_view line(buffer.data(), length);
					//  buffer.erase(0, length);

					HttpServerSentEvent event;
					if (event.Parse(buffer)) {
						fn(&event, {});
					}
					else {
						fn(nullptr, std::make_error_code(std::errc::bad_message));
					}
					buffer.clear();

					readServerEventStream(reader, fn);
				}
			);
		}
	}

	std::shared_ptr<asio::steady_timer> createTimeout(asio::io_context& ctx, Time::Milliseconds ms, std::function<void(const std::error_code&)> onTimeout)
	{
		auto timer = std::make_shared<asio::steady_timer>(ctx);
		timer->expires_after(std::chrono::duration_cast<std::chrono::milliseconds>(Time::Milliseconds(ms)));
		timer->async_wait([fn = std::move(onTimeout)](const std::error_code& ec) {
			if (!ec) {
				fn(std::make_error_code(std::errc::operation_canceled));
			}
		});
		return timer;
	}


}