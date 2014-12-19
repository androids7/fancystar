#ifndef __HEADER_WYC_XCONFIG
#define __HEADER_WYC_XCONFIG

#include <cassert>
#include <string>
#include <map>
#include "wyc/basedef.h"
#include "wyc/util/var.h"

namespace wyc
{

class xconfig
{
	typedef std::map<std::string,intptr_t> SECTION_GROUP;
	typedef std::pair<std::string,intptr_t> SECTION_NODE;
	typedef std::map<std::string,std::string> SECTION;
	typedef std::pair<std::string,std::string> KEYNODE;
	typedef SECTION::const_iterator section_iterator;
	enum TOKEN {
		TOKEN_ERROR=-1,
		TOKEN_ERR=-1,
		TOKEN_EOF=0,
		TOKEN_IDENTIFIER=1,
		TOKEN_INTEGER,
		TOKEN_FLOAT,
		TOKEN_STRING,
		TOKEN_USTRING,
		TOKEN_LEFT_BRACKET='(',
		TOKEN_RIGHT_BRACKET=')',
		TOKEN_NEW_LINE='\n',
		TOKEN_SPACE=' ',
		TOKEN_COMMA=',',
	};
	enum READ_TOKEN_STATE {
		TOKEN_READ_NULL=0,
		TOKEN_READ_IDENTIFIER,
		TOKEN_READ_NUMBER,
		TOKEN_READ_HEX_NUMBER,
		TOKEN_READ_FLOAT,
		TOKEN_READ_STRING,
		TOKEN_READ_USTRING,
	};
	enum SCRIPT_STATE {
		SC_EXPECT_IDENTIFIER,
		SC_EXPECT_BRACKET,
		SC_EXPECT_PARAMETER,
		SC_EXPECT_COMMA,
		SC_EXPECT_END,
	};
	std::string m_cfgname;
	SECTION_GROUP m_sections;
	mutable SECTION *m_pcurmap;
	mutable std::string m_mapname;
	bool m_bChanged;
public:
	xconfig();
	~xconfig();
	bool load(const char *filename);
	bool save(const char *filename=0);
	void clear();
	inline const std::string& get_name() const {
		return m_cfgname;
	}
	inline bool need_save() const {
		return m_bChanged;
	}
	const std::string& cur_section() const {
		return m_mapname;
	}
	// ���õ�ǰsection,���bAddΪtrue,����Զ�����
	bool set_cur_section(const std::string &secname, bool bAdd=false);
	// ��յ�ǰsection
	void clear_cur_section();
	// ɾ��section
	void remove_section(const std::string &secname);
	// ��ȡkey
	bool get_key(const std::string &key, std::string &value) const;
	// ����key,���û���ҵ�key���Զ����
	void set_key(const std::string &key, const std::string &value);
	// ɾ��key
	void del_key(const std::string &key);
	// ���ͽű�
	bool parse_script(const std::string &sc, uint32_t &pos, std::string &func, xargs &param) const;
	// ���ʵ�һ��section
	bool first_section();
	// ������һ��section
	bool next_section();
	// ������,������ǰsection�µ�����key
	inline section_iterator section_begin() const {
		assert(m_pcurmap);
		return m_pcurmap->begin();
	}
	inline section_iterator section_end() const {
		assert(m_pcurmap);
		return m_pcurmap->end();
	}
	// debug�ӿ�,��ӡ��Ϣ
#ifdef _DEBUG
	void print_script(const std::string &sc) const;
	void dump_config() const;
	void dump_script() const;
	void dump_function(const std::string &func, const xargs &param) const;
#endif
private:
	inline void clear_error(std::istream &is) const {
		is.clear(is.rdstate()&~(std::ios::failbit | std::ios::badbit));
	}
	inline void ignore_line(std::istream &is) const {
		is.ignore(100000,'\n');
	}
	// ȡ�﷨��Ԫ
	int get_token(std::istream &is) const;
	// ȡ��(���Կհ�)
	bool get_word(std::istream &is, std::string &word, char delim=0) const;
	// ȡ��(�����հ�)
	bool get_word_space(std::istream &is, std::string &word, char delim=0) const;
	// ������������
	bool parse_function(const char *exp, std::string &func, xargs &param) const;
	// �����﷨��Ԫ
	TOKEN parse_token(const char *exp, unsigned &pos, int &ival, float &fval, std::string &str) const;
	// ת���ַ�
	char translate_char(char c) const;
};

} // namespace wyc

#endif // end of __HEADER_WYC_XCONFIG
