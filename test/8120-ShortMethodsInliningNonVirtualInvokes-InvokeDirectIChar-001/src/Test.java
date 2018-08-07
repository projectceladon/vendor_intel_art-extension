class Test {
    char thingies = 'a';

    private char getThingies() {
        return thingies;
    }

    private void setThingies(char newThingies) {
        thingies = newThingies;
    }

    public char gimme() {
       return (char)(getThingies() + 1);
    }

    public void hereyouare(char newThingies) {
        setThingies((char)(newThingies - 1));
    }
}

