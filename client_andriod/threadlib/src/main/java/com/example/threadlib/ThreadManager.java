package com.example.threadlib;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * 负责线程的管理
 */
public class ThreadManager {
    public void start() {
        m_exec = Executors.newCachedThreadPool();
        for(int i = 0; i < 4; i ++) {
            m_exec.execute(new TaskThread());
        }
    }

    public void stop() {
        if(null != m_exec) {
            m_exec.shutdownNow();
        }
    }

    private ExecutorService m_exec;
}
