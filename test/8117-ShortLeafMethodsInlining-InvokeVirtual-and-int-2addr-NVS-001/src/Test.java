class Test {

    public int simple_method(int jj, int kk) {
        jj = kk & jj;
        jj = jj & kk;
        return jj;
    }

}
