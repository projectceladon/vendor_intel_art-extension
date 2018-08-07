class Test {
    short thingies = -32768;

    static short getThingies(Test t) {
        return t.thingies;
    }

    static void setThingies(Test t, short j) {
        t.thingies = j;
    }
}

