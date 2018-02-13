class Test {
    Foo thingies = new Foo();

    static Foo getThingies(Test t) {
        return t.thingies;
    }

    static void setThingies(Test t, Foo j) {
        t.thingies = j;
    }
}

