// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.
#pragma once

namespace ZeronEngine
{
	class IEventHandler;
	
	class EventDispatcher
	{
		
	private:
		/*
		 * Map that contains callbacks of event handlers that are associated with events
		 * Key is expected to be class name of the event
		 */
		mutable std::unordered_map<std::string, std::vector<std::pair<void*, std::any>>> m_EventMap;
		
	public:
		/*
		 * Register an EventHandler with a callback for a specific event to listen
		 */
		template<typename E>
		void Register(void* handle, std::function<void(const E&)> callback) const
		{
			m_EventMap[E::GetName()].push_back(std::make_pair(handle, callback));
		}

		/*
		 * Dispatch an event to all the Event Handlers that are subscribed to that specific event
		 * If event is consumed it will no longer be available to be processed
		 * Order of registration determines which Event Handler have priority of over the event
		 */
		template<typename E>
		void Dispatch(const E& e) const
		{
			if(m_EventMap.count(e.GetName()) == 0){
				return;
			}
			for (const std::pair<void*, std::any>& value : m_EventMap.at(e.GetName()))
			{
				if(value.first == nullptr ||  e.IsConsumed() == true){
					return;
				}
				auto callback = std::any_cast<std::function<void(const E&)>>(value.second);
				if(callback){
					callback(e);
				}
			}
		}

		/*
		 * Remove an Event Handler from listening to a certain event type
		 */
		void Remove(void* eventHandler) const
		{
			for(auto& eventContainer : m_EventMap)
			{
				auto it = eventContainer.second.begin();
				while (it != eventContainer.second.end())
				{
					if (it->first == eventHandler) {
						it = eventContainer.second.erase(it);
					}
					else {
						++it;
					}
				}
			}

		}
		
		/*
		 * Remove an Event Handler from listening to a certain event type
		 */
		template<typename E>
		void Remove(void* eventHandler) const
		{
			if (m_EventMap.count(E::GetName()) == 0) {
				return;
			}
			
			auto& eventContainer = m_EventMap.at(E::GetName());
			auto it = eventContainer.begin();
			while (it != eventContainer.end())
			{
				if (it->first == eventHandler){
					it = eventContainer.erase(it);
				}else{
					++it;
				}
			}
		}
		
	};


}

