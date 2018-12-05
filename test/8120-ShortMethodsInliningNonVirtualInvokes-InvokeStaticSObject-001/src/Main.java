class Main {
    final static int iterations = 10;
    static Foo thingies= new Foo();

    public static Foo getThingies() {
        return thingies;
    }

    public static void setThingies(Foo newThingies) {
        thingies = newThingies;
    }
    
    public static void main(String[] args) {
        Foo nextThingy = thingies;

        System.out.println("Initial nextThingy value is " + nextThingy.getClass().toString());

        for(int i = 0; i < iterations; i++) {
            nextThingy = getThingies();
            setThingies(nextThingy);
        }

        System.out.println("Final nextThingy value is " + nextThingy.getClass().toString());

    }
}
