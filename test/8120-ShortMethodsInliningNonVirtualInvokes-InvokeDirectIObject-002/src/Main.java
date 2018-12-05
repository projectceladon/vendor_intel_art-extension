class Main {
    final static int iterations = 10;
    public static Foo tmpThingy;
    
    public static void main(String[] args) {
        Test test = new Test();
        Foo nextThingy = new Foo('F');

        System.out.println("Initial nextThingy value is " + nextThingy.getClass().toString());
        System.out.println("Initial nextThingy.a is " + nextThingy.a);

        for(int i = 0; i < iterations; i++) {
            nextThingy = test.gimme();
            tmpThingy = nextThingy.toFoo(); 
            test.hereyouare(tmpThingy);
        }

        System.out.println("Initial nextThingy value is " + nextThingy.getClass().toString());
        System.out.println("Initial nextThingy.a is " + nextThingy.a);

    }
}
