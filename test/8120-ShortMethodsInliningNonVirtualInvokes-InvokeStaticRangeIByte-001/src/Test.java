class Test {
    byte thingies = -128;

    static byte getThingies(Test t, byte b1, byte b2, byte b3, byte b4, byte b5, byte b6) {
        return t.thingies;
    }

    static void setThingies(Test t, byte j, byte b1, byte b2, byte b3, byte b4, byte b5, byte b6) {
        t.thingies = j;
    }
}

