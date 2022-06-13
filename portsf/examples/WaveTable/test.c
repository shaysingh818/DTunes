ARG_RES decode_arguments(int argc, char **argv){
	int i = 0;
	char *a = argv[0];
	while (--argc) {
		a = argv[i];
		if (a[0]=='-') { /* tag found */
			switch (a[1]) {
				case 'w': /* Waveform */
					if (argc == 0) {
						printf("No waveform argument\n"); exit(1);
					}
					wavename = argv[i+1];
					argc--; i += 2;
					break;
				case 'f': /* formula name */
					if (argc == 0) {
						printf("No waveform argument\n"); exit(1);
					}
					if (strcmp(argv[i+1], "sine")==0){
						formula = SINE;
					}else if (strcmp(argv[i+1], "cosine")==0){
						formula = COSINE;
					}else { /* we do not have this function yet */417
						printf("No valid formula name\n"); exit(1);
					}
					argc--; i += 2;
					break;
				case 's':
					if (argc == 0) {
						printf("No speed given\n"); exit(1);
					}
					speed = atof(argv[i+1]);
					argc--; i += 2;
					break;
				case 't':
					if (argc == 0) {
						printf("No duration given\n"); exit(1);
					}
					duration = atof(argv[i+1]);
					argc--; i += 2;
					break;
				case 'o':
					if (argc == 0) {
						printf("No output argument\n"); exit(1);
					}
					output = argv[i+1];
					argc--; i += 2;
					break;
				}
		}else {
			printf("No tag found\n"); exit(1);
		}
	}
	/* Now the checks */
	if (output == NULL) {
		printf("No output file given\n"); exit(1);
	}
	if (wavename == NULL && formula == -1) {
		printf("Neither waveform nor formula specified\n");
		exit(1);
	}
	if (wavename != NULL && formula != -1) {
		printf("Both waveform and formula specified\n");
		exit(1);
	}

	if (wavename == NULL) return FORMULA_GIVEN;
	else return WAVEFORM_GIVEN;
}
