struct ViewMatrix
{
    float* matrix;

    ViewMatrix(const int length = 16)
    {
        matrix = new float[length]{};
    }
};