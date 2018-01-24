#include "moving_average.h"

uint32_T moving_average(uint32_T* data, uint32_T* output, uint32_T data_length, uint32_T window_size) {
	/* check inputs */
	if( 
		(output == NULL) || 
		(data == NULL) || 
		(data_length == 0) || 
		(window_size == 0) || 
		(window_size > data_length) 
	) {
		return 0;
	}
		
	{ /* computation block */
		uint32_T sum = data[0];
		uint32_T i;
		
		output[0] = sum;
		
		/* computes until the window passes fully into the signal */		
		for(i = 1; i < window_size; i++) {
			sum += data[i];
			output[i] = sum / (i + 1);				
		}
		
		/* computes till the window reaches the end of the sequence */
		for(; i < data_length; i++) {
			sum += data[i] - data[i - window_size];
			output[i] = sum / window_size;
		}
		return 1;
	}
}

/* MATLAB entry point */
void mexFunction( int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[] ) {	

	{ /* check number of input/output arguments */
		if(nlhs != 1) {
			mexErrMsgTxt("Number of left handside elements should be 1");
		}
		if(nrhs != 2) {
			mexErrMsgTxt("Number of right handside elements should be 2");
		}
	}
	
	{ /* check and parse data */
		const uint32_T scalar_dimension = 2; 					/* nr of dimensions of a 1 x 1 matrix*/
		const mxArray* data = prhs[0];							/* data passed by MATLAB */		
		mxArray* output;										/* data to be passed to MATLAB*/
		const mxArray* window_size_matrix = prhs[1];			/* 1 x 1 MATLAB matrix that stores the window size*/
		const uint32_T data_size = (uint32_T) mxGetN(data);		
		int dimension_size[2];									
		uint32_T* window_size_ptr;
		uint32_T check_value;
		
		dimension_size[0] = 1;									/* 1 x data size */
		dimension_size[1] = data_size;
		
				
		if( (mxGetNumberOfDimensions(data) > 2) || (mxGetM(data) > 1) ) {
			mexErrMsgTxt("Data must be a 1 x N matrix");
		}
		
		if(	(mxGetM(window_size_matrix) > 1) || 
			(mxGetN(window_size_matrix) > 1) || 
			(mxGetNumberOfDimensions(window_size_matrix) > 2) 
		) {
			mexErrMsgTxt("Window Size must be a 1 x 1 matrix");
		}
 	
		if(!mxIsUint32(data)) {	
			mexErrMsgTxt("Data must be of type uint32");
		}
		
		if(!mxIsUint32(window_size_matrix)) {
			mexErrMsgTxt("Window Size must be of type uint32");
		}
		
		window_size_ptr = (uint32_T*) mxGetData(window_size_matrix);
		if(data_size < (*window_size_ptr) ) {
			mexErrMsgTxt("Window Size must be < or = than the size of data");
		}
		
		output = mxCreateNumericArray(scalar_dimension, dimension_size, mxUINT32_CLASS, mxREAL);
		plhs[0] = output;
		
		
		check_value = moving_average(mxGetData(data), mxGetData(output), data_size, *window_size_ptr);
		
		if(check_value == 0 ) {
			mexErrMsgTxt("C function error");
		}	
	}		
}








