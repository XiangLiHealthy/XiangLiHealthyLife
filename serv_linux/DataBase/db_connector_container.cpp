#include "db_connector_container.h"
#include "../config/config_manager.h"
#include "../log/log.h"

DBConnectorContainer::DBConnectorContainer()
{

}

DBConnectorContainer::~DBConnectorContainer()
{

}

shared_ptr<DataBase> DBConnectorContainer::GetDB()
{
    std::lock_guard<mutex> lock(m_lock);

    //try get a free connect
    for (int index = 0; index < m_db_connectors.size(); index ++)
    {
        if (m_db_connectors[index].unique())
            return m_db_connectors[index];
    }

    const config_t config = ConfigManager::GetInstance().GetConfig();
    if (m_db_connectors.size() >= config.tdb.limit_count)
    {
        LOG_INFO("all db connect is used,count is %d", m_db_connectors.size());
        return nullptr;
    }

    //add a connect if the count of connect is smaller than limit
    shared_ptr<DataBase> ptrDB = std::make_shared<DataBase>();
    if (!ptrDB)
    {
        LOG_ERROR("no memory");
        return nullptr;
    }

    //connect to mysql
    if (ptrDB->Connect(config.tdb.strHost, config.tdb.strDBName, config.tdb.strUsr, config.tdb.strPassword) < 0)
    {
        LOG_ERROR("db connect failed");
        return nullptr;
    }

    m_db_connectors.push_back(ptrDB);
    return ptrDB;
}

