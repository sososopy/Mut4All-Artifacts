
template < class T > struct S1 {
       	struct S2 {
	       	friend struct S1 <T>::S3;
	};
};

