BEGIN { name=""; for(i=1;i<100;i++) {modu[i]=""}; nm=1; } {
if ( $1 != name ) {
 if ( name != "" ) {
  printf("%s:",name);
  for (i=1;i<nm;i++) { printf(" %s",modu[i]) };
  printf("\n");
 };
  name=$1;
  ix=index(name,"."); base=substr(name,1,ix-1);
  modu[1]=$2; nm=2; next;
} 
for (i=1;i<nm;i++) { if ( modu[i] == $2 ) {next} }
modu[nm]=$2; nm++; next
} 
END { printf("%s:",name);
  for (i=1;i<nm;i++) { printf(" %s",modu[i]) };
  printf("\n");
}
