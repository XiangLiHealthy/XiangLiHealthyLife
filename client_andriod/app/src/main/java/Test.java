interface A{
    void method1();
    void method2();
        }


public class Test {
    A getA(){
        return  new A() {
            @Override
            public void method1() {
            }

            @Override
            public void method2() {

            }
        };
    }

    public static void main(String[] args){
        Test t = new Test();
        A a = t.getA();
        a.method1();
        a.method2();
    }
}

class MNN {
    class A{
        class B{

        }
    };

    static class  C{

    }

    public static void main(String[] args){
        //MNN a = new MNN();
        //MNN.A aa = a.new A();
        ////MNN.A aa-a = new MNN.A();
        //MNN.A.B aaa = aa.new B();
        //MNN.C c = new MNN.C();
    }
}
