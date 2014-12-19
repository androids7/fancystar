#ifndef __HEADER_WYC_XFRAMETIME
#define __HEADER_WYC_XFRAMETIME

#include "wyc/util/time.h"

namespace wyc {

class xframe_timer;

//
// ֡ʱ�ӣ�����ȡϵͳʱ�䣬������֪ͨ����ʱ��
//
class xframe_clock
{
	double m_current_time;		// ��ǰϵͳʱ��
	double m_duration;			// ʱ����
	double m_elapsed_time;		// �ۼƾ�����ʱ��
	typedef std::list<xframe_timer*> timer_list_t;
	timer_list_t m_timers;		// ��ʱ���б�
public:
    xframe_clock();
	void reset(double time_delta=0);
	void tick();
	void add_timer(xframe_timer *timer);
	double time() const;
	double duration() const;
	double elapsed_time() const;
};

inline void xframe_clock::add_timer(xframe_timer *timer)
{
	m_timers.push_back(timer);
}

inline double xframe_clock::time() const
{ 
	return m_current_time; 
}

inline double xframe_clock::elapsed_time() const
{
	return m_elapsed_time;
}

inline double xframe_clock::duration() const
{
	return m_duration;
}

///////////////////////////////////////////////////////////////////////////

//
// ֡��ʱ��
//
class xframe_timer
{
	friend class xframe_clock;
	double	m_elapsed_time;
	bool	m_paused;
public:
	xframe_timer() 
	{
		m_elapsed_time=0;
		m_paused=false;
	}
	inline double elapsed_time() const 
	{ 
		return m_elapsed_time; 
	}
	inline void reset()
	{
		m_elapsed_time=0;
	}
	inline void consume(double t) 
	{
		m_elapsed_time-=t;
	}
	inline void pause(bool b)
	{
		m_paused=b;
	}
	inline bool is_paused() const
	{
		return m_paused;
	}
};

} // namespace wyc

#endif // end of __HEADER_WYC_XFRAMETIME

