#ifndef __HEADER_WYC_INPUT_WIN
#define __HEADER_WYC_INPUT_WIN

#include "wyc/basedef.h"
#include "wyc/thread/thread.h"
#include "wyc/game/input.h"

namespace wyc {

/**************************************************************
  �̰߳�ȫ�ԣ�д��������̰߳�ȫ�ģ��������߳�ͬʱ����Ϣд��
  ���У�����ȡ�������ǣ�����ȡ��Ϣ���е�ʱ�򣬶����е����ݿ�
  ��������д��ͨ������£�ֻ��һ���߳��������update_input()��
  get_buffer()����ȡ�����е���Ϣ
**************************************************************/

class xinput
{
protected:
	xcritical_section m_inputLock;
	uint8_t m_widx;
	xinput_buffer m_msgbuff[2];
	int16_t m_lastx, m_lasty;
public:
	xinput();
	// ������Ϣ����
	void reset();
	// ���ͼ���������Ϣ(�����߳�)
	void key_event(int32_t msg, uint32_t key);
	// �������������Ϣ(�����߳�)
	void mouse_button(int32_t msg, int16_t x, int16_t y, uint32_t button);
	void mouse_move(int16_t x, int16_t y);
	void mouse_wheel(int16_t z);
	// �����ַ�������Ϣ(�����߳�)
	void add_character(wchar_t wc);
	// ����/��ȡ��Ϣ����(��ȡ�߳�)
	void update_input();
	const xinput_buffer& get_buffer() const;
	// �����Ϣ�����е�����(��ȡ�߳�)
	void trace_keyque(const xkeyque& msgque) const;
	void trace_mouseque(const xmouseque& msgque) const;
};

inline const xinput_buffer& xinput::get_buffer() const
{
	return m_msgbuff[1-m_widx];
}

}; // namespace wyc

#endif // end of __HEADER_WYC_INPUT_WIN

