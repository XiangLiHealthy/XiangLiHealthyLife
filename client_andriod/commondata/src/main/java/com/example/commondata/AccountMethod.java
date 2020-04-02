package com.example.commondata;

public enum AccountMethod
{
    LOGIN("Login"),
    LOGOUT("Logout"),
    REGISTER("Register"),
    UNREGISTER("Unregister");

    private  String m_desc;
    AccountMethod(String desc)
    {
        this.m_desc = desc;
    }

    public String getDescription()
    {
        return m_desc;
    }
};



