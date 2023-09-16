#pragma once

namespace LarasEngine
{
	class Input
	{
	public:
		typedef Qt::Key Key;

		static Input& Me ();
		void SetKey (Input::Key key, bool active);
		bool TestKey (Input::Key key) const;
		inline void ResetPlayerInput () { playerInput.clear (); };

	private:
		std::map <Key, bool> playerInput;
	};
}

