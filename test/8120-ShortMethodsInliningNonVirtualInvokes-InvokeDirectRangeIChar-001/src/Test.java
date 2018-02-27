class Test {
    char thingies = 'a';

    private char getThingies(char c1, char c2, char c3, char c4, char c5, char c6) {
        return thingies;
    }

    private void setThingies(char newThingies, char c1, char c2, char c3, char c4, char c5, char c6) {
        thingies = newThingies;
    }

    public char gimme(char c1, char c2, char c3, char c4, char c5, char c6) {
       return (char)(getThingies(c1, c2, c3, c4, c5, c6) + 1);
    }

    public void hereyouare(char newThingies, char c1, char c2, char c3, char c4, char c5, char c6) {
        setThingies((char)(newThingies - 1), c1, c2, c3, c4, c5, c6);
    }
}

