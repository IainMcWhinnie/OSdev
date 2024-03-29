;#################################################################################################
;#------------------------------------------space.asm--------------------------------------------#
;#################################################################################################
;#  Simply define 15 empty sectors (1 sector is 512 bytes) of zero bytes. This becomes important #
;#  when emulating the image file since the image file is only as long as all the compiled       #
;#  material, it is not a physical storage device!                                               #
;#################################################################################################
;#-----------------------------<Iain McWhinnie>----<13/05/2020>----------------------------------#
;#################################################################################################

;# define empty space that is 15*512 bytes long
times (512*15) db 0