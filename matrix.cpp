#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//初始化,所有元素为0
Mat init(int rows, int cols) {
	int i;
	Mat M = (Mat)malloc(sizeof(MatStruct));
	M->rows = rows;
	M->cols = cols;
	M->matrix = (float**)malloc(rows * sizeof(float*));
	for (i = 0; i < rows; i++) {
		M->matrix[i] = (float*)malloc(cols * sizeof(float));
	}
	zeros(M);
	return M;
};

//全部赋值为0
void zeros(Mat M) {
	int i, j;
	int rows = M->rows, cols = M->cols;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			M->matrix[i][j] = 0;
		}
	}
}

//转置
Mat transpose(Mat A){
	int i, j;
	int a = A->rows, b = A->cols;
	Mat At = init(b, a);
	for (i = 0; i < a; i++) {
		for (j = 0; j < b; j++) {
			At->matrix[j][i] = A->matrix[i][j];
		}
	}
	return At;
}

Mat multiply(Mat M1, Mat M2) {
	 if (M1->cols != M2->rows) {
		 printf("\nError in Matrix Multiplication\n");
		 system("pause");
		 exit(0);
	 }
	 Mat Mr = init(M1->rows, M2->cols);
	 int percent = 0;
	 for (int i = 0; i < M1->rows; i++) {
		 for (int j = 0; j < M2->cols; j++) {
			 float sum = 0;
			 for (int m = 0; m < M1->cols; m++) {
				 sum += M1->matrix[i][m] * M2->matrix[m][j];
			 }
			 Mr->matrix[i][j] = sum;
		 }
		 if (M1->rows > 100 && i % (M1->rows / 10) == 0) {
			 printf("multiply has completed %d %%\n", percent);
			 percent += 10;
		 }
	 }
	 return Mr;
 }

Mat multiply(Mat M, float D) {
	 Mat Mr = clone(M);
	 for (int i = 0; i < M->rows; i++) {
		 for (int j = 0; j < M->cols; j++) {
			 Mr->matrix[i][j] = M->matrix[i][j] * D;
		 }
	 }
	 return Mr;
 }

Mat divide(Mat M, float D) {
	 Mat Mr = clone(M);
	 for (int i = 0; i < M->rows; i++) {
		 for (int j = 0; j < M->cols; j++) {
			 Mr->matrix[i][j] = M->matrix[i][j] / D;
		 }
	 }
	 return Mr;
 }

Mat add(Mat M1, Mat M2) {
	 int i, j;
	 int a = M1->rows, b = M1->cols;
	 int c = M2->rows, d = M2->cols;
	 Mat Mr = NULL;
	 if (a != c || b != d) {
		 printf("\nError in Matrix Add\n");
		 system("pause");
		 exit(0);
	 }
	 Mr = init(a, b);
	 for (i = 0; i < a; i++) {
		 for (j = 0; j < b; j++) {
			 Mr->matrix[i][j] = M1->matrix[i][j] + M2->matrix[i][j];
		 }
	 }
	 return Mr;
 }

Mat AddD(Mat M, float D) {
	 Mat Mr = clone(M);
	 for (int i = 0; i < M->rows; i++) {
		 for (int j = 0; j < M->cols; j++) {
			 Mr->matrix[i][j] = M->matrix[i][j] + D;
		 }
	 }
	 return Mr;
 }

 Mat subtract(Mat M1, Mat M2) {
	 int i, j;
	 int a = M1->rows, b = M1->cols;
	 int c = M2->rows, d = M2->cols;
	 Mat Mr = NULL;
	 if (a != c || b != d) {
		 printf("\nError in Matrix subtraction\n");
		 system("pause");
		 exit(0);
	 }
	 Mr = init(a, b);
	 for (i = 0; i < a; i++) {
		 for (j = 0; j < b; j++) {
			 Mr->matrix[i][j] = M1->matrix[i][j] - M2->matrix[i][j];
		 }
	 }
	 return Mr;
 }

 Mat inverse(Mat Mori) {
	 int n = Mori->rows;
	 float d;
	 int* JS = (int*)malloc(sizeof(int)*n);
	 int* IS = (int*)malloc(sizeof(int)*n);

	 if (Mori->rows != Mori->cols) {
		 printf("矩阵无法求逆\n"); 
		 system("pause");
		 exit(0);
	 }
	 Mat M = clone(Mori);
	 for (int k = 0; k < n; k++)
	 {
		 d = 0;
		 for (int i = k; i < n; i++) {
			 for (int j = k; j < n; j++) {
				 if (fabs(M->matrix[i][j])>d) {
					 d = fabs(M->matrix[i][j]);
					 IS[k] = i;
					 JS[k] = j;
				 }
			 }
		 }
		 if (d + 1.0 == 1.0) return 0;
		 if (IS[k] != k)
			 for (int j = 0; j < n; j++)
				 swap(&M->matrix[k][j], &M->matrix[IS[k]][j]);
		 if (JS[k] != k)
			 for (int i = 0; i < n; i++)
				 swap(&M->matrix[i][k], &M->matrix[i][JS[k]]);
		 M->matrix[k][k] = 1 / M->matrix[k][k];
		 for (int j = 0; j < n; j++)
			 if (j != k) M->matrix[k][j] = M->matrix[k][j] * M->matrix[k][k];
		 for (int i = 0; i < n; i++)
			 if (i != k)
				 for (int j = 0; j < n; j++)
					 if (j != k) M->matrix[i][j] = M->matrix[i][j] - M->matrix[i][k] * M->matrix[k][j];
		 for (int i = 0; i < n; i++)
			 if (i != k) M->matrix[i][k] = -M->matrix[i][k] * M->matrix[k][k];
	 }
	 for (int k = n - 1; k >= 0; k--)
	 {
		 for (int j = 0; j < n; j++)
			 if (JS[k] != k) swap(&M->matrix[k][j], &M->matrix[JS[k]][j]);
		 for (int i = 0; i < n; i++)
			 if (IS[k] != k) swap(&M->matrix[i][k], &M->matrix[i][IS[k]]);
	 }
	 free(JS);
	 free(IS);
	 return M;
 }

//取特定的列
 Mat colRange(Mat M, int A, int B) {
	 int i, j;
	 int rows, cols;
	 if (M == NULL) {
		 printf("\nError in mCopy, M == NULL \n");
	 }
	 rows = M->rows; cols = B - A;
	 Mat M2 = init(rows, cols);
	 for (i = 0; i < rows; i++) {
		 for (j = A; j < B; j++) {
			 M2->matrix[i][j-A] = M->matrix[i][j];
		 }
	 }
	 return M2;
	
 }

 //取特定的行
 Mat rowRange(Mat M, int A, int B) {
	 int i, j;
	 int rows, cols;
	 if (M == NULL) {
		 printf("\nError in mCopy, M == NULL \n");
	 }
	 rows = B - A; cols = M->cols;
	 Mat M2 = init(rows, cols);
	 for (i = A; i < B; i++) {
		 for (j = 0; j < cols; j++) {
			 M2->matrix[i-A][j] = M->matrix[i][j];
		 }
	 }
	 return M2;
 }

//拷贝
 Mat clone(Mat M) {
	int i, j;
	int rows, cols;
	if (M == NULL) {
		printf("\nError in mCopy, M == NULL \n");
	}
	rows = M->rows, cols = M->cols;
	Mat M2 = init(rows, cols);
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			M2->matrix[i][j] = M->matrix[i][j];
		}
	}
	return M2;
}
 
void clear(Mat M) {
	for (int i = 0; i < M->rows; i++) {
		delete[] M->matrix[i];
	}
	delete[] M->matrix;
}

//SVD分解求特征值，返回的结果存入 Mat[2],分别是特征向量的矩阵和一行特征值 
Mat* eigen(Mat M) {

	int nDim = M->rows;
	float dbEps = 0.0000001;//精度要求
	int nJt = 50000;//最大迭代次数  
	int i, j, k;
	float * pMatrix = (float*)malloc(sizeof(float)*nDim*nDim);

	float *pdblVects = (float*)malloc(sizeof(float)*nDim*nDim);
	float *pdbEigenValue = (float*)malloc(sizeof(float)*nDim);

	printf("Computing eigen value, about 3 minuts......\n");
	//返回的结果
	Mat* Result = (Mat*)malloc(sizeof(MatStruct) * 2);
	Mat MpdblVects = init(nDim, nDim);//特征向量(特征向量按列存储)  
	Mat MpdbEigenValue = init(nDim, nDim);//特征值(n*n，存在对角线上)
	Result[0] = MpdblVects;
	Result[1] = MpdbEigenValue;
	//
	for (int i = 0; i < nDim; i++) {
		for (int j = 0; j < nDim; j++) {
			pMatrix[i*nDim + j] = M->matrix[i][j];
		}
	}

	for (int i = 0; i < nDim; i++) {
		pdblVects[i*nDim + i] = 1.0f;
		for (int j = 0; j < nDim; j++) {
			if (i != j)
				pdblVects[i*nDim + j] = 0.0f;
		}
	}

	int nCount = 0;     //迭代次数  
	while (1) {
		//在pMatrix的非对角线上找到最大元素  
		float dbMax = pMatrix[1];
		int nRow = 0;
		int nCol = 1;
		for (int i = 0; i < nDim; i++)          //行  
		{
			for (int j = 0; j < nDim; j++)      //列  
			{
				float d = fabs(pMatrix[i*nDim + j]);

				if ((i != j) && (d> dbMax))
				{
					dbMax = d;
					nRow = i;
					nCol = j;
				}
			}
		}

		if (dbMax < dbEps)     //精度符合要求   
			break;

		if (nCount > nJt)       //迭代次数超过限制  
			break;

		nCount++;

		float dbApp = pMatrix[nRow*nDim + nRow];
		float dbApq = pMatrix[nRow*nDim + nCol];
		float dbAqq = pMatrix[nCol*nDim + nCol];

		//计算旋转角度  
		float dbAngle = 0.5*atan2(-2 * dbApq, dbAqq - dbApp);
		float dbSinTheta = sin(dbAngle);
		float dbCosTheta = cos(dbAngle);
		float dbSin2Theta = sin(2 * dbAngle);
		float dbCos2Theta = cos(2 * dbAngle);

		pMatrix[nRow*nDim + nRow] = dbApp*dbCosTheta*dbCosTheta +
			dbAqq*dbSinTheta*dbSinTheta + 2 * dbApq*dbCosTheta*dbSinTheta;
		pMatrix[nCol*nDim + nCol] = dbApp*dbSinTheta*dbSinTheta +
			dbAqq*dbCosTheta*dbCosTheta - 2 * dbApq*dbCosTheta*dbSinTheta;
		pMatrix[nRow*nDim + nCol] = 0.5*(dbAqq - dbApp)*dbSin2Theta + dbApq*dbCos2Theta;
		pMatrix[nCol*nDim + nRow] = pMatrix[nRow*nDim + nCol];

		for (int i = 0; i < nDim; i++)
		{
			if ((i != nCol) && (i != nRow))
			{
				int u = i*nDim + nRow;  //p    
				int w = i*nDim + nCol;  //q  
				dbMax = pMatrix[u];
				pMatrix[u] = pMatrix[w] * dbSinTheta + dbMax*dbCosTheta;
				pMatrix[w] = pMatrix[w] * dbCosTheta - dbMax*dbSinTheta;
			}
		}

		for (int j = 0; j < nDim; j++)
		{
			if ((j != nCol) && (j != nRow))
			{
				int u = nRow*nDim + j;  //p  
				int w = nCol*nDim + j;  //q  
				dbMax = pMatrix[u];
				pMatrix[u] = pMatrix[w] * dbSinTheta + dbMax*dbCosTheta;
				pMatrix[w] = pMatrix[w] * dbCosTheta - dbMax*dbSinTheta;
			}
		}

		//计算特征向量  
		for (int i = 0; i < nDim; i++)
		{
			int u = i*nDim + nRow;      //p     
			int w = i*nDim + nCol;      //q  
			dbMax = pdblVects[u];
			pdblVects[u] = pdblVects[w] * dbSinTheta + dbMax*dbCosTheta;
			pdblVects[w] = pdblVects[w] * dbCosTheta - dbMax*dbSinTheta;
		}

	}

	//对特征值进行排序以及重新排列特征向量,特征值即pMatrix主对角线上的元素  
	//std::map<float,int> mapEigen;  
	float* mapEigen = (float*)malloc(sizeof(float)*nDim);
	int* mapEigen2 = (int*)malloc(sizeof(int)*nDim);

	for (int i = 0; i < nDim; i++)
	{
		pdbEigenValue[i] = pMatrix[i*nDim + i];
		mapEigen[i] = pdbEigenValue[i];
		mapEigen2[i] = i;
	}

	float *pdbTmpVec = (float*)malloc(sizeof(float)*nDim*nDim);
	//std::map<float,int>::reverse_iterator iter = mapEigen.rbegin();  
	for (int j = 0; j < nDim; ++j)
	{
		int temp_j = nDim - 1 - j;
		for (int i = 0; i < nDim; i++)
		{
			pdbTmpVec[i*nDim + j] = pdblVects[i*nDim + mapEigen2[temp_j]];
		}

		//特征值重新排列  
		pdbEigenValue[j] = mapEigen[temp_j];
	}

	//设定正负号  
	for (int i = 0; i < nDim; i++)
	{
		float dSumVec = 0;
		for (int j = 0; j < nDim; j++)
			dSumVec += pdbTmpVec[j * nDim + i];
		if (dSumVec<0)
		{
			for (int j = 0; j < nDim; j++)
				pdbTmpVec[j * nDim + i] *= -1;
		}
	}

	for (i = 0; i<nDim; i++) {
		for (j = 0; j<nDim; j++) {
			pdblVects[i*nDim + j] = pdbTmpVec[i*nDim + j];
		}
	}

	for (i = 0; i<nDim; i++) {
		for (j = 0; j<nDim; j++) {

			MpdblVects->matrix[i][j] = pdblVects[i*nDim + j];
		}

		MpdbEigenValue->matrix[i][i] = pdbEigenValue[i];
	}

	free(pdblVects);
	free(pdbTmpVec);
	free(pdbEigenValue);
	free(pMatrix);
	free(mapEigen2);
	free(mapEigen);
	return Result;
}

void swap(float* pa, float* pb) {
	float temp = *pa;
	*pa = *pb;
	*pb = temp;
}