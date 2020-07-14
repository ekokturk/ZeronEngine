// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace ZeronEngine
{
	// =======================================================================================
	// ---------------------------------------- MOUSE ----------------------------------------
	// =======================================================================================

	struct MouseCode
	{
		enum Type : uint8_t
		{
			Unknown = 0,
			LeftButton = 1,
			RightButton = 2,
			MiddleButton = 3,
			Button3 = 4,
			Button4 = 5,
			Button5 = 6,
			Button6 = 7,
			Button7 = 8
		};

		MouseCode(Type button) : m_Type(button) {}
		MouseCode(int button) : m_Type(static_cast<Type>(button)) {}

		operator Type() const { return m_Type; }
		operator const int() const { return static_cast<int>(m_Type); }

		// Get string name of the mouse code
		std::string ToString() const;


	private:
		Type m_Type;
	};


	enum class MouseAction : uint8_t
	{
		None = 0,
		Pressed,
		Released
	};

	// =======================================================================================
	// ------------------------------------- KEYBOARD ----------------------------------------
	// =======================================================================================

	struct KeyCode
	{
		/* Keycode Types */
		enum Type : uint16_t
		{
			Unknown		= 0,
			
			N0, N1, N2, N3, N4, N5, N6, N7, N8, N9,
			
			A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			
			Escape, Enter, Tab, Backspace, Insert, Delete,

			LeftShift,  LeftControl,  LeftAlt,  LeftSuper,
			RightShift, RightControl, RightAlt, RightSuper,
			
			Right, Left, Down, Up,
			
			PageUp, PageDown, Home, End, CapsLock, ScrollLock, NumLock, PrintScreen, Pause,
			
			F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, F13, F14,
			F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,

			KP0, KP1, KP2, KP3, KP4, KP5, KP6, KP7, KP8, KP9,
			KPDecimal, KPDivide, KPMultiply, KPSubtract, KPAdd, KPEnter, KPEqual,
			
			Space,				/*   */
			Apostrophe,			/* ' */
			Comma,				/* , */
			Minus,				/* - */
			Period,				/* . */
			Slash,				/* / */
			Semicolon,			/* ; */
			Equal,				/* = */
			LeftBracket,		/* [ */
			RightBracket,		/* \ */
			Backslash,			/* ] */
			GraveAccent,		/* ` */

			Menu
		};

		KeyCode(Type type) : m_Type(type) {}

		std::string ToString() const;

	private:
		Type m_Type;
	};


	enum class KeyAction : uint8_t
	{
		None = 0,
		Pressed,
		Released,
		Repeat
	};

	// =======================================================================================
	// -------------------------------------- GAMEPAD ----------------------------------------
	// =======================================================================================


	
	// =======================================================================================
	// ------------------------------------ MODIFIER KEYS ------------------------------------
	// =======================================================================================

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
		std::string ToString() const;
		
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
