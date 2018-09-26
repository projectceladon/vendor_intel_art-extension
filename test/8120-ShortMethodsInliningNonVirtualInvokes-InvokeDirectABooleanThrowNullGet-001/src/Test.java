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

    private boolean getThingies(boolean[] arr, int i) {
        return arr[i];
    }

    private void setThingies(boolean[] arr, boolean newThingy, int i) {
        arr[i] = newThingy;
    }

    public boolean gimme(boolean[] arr, int i) {
       if (i == iterr - 1)
           arr = null;
       return getThingies(arr, i);
    }

    public void hereyouare(boolean[] arr, boolean newThingy, int i) {
        setThingies(arr, newThingy, i);
    }
}

