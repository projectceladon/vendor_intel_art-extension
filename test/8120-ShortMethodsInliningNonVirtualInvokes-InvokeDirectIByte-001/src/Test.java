class Test {
    byte thingies = -128;

    private byte getThingies() {
        return thingies;
    }

    private void setThingies(byte newThingies) {
        thingies = newThingies;
    }

    public byte gimme() {
       return (byte)(getThingies() + 1);
    }

    public void hereyouare(byte newThingies) {
        setThingies((byte)(newThingies - 1));
    }
}

