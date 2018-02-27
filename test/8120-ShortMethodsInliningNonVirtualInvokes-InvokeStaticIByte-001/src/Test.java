class Test {
    byte thingies = -128;

    static byte getThingies(Test t) {
        return t.thingies;
    }

    static void setThingies(Test t, byte j) {
        t.thingies = j;
    }
}

