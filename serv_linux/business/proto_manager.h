#include<map>
#include <string>
using std::string;

class Proto;

class ProtoManager {
	public:
		ProtoManager();
		~ProtoManager();

		const Proto* GetProto(const string& strName);
		
	private:
		std::map<string, const Proto*> m_mapProto;

	private:
		void regist(Proto* ptrProto);
		void PrintAllProtocols();
};

extern ProtoManager g_protoManagerObj;
