

void Char_to_PC_Basic(char);
void Int_to_PC_Basic(long);
void String_to_PC_Basic(char*);
void newline_Basic(void);


#define zero "abcdef"                   //chars a,b,c,d,e and f are stored in an array named "zero"
#define one "bc"                        //chars b and c are stored in an array named "one"
#define two "abdeg"                     //Note: the compiler terminates each string in zero
#define three "abcdg"
#define four "bcfg"
#define five "acdfg"
#define six "gcdef"
#define seven "bca"
#define eight "abcdefg"
#define nine "gabcf"



#define print_memory_contents \
String_to_PC_Basic(message_1);\
while(1){if (!(*(zero + letter_counter))){Char_to_PC_Basic('0');string_counter += 1;\
if(string_counter == 10)break;\
}else Char_to_PC_Basic(*(zero + letter_counter));\
letter_counter += 1;}\
\
String_to_PC_Basic(message_2);\
for(int m = 0; m < 10; m++)String_memory_dump(digit++);\
\
Int_to_PC_Basic((int)message_1);\
newline_Basic();\
Int_to_PC_Basic((int)message_2);\
newline_Basic();





void String_memory_dump(char num){
Char_to_PC_Basic(num); Char_to_PC_Basic('\t');
switch(num){
case '0': Int_to_PC_Basic((int)zero);break;
case '1': Int_to_PC_Basic((int)one);break;
case '2': Int_to_PC_Basic((int)two);break;
case '3': Int_to_PC_Basic((int)three);break;
case '4': Int_to_PC_Basic((int)four);break;
case '5': Int_to_PC_Basic((int)five);break;
case '6': Int_to_PC_Basic((int)six);break;
case '7': Int_to_PC_Basic((int)seven);break;
case '8': Int_to_PC_Basic((int)eight);break;
case '9': Int_to_PC_Basic((int)nine);break;}
Char_to_PC_Basic('\t');
switch(num){
case '0': String_to_PC_Basic(zero);break;
case '1': String_to_PC_Basic(one);break;
case '2': String_to_PC_Basic(two);break;
case '3': String_to_PC_Basic(three);break;
case '4': String_to_PC_Basic(four);break;
case '5': String_to_PC_Basic(five);break;
case '6': String_to_PC_Basic(six);break;
case '7': String_to_PC_Basic(seven);break;
case '8': String_to_PC_Basic(eight);break;
case '9': String_to_PC_Basic(nine);break;}
newline_Basic();}



/********************************************************************************************/
