#pragma once

namespace LarasEngine
{
	enum ObjectUpdateStatus
	{
		OK,
		ERROR,
		DELETE
	};

	enum GameStatus
	{
		STATUS_EXIT,
		STATUS_MENUE,
		STATUS_PAUSE,
		STATUS_MINI_PAUSE,
		STATUS_GAME
	};
}