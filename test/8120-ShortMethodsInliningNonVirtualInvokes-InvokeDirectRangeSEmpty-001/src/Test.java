class Test {
    static int thingies = 0;

    private int getThingies(char c1, char c2, char c3, char c4, char c5, char c6) {
        return thingies;
    }

    private void setThingies(int newThingies, char c1, char c2, char c3, char c4, char c5, char c6) {
// empty method
    }

    public int gimme(char c1, char c2, char c3, char c4, char c5, char c6) {
       return getThingies(c1, c2, c3, c4, c5, c6);
    }

    public void hereyouare(int newThingies, char c1, char c2, char c3, char c4, char c5, char c6) {
        setThingies(newThingies, c1, c2, c3, c4, c5, c6);
    }
}

