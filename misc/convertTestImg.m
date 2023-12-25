img = imread('testimg.png');
c0 = img(:,:,1);
fd = fopen('testdata.bin' , 'w');
fwrite(fd, c0.', 'float');
fclose(fd);


