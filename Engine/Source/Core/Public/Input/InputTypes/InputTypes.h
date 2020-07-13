// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace ZeronEngine
{
	// =============================================================
	// --------------------------- MOUSE ---------------------------
	// =============================================================

	struct MouseCode
	{
		enum Type : uint8_t
		{
			LeftButton = 0,
			RightButton = 1,
			MiddleButton = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			Button6 = 6,
			Button7 = 7
		};

		MouseCode(Type button) : m_Type(button) {}
		MouseCode(int button) : m_Type(static_cast<Type>(button)) {}

		operator Type() const { return m_Type; }
		operator const int() const { return static_cast<int>(m_Type); }

		std::string ToString() const
		{
			switch (m_Type)
			{
				case LeftButton:	return std::string("Mouse Button Left");
				case RightButton:	return std::string("Mouse Button Right");
				case MiddleButton:	return std::string("Mouse Button Middle");
				case Button3:		return std::string("Mouse Button 3");
				case Button4:		return std::string("Mouse Button 4");
				case Button5:		return std::string("Mouse Button 5");
				case Button6:		return std::string("Mouse Button 6");
				case Button7:		return std::string("Mouse Button 7");
				default:			return std::string("Invalid Mouse Button");
			}
		}

	private:
		Type m_Type;
	};


	enum class MouseAction : uint8_t
	{
		None = 0,
		Pressed,
		Released
	};

	// =============================================================
	// ------------------------ KEYBOARD ---------------------------
	// =============================================================

	struct KeyCode
	{
		enum Type : uint16_t
		{

		};
		
	};


	enum class KeyAction : uint8_t
	{
		None = 0,
		Pressed,
		Released,
		Repeat
	};

	// =============================================================
	// ----------------------- MODIFIER KEYS -----------------------
	// =============================================================

	/*
	 * Modifier Keys that are active during mouse and key events
	 */
	struct ModifierKeys
	{
		enum Type : uint8_t
		{
			None		= 0,
			Shift		= 1 << 0,
			Control		= 1 << 1,
			Alt			= 1 << 2,
			Super		= 1 << 3,
			CapsLock	= 1 << 4,
			NumLock		= 1 << 5
		};

		ModifierKeys()			:m_Type(None) {}
		ModifierKeys(Type type)	:m_Type(type) {}
		ModifierKeys(int type)  :m_Type(static_cast<Type>(type)) {}

		
		operator Type() const						{ return m_Type; }
		operator const int() const					{ return static_cast<int>(m_Type); }
		bool operator ==(ModifierKeys other) const  { return m_Type == other.m_Type; }
		bool operator !=(ModifierKeys other) const	{ return m_Type != other.m_Type; }
		
		ModifierKeys operator |(const ModifierKeys& other) const
		{
			return ModifierKeys(static_cast<Type>(static_cast<uint8_t>(m_Type) | static_cast<uint8_t>(other.m_Type)));
		}

		ModifierKeys operator &(const ModifierKeys& other) const
		{
			return ModifierKeys(static_cast<Type>(static_cast<uint8_t>(m_Type) & static_cast<uint8_t>(other.m_Type)));
		}
		
		ModifierKeys& operator |= (const ModifierKeys& other)
		{
			m_Type = static_cast<Type>(static_cast<uint8_t>(m_Type) | static_cast<uint8_t>(other.m_Type));
			return *this;
		}

		ModifierKeys& operator |= (Type other)
		{
			m_Type = static_cast<Type>(static_cast<uint8_t>(m_Type) | static_cast<uint8_t>(other));
			return *this;
		}

		ModifierKeys& operator &= (const ModifierKeys& other)
		{
			m_Type = static_cast<Type>(static_cast<uint8_t>(m_Type) & static_cast<uint8_t>(other.m_Type));
			return *this;
		}

		ModifierKeys& operator &= (Type other)
		{
			m_Type = static_cast<Type>(static_cast<uint8_t>(m_Type) & static_cast<uint8_t>(other));
			return *this;
		}
		
		bool HasKey(Type type) const
		{
			return (static_cast<uint8_t>(m_Type) & static_cast<uint8_t>(type)) > 0;
		}

		// TODO String conversion for modifier keys
		std::string ToString() const
		{
			if(m_Type == None){
				return "'No Modifier Keys'";
			}

			return "";
		}
		
	private:
		// Modifier mask
		Type m_Type; 
		
	};

	inline ModifierKeys operator|(ModifierKeys::Type lhs, ModifierKeys::Type rhs)
	{
		return ModifierKeys(static_cast<ModifierKeys::Type>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs)));
	}

	inline ModifierKeys operator&(ModifierKeys::Type lhs, ModifierKeys::Type rhs)
	{
		return ModifierKeys(static_cast<ModifierKeys::Type>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs)));
	}
}
