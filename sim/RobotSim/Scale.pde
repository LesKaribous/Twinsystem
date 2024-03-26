

PVector toMM(PVector px){
  return new PVector(px.x*3, px.y*3);
}

PVector toPixel(PVector mm){
  return new PVector(mm.x/3, mm.y/3);
}

float toMM(int px){
  return px*3.0;
}

int toPixel(float mm){
  return int(mm/3.0);
}
