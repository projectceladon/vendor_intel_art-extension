class Test {
    public long shim(long jj, long ii){
        return simple_method(jj, ii);
    }

    private long simple_method(long jj, long ii) {
        jj = ii * jj;
        jj = ii * jj;
        return jj;
    }

}
