#include <stdio.h>
#include <stdint.h>
#include<string.h>
//this user interface built that tha maximum size of variable is uint16_t and cand be divided in the buffer in max 3 bytes
const static uint16_t Bitmasks[] = {
	0b0,
	0b1,
	0b11,
	0b111,
	0b1111,
	0b11111,
	0b111111,
	0b1111111,
	0b11111111,
	0b111111111,
	0b1111111111,
	0b11111111111,
	0b111111111111,
	0b1111111111111,
	0b11111111111111,
	0b111111111111111,
	0b1111111111111111,
};

/*

10bit = 9bit + sign
-1

input
 1111 1111 1111 1111

1 111111111
- 1
*/
//put start bit in zero from zero index 0 to 63
uint8_t encode_data(uint8_t* MyTxbuffer, uint8_t Start_Bit, uint8_t Length, uint16_t Value, uint8_t Sign) {
	/*if (length < (1 + sign)) {
		return 0;
	}
	// check if value -ve or positive from first bit
	uint16_t sign_bit = 0;
	if (sign) {
		sign_bit = value & 1000000000000000 ? 1 : 0;

	}

	value &= bitmasks[sign?length-1:length];
	  //1 111 1111 1111 &
	 //0  001 1111 1111
	// 
	//to remove any extra bits not used in 16 bits as there no uint12_t if the value 12 bit so i need to exclude extra 4 bits not used
	
	uint16_t final_value&
	 final_value = (sign_bit << length ) | value;*/

	uint8_t Byte_Index=0;
	uint8_t Bit_Index=0;
	//signed values
	if (Sign == 1) {
		//the user must give me the total lenghth sign+data
		//read the value of the bit and -1 to keep place for msbit of the sign to be added lately
		uint16_t Readvalue_signed = (Value & Bitmasks[Length-1]); //only read the data feild and will put the the sign bit in the following instruction
		uint16_t Put_Msb = (Readvalue_signed | (1<<Length-1));//after this instruction the length will be as given frmo the the user
		printf("%d\n", Put_Msb);


		Byte_Index = Start_Bit / 8;
		uint8_t Final_Byteindex = Byte_Index/* + 1*/;//to escape from zero index (1-8 bytes)

		Bit_Index = Start_Bit % 8;
		uint8_t Final_Bitindex = Bit_Index + 1;//to escape from zero index (1-64)


		//to see if he need to use the encoding system or not
		if ((8 - (Final_Bitindex + Length) + 1) >= 0) //to get the distance between last bit and and bit 8 in leading byte
		{
			MyTxbuffer[Final_Byteindex] &= ~Bitmasks[Length];
			MyTxbuffer[Final_Byteindex] |= Put_Msb << (Final_Bitindex - 1);
		}


		else
		{


			uint8_t Used_In_leadingbyte = ((8 - Final_Bitindex) + 1);  //2 bits star at bit3 //shof lw fih -1 3ashna moshkelet el index  zy ta7t used byte aw shoof lw trg3 
			//8-3

			MyTxbuffer[Final_Byteindex] &= ~Bitmasks[Used_In_leadingbyte] << (Final_Bitindex - 1);
			MyTxbuffer[Final_Byteindex] |= (Put_Msb << (Final_Bitindex - 1));

			//in the middle byte
			if (8 - (Length - Used_In_leadingbyte) >= 0)
			{
				MyTxbuffer[Final_Byteindex + 1] &= ~Bitmasks[Length - Used_In_leadingbyte];
				MyTxbuffer[Final_Byteindex + 1] |= Put_Msb >> (Used_In_leadingbyte);

			}

			else {
				uint8_t Used_In_middlebyte = /*Length - Used_In_leadingbyte;*/ 8; //if it passed from if condition to this else so the middle bits are full
				MyTxbuffer[Final_Byteindex + 1] &= ~Bitmasks[Used_In_middlebyte];//no shift needed as if u reached this byte you wil start at leading byte
				MyTxbuffer[Final_Byteindex + 1] |= Put_Msb >> Used_In_leadingbyte;

				uint8_t Used_In_endingbyte = (Length - 8 - Used_In_leadingbyte);
				//note that 8 in the equation because it must be fully used the middle byte to reach the ending byte

				//if (Used_In_endingbyte > 0) {
				MyTxbuffer[Final_Byteindex + 2] &= ~Bitmasks[Used_In_endingbyte];
				//-1 is used as if u have one element will be used so it needs the mask of [0] which is 0b1 as the used 
				MyTxbuffer[Final_Byteindex + 2] |= Put_Msb >> (Length - Used_In_endingbyte);



			}



		}




	}



	else
	{//unsigned values
		uint16_t Readvalue_Unsigned=0 /*Value & Bitmasks[Length]*/;
		printf("%d\n", Readvalue_Unsigned);
		//place the value bits in the target bits in Tx_Buffer
		//leading byte
		Byte_Index = Start_Bit / 8;
		uint8_t Final_Byteindex = Byte_Index /*+ 1*/;//to escape from zero index (1-8 bytes)

	    Bit_Index = Start_Bit % 8;
		uint8_t Final_Bitindex = Bit_Index + 1;//to escape from zero index (1-64) so this code is built on no zero index but the user is asked to start bit with zero index
		
		
		//to see if he need to use the encoding system or not
		if ((8-(Final_Bitindex+Length)+1)>=0) //to get the distance between last bit and and bit 8 in leading byte
		{
			MyTxbuffer[Final_Byteindex] &= ~Bitmasks[Length];
			MyTxbuffer[Final_Byteindex] |= Readvalue_Unsigned << (Final_Bitindex-1);
		}


		else
		{


			uint8_t Used_In_leadingbyte = ((8 - Final_Bitindex)+1);  //2 bits star at bit3 //shof lw fih -1 3ashna moshkelet el index  zy ta7t used byte aw shoof lw trg3 
			//8-3

			MyTxbuffer[Final_Byteindex] &= ~Bitmasks[Used_In_leadingbyte] << (Final_Bitindex-1);
			MyTxbuffer[Final_Byteindex] |= ( Readvalue_Unsigned << (Final_Bitindex-1));

			//in the middle byte
			if (8-(Length-Used_In_leadingbyte)>=0)
			{
				MyTxbuffer[Final_Byteindex + 1] &= ~Bitmasks[Length - Used_In_leadingbyte];
				MyTxbuffer[Final_Byteindex + 1] |= Readvalue_Unsigned >> (Used_In_leadingbyte);

			}
			
			else  {
				uint8_t Used_In_middlebyte = /*Length - Used_In_leadingbyte;*/ 8; //if it passed from if condition to this else so the middle bits are full
				MyTxbuffer[Final_Byteindex + 1] &= ~Bitmasks[Used_In_middlebyte];//no shift needed as if u reached this byte you wil start at leading byte
				MyTxbuffer[Final_Byteindex + 1] |= Readvalue_Unsigned >> Used_In_leadingbyte;

				uint8_t Used_In_endingbyte = (Length - 8 - Used_In_leadingbyte);
				//note that 8 in the equation because it must be fully used the middle byte to reach the ending byte

				//if (Used_In_endingbyte > 0) {
					MyTxbuffer[Final_Byteindex + 2] &= ~Bitmasks[Used_In_endingbyte];
					//-1 is used as if u have one element will be used so it needs the mask of [0] which is 0b1 as the used 
					MyTxbuffer[Final_Byteindex + 2] |= Readvalue_Unsigned >> (Length - Used_In_endingbyte);

				

			}



		  }
		

		







         
	}
	//    11 1111 1111|
	//  1000 0000 0000

	/*uint8_t i = 0;
	for (i = 0; i < Length; i++) {
		uint8_t bit_pos = Start_Bit + i;
		uint8_t byte_index = bit_pos / 8;
		uint8_t bit_index = bit_pos % 8;
		MyTxbuffer[byte_index] &= ~(1 << bit_index);
		MyTxbuffer[byte_index] |= (1 << bit_index);

	}*/
}











uint16_t decode_data(const uint8_t* MyRxBuffer, uint8_t Start_Bit, uint8_t Length, uint8_t Sign) {
	uint16_t decoded_value = 0;
	

	// Calculate byte and bit positions (1-based index)
	uint8_t Byte_Index = Start_Bit / 8 ;
	uint8_t Bit_Index = Start_Bit % 8 + 1;

	if (Sign) {
		// For signed values
		uint8_t total_bits = Length;
		uint8_t data_bits = Length - 1;

		// Check if all bits fit in one byte
		if ((8 - Bit_Index + 1) >= total_bits) {
			// Extract all bits from single byte
			uint16_t extracted = (MyRxBuffer[Byte_Index] >> (Bit_Index - 1)) & Bitmasks[total_bits];

			// Check sign bit
			int16_t signed_value = (extracted & (1 << (total_bits - 1))) ?
				-(extracted & Bitmasks[data_bits]) :
				(extracted & Bitmasks[data_bits]);
			return (uint16_t)signed_value;
			printf("%02x", signed_value);

		}
		else {
			// Multi-byte extraction
			uint8_t bits_in_first_byte = 8 - Bit_Index + 1;
			uint8_t bits_in_last_byte = (total_bits - bits_in_first_byte) % 8;
			uint8_t full_bytes = (total_bits - bits_in_first_byte) / 8;

			// Extract from first byte
			uint16_t value = MyRxBuffer[Byte_Index] >> (Bit_Index - 1);

			// Extract from middle bytes (if any)
			for (uint8_t i = 1; i <= full_bytes; i++) {
				value |= (uint16_t)MyRxBuffer[Byte_Index + i] << (bits_in_first_byte + 8 * (i - 1));
			}

			// Extract from last byte (if any remaining bits)
			if (bits_in_last_byte > 0) {
				value |= (uint16_t)(MyRxBuffer[Byte_Index + full_bytes + 1] & Bitmasks[bits_in_last_byte])
					<< (bits_in_first_byte + 8 * full_bytes);
			}

			// Mask to get only the required bits
			value &= Bitmasks[total_bits];

			// Handle sign
			int16_t signed_value = (value & (1 << (total_bits - 1))) ?
				-(value & Bitmasks[data_bits]) :
				(value & Bitmasks[data_bits]);
			return (uint16_t)signed_value;
			printf("%02x", signed_value);

		}
	}
	else {
		// For unsigned values
		// Check if all bits fit in one byte
		if ((8 - Bit_Index + 1) >= Length) {
			// Extract all bits from single byte
			return (MyRxBuffer[Byte_Index] >> (Bit_Index - 1)) & Bitmasks[Length];
			printf("%02x", (MyRxBuffer[Byte_Index] >> (Bit_Index - 1)) & Bitmasks[Length]);

		}
		else {
			// Multi-byte extraction
			uint8_t bits_in_first_byte = 8 - Bit_Index + 1;
			uint8_t bits_in_last_byte = (Length - bits_in_first_byte) % 8;
			uint8_t full_bytes = (Length - bits_in_first_byte) / 8;

			// Extract from first byte
			uint16_t value = MyRxBuffer[Byte_Index] >> (Bit_Index - 1);

			// Extract from middle bytes (if any)
			for (uint8_t i = 1; i <= full_bytes; i++) {
				value |= (uint16_t)MyRxBuffer[Byte_Index + i] << (bits_in_first_byte + 8 * (i - 1));
			}

			// Extract from last byte (if any remaining bits)
			if (bits_in_last_byte > 0) {
				value |= (uint16_t)(MyRxBuffer[Byte_Index + full_bytes + 1] & Bitmasks[bits_in_last_byte])
					<< (bits_in_first_byte + 8 * full_bytes);
			}

			// Mask to get only the required bits
			return value & Bitmasks[Length];
			printf("%02x", value & Bitmasks[Length]);

		}
	}
}




uint8_t TxFrame[8];
uint8_t RxFrame[8];


int main() {
	memset(TxFrame, 0, 8);
	memset(RxFrame, 0, 8);
	encode_data(TxFrame, 0 ,6, 55, 1);
	decode_data(RxFrame, 0, 6, 55, 1);


	printf("%02x %02x %02x %02x %02x %02x %02x %02x\n",
		TxFrame[0], TxFrame[1], TxFrame[2], TxFrame[3], TxFrame[4], TxFrame[5], TxFrame[6], TxFrame[7]);
	/*printf("%02x %02x %02x %02x %02x %02x %02x %02x\n",
		RxFrame[0], RxFrame[1], RxFrame[2], RxFrame[3], RxFrame[4], RxFrame[5], RxFrame[6], RxFrame[7]);*/

	return 0;
}


