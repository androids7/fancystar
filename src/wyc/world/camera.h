#ifndef __HEADER_WYC_XCAMERA
#define __HEADER_WYC_XCAMERA

#include "wyc/math/vecmath.h"
#include "wyc/world/gameobj.h"

namespace wyc
{

class xcamera : public xgameobj
{
	USE_RTTI;
	xmat4f_t m_mat_projection;
	xmat4f_t m_mat_mvp;
public:
	xcamera();
	/// ��������ͶӰ����
	void set_orthograph(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);
	/**	@brief ����͸�Ӳ���
		@param yfov ��ֱ��׶��
		@param aspect ��߱�
		@param fnear ���ü�����ԭ��ľ��룬�������0
		@param ffar Զ�ü�����ԭ��ľ��룬�������fnear
	*/
	void set_perspective(float yfov, float aspect, float fnear, float ffar);
	/// �����������
	void look_at(const xvec3f_t &target, const xvec3f_t &up);
	/// ȡ�������
	const xmat4f_t& get_projection() const;
	const xmat4f_t& get_mvp_matrix() const;
	/// ����UI�任
	void set_ui_view(float screen_width, float screen_height, float z_range=256);
};

inline void xcamera::look_at(const xvec3f_t &target, const xvec3f_t &up)
{
	xvec3f_t forward=m_transform->position()-target;
	m_transform->set_forward(forward,up);
}

inline const xmat4f_t& xcamera::get_projection() const
{
	return m_mat_projection;
}

inline const xmat4f_t& xcamera::get_mvp_matrix() const
{
	return m_mat_mvp;
}


}; // namespace wyc

#endif // end of __HEADER_WYC_XCAMERA

