package com.example.threadlib;

/**
 * 每一个发起网络请求的页面都应该同时继承这个类,并实现notify方法;
 * 然后在每个model里面都会实现数据处理,然后调用notify
 *
 *
 */
public abstract class Notify {
    //会产生递归调用,函数栈不要嵌套太多,否则可能导致栈溢出
    public abstract void notify(Notify notify);
    public void setNotify(Notify notify ){m_notify = notify;}
    public Notify m_notify;
}
