// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Platform/SystemEvent.h>

namespace Zeron
{
	class Platform;
	class Window;
	class SystemEvent;

	class SystemEventProcessor {
	public:
		using DispatchFn = std::function<void(const SystemEvent&, const SystemEvent::Context&)>;
		using ContextFn = std::function<SystemEvent::Context(SystemHandle)>;

		SystemEventProcessor(DispatchFn dispatchFn, ContextFn contextFn)
			: mDispatchCallback(std::move(dispatchFn)), mContextCallback(std::move(contextFn)) {}
		virtual ~SystemEventProcessor() = default;
		virtual void ProcessEvents() = 0;

	protected:
		DispatchFn mDispatchCallback;
		ContextFn mContextCallback;
	};

	class UniqueSystemEventProcessorList {
		using ProcessorPair = std::pair<size_t, std::unique_ptr<SystemEventProcessor>>;
	public:

		void ProcessEvents() const {
			for(auto& pair : mList) {
				pair.second->ProcessEvents();
			}
		}

		SystemEventProcessor* Get(size_t id) const {
			const auto found = std::find_if(mList.begin(), mList.end(), 
				[&id](const ProcessorPair& pair) { return pair.first == id; });
			return found != mList.end() ? found->second.get() : nullptr;
		}

		template<typename T, typename... Args>
		SystemEventProcessor* Add(size_t id, Args&&... args) {
			SystemEventProcessor* proc = Get(id);
			if (!proc) {
				mList.emplace_back(id, std::make_unique<T>(std::forward<Args>(args)...));
				return mList.back().second.get();
			}
			return proc;
		}

		void Remove(size_t id) {
			mList.erase(std::remove_if(mList.begin(), mList.end(),
				[&](const ProcessorPair& pair) { return pair.first == id; }), mList.end());
		}

	private:
		std::vector<ProcessorPair> mList;
	};

}
