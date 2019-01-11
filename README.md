
# QuadrangleInPicture

## Quick introduction

Shows how to embed a quadrangle video into a picture using OpenCV C++. 

This is the output.
![enter image description here](https://i.imgur.com/m3EFkP2.jpg)
## Description
You have an image and want to embed on top of the image another image. **Note that when I say image, it can also be video as video is a series of images. If the image you want to embed is a square or rectangle, there are other simpler methods. This method is overkill for square or rectangle. But if the image you want to embed is a quadrangle(four sided polygon) but not square or rectangle, this method suits you. We will use homography matrix and warped perspective.

## Your inputs: 
1. background image
![enter image description here](https://i.imgur.com/b2WXqtU.jpg)
2. video to be embed
![Freely available The Incredibles 2 trailer](https://i.imgur.com/DUwb4R2.png)
3. The coordinate of the embedded target.

## Steps

1. Setup the background image and video.

		// setup background image
		String backgroundFilename("background.jpg"); 
		Mat background = imread(backgroundFilename, CV_LOAD_IMAGE_COLOR);

		// setup video capture
		VideoCapture cap("incredibles-2-trailer-1_h480p.mov");
		if (!cap.isOpened())
		{
			cout << "Error opening";
			return -1;
		}

		// the video we want to put in background
		Mat video;
		cap >> video;

2. Define the video region (we use whole video resolution) and its corresponding region in the background image.

		// video source
		vector<Point> left_image;      
		left_image.push_back(Point(0, 0));
		left_image.push_back(Point(0, video.rows));
		left_image.push_back(Point(video.cols, video.rows));
		left_image.push_back(Point(video.cols, 0));

		// corresponding video coordinate destination
		vector<Point> right_image;
		right_image.push_back(Point(100, 100));
		right_image.push_back(Point(200, 500));
		right_image.push_back(Point(900, 650));
		right_image.push_back(Point(700, 200));

3. Calculate homography matrix.

		// once we get 4 corresponding points in both images calculate homography matrix
		Mat H = findHomography(left_image, right_image, 0);

4. Warped perspective.

		// warped perspective
		Mat warpedImage;
		warpPerspective(video, warpedImage, H, background.size());

![enter image description here](https://i.imgur.com/p3kc6Ee.png)

5. Paint the region in the background image where we want to paste the video to black.

		// create a mask at background
		int num = 4;
		const Point *pt4 = &(right_image[0]);
		fillPoly(background, &pt4, &num, 1, Scalar(0, 0, 0), 8);

![enter image description here](https://i.imgur.com/g5IFgqH.jpg)
6. Sum background image with warped perspective image. (Any colored pixel summed with black pixel will results to the colored pixel).

![enter image description here](https://i.imgur.com/m3EFkP2.jpg)
## Compile notes
The project is setup based on OpenCV official tutorial (property sheets setup). Check Visual Studio property manager and assign your configured property sheets.



