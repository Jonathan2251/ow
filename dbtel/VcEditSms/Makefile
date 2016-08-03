a.out: EditSms.o dr_lcd.o
	cc -Xlinker EditSms.o dr_lcd.o
EditSms.o:
	cc -c EditSms.c
dr_lcd.o:
	cc -c dr_lcd.c
clean:
	rm -f *.o a.out
