// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Test {
    boolean[] thingiesArray;

    public Test (int iter) {
        this.thingiesArray = new boolean[iter];
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
       return getThingies(arr, i);
    }

    public void hereyouare(boolean[] arr, boolean newThingy, int i) {
        setThingies(arr, newThingy, i);
    }
}

