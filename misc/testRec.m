
cnt = 0;
while 1

pth = sprintf("/f/TMP/thermal/img%05d.bin", cnt);
fd = fopen(pth, 'r');
data = fread(fd, 'double');
fclose(fd);
dr = reshape(data, 256, 192);
imagesc(dr);
pause(0.1)
cnt = cnt+1;
end
