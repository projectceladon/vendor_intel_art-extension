class Test {
    short thingies = -32768;

    private short getThingies(short s1, short s2, short s3, short s4, short s5, short s6) {
        return thingies;
    }

    private void setThingies(short newThingies, short s1, short s2, short s3, short s4, short s5, short s6) {
        thingies = newThingies;
    }

    public short gimme(short s1, short s2, short s3, short s4, short s5, short s6) {
       return (short)(getThingies(s1, s2, s3, s4, s5, s6) + 1);
    }

    public void hereyouare(short newThingies, short s1, short s2, short s3, short s4, short s5, short s6) {
        setThingies((short)(newThingies - 1), s1, s2, s3, s4, s5, s6);
    }
}

