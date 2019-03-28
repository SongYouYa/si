一、ctemplate编译：
1 在src\htmlparser下运行：
python generate_fsm.py htmlparser_fsm.config > htmlparser_fsm.h
python generate_fsm.py jsparser_fsm.config > jsparser_fsm.h

下面的操作在2.3版本里不需要了
2 修改template.h头文件：
把 namespace google_ctemplate_streamhtmlparser 修改为 ctemplate_htmlparser
3 修改工程配置：
Unicode
4 find_ptr0:
在find_ptr.h中增加：
template <class T, class U>
const typename T::value_type* find_ptr0(const T& c, U v)
{
	typename T::const_iterator i = c.find(v);
	return i == c.end() ? NULL : &*i;
}
5 修改生成库名
