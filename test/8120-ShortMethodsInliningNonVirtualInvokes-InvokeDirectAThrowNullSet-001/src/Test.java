// The test checks that stack after NullPointerException occurs is correct despite inlining
class Test {
//    int thingies = 0;
    int[] thingiesArray;
    int iterr;

    public Test (int iter) {
        this.thingiesArray = new int[iter];
        this.iterr = iter;
        for(int i = 0; i < iter; i++) {
            this.thingiesArray[i] = i;
        }
    }

    private int getThingies(int[] arr, int i) {
        return arr[i];
    }

    private void setThingies(int[] arr, int newThingy, int i) {
        arr[i] = newThingy;
    }

    public int gimme(int[] arr, int i) {
       return getThingies(arr, i);
    }

    public void hereyouare(int[] arr, int newThingy, int i) {
       if (i == iterr - 1)
           arr = null;
        setThingies(arr, newThingy, i);
    }
}

