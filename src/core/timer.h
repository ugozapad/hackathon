#ifndef TIMER_H
#define TIMER_H

namespace engine
{
	class Timer : public Singleton<Timer>
	{
	private:
		static Timer ms_timer;

	public:
		void init();
		void reset();

		void update();

		float getDelta();
		float getTime();

	private:
		LARGE_INTEGER m_startTime;
		LARGE_INTEGER m_timeTime;
		LARGE_INTEGER m_endTime;
		double m_freq;
		float m_deltaTime;
	};
}


#endif // !TIMER_H
