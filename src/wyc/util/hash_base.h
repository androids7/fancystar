#ifndef __HEADER_WYC_XHASH_BASE
#define __HEADER_WYC_XHASH_BASE

#include <cstdlib>
#include <cassert>
#include <iterator>

/*******************************************************************************
* ע�⣺
* ���赱ǰ��������ݣ�n
* ��ɾ�������ݣ�d
* ����Ϊ��c
* ����������ֵ��g
* ����������ֵ��s
* ����c��2����ʽ��������n+d>=cg��ʱ������������2c��
* Ȼ��rehash��d=0����ʱ���n<=2cs��
* ��ôһ��ɾ���������ֻᵼ������������rehash
* �����״�����ٽ�ֵΪ��
* (1) n+d>=cg
* (2) n<=2cs
* �ɷ������ã�
* c=d/(g-2s)
* ����g��c�Ĺ�ϵӦ�����㣺g=2s����ʱcΪ���޴���Զ��������������
*******************************************************************************/

// defaut initia table size
#define HASH_TABLE_DEF_SIZE	16
// minimun table size
#define HASH_TABLE_MIN_SIZE	8
// percentage of capacity to shrink table size and rehash
#define HT_SHRINK_PERCENT 0.4f
// percentage of capacity to grow table size and rehash, should be 2*HT_SHRINK_PERCENT
#define HT_GROW_PERCENT 0.8f

// invalid position
#define HT_NULL_POS size_t(-1)

// default key for empty and deleted entry
#define HT_EMPTY_POINTER ((void*)0xFFFFFFFE)
#define HT_DUMMY_POINTER ((void*)0xFFFFFFFF)

namespace wyc
{

template<typename key_type, typename value_type>
struct hasher_template
{
	/// ����hashֵ
	static inline size_t hash (key_type key);
	/// ��ȡkey
	static inline key_type get_key (const value_type &item);
	/// item�Ƿ���Ϊ��
	static inline bool is_empty (const value_type &item);
	/// item�Ƿ���Ϊɾ��
	static inline bool is_deleted (const value_type &item);
	/// ��item���Ϊ��
	static inline void set_empty (value_type &item);
	/// ��item���Ϊɾ��
	static inline void set_deleted (value_type &item);
};

//**************************************************************************************************
// ע�⣺
// [1] Key��Value���͵ĸ��Ʋ���Ӧ���ǵ����ĵ�
// [2] get_key��set_deleted��������empty��deletedԪ������Ч�ģ�����ȷ��is_empty��is_deleted��Ϊfalse���ܵ���
//**************************************************************************************************
template<typename Key, typename Value, typename Hasher>
class xhashtable
{
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef Hasher hasher_type;
	class const_iterator : public std::iterator<std::forward_iterator_tag,value_type>
	{
	protected:
		xhashtable *m_ptable;
		value_type *m_pos, *m_end;
	public:
		const_iterator(const xhashtable *ptab=0, const value_type *pos=0, const value_type *end=0) {
			m_ptable=const_cast<xhashtable*>(ptab);
			m_pos=const_cast<value_type*>(pos);
			m_end=const_cast<value_type*>(end);
			if(pos!=end) advance();
		}
		const_iterator(const const_iterator &iter) {
			m_ptable=iter.m_ptable;
			m_pos=iter.m_pos;
			m_end=iter.m_end;
		}
		const_iterator& operator = (const const_iterator &iter) {
			if(this!=&iter) {
				m_ptable=iter.m_ptable;
				m_pos=iter.m_pos;
				m_end=iter.m_end;
			}
			return *this;
		}
		const value_type& operator * () const {
			return *m_pos;
		}
		const value_type* operator ->() const {
			return m_pos;
		}
		const_iterator& operator ++ () {
			assert(m_pos!=m_end);
			++m_pos;
			advance();
			return *this;
		}
		const_iterator operator ++ (int) {
			const_iterator itmp=*this; 
			++(*this);
			return itmp;
		}
		bool operator == (const const_iterator &iter) const {
			return m_pos==iter.m_pos;
		}
		bool operator != (const const_iterator &iter) const {
			return m_pos!=iter.m_pos;
		}
	private:
		void advance() {
			while(m_pos!=m_end&&(Hasher::is_empty(*m_pos)||Hasher::is_deleted(*m_pos))) 
				++m_pos;
		}
	};
	class iterator : public const_iterator
	{
	public:
		iterator(xhashtable *ptab=0, value_type *pos=0, value_type *end=0) :
			const_iterator(ptab,pos,end) {
		}
		value_type& operator * () const {
			return *m_pos;
		}
		value_type* operator ->() const {
			return m_pos;
		}
	};
	xhashtable(size_t size=0)
	{
		if(size>0) {
			size=min_size(size);
			assert((size&(size-1))==0);
			m_capacity=size;
			m_ptable=new_table(m_capacity);
		}
		else {
			m_capacity=0;
			m_ptable=0;
		}
		m_element=m_deleted=0;
		m_may_shrink=false;
		m_auto_shrink=false;
	}
	~xhashtable()
	{
		if(m_ptable) {
			free(m_ptable);
			m_ptable=0;
		}
	}
	void reserve(size_t size) 
	{
		size=min_size(size);
		assert((size&(size-1))==0);
		if(size<=m_capacity)
			return;
		if(m_ptable) {
			rehash(size);
		}
		else {
			m_capacity=size;
			m_ptable=new_table(m_capacity);
		}
	}
	size_t find(const key_type &key) const
	{
		if(!m_ptable)
			return HT_NULL_POS;
		size_t mask=m_capacity-1, prob=0;
		size_t pos=Hasher::hash(key)&mask;
		while(!Hasher::is_empty(m_ptable[pos])) {
			if(!Hasher::is_deleted(m_ptable[pos]) && Hasher::get_key(m_ptable[pos])==key)
				return pos;
			prob+=1;
			pos=(pos+prob)&mask;
			assert(prob<m_capacity);
		}
		return HT_NULL_POS;
	}
	bool get(const key_type &key, value_type &value) const
	{
		size_t pos=find(key);
		if(HT_NULL_POS==pos)
			return false;
		value=m_ptable[pos];
		return true;
	}
	bool set(const key_type &key, const value_type &value) {
		size_t pos=find(key);
		if(HT_NULL_POS==pos)
			return false;
		m_ptable[pos]=value;
		return true;
	}
	bool add(const key_type &key, const value_type &value)
	{
		if(m_ptable) {
			resize(1);
		}
		else {
			reserve(HASH_TABLE_DEF_SIZE);
			assert(m_ptable);
		}
		size_t pos;
		return insert_no_resize(key,value,pos);
	}
	bool del(const key_type &key)
	{
		size_t pos=find(key);
		if(HT_NULL_POS==pos)
			return false;
		Hasher::set_deleted(m_ptable[pos]);
		++m_deleted;
		m_may_shrink=true;
		return true;
	}
	bool pop(const key_type &key, value_type &value)
	{
		size_t pos=find(key);
		if(HT_NULL_POS==pos)
			return false;
		value=m_ptable[pos];
		Hasher::set_deleted(m_ptable[pos]);
		++m_deleted;
		m_may_shrink=true;
		return true;
	}
	size_t set_default (const key_type &key, const value_type &value) {
		if(m_ptable) {
			resize(1);
		}
		else {
			reserve(HASH_TABLE_DEF_SIZE);
			assert(m_ptable);
		}
		size_t pos;
		insert_no_resize(key,value,pos);
		return pos;
	}
	void clear()
	{
		if(!m_ptable) return;
		if(m_auto_shrink) {
			m_ptable=(value_type*)realloc(m_ptable,sizeof(value_type)*HASH_TABLE_MIN_SIZE);
			assert(m_ptable);
			m_capacity=HASH_TABLE_MIN_SIZE;
		}
		set_range_empty(m_ptable,m_ptable+m_capacity);
		m_element=0;
		m_deleted=0;
		m_may_shrink=false;
	}
	inline bool contain(const key_type &key) const {
		size_t pos=find(key);
		return HT_NULL_POS!=pos;
	}
	inline size_t size() const {
		return m_element-m_deleted;
	}
	inline size_t capacity() const {
		return m_capacity;
	}
	inline size_t num_deleted() const {
		return m_deleted;
	}
	inline size_t num_element() const {
		return m_element;
	}
	inline const value_type& operator[] (size_t pos) const {
		assert(m_ptable);
		return m_ptable[pos];
	}
	inline value_type& operator[] (size_t pos) {
		assert(m_ptable);
		return m_ptable[pos];
	}
	inline iterator begin() {
		return size()>0?iterator(this,m_ptable,m_ptable+m_capacity):end();
	}
	inline iterator end() {
		return iterator(this,m_ptable+m_capacity,m_ptable+m_capacity);
	}
	inline const_iterator begin() const {
		return size()>0?const_iterator(this,m_ptable,m_ptable+m_capacity):end();
	}
	inline const_iterator end() const {
		return const_iterator(this,m_ptable+m_capacity,m_ptable+m_capacity);
	}
	inline void set_auto_shrink(bool b) {
		m_auto_shrink=b;
	}
	inline bool is_auto_shrink() const {
		return m_auto_shrink;
	}
private:
	xhashtable(const xhashtable &ht);
	xhashtable& operator = (const xhashtable &ht);
	value_type* new_table(size_t sz)
	{
		value_type* ptable=(value_type*)::malloc(sizeof(value_type)*sz);
		set_range_empty(ptable,ptable+sz);
		return ptable;
	}
	size_t min_size(size_t num_element)
	{
		size_t cap=HASH_TABLE_MIN_SIZE;
		while(num_element>=cap*HT_GROW_PERCENT) {
			cap<<=1;
		}
		return cap;
	}
	void resize(size_t delta)
	{
		if(m_auto_shrink && m_may_shrink) 
			check_shrink();
		if(m_element+delta<=m_capacity*HT_GROW_PERCENT) 
			return;
		size_t cap=min_size(m_element-m_deleted+delta);
		rehash(cap);
	}
	void check_shrink()
	{
		assert(m_element>=m_deleted);
		size_t sz=m_element-m_deleted;
		if(m_capacity>HASH_TABLE_MIN_SIZE && sz<m_capacity*HT_SHRINK_PERCENT) {
			size_t cap=m_capacity>>1;
			while(sz<cap*HT_SHRINK_PERCENT) 
				cap>>=1;
			if(cap<HASH_TABLE_MIN_SIZE)
				cap=HASH_TABLE_MIN_SIZE;
			rehash(cap);
		}
		else m_may_shrink=false;
	}
	void rehash(size_t sz)
	{
		assert((sz&(sz-1))==0);
		assert(m_element>=m_deleted);
		assert(sz>m_element-m_deleted);
		value_type *ptable=new_table(sz);
		assert(ptable);
		std::swap(m_ptable,ptable);
		std::swap(m_capacity,sz);
		m_element=0;
		size_t pos;
		for(size_t i=0; i<sz; ++i)
		{
			if(!Hasher::is_empty(ptable[i]) && !Hasher::is_deleted(ptable[i])) {
				insert_no_resize(Hasher::get_key(ptable[i]),ptable[i],pos);
			}
		}
		m_deleted=0;
		m_may_shrink=false;
		free(ptable);
	}
	bool insert_no_resize(const key_type &key, const value_type &value, size_t &insert_pos)
	{
		size_t mask=m_capacity-1, prob=0;
		size_t pos=Hasher::hash(key)&mask;
		insert_pos=HT_NULL_POS;
		while(!Hasher::is_empty(m_ptable[pos])) {
			if(Hasher::is_deleted(m_ptable[pos])) {
				if(insert_pos==HT_NULL_POS)
					insert_pos=pos;
			}
			else if(Hasher::get_key(m_ptable[pos])==key) {
				insert_pos=pos;
				return false;
			}
			prob+=1;
			pos=(pos+prob)&mask;
			assert(prob<m_capacity);
		}
		if(insert_pos==HT_NULL_POS) {
			insert_pos=pos;
			++m_element;
		}
		else --m_deleted;
		m_ptable[insert_pos]=value;
		return true;
	}
	void set_range_empty(value_type *beg, value_type *end)
	{
		while(beg!=end) 
			Hasher::set_empty(*beg++);
	}
private:
	value_type *m_ptable;
	size_t m_capacity;
	size_t m_element, m_deleted;
	bool m_may_shrink;
	bool m_auto_shrink;
};


} // namespace wyc

#endif // end of __HEADER_WYC_XHASH_BASE
