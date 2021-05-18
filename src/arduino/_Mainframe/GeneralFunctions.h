int boolean_to_integer(bool bol[]){
  int result=0;
  for (int i = 0; i<8; i++){
    result += bol[i]*(1 << (7-i));
  }
  return result;
}

byte boolarr_to_byte(bool bol[]){
    int result=0;
    for(int i=0; i<8; i++){
        if(bol[i]){
            result |= (1 << (7-i));
        }
    }
    return result;
}