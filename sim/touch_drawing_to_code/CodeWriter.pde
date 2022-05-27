PrintWriter output;
boolean created = false;

void initWriter(){
  output = createWriter("draw.code");  
  created = true;
}

void writeToFile(String code){
  if(!created) initWriter();
  output.println(code);
}

void saveFile(){
  output.flush(); // Writes the remaining data to the file
  output.close(); // Finishes the file
  created = false;
}
