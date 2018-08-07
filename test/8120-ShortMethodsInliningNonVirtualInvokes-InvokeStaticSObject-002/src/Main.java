class Main {
    final static int iterations = 10;
    static Foo thingies = new Foo();
    static Foo nextThingy = new Foo('Q');

    public static Foo getThingies() {
        return thingies;
    }

    public static void setThingies(Foo newThingies) {
        thingies = newThingies;
    }
    
    public static void main(String[] args) {

        System.out.println("Initial nextThingy value is " + nextThingy.getClass().toString());
        System.out.println("Initial nextThingy.a value is " + nextThingy.a);

        for(int i = 0; i < iterations; i++) {
            nextThingy = getThingies();
            setThingies(nextThingy.toFoo());
        }

        System.out.println("Final nextThingy value is " + nextThingy.getClass().toString());
        System.out.println("Final nextThingy.a value is " + nextThingy.a);

    }
}
