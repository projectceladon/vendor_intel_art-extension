class Test {
    public int shim(long jj){
        return simple_method(jj);
    }

    private int simple_method(long jj) {
        int ii;
        ii = (int)jj;
        return ii;
    }

}
