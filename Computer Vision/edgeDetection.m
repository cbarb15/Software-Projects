clc;
clear all;
close all;

LINE_SET = {};
ITERS = 0;
TOTAL_NO_ITERS = 10000;
MAX_PAIR_DISTANCE = 100;
MIN_POINTLINE_DISTANCE = 1;
MIN_LINE_PIXEL_NUM = 150;
EDGE_SET = {};

origEdgeImage = imread('Inputs/lineDetect1.bmp');
imwrite(origEdgeImage, 'Outputs/origEdgeImage.png', 'png');
edgeImage1 = edge(rgb2gray(origEdgeImage), 'canny', 0.1);

for i = 1:1:size(edgeImage1, 1)
    for j = 1:1:size(edgeImage1, 2)
        if edgeImage1(i,j) == 1
            EDGE_SET{ end + 1 } = [i, j];
        end
    end
end

while ITERS ~= TOTAL_NO_ITERS
    ITERS = ITERS + 1;
    randP = randi([1 size(EDGE_SET, 2)]);
    
    % gets cell out of the array of cells
    pPoint = EDGE_SET(randP);
    % Takes array out of to give an array of numbers
    pPointArray = pPoint{ 1,1 };
     
    distancePixel = MAX_PAIR_DISTANCE + 1;
    randQ = 0;
    
    while distancePixel > MAX_PAIR_DISTANCE
        randQ = randi([1 size(EDGE_SET, 2)]);
        qPoint = EDGE_SET(randQ);
        qPointArray = qPoint{ 1,1 };
        
        
        X = [pPointArray(1) pPointArray(2); qPointArray(1) qPointArray(2)];
        distancePixel = pdist(X);
    end
    
    INLIERS = {};
    sizeOfEdgeSet = size(EDGE_SET, 2);
    i = 1;
    
    while(i < sizeOfEdgeSet)
        
        point = EDGE_SET(i);
        pointArray = point{ 1,1 };
        distanceOfPointFromLine = point_to_line(pPointArray, qPointArray, pointArray);
        if(distanceOfPointFromLine <= MIN_POINTLINE_DISTANCE)
            INLIERS{ end + 1 } = point;
            EDGE_SET(i) = [];
        end
        sizeOfEdgeSet = sizeOfEdgeSet - 1;
        i = i + 1;
    end
    
    if(size(INLIERS, 2) >= MIN_LINE_PIXEL_NUM)
        LINE_SET{ end + 1 } = INLIERS;  
    end
end

[M,N,rgb] = size(origEdgeImage);
outputImage = origEdgeImage;
for i = 1:M
    for j = 1:N
        for k = 1:rgb
            outputImage(i,j,k)=0;
        end
    end
end


% outputImage = uint8(zeros(size(LINE_SET, 1), size(LINE_SET, 2)));
for i = 1:1:size(LINE_SET, 2)
    inlierSet = LINE_SET(i);
    inlierCellSet = inlierSet{ 1,1 };
    inlierCoords = inlierCellSet;
    rColor = randi(255);
    gColor = randi(255);
    bColor = randi(255);
    
    for j = 1:1:size(inlierCoords, 2)
        coordCell = inlierCoords(j);
        coords = coordCell{ 1,1 }{1,1};
       
        outputImage(coords(1),coords(2), 1) = rColor;
        outputImage(coords(1),coords(2), 2) = gColor;
        outputImage(coords(1),coords(2), 3) = bColor;
    
    end
end

%  imshow(outputImage)
imwrite(outputImage, 'Outputs/outputEdgeImage.png', 'png');

