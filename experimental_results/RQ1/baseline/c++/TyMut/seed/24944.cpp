
unsigned int iMax(10);
#pragma omp parallel for schedule(dynamic,1) shared(iMax) private(i, j) collapse(2)
for( i = 1; i < 4; i++ )
{
	for( j = 0; j <= iMax - i; j++ )
	{
	}
}

