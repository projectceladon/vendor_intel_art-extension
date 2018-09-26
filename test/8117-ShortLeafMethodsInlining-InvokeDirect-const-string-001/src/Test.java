class Test {
    public String shim(){
        return simple_method();
    }

    private String simple_method() {
        String string;
        string = "This is a string";
        return string;
    }

}
