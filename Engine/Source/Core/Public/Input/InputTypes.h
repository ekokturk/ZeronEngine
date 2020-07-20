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
			Unknown	= 0,
			LeftButton,
			RightButton,
			MiddleButton,
			Button3,
			Button4,
			Button5,
			Button6,
			Button7,
			AxisX,
			AxisY,
			AxisScroll
		};

		MouseCode() : m_Type(Unknown) {}
		MouseCode(Type button) : m_Type(button) {}
		MouseCode(int button) : m_Type(static_cast<Type>(button)) {}
		MouseCode(const MouseCode& other) = default;

		operator Type() const { return m_Type; }
		operator const int() const { return static_cast<int>(m_Type); }
		operator const bool() const { return m_Type != Unknown; }
		bool operator ==(const MouseCode& other) const { return m_Type == other.m_Type; }
		bool operator !=(const MouseCode& other) const { return m_Type != other.m_Type; }
		bool operator ==(const Type& other) const { return m_Type == other; }
		bool operator !=(const Type& other) const { return m_Type != other; }

		
		MouseCode& operator =(const MouseCode& other) = default;
		
		// Get string name of the mouse code
		std::string ToString() const;


	private:
		Type m_Type;
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

		KeyCode() : m_Type(Type::Unknown){}
		KeyCode(Type type) : m_Type(type) {}
		KeyCode(int button) : m_Type(static_cast<Type>(button)) {}
		KeyCode(const KeyCode& other) = default;
		
		operator Type() const { return m_Type; }
		operator const int() const { return static_cast<int>(m_Type); }
		operator const bool() const { return m_Type != Unknown; }
		bool operator ==(const KeyCode& other) const { return m_Type == other.m_Type; }
		bool operator !=(const KeyCode& other) const { return m_Type != other.m_Type; }
		bool operator ==(const Type& other) const { return m_Type == other; }
		bool operator !=(const Type& other) const { return m_Type != other; }
		
		KeyCode& operator =(const KeyCode& other) = default;
		
		std::string ToString() const;

	private:
		Type m_Type;
	};




	// =======================================================================================
	// -------------------------------------- GAMEPAD ----------------------------------------
	// =======================================================================================


	
	// =======================================================================================
	// ------------------------------------ OTHER --------------------------------------------
	// =======================================================================================

	/*
	 * State of the input 
	 */
	enum class InputState : uint8_t
	{
		None = 0,
		Released,
		Pressed,
	};
	
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
		bool operator ==(const ModifierKeys& other) const  { return m_Type == other.m_Type; }
		bool operator !=(const ModifierKeys& other) const	{ return m_Type != other.m_Type; }
		
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


	// =======================================================================================
	// --------------------------------------- BINDINGS --------------------------------------
	// =======================================================================================

	// Type safe variable for all input types
	using InputType = std::variant < MouseCode, KeyCode >;
	
	struct InputActionBinding
	{
		InputActionBinding():Code(InputType()){}
		InputActionBinding(InputType inputCode, const ModifierKeys& modifiers)
			:Code(std::move(inputCode)), Modifiers(modifiers){}
		InputActionBinding(const InputActionBinding& other) = default;

		constexpr bool operator ==(const InputActionBinding& other) const
		{
			return Code == other.Code && Modifiers == other.Modifiers;
		}
		
		InputActionBinding& operator =(const InputActionBinding& other) = default;
		
		InputType Code;
		ModifierKeys Modifiers;
	};

	struct InputPollBinding
	{
		InputPollBinding() :Code(InputType()),Scale(1.f) {}
		InputPollBinding(InputType inputCode, const ModifierKeys& modifiers)
			:Code(std::move(inputCode)), Modifiers(modifiers), Scale(1.f) {}
		InputPollBinding(const InputPollBinding& other) = default;

		
		InputPollBinding& operator =(const InputPollBinding& other) = default;

		constexpr bool operator ==(const InputPollBinding& other) const
		{
			return Code == other.Code && Modifiers == other.Modifiers && Scale == other.Scale;
		}

		InputType Code;
		ModifierKeys Modifiers;
		float Scale;
	};

	// Type safe variable for all input binding
	using InputBinding = std::variant<InputActionBinding, InputPollBinding>;

}

// Custom hashing for Input types so they can be used as keys
namespace std
{
	template <>
	struct hash<ZeronEngine::KeyCode>
	{
		std::size_t operator()(const ZeronEngine::KeyCode& k) const
		{
			return std::hash<ZeronEngine::KeyCode::Type>()(k);
		}
	};

	template <>
	struct hash<ZeronEngine::MouseCode>
	{
		std::size_t operator()(const ZeronEngine::MouseCode& k) const
		{
			return std::hash<ZeronEngine::MouseCode::Type>()(k);
		}
	};


	template <>
	struct hash<ZeronEngine::ModifierKeys>
	{
		std::size_t operator()(const ZeronEngine::ModifierKeys& k) const
		{
			return std::hash<int>()(k);
		}
	};

	template <>
	struct hash<ZeronEngine::InputActionBinding>
	{
		std::size_t operator()(const ZeronEngine::InputActionBinding& k) const
		{
			return std::hash<ZeronEngine::InputType >()(k.Code) ^
			hash<ZeronEngine::ModifierKeys>()(k.Modifiers);
		}
	};

	template <>
	struct hash<ZeronEngine::InputPollBinding>
	{
		std::size_t operator()(const ZeronEngine::InputPollBinding& k) const
		{
			return std::hash<ZeronEngine::InputType >()(k.Code) ^
						hash<ZeronEngine::ModifierKeys>()(k.Modifiers);
		}
	};
}
