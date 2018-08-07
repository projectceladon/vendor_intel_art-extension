class Test {
    public long shim(long jj){
        return simple_method(jj);
    }

    private long simple_method(long jj) {
        return (-jj);
    }

}
