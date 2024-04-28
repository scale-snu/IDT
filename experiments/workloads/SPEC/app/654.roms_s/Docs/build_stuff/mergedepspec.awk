BEGIN { name=""; for(i=1;i<100;i++) {modu[i]=""}; nm=1; } {
if ( $1 != name ) {
 if ( nm > 2 ) {
  printf("\t'%s' => [\n",modu[1]);
  for (i=2;i<nm;i++) { printf("\t\t'%s',\n",modu[i]) };
  printf("\t\t],\n");
 };
  name=$1;
  ix=index(name,"."); base=substr(name,1,ix-1);
  modu[1]=$2; nm=2; next;
} 
if ( $2 == " mpif.h" ) { next }
for (i=1;i<nm;i++) { if ( modu[i] == $2 ) {next} }
modu[nm]=$2; nm++; next
} 
END { 
 if ( nm > 2 ) {
  printf("\t'%s' => [\n",modu[1]);
  for (i=2;i<nm;i++) { printf("\t\t'%s',\n",modu[i]) };
  printf("\t\t],\n");
}}
