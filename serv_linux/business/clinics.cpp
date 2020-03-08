#include"clinics.h"
#include<string.h>
#include<pthread.h>
#include"coder.h"
#include"../DataBase/DataBase.h"
#include "../log/log.h"

Clinics Clinics::m_clinics;
/**********************************************
 *这里指定协议的名称为类名
 *
 * *******************************************/ 
Clinics::Clinics() {
	strcpy(m_szName, "Clinics");
}

Clinics::~Clinics() {

}

/********************************************
 *获取静态对象的引用,注册到协议管理器;每个协议 
 *只实例化一个对象
 * ******************************************/ 
const Proto* Clinics::getInstance() {
	return &m_clinics;
	}

/********************************************
 *功能:获取协议的名称
 *
 * ******************************************/ 
const char* Clinics::getName() {
	return m_szName;
}

/************************************************
 *功能:将诊断协议解析成结构体,并完成业务功能
 *参数:Json jData:客户端传过来的协议数据
 *返回值:返回一个json,这个json包装好要发送的数据
 *       调用函数需要释放返回json对象的内存资源
 * ********************************************/
int Clinics::dispatch(const Json::Value& jData, Handle fd)
//Json::Value Clinics::dispatch(const Json::Value jData) 
{
	Json::Value jRet;

	//解析出treatment_t
	treatment_t treatment;
	Coder coder;
	if(coder.DecodeTreatment(jData, treatment) < 0) {
		jRet["code"] = -1;
		jRet["error"]	= coder.GetLastError();
		return -1;
	}

	//使用合适的方法更细致的业务
	DataBase* pDB = nullptr;
	const char* method = treatment.protocol.c_str();
	if(0 == strcmp("GetSymptom", method) ) {
		jRet = GetSymptom(treatment,pDB);
	}
	else if(0 == strcmp("GetCause", method)) {
		jRet = GetCause(treatment, pDB);
	}
	else if (0 == strcmp("GetDiagnosis", method)) {
		jRet = GetDiagnosis(treatment, pDB);
	}
	else if(0 == strcmp("GetSolution", method)) {
		jRet = GetSolution(treatment, pDB);
	}
	else if(0 == strcmp("Finish", method)) {
		jRet = Finish(treatment, pDB);
	}
	else{
		jRet["code"] = -1;
		string error 	= "不支持的协议:" +treatment.protocol;
		jRet["error"]	= error;
	}
	
	return 0;
}

/****************************************************************************************
 *功能: 将json数据格式解析成symotom结构体,然后从数据库获取对应的数据;按照算法进行统计,然后 
 *		将数据包装成json返回;json编码解码使用专门的类来代理处理
 *参数: jData:携带请求数据的json对象,用引用可以减少内存的拷贝 
 *
 * **************************************************************************************/ 
Json::Value Clinics::GetSymptom(treatment_t treatment, DataBase* pDB) {
	pthread_t pid;
	LOG_DEBUG("get symptom");

	Json::Value jRet;

	if(treatment.symptom.detail.size() < 1	&& treatment.symptom.elements.size() < 1) {
		jRet["code"] = -1;
		jRet["error"] = "空的症状描述";
		return jRet;
	}

	/*1,词法拆分得到描述表*/
	std::vector<string> details = SplitDetail(treatment.symptom.detail);
	

	/*sql 根据描述表获取diagnosis_element_id */
	std::vector<LONG> element_ids = GetSymptomID(details, pDB);


	/*合并element_id*/
	vector<LONG> merge_ids = element_ids;
	for(int i = 0; i < treatment.symptom.elements.size(); i++ ) {
		merge_ids.push_back(treatment.symptom.elements[i].diagnosis_element_id);
	}

	/*sql 从diagnosis表获取更多相关描述id*/
	std::vector<symptom_t> symptoms = GetSymptomContent(merge_ids, pDB);
	
	/*sql 标记出来自于拆分的描述*/
	for(int i = 0 ; i < element_ids.size(); i++) {
		int count = symptoms.size();
		for(int j = 0; j < count; j++) {
			if(element_ids[i] == symptoms[j].diagnosis_element_id) {
				symptoms[j].type = 1;//主要类型
			}
		}
	}
	


	/*把结果编码成json*/
	treatment.symptom.elements = symptoms;

	Json::Value jSymptom;
	Coder coder;

	treatment.Clear();
	treatment.protocol = "GetSymptom";
	coder.EncodeTreatment(treatment, jSymptom);

	jRet["ret"]  	= 0;
	jRet["data"]	= jSymptom;
	return jRet;
}

Json::Value Clinics::GetCause(treatment_t treatment, DataBase* pDB) {
	LOG_DEBUG("get cause");

	Json::Value jRet;
	/*1.将原因自定义描述进行词法拆分*/

	/*2.从diagnosis_element_id获取对应的id*/

	/*3.将新获取的id合并起来*/

	/*4. 从diagnosis+diagnosis_element_id连表查询diagnosis_element_id,content,feedback*/
	vector<cause_t> causes = GetCauseContent(treatment,pDB);

	/*考虑把自定义拆分的id标记出来,这个算是原创描述,值得特殊考虑*/
	/*从数据库获取记录症状描述和统计结果*/
	treatment.Clear();
	treatment.protocol 		= "GetCause";
	treatment.cause.elements 	= causes;
	
	
	/*将数据包装成json*/
	Coder coder;
	Json::Value jCause;
	coder.EncodeTreatment(treatment, jCause);

	jRet["data"] 	= jCause;
	jRet["code"]	= 0;
	
	return jRet;
	

}

Json::Value Clinics::GetDiagnosis(treatment_t treatment, DataBase* pDB) {
	LOG_DEBUG("get diagnosis");

	Json::Value jRet;
	/*使用词法拆分将自定义描述拆分*/

	/*从diagnosis_elements表获取id*/

	/*把获取的id合并到diagnosis,自定的id应该特别标记出来,需要赋予不同的额权重*/
	/*合并id到diagnosi*/
	

	/*从数据库获取记录症状描述和统计结果*/
	vector<diagnosis_t> diagnosises = GetDiagnosisContent(treatment,pDB);
	
	/*将数据包装成json*/
	Json::Value jDiagnosis;
	Coder coder;

	treatment.Clear();
	treatment.protocol = "GetDiagnosis";
	coder.EncodeTreatment(treatment, jDiagnosis);

	jRet["data"] 	= jDiagnosis;
	jRet["code"]	= 0;
	
	return jRet;
}

Json::Value Clinics::GetSolution(treatment_t treatment, DataBase* pDB) {
	pthread_t pid;
	LOG_DEBUG("get solution");

	Json::Value jRet;
	/*1.对自定义方法进行词法拆分*/
	/*2.从diagnosis获取到对应的id,可能没有id,第一次听说这样的描述,
	 * 暂时不存储,等用户确定保存的时候才把新的描述添加到数据库*/
	/*3.合并id到solution*/

	/*4.从数据库获取记录症状描述和统计结果*/
	vector<solution_t> solutions = GetSolutionContent(treatment,pDB);
	
	/*将数据包装成json*/
	Json::Value jSolution;
	Coder coder;

	treatment.Clear();
	treatment.protocol = "GetSolution";
	coder.EncodeTreatment(treatment, jSolution);

	jRet["data"]	= jSolution;
	jRet["code"]	= 0;
	
	return jRet;
	

}

vector<string> Clinics:: SplitDetail(const string detail) {
	//以; . ! 为拆分依据 
	int min;
	std::vector<string>	details;
	std::string detail_buf = detail;

	do {
		//算出拆分点的最小位置
		int pos1 	= detail_buf.find(";");
		int pos2 	= detail_buf.find("!");
		int pos3 	= detail_buf.find(".");

		min 		= pos1 > pos2 ? pos2 : pos1;
		min 		= min  > pos3 ? pos3 : min;

		//执行拆分
		if(min > -1) {
			std::string aDetail = detail_buf.substr(0, min - 1);
			details.push_back(aDetail);

			detail_buf = detail_buf.substr(min + 1, detail.size());
		}else {
			details.push_back(detail);
		}

	}while(min  > -1);
	//缓存拆分结果
	
	return details;
}


vector<LONG> Clinics::GetSymptomID(const vector<string> details, DataBase* pDB) {
	LOG_DEBUG("get symptom id");
	vector<LONG> ids;
	//sql 很久描述获取id
	string sql = "slect diagnosis_element_id from diagnosis_element where \
				  type = 1 and content in ( ";
	for(int i = 0 ; i < details.size(); i ++) {
		sql	+= " ' " + details[i] + " ' ,";
	}
	sql = sql.substr(0, sql.size() - 1);
	sql += ")";

	
	if(pDB->Exec(sql.c_str()) < 0) {
		LOG_ERROR("perform db query failed: %s", pDB->GetLastError());
		return ids;
	}

	/*提取结果集合*/
	while(pDB->HasNext()) {
		LONG id = -1;
		pDB->GetFieldValue("diagnosis_element_id", id);
		ids.push_back(id);
	}

	return ids;
}

/*******************************************************************
 *直接按照记录出现的次数来排序似乎不科学,应该考虑到数据源的权重(病例/医学库)
 *还有就是症状描述度个例符合程度,比如三个原始描述刚好满足一个病例的所有描述,
 *应该匹配度最高;还有自己的病历有这样的描述应该特殊考虑,还有按用户的年龄/职业 
 *性别/地理位置/季节等进行综合分析;我们的优势是精准,而不是纯粹的搜索,比医生准;element_ids
 *
 ********************************************************************/
vector<symptom_t> Clinics::GetSymptomContent(const vector<LONG> element_ids, DataBase* pDB) {
	LOG_DEBUG("get symptom content");

	vector<symptom_t> symptoms;
	if(NULL == pDB) {
		LOG_ERROR("空指针pDB;");
		return symptoms;
	}

	/*获取相关元素的id集合,并连表查询取得内容,还要统计出病例中出现的次数*/
	string sql = "select diagnosis_element_id, content from diagnosis f join diagnosis_element s on diagnosis.diagnosis_element_id = diagnosis.diagnosis_element_id where f.diagnosis_element_id in (select \
				  diagnosis_element_id from f where diagnosi_id in (\
					  select diagnosis_id from f where diagnosis_element_id in (";
	
	for(int i = 0; i < element_ids.size(); i++) {
		char szID[32];
		sprintf(szID, "%lld,", element_ids[i]);

		sql += szID;
	}
	sql = sql.substr(0, sql.size() - 1);
	
	sql += ")))";

	sql	+= "group by diagnosis_element_id count(*) as feedback order by feedback;";

	if(pDB->Exec(sql.c_str()) < 0) {
		LOG_ERROR("数据库查询失败:%s \n" , pDB->GetLastError());
		return symptoms;
	}

	while(pDB->HasNext()){
		symptom_t symptom;
		symptom.Clear();

		pDB->GetFieldValue("diagnosis_element_id", symptom.diagnosis_element_id);
		pDB->GetFieldValue("content", symptom.content);
		pDB->GetFieldValue("feedback", symptom.feedback);

		symptoms.push_back(symptom);
	}

	return symptoms;
}

vector<cause_t>	Clinics::GetCauseContent(treatment_t treatment, DataBase* pDB) {
	vector<cause_t> causes;
	/*1.sql 连表查询diagnosis_element_id, content, feedback*/
	string sql = "select diagnosis_element_id, content, feedback \
				  from diagnosis f join diagnosis_element s on \
				  f.diagnosis_element_id = s.diagnosis_element_id \
				  where diagnosis_id in (select diagnosis_id from \
				  f where diagnosis_element_id in (";
	/*插入症状描述*/
	int i = 0;
	char szID[32];

	for( i = 0; i < treatment.symptom.elements.size(); i++) {
		sprintf(szID, "%lld,", treatment.symptom.elements[i].diagnosis_element_id);
		sql += szID;
	}
	sql = sql.substr(0, sql.size() - 1);

	/*插入原因描述,这是一个数量级增加的算法,原因会越来越多*/
	for(i = 0; i < treatment.cause.elements.size(); i++) {
		sprintf(szID, "%lld.", treatment.cause.elements[i].diagnosis_element_id);
		sql += szID;
	}
	sql = sql.substr(0, sql.size() - 1);
	sql += "))";

	if(pDB->Exec(sql.c_str()) < 0) {
		LOG_ERROR("执行数据库失败:%s \n" , pDB->GetLastError());
		return causes;
	}

	/*2.提取结果集*/
	while(pDB->HasNext()) {
		cause_t cause;
		cause.Clear();

		pDB->GetFieldValue("diagnosis_element_id", cause.diagnosis_element_id);
		pDB->GetFieldValue("content",	cause.content);
		pDB->GetFieldValue("feedback", 	cause.feedback);
		causes.push_back(cause);
	}

	/*3.sql 查询本user_id diagnosis_element_id的feedback次数*/
	
	/*4.标记自己怀疑次数*/

	return causes;
}

vector<diagnosis_t> Clinics::GetDiagnosisContent(treatment_t treatment, DataBase* pDB) {
	vector<diagnosis_t> diagnosises;
	/*1.sql 查询到diagnosis_id 集合*/
	string sql = "select diagnosi_id from diagnosis where diagnosi_id in (select diagnosi_id from diagnosis where type = 1\
				  and  diagnosis_element_id in (";
	int i = 0;
	char szID[32] ;
	for(i = 0; i < treatment.symptom.elements.size(); i++) {
		sprintf(szID, "%lld,", treatment.symptom.elements[i].diagnosis_element_id);
		sql += szID;
	}
	sql = sql.substr(0, sql.size() - 1);

	sql += ") ) and diagnosis_element_id in ("; 
	for(i = 0; i < treatment.cause.elements.size(); i++){
		sprintf(szID, "%lld,", treatment.cause.elements[i].diagnosis_element_id);
		sql += szID;
	}
	sql = sql.substr(0, sql.size() - 1);

	sql += ")";
	
	if(pDB->Exec(sql.c_str()) < 0) {
		LOG_ERROR("perform db query failed:%s \n" , pDB->GetLastError());
		return diagnosises;
	}

	diagnosis_t diagnosis;
	while(pDB->HasNext()) {
		diagnosis.Clear();
		pDB->GetFieldValue("diagnosis_id", diagnosis.diagnosis_id);
		diagnosises.push_back(diagnosis);
	}

	for(i = 0; i < diagnosises.size(); i ++) {
		sql  = "select symptom_count, cause_count from ( \
				select count(*) as symptom_count from diagnosis \
				where diagnosis_id = ";

		diagnosis = diagnosises[i];
		sprintf(szID, "%lld" ,diagnosis.diagnosis_id);
		sql += szID;
		
		sql += ") and diagnosis_element_id in (";
		
		sql += ") , \
				select count(*) as casue_count from diagnosis where\
				diagnosi_id = ";
		
		sql += ")";
	}
	/*2.sql查询所有diagnosis_id集合对应的症状描述集合和原因集合*/

	/*逐个计算症状描述和原因的匹配率 p = (用户症状匹配数数/病例症状描述条数)
	 *  乘以 (用户原因匹配数/病例原因总数) ;然后把相同诊断id概率加起来,
	 *  再除以本次统计总数, 根据概率从高到底排序,还应该考虑到医学知识库的权
	 *  重*/

	/*4.严重成都需要从另外一张表用element_id查出来*/

	return diagnosises;
}

vector<solution_t> Clinics::GetSolutionContent(treatment_t treatment,DataBase* pDB) {
	vector<solution_t> solutions;


	return solutions;
}

Json::Value Clinics::Finish(treatment_t treatment, DataBase* pDB) {
	Json::Value jRet;

	return jRet;
}
