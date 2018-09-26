class Test {
    public int shim(float jj){
        return simple_method(jj);
    }

    private int simple_method(float jj) {
        int ii;
        ii = (int)jj;
        return ii;
    }

}
