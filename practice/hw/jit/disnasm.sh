nl=$'\n' 
echo "BITS 64${nl} $@" > temp.asm && nasm -f bin -o temp.com temp.asm && ndisasm temp.com
rm -rf temp.asm temp.com
