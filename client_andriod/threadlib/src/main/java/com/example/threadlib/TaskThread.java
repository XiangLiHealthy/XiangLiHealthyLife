package com.example.threadlib;

public class TaskThread implements Runnable{
    //private Lock = new ReentrantLock;

    public void run() {
        try{
            while (!Thread.interrupted()) {
                //从队列中获得任务
                Task task = TaskQueue.taskQueue.take();

                //运行
                if(null != task) {
                    task.runTask();
                }
            }
        }catch (Exception e) {
            System.out.println(e);
        }

    }
}
