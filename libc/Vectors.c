//----------------------------------------------------------------------------------------------------
/*
Copyright (C) 31.08.2021 Aleksei Likhachev
 
This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the KI General Public License version for more details.
*/
//----------------------------------------------------------------------------------------------------


/**
 Metacode for C polymorphism implementation for working with data vectors and tuples.
 */
#ifndef TEMPLATE
#define TEMPLATE
#ifdef T 

	_GET_FUNCTION(__vector2, T)* _GET_NAME(vector2_create, T)(void)
	{
		static _GET_NAME(__vector2, T)* vector;
		vector = malloc(sizeof(_GET_NAME(__vector2, T)));
		if(vector == NULL){
			_vector_error_id = _VECTOR_OUT_OF_MEMORY;
			return NULL;
		}
		else{
			return vector;
			
		}
	}

	_GET_FUNCTION(__vector3, T)* _GET_NAME(vector3_create, T)(void)
	{
		static _GET_NAME(__vector3, T)* vector;
		vector = malloc(sizeof(_GET_NAME(__vector3, T)));
		if(vector == NULL){
			_vector_error_id = _VECTOR_OUT_OF_MEMORY;
			return NULL;
		}
		else{
			return vector;
			
		}
	}

	_GET_FUNCTION(__vector4, T)* _GET_NAME(vector4_create, T)(void)
	{
		static _GET_NAME(__vector4, T)* vector;
		vector = malloc(sizeof(_GET_NAME(__vector4, T)));
		if(vector == NULL){
			_vector_error_id = _VECTOR_OUT_OF_MEMORY;
			return NULL;
		}
		else{
			return vector;
			
		}
	}

	_GET_FUNCTION(__vector5, T)* _GET_NAME(vector5_create, T)(void)
	{
		static _GET_NAME(__vector5, T)* vector;
		vector = malloc(sizeof(_GET_NAME(__vector5, T)));
		if(vector == NULL){
			_vector_error_id = _VECTOR_OUT_OF_MEMORY;
			return NULL;
		}
		else{
			return vector;
		}
	}

	_GET_FUNCTION(__vectorN, T)* _GET_NAME(vectorN_create, T)(void)
	{
		static _GET_NAME(__vectorN, T)* vector;
		vector = malloc(sizeof(_GET_NAME(__vectorN, T)));
		if(vector == NULL){
			_vector_error_id = _VECTOR_OUT_OF_MEMORY;
			return NULL;
		}
		else{
			return vector;
		}
	}

#ifdef T2
    _GET_FUNCTION(__vector, T2)* _GET_NAME(vector_create, T2)(uint32_t size){
        static _GET_NAME(__vector, T2)* vector;
        vector = malloc(sizeof(_GET_NAME(__vector, T2)));
        vector->data = (T2**)malloc(size*sizeof(T2**));
        vector->size = size;
        if(vector == NULL && vector->data == NULL){
            _vector_error_id = _VECTOR_OUT_OF_MEMORY;
            return NULL;
        }
        else{
            return vector;
        }
    }
#endif

	void _GET_NAME(vector2_init, T)(_GET_NAME(__vector2, T)* left, T param1, T param2)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{
			left->x = param1;
			left->y = param2;
		}
	}

	void _GET_NAME(vector3_init, T)(_GET_NAME(__vector3, T)* left, T param1, T param2, T param3)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{
			left->x = param1;
			left->y = param2;
			left->z = param3;
		}
	}

	void _GET_NAME(vector4_init, T)(_GET_NAME(__vector4, T)* left, T param1, T param2, T param3, T param4)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{
			left->x = param1;
			left->y = param2;
			left->z = param3;
			left->t = param4;
		}
	} 

	void _GET_NAME(vector5_init, T)(_GET_NAME(__vector5, T)* left, T param1, T param2, T param3, T param4, T param5)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{
			left->x = param1;
			left->y = param2;
			left->z = param3;
			left->t = param4;
			left->w = param5;
		}
	}

	void _GET_NAME(vectorN_init, T)(_GET_NAME(__vectorN, T)* left, T* array, uint16_t size)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{
			left->data = malloc(size * sizeof(T));
			left->size = size;
			if(left->data != NULL){
				for(uint16_t i = 0; i < size; ++i){
					*(left->data + i) = *(array + i);
				}
			} else{
				_vector_error_id = _VECTOR_OUT_OF_MEMORY;
			}
		}
	}

#ifdef T2
    void _GET_NAME(vector_init, T2)(_GET_NAME(__vector, T2)* left, T2 ** array, uint32_t size){
        if(left == NULL){
            _vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
        }
        else{
            left->size = size;
            if(left->data != NULL){
                for(uint32_t i = 0; i < size; ++i){
                    //*(left->data + i) = *(array + i);
                    left->data[i] = array[i];
                    xln_print(left->data[i]);
                }
                
            } else{
                _vector_error_id = _VECTOR_OUT_OF_MEMORY;
            }
        }
    }

#endif

	double _GET_NAME(vector2_norm, T)(_GET_NAME(__vector2, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
            return 0;
		}
		else{
			return sqrt((left->x)*(left->x) + (left->y)*(left->y));
		}
	}

	double _GET_NAME(vector3_norm, T)(_GET_NAME(__vector3, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
            return 0;
		}
		else{
			return sqrt((left->x)*(left->x) + (left->y)*(left->y) + (left->z)*(left->z));
		}
	}

	double _GET_NAME(vector4_norm, T)(_GET_NAME(__vector4, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
            return 0;
		}
		else{
			return sqrt((left->x)*(left->x) + (left->y)*(left->y) + (left->z)*(left->z) + (left->t)*(left->t));
		}
	}

	double _GET_NAME(vector5_norm, T)(_GET_NAME(__vector5, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
            return 0;
		}
		else{
			return sqrt((left->x)*(left->x) + (left->y)*(left->y) + (left->z)*(left->z) + (left->t)*(left->t)
				       + (left->w)*(left->w));
		}
	}

	double _GET_NAME(vectorN_norm, T)(_GET_NAME(__vectorN, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
			return 0;
		}
		else{
			double norm = 0.00;
			for(uint16_t i = 0; i < left->size; ++i){
				norm += left->data[i] * left->data[i];
			}
			return sqrt(norm);
		}
	}

	_GET_FUNCTION(__vector2, double)* _GET_NAME(vector2_normalize, T)(_GET_NAME(__vector2, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
			return NULL;
		}
		else{
			double norm = _GET_FUNCTION(vector2_norm, T)(left);
			__vector2_double* result = vector2_create_double();
			//_GET_NAME(__vector2, T)* result = _GET_FUNCTION(vector2_create, T)();
			if(result == NULL){
				_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
				return NULL;
			}
			else {
				if(norm == 0){
					vector2_init_double(result, 0.0, 0.0);
					//_GET_FUNCTION(vector2_init, T)(result, 0.0, 0.0);
					return result;
				}
				else{
					vector2_init_double(result, (left->x/norm), (left->y/norm));
					//_GET_FUNCTION(vector2_init, T)(result, (left->x/norm), (left->y/norm));
					return result;
				}
			}
		}
	}

	_GET_FUNCTION(__vector3, double)* _GET_NAME(vector3_normalize, T)(_GET_NAME(__vector3, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
			return NULL;
		}
		else{
			double norm = _GET_NAME(vector3_norm, T)(left);
			__vector3_double* result = vector3_create_double();
			if(result == NULL){
				_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
				return NULL;
			}
			else {
				if(norm == 0){
					vector3_init_double(result, 0.0, 0.0, 0.0);
				}
				else {
					vector3_init_double(result, (left->x/norm), (left->y/norm), (left->z/norm));
				}
				return result;
			}
		}
	}

	_GET_FUNCTION(__vector4, double)* _GET_NAME(vector4_normalize, T)(_GET_NAME(__vector4, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
			return NULL;
		}
		else{
			double norm = _GET_NAME(vector4_norm, T)(left);
			__vector4_double* result = vector4_create_double();
			if(result == NULL){
				_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
				return NULL;
			}
			else {
				if(norm == 0){
					vector4_init_double(result, 0.0, 0.0, 0.0, 0.0);
				}
				else{
					vector4_init_double(result, (left->x/norm), (left->y/norm), (left->z/norm), (left->t/norm));
				}
				return result;
			}
		}
	}

	_GET_FUNCTION(__vector5, double)* _GET_NAME(vector5_normalize, T)(_GET_NAME(__vector5, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
			return NULL;
		}
		else{
			double norm = _GET_NAME(vector5_norm, T)(left);
			__vector5_double* result = vector5_create_double();
			if(result == NULL){
				_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
				return NULL;
			}
			else {
				if(norm == 0){
					vector5_init_double(result, 0.0, 0.0, 0.0, 0.0, 0.0);
				}
				vector5_init_double(result, (left->x/norm), (left->y/norm), (left->z/norm), (left->t/norm), (left->w/norm));
				return result;
			}
		}
	}

	_GET_FUNCTION(__vectorN, double)* _GET_NAME(vectorN_normalize, T)(_GET_NAME(__vectorN, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
			return NULL;
		}
		else{
			double norm = _GET_FUNCTION(vectorN_norm, T)(left);
			__vectorN_double* result = vectorN_create_double();
			//_GET_NAME(__vector2, T)* result = _GET_FUNCTION(vector2_create, T)();
			if(result == NULL){
				_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
				return NULL;
			}
			else {
				if(norm == 0){
					for(uint16_t i = 0; i < left->size; ++i){
						left->data[i] = 0.0;
					}
					return result;
				}
				else{
					for(uint16_t i = 0; i < left->size; ++i){
						left->data[i] /= norm;
					}
					return result;
				}
			}
		}
	}

	void _GET_NAME(vector2_add, T)(_GET_NAME(__vector2, T)* result, _GET_NAME(__vector2, T)* left, _GET_NAME(__vector2, T)* right)
	{
		if(left == NULL || right == NULL || result == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{
				result->x = left->x + right->x;
				result->y = left->y + right->y; 
		}
	}

	void _GET_NAME(vector3_add, T)(_GET_NAME(__vector3, T)* result, _GET_NAME(__vector3, T)* left, _GET_NAME(__vector3, T)* right)
	{
		if(left == NULL || right == NULL || result == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{
			result->x = left->x + right->x;
			result->y = left->y + right->y;
			result->z = left->z + right->z;
		}
	}

	void _GET_NAME(vector4_add, T)(_GET_NAME(__vector4, T)* result, _GET_NAME(__vector4, T)* left, _GET_NAME(__vector4, T)* right)
	{
		if(left == NULL || right == NULL || result == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{
			result->x = left->x + right->x;
			result->y = left->y + right->y;
			result->z = left->z + right->z;
			result->t = left->t + right->t;
		}
	}

	void _GET_NAME(vector5_add, T)(_GET_NAME(__vector5, T)* result, _GET_NAME(__vector5, T)* left, _GET_NAME(__vector5, T)* right)
	{
		if(left == NULL || right == NULL || result == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{
			result->x = left->x + right->x;
			result->y = left->y + right->y;
			result->z = left->z + right->z;
			result->t = left->t + right->t;
			result->w = left->w + right->w;
		}
	}

	void _GET_NAME(vectorN_add, T)(_GET_NAME(__vectorN, T)* result, _GET_NAME(__vectorN, T)* left, _GET_NAME(__vectorN, T)* right)
	{
		if(left == NULL || right == NULL || result == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{

			if(left->size > right->size){
				_GET_FUNCTION(vectorN_init, T)(result, left->data, left->size);
			} else{
				_GET_FUNCTION(vectorN_init, T)(result, right->data, right->size);
			}
			for(uint16_t i = 0; i < (left->size < right->size ? left->size : right->size); ++i){
				result->data[i] = left->data[i] + right->data[i];
			}
		}
	}

	void _GET_NAME(vector2_free, T)(_GET_NAME(__vector2, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{
			free(left);
		}
	}

	void _GET_NAME(vector3_free, T)(_GET_NAME(__vector3, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{
			free(left);
		}
	}

	void _GET_NAME(vector4_free, T)(_GET_NAME(__vector4, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{
			free(left);
		}
	}

	void _GET_NAME(vector5_free, T)(_GET_NAME(__vector5, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{
			free(left);
		}
	}

	void _GET_NAME(vectorN_free, T)(_GET_NAME(__vectorN, T)* left)
	{
		if(left == NULL){
			_vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
		}
		else{
			free(left->data);
			free(left);
		}
	}

    #ifdef T2
    void _GET_NAME(vector_free, T2)(_GET_NAME(__vector, T2)* left)
    {
        if(left == NULL){
            _vector_error_id = _VECTOR_ATTEMPT_TO_ACCESS_A_NONEXISTENT_ENTITY;
        }
        else{
            free(left->data);
            free(left);
        }
    }
    #endif



	#undef T
#endif
#undef TEMPLATE
#endif





