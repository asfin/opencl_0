void kernel ocl_src(global const int* A, global const int* B, global int* C) {
    for (int i = 0; i < 2; i++) {
        C[get_global_id(0)] = get_global_id(0); //A[get_global_id(0)]+B[get_global_id(0)];
    }
}

