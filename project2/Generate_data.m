max = 0;
all = 0;
for number = 158:500
	pixels = 0;
	frame = imread(['..\data-file\picture\',num2str(number),'.jpg']);
	frame = im2bw(frame,graythresh(frame));
    frame = imresize(frame,255/960);
	sobelchange = edge(frame,'sobel');
    [m,n] = size(frame);
    delete(['..\data-file\boundary-xy8bit\',num2str(number),'.txt']);
    fid = fopen(['..\data-file\boundary-xy8bit\',num2str(number),'.txt'],'wt+');
    for x = 1:m
		for y = 1:n
			if sobelchange(x,y)
				pixels = pixels+1;
                fwrite(fid,x,'unsigned char');
                fwrite(fid,y,'unsigned char');
			end
		end
    end
    all = all+pixels;
	if max<pixels
        max = pixels;
    end
	imshow(sobelchange);
    fclose(fid);
end
