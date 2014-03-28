cd src/

javac Driver.java

if [ $# -ne 1 ]
then
	exit
fi
if [ -f $1 ]
then
	
	java Driver "$1"
	
fi
cd ..
