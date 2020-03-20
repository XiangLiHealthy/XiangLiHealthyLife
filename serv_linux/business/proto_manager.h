#include<map>
class Proto;

class ProtoManager {
	public:
		ProtoManager();
		~ProtoManager();

		const Proto* GetProto(const char* szName);
		
	private:
		std::map<const char*, const Proto*> m_mapProto;

	private:
		void regist(Proto* ptrProto);
};

extern ProtoManager g_protoManagerObj;
