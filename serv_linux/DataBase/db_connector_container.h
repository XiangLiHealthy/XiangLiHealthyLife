#ifndef DB_CONNECTOR_CONTAINER_
#define DB_CONNECTOR_CONTAINER_
#include "DataBase.h"
#include <vector>
#include <memory>
#include <mutex>

using std::vector;
using std::shared_ptr;
using std::mutex;


class DBConnectorContainer
{
public:
    static DBConnectorContainer& GetInstance()
    {
        static DBConnectorContainer singleton;
        return singleton;
    }

    std::shared_ptr<DataBase> GetDB();
    void ReleaseDB(std::shared_ptr<DataBase> ptr_db);
private:
   mutex m_lock;
    vector<shared_ptr<DataBase>> m_db_connectors;

private:
    DBConnectorContainer();
    ~DBConnectorContainer();
};

#endif
