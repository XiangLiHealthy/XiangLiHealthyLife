package com.example.threadlib;
import java.util.concurrent.LinkedBlockingQueue;

public class TaskQueue extends LinkedBlockingQueue<Task> {
    public static TaskQueue taskQueue = new TaskQueue();
}
