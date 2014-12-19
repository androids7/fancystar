#ifndef __HEADER_WYC_MOVE
#define __HEADER_WYC_MOVE

#include "collision_detector.h"
#include "recorder.h"

namespace wyc
{

class xmove
{
	xcollision_agent *m_agent;
	xcollision_agent *m_terrain;
	float m_inflections[3];
	unsigned char m_cursec, m_cntsec;
	char m_apex;
	std::vector<float> m_kpos;
public:
	// ������ײ��
	static unsigned ms_terrain_filter;
	// ƽ̨��ײ��
	static unsigned ms_platform_filter;
	// ��վ����ײ��
	static unsigned ms_standon_filter;
	// �������ϰ���ľ���,�����򸡵����´���
	static float ms_drawback;
	// С�ڸø߶ȵ�ƽ̨��Ϊ����ֱ�ӵ��ϵ�̨��
	static float ms_stair_height;
	// ֻ����б�ȵ��ڸ�ֵ��б�²�������
	// tan(30��)=0.577; tan(45��)=1; tan(60��)=1.732
	static float ms_slope_can_stand;
	// ���߹���, Ĭ��88�ȡ�92��
	// ���ڶԿ���ƽ̨����ײ���з��߹��ˣ������ײ�����ڸ������ڣ�����Ϊ������ײ
	static xvec2f_t ms_min_dir;
	static xvec2f_t ms_max_dir;
	// ��¼��������, ����DEBUG
	static xrecorder ms_rec;

	static void init_move_env(unsigned terrain_filter, unsigned platform_filter);
		
	xmove ();
	inline void set_agent(xcollision_agent *agent) {
		m_agent = agent;
	}
	inline xcollision_agent* get_agent() {
		return m_agent;
	}
	inline bool is_on_ground() const {
		return m_terrain!=0;
	}
	bool stick_on_ground ();
	void set_terrain (xcollision_agent *terrain);
	xcollision_agent* get_terrain() const {
		return m_terrain;
	}
	inline void leave_terrain () {
		m_terrain = 0;
	}
	const std::vector<float>& get_kpos() const {
		return m_kpos;
	}
	void clear_kpos() {
		m_kpos.clear();
	}
	const wyc::xrecorder& get_record() const {
		return ms_rec;
	}
	xcollision_agent* walk (float &t, float spdx, unsigned extra_filter=0, unsigned max_iteration=3);
	xcollision_agent* fall (float &t, xvec2f_t &speed, float accy, unsigned extra_filter=0, unsigned max_iteration=3);
	xcollision_agent* jump (float &t, xvec2f_t &speed, float accy, unsigned extra_filter=0, unsigned max_iteration=3);
	xcollision_agent* fly  (float &t, const xvec2f_t speed, unsigned extra_filter=0);

protected:
	xcollision_agent* _detect_collision (const xvec2f_t& offset, unsigned filter, float &dt, xvec2f_t &normal, unsigned normal_filter=0);
	xcollision_agent* _detect_stand_point (xvec2f_t &pos);
}; // class xmove

}// namespace wyc

#endif // __HEADER_WYC_MOVE