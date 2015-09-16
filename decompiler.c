//Olivia Kyle -- okyl613 -- 8397153 -- Assignment 2
#include<stdio.h>
#include<string.h>

main(int argc, char *argv[]) {
	
	FILE *file = fopen(argv[1], "r");
	char *address = argv[2];
	char hexArray[100] = "";
	char binaryArray[400] = "";
	getHexArray(file, hexArray);
	hexToBinary(hexArray, binaryArray);
	binaryToLC3(binaryArray, address);
	
}

/*This function reads obj.txt and modifies the parameter array hexArray to store the hexadecimal values in obj.txt.
*/
int getHexArray(FILE *file, char hexArray[]){
	
	int i = 0;
	char c;
	c = fgetc(file);
		
	while(c != EOF){
		hexArray[i] = c;
		i++;
		c = fgetc(file);
	}
	fclose(file);
	return 0;
}

/*This function is passed the hexArray created in the previous function, and an empty array to modify, created in main(). It loops through the hex array assigning the binary equivalent of encountered hexadecimal characters, and a delimiter '2' when it encounters a new line.
*/
int hexToBinary(char hexArray[], char binaryArray[]){

	int i = 0;
	
	while(hexArray[i]){
		switch(hexArray[i]){
			case '0': strcat(binaryArray, "0000"); break;
			case '1': strcat(binaryArray, "0001"); break;
			case '2': strcat(binaryArray, "0010"); break;
			case '3': strcat(binaryArray, "0011"); break;
			case '4': strcat(binaryArray, "0100"); break;
			case '5': strcat(binaryArray, "0101"); break;
			case '6': strcat(binaryArray, "0110"); break;
			case '7': strcat(binaryArray, "0111"); break;
			case '8': strcat(binaryArray, "1000"); break;
			case '9': strcat(binaryArray, "1001"); break;
			case 'a': strcat(binaryArray, "1010"); break;
			case 'b': strcat(binaryArray, "1011"); break;
			case 'c': strcat(binaryArray, "1100"); break;
			case 'd': strcat(binaryArray, "1101"); break;
			case 'e': strcat(binaryArray, "1110"); break;
			case 'f': strcat(binaryArray, "1111"); break;
			case 'A': strcat(binaryArray, "1010"); break;
			case 'B': strcat(binaryArray, "1011"); break;
			case 'C': strcat(binaryArray, "1100"); break;
			case 'D': strcat(binaryArray, "1101"); break;
			case 'E': strcat(binaryArray, "1110"); break;
			case 'F': strcat(binaryArray, "1111"); break;
			default: strcat(binaryArray, "2"); break;
		}
		i++;
	}
	strcat(binaryArray, "22");
	
	return 0;
}

/*This function is passed the completed binary array, splits it into individual lines of instruction (called newCommand), and calls the method getOps() on each instruction to find the opcode / operand. 
*/
int binaryToLC3(char binaryArray[], char *address){
	
	char tempArray[400] = "";
	char newCommand[100] = "";
	
	strcpy(tempArray, binaryArray);
	
	int i = 0;
	int j = 0;
	while(binaryArray[i]){
		if(binaryArray[i] != '2'){
			newCommand[j] = binaryArray[i];
		}
		else{
			if(binaryArray[i - 1] != '2'){
				getOps(newCommand, address);
				i++;
				j = -1;
			}
		}
		i++;
		j++;
	}
	
	return 0;
}

/*getOps() finds the opcode of each instruction passed as a parameter as newCommand[]. Once the opcode is found, it calls opcodeToInstruction() with the opcode as a parameter, which identifies and prints the instruction. Then the function finds the operands and and destination register, and prints them.
*/
int getOps(char newCommand[], char *address){
	
	char tempArray[100] = "";
	char opcode[5] = "";
	char operand1[4] = "";
	char operand2[17] = "";
	char destination[4] = "";
	char nBit[2] = "";
	char zBit[2] = "";
	char pBit[2] = "";
	
	strcpy(tempArray, newCommand);
	memcpy(opcode, tempArray, 4);
	
	if(strcmp(opcode, "0000") == 0){
		printf("br");
		tempArray[0] = tempArray[4];
		memcpy(nBit, tempArray, 1);
		if(nBit[0] ==  '1')
			printf("n");
		tempArray[0] = tempArray[5];
		memcpy(zBit, tempArray, 1);
		if(zBit[0] ==  '1')
			printf("z");
		tempArray[0] = tempArray[6];
		memcpy(pBit, tempArray, '1');
		if(pBit[0] ==  '1')
			printf("p");
		printf(" 0x%s\n", address);
		return 0;
	}
	
	if(strcmp(opcode, "1100") == 0){
		tempArray[0] = tempArray[7];
		tempArray[1] = tempArray[8];
		tempArray[2] = tempArray[9];
		memcpy(operand1, tempArray, 3);
	}
	else{
		tempArray[0] = tempArray[4];
		tempArray[1] = tempArray[5];
		tempArray[2] = tempArray[6];
		memcpy(destination, tempArray, 3);
		
		tempArray[0] = tempArray[7];
		tempArray[1] = tempArray[8];
		tempArray[2] = tempArray[9];
		memcpy(operand1, tempArray, 3);
		
		tempArray[0] = tempArray[13];
		tempArray[1] = tempArray[14];
		tempArray[2] = tempArray[15];
		memcpy(operand2, tempArray, 3);
	}
	
	if(tempArray[10] == '0'){
		if(strcmp(opcode, "1100") == 0){
			opcodeToInstruction(opcode);
			printf(" r%d\n", binToDec(operand1));
		}
		else{
			opcodeToInstruction(opcode);
			printf(" r%d,r%d,r%d\n", binToDec(destination), binToDec(operand1), binToDec(operand2));
		}
	}
	else{
		opcodeToInstruction(opcode);
		printf(" r%d,r%d,%d\n", binToDec(destination), binToDec(operand1), LEAbit(operand2));
	}
		
	return 0;	
}

/*This function is passed a binary array of length 3 (the operands) and translates it to a decimal. This function is called to find the register in getOps(), and in LEAbit if the sign-bit is positive.
*/
int binToDec(char binaryNumber[]){
	int decimal = 0;
	
	if(binaryNumber[0] == '1')
		decimal += 4;
	if(binaryNumber[1] == '1')
		decimal += 2;
	if(binaryNumber[2] == '1')
		decimal += 1;
		
	return decimal;
}

/*Identifies the opcode and prints the instruction, or prints an error.
*/
int opcodeToInstruction(char opcode[]){
	if(strcmp(opcode, "0001") == 0)
		printf("add");
	else if(strcmp(opcode, "0101") == 0)
		printf("and");
	else if(strcmp(opcode, "1100") == 0)
		printf("jmp");
	else if(strcmp(opcode, "0000") == 0)
		return 0;
	else{
		printf("Invalid. Opcode: %s", opcode);
	}
	return 0;
}

/*LEAbit changes signed binary numbers into decimals. It is passed an array of length 3 containing an operand, sign-extends it to 16 bit, and if the sign bit is 1, flips all digits and adds 1. The resulting binary string is translated to a digit, and returned. 
*/
int LEAbit(char binaryNumber[]){
	int isNegative = binaryNumber[0];
	int decimal = 0;
	char signExt[6] = "";
	int i;
	for(i = 0; i < 2; i++){
		if(binaryNumber[0] == '0')
			signExt[i] = '0';
		else
			signExt[i] = '1';
	}
	signExt[2] = binaryNumber[0];
	signExt[3] = binaryNumber[1];
	signExt[4] = binaryNumber[2];
	
	if(binaryNumber[0] == '0')
		binToDec(binaryNumber);
	else{
		i = 0;
		while(signExt[i]){
			if(signExt[i] == '1')
				signExt[i] = '0';
			else
				signExt[i] = '1';
			i++;
		}
		
		if(signExt[4] == '0')
			signExt[4] = '1';
		else
			if(signExt[3] == '0')
				signExt[3] = '1';
			else
				if(signExt[2] == '0')
					signExt[2] = '1';
				else
					if(signExt[1] == '0')
						signExt[1] = '1';
					else
						if(signExt[0] == '0')
							signExt[0] = '1';
						else
							for(i = 0; i < 5; i++){
								signExt[i] = '0';
							}
	
	}
	int currentBit = 16;
	for(i = 0; i < 5; i++){
		if(signExt[i] == '1')
			decimal += currentBit;
		currentBit = (currentBit / 2);
	}
		
	if(isNegative == '1')
		decimal = decimal * -1;
	
	return decimal;
}
