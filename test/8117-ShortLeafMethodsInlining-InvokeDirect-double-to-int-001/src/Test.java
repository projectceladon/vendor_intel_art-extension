class Test {
    public int shim(double jj){
        return simple_method(jj);
    }

    private int simple_method(double jj) {
        int ii;
        ii = (int)jj;
        return ii;
    }

}
