class Test {
    public int shim(int jj, int ii){
        return simple_method(jj, ii);
    }

    private int simple_method(int jj, int ii) {
        jj = ii / jj;
        return jj;
    }

}
