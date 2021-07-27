#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace engine
{
	class Node;

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
		GAME_STATE getGameState()                 { return m_gameState; }

		void       setPlayerNode(Node* node)      { m_playerNode = node; }
		Node*      getPlayerNode()                { return m_playerNode; }

	private:
		GAME_STATE m_gameState;

		Node* m_playerNode;
	};
}

#endif // !GAMESTATE_H
