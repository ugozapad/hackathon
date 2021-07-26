#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace engine
{
	class GameState : public Singleton<GameState>
	{
		static GameState ms_instance;
	public:
		enum GAME_STATE
		{
			GAME_STATE_RUNNING,
			GAME_STATE_MAIN_MENU,
			GAME_STATE_END,
		};

		void       setGameState(GAME_STATE state) { m_gameState = state; }
		GAME_STATE getGameState() { return m_gameState; }

	private:
		GAME_STATE m_gameState;
	};
}

#endif // !GAMESTATE_H
