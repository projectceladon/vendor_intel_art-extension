// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Test {
//    int thingies = 0;
    int[] thingiesArray;

    public Test (int iter) {
        this.thingiesArray = new int[iter];
        for(int i = 0; i < iter; i++) {
            this.thingiesArray[i] = i;
        }
    }

    private int getThingies(int[] arr, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
        return arr[i];
    }

    private void setThingies(int[] arr, int newThingy, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
        arr[i] = newThingy;
    }

    public int gimme(int[] arr, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
       return getThingies(arr, i, c1, c2, c3, c4, c5, c6);
    }

    public void hereyouare(int[] arr, int newThingy, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
        setThingies(arr, newThingy, i, c1, c2, c3, c4, c5, c6);
    }
}

