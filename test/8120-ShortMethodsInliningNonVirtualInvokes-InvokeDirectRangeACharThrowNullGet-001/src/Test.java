// The test checks that stack after NullPointerException occurs is correct despite inlining
class Test {
    char[] thingiesArray;
    char bean = 'a';
    int iterr;

    public Test (int iter) {
        this.thingiesArray = new char[iter];
        this.iterr = iter;
        for(int i = 0; i < iter; i++) {
            this.thingiesArray[i] = (char)(bean + 1);
        }
    }

    private char getThingies(char[] arr, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
        return arr[i];
    }

    private void setThingies(char[] arr, char newThingy, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
        arr[i] = newThingy;
    }

    public char gimme(char[] arr, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
       if (i == iterr - 1)
           arr = null;
       return getThingies(arr, i, c1, c2, c3, c4, c5, c6);
    }

    public void hereyouare(char[] arr, char newThingy, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
        setThingies(arr, newThingy, i, c1, c2, c3, c4, c5, c6);
    }
}

