all:	testThuVien
testThuVien:	testThuVien.o
		gcc -g -o testThuVien testThuVien.o libfdr.a -lm
testThuVien.o:	testThuVien.c 
	gcc -g -I/ -c testThuVien.c -lm
clean:	
	rm -f *.o *~
