һ��ctemplate���룺
1 ��src\htmlparser�����У�
python generate_fsm.py htmlparser_fsm.config > htmlparser_fsm.h
python generate_fsm.py jsparser_fsm.config > jsparser_fsm.h

����Ĳ�����2.3�汾�ﲻ��Ҫ��
2 �޸�template.hͷ�ļ���
�� namespace google_ctemplate_streamhtmlparser �޸�Ϊ ctemplate_htmlparser
3 �޸Ĺ������ã�
Unicode
4 find_ptr0:
��find_ptr.h�����ӣ�
template <class T, class U>
const typename T::value_type* find_ptr0(const T& c, U v)
{
	typename T::const_iterator i = c.find(v);
	return i == c.end() ? NULL : &*i;
}
5 �޸����ɿ���
