#include "../../src/include/CCEXP.hpp"

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Please provide a filename\n");
		return -1;
	}

	printf("\n-------------------------------------------------------------\n");
	printf("CCEXP Library (%1.3f): "
				 "https://github.com/terablade2001/CCEXP\n",
				 CCEXP_VERSION
	);
	printf("------- CCEXP Viewer >> [%s] ------------\n", argv[1]);
	__CCEXP_PRINT_TABLES__(argv[1]);
	printf("-------------------------------------------------------------\n\n");

	return 0;
}
