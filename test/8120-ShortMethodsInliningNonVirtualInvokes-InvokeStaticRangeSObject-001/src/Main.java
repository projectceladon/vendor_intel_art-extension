class Main {
    final static int iterations = 10;
    static Foo thingies= new Foo();

    public static Foo getThingies(Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        return thingies;
    }

    public static void setThingies(Foo newThingies, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        thingies = newThingies;
    }
    
    public static void main(String[] args) {
        Foo nextThingy = thingies;
        Foo g1 = new Foo();
        Foo g2 = new Foo();
        Foo g3 = new Foo();
        Foo g4 = new Foo();
        Foo g5 = new Foo();
        Foo g6 = new Foo();

        System.out.println("Initial nextThingy value is " + nextThingy.getClass().toString());

        for(int i = 0; i < iterations; i++) {
            nextThingy = getThingies(g1, g2, g3, g4, g5, g6);
            setThingies(nextThingy, g1, g2, g3, g4, g5, g6);
        }
       
        System.out.println("Final nextThingy value is " + nextThingy.getClass().toString());

    }
}
