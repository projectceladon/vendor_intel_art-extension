class Test {
    public short shim(){
        return simple_method();
    }

    private short simple_method() {
        short   l4;
        l4 = 32767;
        return l4;
    }
}
