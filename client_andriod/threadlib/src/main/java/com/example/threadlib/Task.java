package com.example.threadlib;

public abstract class Task extends Notify{
    //private Notify notify;
    abstract  public void run() throws Exception;
    public void notify(Notify notify) {//任务不需要被通知,默认实现不做事
        }

    public void runTask() {
        //逻辑处理
        try {
            run();
        } catch (Exception e) {
            e.printStackTrace();
        }

        //更新UI
        if(null != m_notify) {
            m_notify.notify(this);
        }
    }
}
