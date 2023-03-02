#include <stdio.h>
#include "ff.h"

void rule0_print(codeLine * cd, FILE * fp){
	if(cd->label != -1){
            fprintf(fp,"\nL%d: \n", cd->label);
	}
}

void rule1_print(codeLine * cd, FILE * fp){ // x = y op z
	if(cd->label!=-1){
                fprintf(fp,"\nL%d: \n", cd->label);
            }

	if(cd->op == 0){
			if(cd->arg1type == 4 && cd->arg2type == 4 && cd->restype == 4){
				fprintf(fp, "mov EAX, [temp%d]\n",cd->arg1->temp);
				fprintf(fp, "add EAX, [temp%d]\n",cd->arg2->temp);
				fprintf(fp, "mov [temp%d], EAX\n",cd->res->temp);
			}
			else if(cd->arg1type == 1 && cd->arg2type == 4 && cd->restype == 4){
				fprintf(fp, "mov EAX, %d\n",cd->arg1->num);
				fprintf(fp, "add EAX, [temp%d]\n",cd->arg2->temp);
				fprintf(fp, "mov [temp%d], EAX\n",cd->res->temp);
			}
			else if(cd->arg1type == 4 && cd->arg2type == 1 && cd->restype == 4){
				fprintf(fp, "mov EAX, [temp%d]\n",cd->arg1->temp);
				fprintf(fp, "add EAX, %d\n",cd->arg2->num);
				fprintf(fp, "mov [temp%d], EAX\n",cd->res->temp);
			}
			else{
				printf("Invalid Arguments Recieved Rule 1 Op 0\n");
			}
			// else if(cd->arg2type == 25){
			// 	fprintf(fp, "movss xmm0, %f\n",cd->arg1->rnum); //Floating Point Nummbers are 4 bytes
			// 	fprintf(fp, "movss xmm0, [%s]\n",cd->arg2->id->lexeme);
			// 		if(cd->restype = 25)
			// 		fprintf(fp, "movss [%s],xmm0\n",cd->res->id->lexeme);
			// 		else if(cd->restype = 27)
			// 		fprintf(fp, "mov [temp%d], AX\n",cd->res->temp);
			// }
        
	}
	else if(cd->op == 1){
			if(cd->arg1type == 4 && cd->arg2type == 4 && cd->restype == 4){
				fprintf(fp, "mov EAX, [temp%d]\n",cd->arg1->temp);
				fprintf(fp, "sub EAX, [temp%d]\n",cd->arg2->temp);
				fprintf(fp, "mov [temp%d], EAX\n",cd->res->temp);
			}
			else if(cd->arg1type == 1 && cd->arg2type == 4 && cd->restype == 4){
				fprintf(fp, "mov EAX, %d\n",cd->arg1->num);
				fprintf(fp, "sub EAX, [temp%d]\n",cd->arg2->temp);
				fprintf(fp, "mov [temp%d], EAX\n",cd->res->temp);
			}
			else if(cd->arg1type == 4 && cd->arg2type == 1 && cd->restype == 4){
				fprintf(fp, "mov EAX, [temp%d]\n",cd->arg1->temp);
				fprintf(fp, "sub EAX, %d\n",cd->arg2->num);
				fprintf(fp, "mov [temp%d], EAX\n",cd->res->temp);
			}
			else{
				printf("Invalid Arguments Recieved Rule 1 Op 1\n");
			}		
        }

	else if(cd->op == 2){
			if(cd->arg1type == 4 && cd->arg2type == 4 && cd->restype == 4){
				fprintf(fp, "mov EAX, [temp%d]\n",cd->arg1->temp);
				fprintf(fp, "imul dword[temp%d]\n",cd->arg2->temp);
				fprintf(fp, "mov [temp%d], EAX\n",cd->res->temp);
			}
			else if(cd->arg1type == 1 && cd->arg2type == 4 && cd->restype == 4){
				fprintf(fp, "mov EAX, %d\n",cd->arg1->num);
				fprintf(fp, "imul dword[temp%d]\n",cd->arg2->temp);
				fprintf(fp, "mov [temp%d], EAX\n",cd->res->temp);
			}
			else if(cd->arg1type == 4 && cd->arg2type == 1 && cd->restype == 4){
				fprintf(fp, "mov EAX, %d\n",cd->arg2->num);
				fprintf(fp, "imul dword[temp%d]\n",cd->arg1->temp);
				fprintf(fp, "mov [temp%d], EAX\n",cd->res->temp);
			}
			else{
				printf("Invalid Arguments Recieved Rule 1 Op 2\n");
			}
        }

	else if(cd->arg2type == 3){
			if(cd->arg1type == 4 && cd->arg2type == 4 && cd->restype == 4){
				fprintf(fp, "mov EAX, [temp%d]\n",cd->arg2->temp);
				fprintf(fp, "idiv dword[temp%d]\n",cd->arg1->temp);
				fprintf(fp, "mov [temp%d], EAX\n",cd->res->temp);
			}
			else if(cd->arg1type == 1 && cd->arg2type == 4 && cd->restype == 4){
				fprintf(fp, "mov EAX, %d\n",cd->arg1->num);
				fprintf(fp, "idiv dword[temp%d]\n",cd->arg2->temp);
				fprintf(fp, "mov [temp%d], EAX\n",cd->res->temp);
			}
			else if(cd->arg1type == 4 && cd->arg2type == 1 && cd->restype == 4){
				fprintf(fp, "mov EAX, [temp%d]\n",cd->arg1->temp);
				fprintf(fp, "idiv %d\n",cd->arg2->num);
				fprintf(fp, "mov [temp%d], EAX\n",cd->res->temp);
			}
			else{
				printf("Invalid Arguments Recieved Rule 1 op 3\n");
			}
        }	
}

void rule2_print(codeLine * cd, FILE * fp){ // x = op y
	if(cd->label!=-1){
                fprintf(fp,"L%d: \n", cd->label);
            }

		if(cd->op == 0){
			if(cd->arg1type == 1){
				if(cd->restype == 4){
				//addtemptoint(cd->res->temp);
				fprintf(fp, "mov dword[temp%d], %d\n", cd->res->temp,cd->arg1->num);
				}
				else 
				fprintf(fp, "mov dword[%s],%d\n", cd->res->id->lexeme,cd->arg1->num);
			}
			// else if(cd->arg1type == 2){
			// 	if(cd->restype == 4){
			// 	///addtemptofloat(cd->res->temp);	
			// 	fprintf(fp, "movss [temp%d], %f\n", cd->res->temp,cd->arg1->rnum);
			// 	}
			// 	else
			// 	fprintf(fp, "movss [%s],%f\n", cd->res->id->lexeme,cd->arg1->rnum);
			// }
			else if(cd->arg1type == 4){
					if(cd->restype == 4){
					//addtemptoint(cd->res->temp);
					fprintf(fp, "mov EAX,[temp%d]\n", cd->arg1->temp);
					fprintf(fp, "mov [temp%d] , EAX\n", cd->res->temp);
					}
					
				}

				// if(cd->arg1->id->type == 25){
				// 	if(cd->restype == 4){
				// 	//addtemptofloat(cd->res->temp);	
				// 	fprintf(fp, "mov [temp%d], [%s]\n", cd->res->temp,cd->arg1->id->lexeme);
				// 	}
				// 	else 
				// 	fprintf(fp, "mov [%s],[%s]\n", cd->res->id->lexeme,cd->arg1->id->lexeme);
				// }
			
		}
		else if(cd->op == 1){
			if(cd->arg1type == 1 && cd->restype == 4){
				//fprintf(fp,"push AX\n");
				fprintf(fp,"mov EAX, %d\n",cd->arg1->num);
				fprintf(fp,"not EAX\n");
				fprintf(fp,"add EAX, 1\n");
				
				//addtemptoint(cd->res->temp);
				fprintf(fp, "mov [temp%d], EAX\n", cd->res->temp);
				//fprintf(fp, "pop AX\n");
				
				// else 
				// {	
				// fprintf(fp, "mov [%s],AX\n", cd->res->id->lexeme);
				// //fprintf(fp, "pop AX\n");
				// }
			}
			//else if(cd->arg1type == 2 && cd->restype == 4){
				
				//addtemptofloat(cd->res->temp);	
				//fprintf(fp, "movss [temp%d], %f\n", cd->res->temp,cd->arg1->rnum);
				
				// else
				// fprintf(fp, "movss [%s],%f\n", cd->res->id->lexeme,cd->arg1->rnum);
			//}
			else if(cd->arg1type == 4 && cd->restype == 4){
					//addtemptoint(cd->res->temp);	
					fprintf(fp, "mov EAX, [temp%d]\n", cd->arg1->temp);
					fprintf(fp,"not EAX\n");
					fprintf(fp,"add EAX, 1\n");	 
					fprintf(fp, "mov [temp%d], EAX\n", cd->res->temp);		
			}
		}			
}

void rule3_print(codeLine * cd, FILE * fp){  // x = y
	//printf("Entered");

	if(cd->label!=-1){
                fprintf(fp,"L%d: \n", cd->label);
            }
	if(cd->arg1type == 1){
				//addtemptoint(cd->res->temp);
				fprintf(fp, "mov dword[temp%d], %d\n", cd->res->temp,cd->arg1->num);				 
		}
	else if(cd->arg1type == 2){
				//addtemptofloat(cd->res->temp);	
				fprintf(fp, "movss qword[temp%d], %f\n", cd->res->temp,cd->arg1->rnum);
		}

	else if(cd->arg1type == 3){
				fprintf(fp, "mov AX, %d\n",cd->arg1->bool);
				//fprintf(fp,"and AH, 0\n");
				fprintf(fp, "mov [temp%d], AX\n",cd->res->temp);
				fprintf(fp, "mov word[temp%d + 2], 0",cd->arg1->temp);
				}
	else if(cd->arg1type == 4){
				fprintf(fp, "mov EAX, [temp%d]\n",cd->arg1->temp);
				fprintf(fp, "mov [temp%d], EAX\n",cd->res->temp);							 					
				}
	else if(cd->arg1type == 6){
				fprintf(fp, "mov EAX, [rettemp%d]\n",cd->arg1->ret);
				fprintf(fp, "mov [temp%d], EAX\n",cd->res->temp);							 					
				}					
	// else if(cd->arg1->id->type == 25){
	// 			// 	if(cd->restype == 4){
	// 			// 	//addtemptofloat(cd->res->temp);	
	// 	    	fprintf(fp, "movss [temp%d], [%s]\n", cd->res->temp,cd->arg1->id->lexeme);
	// 			// 	}
	// 			// 	else 
	// 			// 	fprintf(fp, "mov [%s],[%s]\n", cd->res->id->lexeme,cd->arg1->id->lexeme);
	// 			}			
		else{
			printf("Invalid Arguments Rule 3\n");
		}
	
}

void rule4_print(codeLine * cd, FILE * fp){ //goto L
	//printf("Entered");

	if(cd->label!=-1){
        fprintf(fp,"\nL%d: \n", cd->label);
    }
	if(cd->arg1type == 5)
		fprintf(fp,"jmp L%d\n",cd->arg1->label);
	else
		printf("Invalid Agruments Recieved Rule 4\n");
}

void rule6_print(codeLine * cd, FILE * fp){  //if x relop y goto L
	//printf("Entered");

	if(cd->label!=-1){
        fprintf(fp,"L%d: ", cd->label);
    }

		if(cd->op == 4){
			if(cd->arg1type == 3){
				fprintf(fp,"mov AX, %d\n",cd->arg1->bool);
				fprintf(fp,"cmp AX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jl L%d\n",cd->res->label);
			}
			else if(cd->arg2type == 3){
				fprintf(fp,"mov AX, %d\n",cd->arg2->bool);
				fprintf(fp,"cmp [temp%d], AX\n",cd->arg1->temp);
				fprintf(fp,"jl L%d\n",cd->res->label);
			}
			else if(cd->arg1type == 1){
				fprintf(fp,"mov EAX, %d\n",cd->arg1->num);
				fprintf(fp,"cmp EAX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jl L%d\n",cd->res->label);
			}
			else if(cd->arg2type == 1){
				fprintf(fp,"mov EAX, %d\n",cd->arg2->num);
				fprintf(fp,"cmp [temp%d], EAX\n",cd->arg1->temp);
				fprintf(fp,"jl L%d\n",cd->res->label);
			}
			else if(cd->arg1type == 4 && cd->arg2type == 4){
				fprintf(fp,"mov EAX,[temp%d]\n",cd->arg1->temp);
				fprintf(fp,"cmp EAX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jl L%d\n",cd->res->label);
			}
			else{
				printf("Invalid Agruments Rule 6 Op 4\n");
			}
		}
		else if(cd->op == 5){
			if(cd->arg1type == 3){
				fprintf(fp,"mov AX, %d\n",cd->arg1->bool);
				fprintf(fp,"cmp AX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jle L%d\n",cd->res->label);
			}
			else if(cd->arg2type == 3){
				fprintf(fp,"mov AX, %d\n",cd->arg2->bool);
				fprintf(fp,"cmp [temp%d], AX\n",cd->arg1->temp);
				fprintf(fp,"jle L%d\n",cd->res->label);
			}
			else if(cd->arg1type == 1){
				fprintf(fp,"mov EAX, %d\n",cd->arg1->num);
				fprintf(fp,"cmp EAX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jle L%d\n",cd->res->label);
			}
			else if(cd->arg2type == 1){
				fprintf(fp,"mov EAX, %d\n",cd->arg2->num);
				fprintf(fp,"cmp [temp%d], EAX\n",cd->arg1->temp);
				fprintf(fp,"jle L%d\n",cd->res->label);
			}
			else if(cd->arg1type == 4 && cd->arg2type == 4){
				fprintf(fp,"mov EAX,[temp%d]\n",cd->arg1->temp);
				fprintf(fp,"cmp EAX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jle L%d\n",cd->res->label);
			}
			else{
				printf("Invalid Agruments Rule 6 Op 5\n");
			}
		}
		else if(cd->op == 6){
			if(cd->arg1type == 3){
				fprintf(fp,"mov AX, %d\n",cd->arg1->bool);
				fprintf(fp,"cmp AX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jg L%d\n",cd->res->label);
			}
			else if(cd->arg2type == 3){
				fprintf(fp,"mov AX, %d\n",cd->arg2->bool);
				fprintf(fp,"cmp [temp%d], AX\n",cd->arg1->temp);
				fprintf(fp,"jg L%d\n",cd->res->label);
			}
			else if(cd->arg1type == 1){
				fprintf(fp,"mov EAX, %d\n",cd->arg1->num);
				fprintf(fp,"cmp EAX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jg L%d\n",cd->res->label);
			}
			else if(cd->arg2type == 1){
				fprintf(fp,"mov EAX, %d\n",cd->arg2->num);
				fprintf(fp,"cmp [temp%d], EAX\n",cd->arg1->temp);
				fprintf(fp,"jg L%d\n",cd->res->label);
			}
			else if(cd->arg1type == 4 && cd->arg2type == 4){
				fprintf(fp,"mov EAX,[temp%d]\n",cd->arg1->temp);
				fprintf(fp,"cmp EAX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jg L%d\n",cd->res->label);
			}
			else{
				printf("Invalid Agruments Rule 6 Op 6\n");
			}
		}
		else if(cd->op == 7){
			if(cd->arg1type == 3){
				fprintf(fp,"mov AX, %d\n",cd->arg1->bool);
				fprintf(fp,"cmp AX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jge L%d\n",cd->res->label);
			}
			else if(cd->arg2type == 3){
				fprintf(fp,"mov AX, %d\n",cd->arg2->bool);
				fprintf(fp,"cmp [temp%d], AX\n",cd->arg1->temp);
				fprintf(fp,"jge L%d\n",cd->res->label);
			}
			else if(cd->arg1type == 1){
				fprintf(fp,"mov EAX, %d\n",cd->arg1->num);
				fprintf(fp,"cmp EAX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jge L%d\n",cd->res->label);
			}
			else if(cd->arg2type == 1){
				fprintf(fp,"mov EAX, %d\n",cd->arg2->num);
				fprintf(fp,"cmp [temp%d], EAX\n",cd->arg1->temp);
				fprintf(fp,"jge L%d\n",cd->res->label);
			}
			else if(cd->arg1type == 4 && cd->arg2type == 4){
				fprintf(fp,"mov EAX,[temp%d]\n",cd->arg1->temp);
				fprintf(fp,"cmp EAX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jge L%d\n",cd->res->label);
			}
			else{
				printf("Invalid Agruments Rule 6 Op 7\n");
			}
		}
		else if(cd->op == 8){
			if(cd->arg1type == 3 && cd->arg2type == 4){
				fprintf(fp,"mov AX, %d\n",cd->arg1->bool);
				fprintf(fp,"cmp AX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"je L%d\n",cd->res->label);
			}
			else if(cd->arg2type == 3 && cd->arg1type == 4){
				fprintf(fp,"mov AX, %d\n",cd->arg2->bool);
				fprintf(fp,"cmp [temp%d],AX\n",cd->arg1->temp);
				fprintf(fp,"je L%d\n",cd->res->label);
			}
			else if(cd->arg1type == 1 && cd->arg2type == 4){
				fprintf(fp,"mov EAX, %d\n",cd->arg1->num);
				fprintf(fp,"cmp EAX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"je L%d\n",cd->res->label);
			}
			else if(cd->arg2type == 1 && cd->arg1type == 4){
				fprintf(fp,"mov EAX, %d\n",cd->arg2->num);
				fprintf(fp,"cmp [temp%d], EAX\n",cd->arg1->temp);
				fprintf(fp,"je L%d\n",cd->res->label);
			}
			else if(cd->arg1type == 4 && cd->arg2type == 4){
				fprintf(fp,"mov EAX,[temp%d]\n",cd->arg1->temp);
				fprintf(fp,"cmp EAX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"je L%d\n",cd->res->label);
			}
			else{
				printf("Invalid Agruments Rule 6 Op 8\n");
			}
		}
		else if(cd->op == 9){
			if(cd->arg1type == 3){
				fprintf(fp,"mov AX, %d\n",cd->arg1->bool);
				fprintf(fp,"cmp AX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jne L%d\n",cd->res->label);
			}
			else if(cd->arg2type == 3){
				fprintf(fp,"mov AX, %d\n",cd->arg2->bool);
				fprintf(fp,"cmp AX,[temp%d]\n",cd->arg1->temp);
				fprintf(fp,"jne L%d\n",cd->res->label);
			}
			else if(cd->arg1type == 1){
				fprintf(fp,"mov EAX, %d\n",cd->arg1->num);
				fprintf(fp,"cmp EAX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jne L%d\n",cd->res->label);
			}
			else if(cd->arg2type == 1){
				fprintf(fp,"mov EAX, %d\n",cd->arg2->num);
				fprintf(fp,"cmp [temp%d], EAX\n",cd->arg1->temp);
				fprintf(fp,"jne L%d\n",cd->res->label);
			}
			else if(cd->arg1type == 4 && cd->arg2type == 4){
				fprintf(fp,"mov EAX,[temp%d]\n",cd->arg1->temp);
				fprintf(fp,"cmp EAX,[temp%d]\n",cd->arg2->temp);
				fprintf(fp,"jne L%d\n",cd->res->label);
			}
			else{
				printf("Invalid Agruments Rule 6 Op 9\n");
			}
		}	
}

void rule7_print(codeLine * cd, FILE * fp){
	if(cd->label != -1){
			fprintf(fp,"L%d: ", cd->label);
	}
	// fprintf(fp,"push RAX\n");
	// fprintf(fp,"push RBX\n");
	// fprintf(fp,"push RCX\n");
	// fprintf(fp,"push RDX\n");
	// fprintf(fp,"push RSI\n");
	// fprintf(fp,"push RDI\n");
	 fprintf(fp,"push dword[temp%d]",cd->arg1->temp);
}

void rule8_print(codeLine * cd, FILE * fp){
	fprintf(fp,"call _L%d",cd->arg1->label);
}

void rule9_print(codeLine * cd, FILE * fp){
	
		fprintf(fp,"mov EAX, [rettemp%d]",cd->arg1->ret);
		fprintf(fp,"mov [temp%d], EAX",cd->res->temp);
	
}

void rule10_print(codeLine * cd, FILE * fp){  //Indexed Copy: x = a[i] 
	if(cd->label!=-1){
			fprintf(fp,"L%d: ", cd->label);
		}

	if(cd->arg2type == 1){
		fprintf(fp,"mov RDI, offset temp%d\n",cd->arg1->temp);		
		fprintf(fp,"mov RAX, %d\n",cd->arg2->num);
	}
	else if(cd->arg2type == 4){
		fprintf(fp,"mov RDI, offset temp%d\n",cd->arg2->temp);
		fprintf(fp,"mov RAX, 0\n");
		fprintf(fp,"mov EAX, [temp%d]\n",cd->arg1->temp);
	}
	else{
		printf("Invalid Arguments as Array Indexes\n");
	}
	
	if(getTypeOfTemp(cd->arg1->temp) == 24){
		fprintf(fp,"imul 4\n");
		fprintf(fp,"sub RAX, %d", getStartIndex(cd->arg1->temp));
		fprintf(fp,"add RDI, RAX\n");
		fprintf(fp,"mov EAX, [RDI]\n");
		fprintf(fp,"mov [temp%d], EAX",cd->res->temp);
	}
	else if(getTypeOfTemp(cd->arg1->temp) == 26){
		fprintf(fp,"imul 2\n");
		fprintf(fp,"sub RAX, %d", getStartIndex(cd->arg1->temp));
		fprintf(fp,"add RDI, RAX\n");
		fprintf(fp,"mov EAX, [RDI]\n");
		fprintf(fp,"mov [temp%d], EAX",cd->res->temp);
	}
	else{
		printf("Invalid Arguments Rule 10");
	}
		//Can Type Errors If required
}// Array Print Remaining

void rule11_print(codeLine * cd, FILE * fp){  //Indexed Copy: a[i] = x
	if(cd->label!=-1){
			fprintf(fp,"L%d: ", cd->label);
	}

	if(cd->arg1type == 1){
		fprintf(fp,"mov RDI, offset temp%d\n",cd->res->temp);
		fprintf(fp,"mov RAX, %d\n",cd->arg1->num);
	}
	else if(cd->arg1type == 4){
		fprintf(fp,"mov RDI, offset temp%d\n",cd->res->temp);
		fprintf(fp,"mov RAX, 0\n");
		fprintf(fp,"mov EAX, [temp%d]\n",cd->arg1->temp);
	}
	else{
		printf("Invalid Arguments as Array Indexes\n");
	}
	
	if(getTypeOfTemp(cd->arg2->temp) == 24){
		fprintf(fp,"imul 4\n");
		fprintf(fp,"sub RAX, %d", getStartIndex(cd->arg1->temp));
		fprintf(fp,"add RDI, RAX\n");
		fprintf(fp,"mov EAX, [temp%d]\n",cd->arg2->temp);
		fprintf(fp,"mov [RDI], EAX");
	}
	else if(getTypeOfTemp(cd->arg2->temp) == 26){
		fprintf(fp,"imul 2\n");
		fprintf(fp,"sub RAX, %d", getStartIndex(cd->arg1->temp));
		fprintf(fp,"add RDI, RAX\n");
		fprintf(fp,"mov EAX, [temp%d]\n",cd->arg2->temp);
		fprintf(fp,"mov [RDI], EAX\n");
	}
	else{
		printf("Invalid Arguments Rule 10");
	}
}

void rule12_print(codeLine * cd, FILE * fp){
		fprintf(fp,"add RSP, 8\n");
		fprintf(fp,"pop RBP\n");
		fprintf(fp,"ret\n");
}

void rule13_print(codeLine * cd, FILE * fp){
	fprintf(fp,"push RBP\n");
	fprintf(fp,"mov RBP, RSP\n");
	fprintf(fp,"sub RSP, 8");
	fprintf(fp,"sub RBP, 16");
}

void rule14_print(codeLine * cd, FILE * fp){
	fprintf(fp,"sub RBP, 4");
	fprintf(fp,"mov EAX,[RBP]");
	fprintf(fp,"mov [temp%d], EAX",cd->arg1->temp);
}

void rule15_print(codeLine * cd, FILE * fp){	//Scan Function
	//printf("Entered");

	if(cd->label!=-1){
			fprintf(fp,"L%d: \n", cd->label);
		}
	if(cd->arg1type == 4 && IsTempArray(cd->arg1->temp) == 0){
        if(getTypeOfTemp(cd->arg1->temp) == 24)
			 fprintf(fp,"mov RCX, input_int\n");
		else if(getTypeOfTemp(cd->arg1->temp) == 26)
			fprintf(fp,"mov RCX, input_bool\n");

			fprintf(fp,"mov RDX, 29\n");
			fprintf(fp,"mov RAX,4\n");
			fprintf(fp,"mov RBX, 1\n");
			fprintf(fp,"int 80h\n\n");						
				    
			fprintf(fp,"call scanInt\n");
			fprintf(fp,"mov [temp%d], EBX",cd->arg1->temp);						
	}

	else if(cd->arg1type == 4 && (IsTempArray(cd->arg1->temp) == 26 || IsTempArray(cd->arg1->temp) == 24) ){
		fprintf(fp,"mov RCX, input_arr1\n");
		fprintf(fp,"mov RDX, 11\n");
		fprintf(fp,"mov RAX,4\n");
		fprintf(fp,"mov RBX, 1\n");
		fprintf(fp,"int 80h\n\n");

		fprintf(fp,"mov EAX, %d\n", GetArraySize(cd->arg1->temp));
		fprintf(fp,"call printOP\n");

		if(getTypeOfTemp(cd->arg1->temp) == 24)
			fprintf(fp,"mov RCX, input_arr2_int\n");
		else if(getTypeOfTemp(cd->arg1->temp) == 26)
			fprintf(fp,"mov RCX, input_arr2_bool\n");

		fprintf(fp,"mov RDX, 36\n");
		fprintf(fp,"mov RAX,4\n");
		fprintf(fp,"mov RBX, 1\n");
		fprintf(fp,"int 80h\n\n");

		fprintf(fp,"mov EAX, [temp%d]\n", getStartIndex(cd->arg1->temp));
		fprintf(fp,"call printOP\n");

		fprintf(fp,"mov EAX, [temp%d]\n", getEndIndex(cd->arg1->temp));
		fprintf(fp,"call printOP\n");


			for(int i =0; i < GetArraySize(cd->arg1->temp); i++){
				fprintf(fp,"call scanInt\n");
				fprintf(fp,"mov [temp%d + i], EBX",cd->arg1->temp);	
				fprintf(fp,"call printLF");
		  }	
	}

	else{
		printf("Invalid Arguments Rule 15\n");
	}
}


void rule16_print(codeLine * cd, FILE * fp){
	//printf("Entered");

	if(cd->label!=-1){
			fprintf(fp,"L%d: ", cd->label);
		}

	fprintf(fp,"mov RCX, output\n");
	fprintf(fp,"mov RDX, 9\n");
	fprintf(fp,"mov RAX,4\n");
	fprintf(fp,"mov RBX, 1\n");
	fprintf(fp,"int 80h\n\n");						
	
	if(cd->arg1type == 4 && IsTempArray(cd->arg1->temp) == 0){
			if(getTypeOfTemp(cd->arg1->temp) == 24){
				fprintf(fp,"mov EAX, [temp%d]\n", cd->arg1->temp);
				fprintf(fp,"call printOP\n");
			}
			else if(getTypeOfTemp(cd->arg1->temp) == 26){
				fprintf(fp,"mov EAX, [temp%d]\n", cd->arg1->temp);
				fprintf(fp,"call printbool\n");
			}							
	}
	else if(cd->arg1type == 4 && IsTempArray(cd->arg1->temp) == -2){
			printf("Oopsie, Dynamic Array\n");
	}
	else if(cd->arg1type == 4 && IsTempArray(cd->arg1->temp) == 26){
		  for(int i =0; i < GetArraySize(cd->arg1->temp); i++){
			  	fprintf(fp,"mov EAX, 0\n");
			  	fprintf(fp,"mov AX, [temp%d + i]\n", cd->arg1->temp);
				fprintf(fp,"call printbool\n");
				fprintf(fp,"call printSpace");
		  }
	}
	else if(cd->arg1type == 4 && IsTempArray(cd->arg1->temp) == 24){
		  for(int i =0; i < GetArraySize(cd->arg1->temp); i++){
			  	fprintf(fp,"mov EAX, [temp%d + i]\n", cd->arg1->temp);
				fprintf(fp,"call printOP\n");
				fprintf(fp,"call printSpace");
		  }
	}
	if(cd->arg1type == 3){
			fprintf(fp,"mov EAX, %d\n", cd->arg1->bool);
			fprintf(fp,"call printbool\n");	
	}		
	
	if(cd->arg1type == 1){
			fprintf(fp,"mov eax, %d",cd->arg1->num);
			fprintf(fp,"call printOP\n");				
	}	
}


void assembly_code_generator(intermediateCodeList * intList, char * outputfile){
	 //initialize_all_temp();

	if(intList == NULL){
		 //printf("Intermediate Code Recieved is Empty\n");
	}
	else if(intList->head == NULL){
		 //printf("Code recieved with an Empty Head\n");
	}
	else{
		codeLine * temp_line = (codeLine *)malloc(sizeof(codeLine));
		temp_line = intList->head;
		if(temp_line != NULL){
		//printf("List Recieved\n");
		}
		FILE * fp = fopen(outputfile,"w");
		//printf("File opened Successfully\n");
		sectiondata_print(fp);
		//printf("Data Printed Successfully\n");
		sectiontext_print(fp);
		//printf("Code Printed Successfully\n");


		while(temp_line != NULL){
			//printf("Entered");
			switch(temp_line->rule){
				case 0:
					rule0_print(temp_line, fp);
					temp_line = temp_line->next;
					break;
				case 1: 
					rule1_print(temp_line, fp);
					temp_line = temp_line->next;
					break;

				case 2: 
					rule2_print(temp_line, fp);
					temp_line = temp_line->next;
					break;
				case 3:
					//printf("Entered");
					rule3_print(temp_line, fp);
					temp_line = temp_line->next;
					break;
				case 4:
					rule4_print(temp_line, fp);
					temp_line = temp_line->next;
					break;
				case 6:
					rule6_print(temp_line, fp);
					temp_line = temp_line->next;
					break;
				case 7:
					rule7_print(temp_line, fp);
					temp_line = temp_line->next;
					break;
				case 8:
					rule8_print(temp_line, fp);
					temp_line = temp_line->next;
					break;
				case 9:
					rule9_print(temp_line, fp);
					temp_line = temp_line->next;
					break;
				case 10:
					rule10_print(temp_line, fp);
					temp_line = temp_line->next;
					break;				
				case 11:
					rule11_print(temp_line, fp);
					temp_line = temp_line->next;
					break;
				case 12:
					rule10_print(temp_line, fp);
					temp_line = temp_line->next;
					break;
				case 15:
					rule15_print(temp_line, fp);
					temp_line = temp_line->next;
					break;
				case 16:
					rule16_print(temp_line, fp);
					temp_line = temp_line->next;
					break;
				default:
					break;
					//printf("Invalid Rule Number\n");

			}

		}
		driver_return_print(fp);
		//printf("Returned Out\n");
	}
	return;
}

void sectiontext_print(FILE * fp){
	fprintf(fp, "\n\nSECTION .text\n\n");
	fprintf(fp, "\nglobal main\n\n");
	//fprintf(fp, "extern _scanf\n");
	//fprintf(fp, "extern _printf\n\n");
	fprintf(fp, "_start:\n\n");
	fprintf(fp, "main:\n\n");
}

void driver_return_print(FILE * fp){
	fprintf(fp,"\n\nmov ebx,0\n");
	fprintf(fp,"mov eax,1\n");
	fprintf(fp,"int 80h\n");  //Sys_Exit

	fclose(fp);
}
void sectiondata_print(FILE * fp){
	char c_quote = 34;
	char c_ep = 39;
	fprintf(fp, "\n\n%cinclude %cscanprinthelper.asm%c",'%',c_ep,c_ep);
	fprintf(fp, "\n\nSECTION .data\n\n");
	fprintf(fp,"input_bool db %cInput:Enter an boolean value\n%c, %c0%c\n",c_quote,c_quote,c_ep,c_ep);
	//fprintf(fp,"input_bool_size db 29\n");
	fprintf(fp,"input_int db %cInput:Enter an Integer value\n%c, %c0%c\n",c_quote,c_quote,c_ep,c_ep);
	//fprintf(fp,"input_int_size db 29\n");
	fprintf(fp,"input_real db %cInput:Enter an real value\n%c, %c0%c\n",c_quote,c_quote,c_ep,c_ep);
	//fprintf(fp,"input_real_size db 26\n");
	fprintf(fp,"output db %cOutput: %c, %c0%c\n",c_quote,c_quote,c_ep,c_ep);
	//fprintf(fp,"output_size db 9\n");
	fprintf(fp,"input_arr1 db %cInput:Enter%c, %c0%c",c_quote,c_quote,c_ep,c_ep);
	fprintf(fp,"input_arr2_int db %carray elements of Integer for range %c, %c0%c\n",c_quote,c_quote,c_ep,c_ep);
	fprintf(fp,"input_arr2_bool db %carray elements of Boolean for range %c, %c0%c\n",c_quote,c_quote,c_ep,c_ep);
	fprintf(fp,"false db %cfalse%c\n",c_quote,c_quote);
	fprintf(fp,"true db %ctrue%c\n",c_quote,c_quote);
	fprintf(fp,"space db %c %c\n",c_quote,c_quote);
	//printf("Temp No : %d", tempNo);
	for(int i = 0; i < tempNo; i++){

		if(IsTempArray(i) == 24){
			//printf("In TempArr - Int\n");
			fprintf(fp,"temp%d dd",i);

			for(int j = 0; j < GetArraySize(i);j++){
				fprintf(fp,"0, ");
			}
			fprintf(fp,"0");
		}
		else if(IsTempArray(i) == 26){
			//printf("In TempArr - Bool\n");
			fprintf(fp,"temp%d dw",i);

			for(int j = 0; j < GetArraySize(i);j++){
				fprintf(fp,"0, ");
			}
			fprintf(fp,"0");
		}
		else if(IsTempArray(i) == -2){
			//printf("Dynamic Array, Halt Temporarily");
		}
		else{
			//printf("In Temp - Int\n");
			fprintf(fp, "temp%d dd 0\n",i);
		}		

	}

	for(int i = 0; i < retNo; i++){
		fprintf(fp, "rettemp%d dd 0\n",i);
	}
	//fprintf()
}

				// case 7:
				// 	rule7_print(temp_line, fp);
				// 	temp_line = temp_line->next;
				// 	break;
				// case 8:
				// 	rule8_print(temp_line, fp);
				// 	temp_line = temp_line->next;
				// 	break;
				// case 9:
				// 	rule9_print(temp_line, fp);
				// 	temp_line = temp_line->next;
				// 	break;
				// case 5:
				// 	rule5_print(temp_line, fp);
				// 	temp_line = temp_line->next;
				// 	break;