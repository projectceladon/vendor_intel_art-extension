// The test checks that stack after NullPointerException occurs is correct despite inlining
class Test {
    byte[] thingiesArray;
    byte bean = -128;
    int iterr;

    public Test (int iter) {
        this.thingiesArray = new byte[iter];
        this.iterr = iter;
        for(int i = 0; i < iter; i++) {
            this.thingiesArray[i] = (byte)(bean + 1);
        }
    }

    private byte getThingies(byte[] arr, int i) {
        return arr[i];
    }

    private void setThingies(byte[] arr, byte newThingy, int i) {
        arr[i] = newThingy;
    }

    public byte gimme(byte[] arr, int i) {
       if (i == iterr - 1)
           arr = null;
       return getThingies(arr, i);
    }

    public void hereyouare(byte[] arr, byte newThingy, int i) {
        setThingies(arr, newThingy, i);
    }
}

