class Test {
    short thingies = -32768;

    static short getThingies(Test t, short s1, short s2, short s3, short s4, short s5, short s6) {
        return t.thingies;
    }

    static void setThingies(Test t, short j, short s1, short s2, short s3, short s4, short s5, short s6) {
        t.thingies = j;
    }
}

