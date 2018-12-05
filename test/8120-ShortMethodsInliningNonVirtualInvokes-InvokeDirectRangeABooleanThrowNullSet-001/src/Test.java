// The test checks that stack after NullPointerException occurs is correct despite inlining
class Test {
    boolean[] thingiesArray;
    int iterr;

    public Test (int iter) {
        this.thingiesArray = new boolean[iter];
        this.iterr = iter;
        for(int i = 0; i < iter; i++) {
            this.thingiesArray[i] = true;
        }
    }

    private boolean getThingies(boolean[] arr, int i, int i1, int i2, int i3, int i4, int i5, int i6) {
        return arr[i];
    }

    private void setThingies(boolean[] arr, boolean newThingy, int i, int i1, int i2, int i3, int i4, int i5, int i6) {
        arr[i] = newThingy;
    }

    public boolean gimme(boolean[] arr, int i, int i1, int i2, int i3, int i4, int i5, int i6) {
       return getThingies(arr, i, i1, i2, i3, i4, i5, i6);
    }

    public void hereyouare(boolean[] arr, boolean newThingy, int i, int i1, int i2, int i3, int i4, int i5, int i6) {
       if (i == iterr - 1)
           arr = null;
        setThingies(arr, newThingy, i, i1, i2, i3, i4, i5, i6);
    }
}

