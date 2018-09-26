class Test {
    public char shim(){
        return simple_method();
    }

    private char simple_method() {
        char    l2;
        l2 = '\uffff';
        return l2;
    }
}
