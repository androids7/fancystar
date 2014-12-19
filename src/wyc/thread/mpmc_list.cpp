#include "winpch.h"
#include "wyc/thread/mpmc_list.h"

namespace wyc
{

xmpmc_list::xmpmc_list() 
{
	m_head=0;
}

void xmpmc_list::push(xasync_node *n)
{
	xasync_node *next=m_head;
	n->m_next=next;
	while(InterlockedCompareExchangePointer((volatile PVOID*)&m_head, n, next)!=next) {
		next=m_head;
		n->m_next=next;
	}
}

xasync_node* xmpmc_list::pop() 
{
	xasync_node *head=m_head;
	while(head) 
	{
		// ��CAS֮ǰ,���head��ָ��Ľ�㱻�����߳�pop & push
		// ��head->m_next�����ѱ��޸�,��m_head==head��Ȼ����
		// ��ʱCAS�ɹ�,��m_head��õ�ȴ�Ǿɵ�m_nextֵ
		if(InterlockedCompareExchangePointer((volatile PVOID*)&m_head, head->m_next, head)==head) {
			return head;
		}
		head=m_head;
	}
	return 0;
}

xasync_node* xmpmc_list::pop(xhazard_local<xasync_node> *loc) 
{
	xasync_node *head=m_head;
	while(head) 
	{
		hazard_set(loc,head);
		if(head==m_head) {
			if(InterlockedCompareExchangePointer((volatile PVOID*)&m_head, head->m_next, head)==head) {
				break;
			}
		}
		head=m_head;
	}
	hazard_set(loc,0);
	return head;
}

xasync_node* xmpmc_list::flush() 
{
	xasync_node *head=m_head;
	while(head) {
		if(InterlockedCompareExchangePointer((volatile PVOID*)&m_head, 0, head)==head) {
			return head;
		}
		head=m_head;
	}
	return 0;
}

}; // namespace wyc

