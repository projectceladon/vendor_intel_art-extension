class Test {
    Foo thingies = new Foo();

    static Foo getThingies(Test t, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        return t.thingies;
    }

    static void setThingies(Test t, Foo j, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        t.thingies = j;
    }
}

