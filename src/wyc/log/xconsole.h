/**	@file Windows GUI������ҿ���̨
	@version 1.0.1
	@author ycwang
	@date 2010-07-17
*/
#ifndef __HEADER_WYC_XCONSOLE
#define __HEADER_WYC_XCONSOLE

#include <cassert>

namespace wyc
{

/** @brief ��ҿ���̨������Windows GUI���������������Ϣ��ִ�е���ָ��
	@details ÿ��Ӧ�ó���ֻ�ܴ���һ��ȫ�ֵ�xconsole����ͨ��singleton()���Ի�ȡ��ȫ�ֶ��������\n
	������̨������ʱ��ͬʱ�ᴴ��һ�������̣߳��ȴ��ʹ����û�ָ�Ŀǰ���õ�ָ���У�\n
	- <i>help</i>: ��ӡ������Ϣ
	- <i>print [string]</i>: ��ӡһ���ַ���
	- <i>hash [string]</i>: ����ַ�����Hashֵ�������Ƶ�������
	- <i>hide</i>:	���ؿ���̨
	- <i>cls</i>:	����
	- <i>exit</i>:	�˳�����̨
*/
class xconsole
{
	HWND m_hWnd;
	HANDLE m_hOut, m_hInput;
	DWORD m_curFlag;
	static xconsole *ms_pConsole;
public:
	/**	@brief ����ȫ�ֿ���̨������
		@details �������̨û�д����������׳��쳣
	*/
	static xconsole& singleton();
	/// ����̨�Ƿ����
	static bool has_console();
	/**	@brief ��������̨
		@param window_width ���ڵĿ�ȣ���Ϊ0��ʾʹ��Ĭ�Ͽ��
		@param window_height ���ڵĸ߶ȣ���Ϊ0��ʾʹ��Ĭ�Ͽ��
		@param redirect_stdio �Ƿ��ض���C++��׼���������
		@return �ɹ�����true��ʧ�ܷ���false
	*/
	static bool create_console(unsigned window_width=0, unsigned window_height=0, bool redirect_stdio=true);
	/// ɾ������̨
	static void free_console();

	//==========================================

	/**	@brief ���ÿ���̨��λ�úʹ�С
		@param x ����x����
		@param y ����y����
		@param w ���ڿ��
		@param h ���ڸ߶�
	*/
	void set_posize(int x, int y, int w, int h);
	/// ��ʾ����̨
	void show();
	/// ���ؿ���̨
	void hide();
	/// ȡ���ھ��
	HWND get_window() const;
	/**	@brief ���ÿ���̨�����ʽ
		@param flag ��ʽ������ȡֵ���£�
			- 0 �û����
			- 1 ϵͳ���
			- 2 ����
			- 3 ����
	*/
	void set_flag(unsigned flag);
	/// �ڿ���̨�ϴ�ӡһ����Ϣ
	unsigned write(const char *pstr, unsigned size);
	unsigned write(const wchar_t *pwstr, unsigned size);
private:
	xconsole();
	xconsole(const xconsole &console);
	xconsole& operator = (const xconsole &console);
	~xconsole();
};

#define XCONSOLE_VERSION "1.0.1"

inline xconsole& xconsole::singleton() {
	assert(ms_pConsole || !"wyc_console has not been created");
	return *ms_pConsole;
}

inline bool xconsole::has_console() {
	return ms_pConsole!=0;
}

inline HWND xconsole::get_window() const
{
	return ms_pConsole->m_hWnd;
}

inline void xconsole::show() {
	if(m_hWnd==NULL) return;
	::ShowWindow(m_hWnd,SW_SHOWNOACTIVATE);
}

inline void xconsole::hide() {
	if(m_hWnd==NULL) return;
	::ShowWindow(m_hWnd,SW_HIDE);
}

inline unsigned xconsole::write(const char *pstr, unsigned size)
{
	DWORD ret;
	::WriteConsoleA(m_hOut,pstr,size,&ret,0);
	return ret;
}

inline unsigned xconsole::write(const wchar_t *pwstr, unsigned size)
{
	DWORD ret;
	::WriteConsoleW(m_hOut,pwstr,size,&ret,0);
	return ret;
}


} // namespace wyc

#endif // end of __HEADER_WYC_XCONSOLE


