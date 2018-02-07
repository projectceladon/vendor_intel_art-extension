class Test {
    public int shim(int jj, int kk){
        return simple_method(jj, kk);
    }

    private int simple_method(int jj, int kk) {
        jj = kk | jj;
        jj = jj | kk;
        return jj;
    }

}
