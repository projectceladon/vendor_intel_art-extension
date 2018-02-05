class Test {
    public long shim(long jj, long ii){
        return simple_method(jj, ii);
    }

    private long simple_method(long jj, long ii) {
        long a = jj;
        a = jj - a;
        a = a - ii;
        return a;
    }

}
