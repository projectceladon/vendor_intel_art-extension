// The test checks that stack after NullPointerException occurs is correct despite inlining
class Test {
    short[] thingiesArray;
    short bean = -32768;
    int iterr;

    public Test (int iter) {
        this.thingiesArray = new short[iter];
        this.iterr = iter;
        for(int i = 0; i < iter; i++) {
            this.thingiesArray[i] = (short)(bean + 1);
        }
    }

    private short getThingies(short[] arr, int i, short s1, short s2, short s3, short s4, short s5, short s6) {
        return arr[i];
    }

    private void setThingies(short[] arr, short newThingy, int i, short s1, short s2, short s3, short s4, short s5, short s6) {
        arr[i] = newThingy;
    }

    public short gimme(short[] arr, int i, short s1, short s2, short s3, short s4, short s5, short s6) {
       return getThingies(arr, i, s1, s2, s3, s4, s5, s6);
    }

    public void hereyouare(short[] arr, short newThingy, int i, short s1, short s2, short s3, short s4, short s5, short s6) {
       if (i == iterr - 1)
           arr = null;
        setThingies(arr, newThingy, i, s1, s2, s3, s4, s5, s6);
    }
}

