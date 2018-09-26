// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Test {
    short[] thingiesArray;
    short bean = -32768;

    public Test (int iter) {
        this.thingiesArray = new short[iter];
        for(int i = 0; i < iter; i++) {
            this.thingiesArray[i] = (short)(bean + 1);
        }
    }

    private short getThingies(short[] arr, int i) {
        return arr[i];
    }

    private void setThingies(short[] arr, short newThingy, int i) {
        arr[i] = newThingy;
    }

    public short gimme(short[] arr, int i) {
       return getThingies(arr, i);
    }

    public void hereyouare(short[] arr, short newThingy, int i) {
        setThingies(arr, newThingy, i);
    }
}

