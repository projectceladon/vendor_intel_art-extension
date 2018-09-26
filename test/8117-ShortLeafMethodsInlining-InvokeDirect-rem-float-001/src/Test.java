class Test {
    public float shim(float jj, float kk){
        return simple_method(jj, kk);
    }

    private float simple_method(float jj, float kk) {
        jj = kk % jj;
        return jj;
    }

}
