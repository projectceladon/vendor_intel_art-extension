class Test {
    short thingies = -32768;

    private short getThingies() {
        return thingies;
    }

    private void setThingies(short newThingies) {
        thingies = newThingies;
    }

    public short gimme() {
       return (short)(getThingies() + 1);
    }

    public void hereyouare(short newThingies) {
        setThingies((short)(newThingies - 1));
    }
}

