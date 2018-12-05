// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Test {
    char[] thingiesArray;
    char bean = 'a';

    public Test (int iter) {
        this.thingiesArray = new char[iter];
        for(int i = 0; i < iter; i++) {
            this.thingiesArray[i] = (char)(bean + 1);
        }
    }

    private char getThingies(char[] arr, int i) {
        return arr[i];
    }

    private void setThingies(char[] arr, char newThingy, int i) {
        arr[i] = newThingy;
    }

    public char gimme(char[] arr, int i) {
       return getThingies(arr, i);
    }

    public void hereyouare(char[] arr, char newThingy, int i) {
        setThingies(arr, newThingy, i);
    }
}

