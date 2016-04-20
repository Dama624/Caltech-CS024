You can try out rlenc and rldec on the .bmp files in the local directory.
The Microsoft Bitmap format is an uncompressed image format, and with large
black-and-white images the benefit of using run-length encoding is very
obvious.  You can try these programs to see if rlenc and rldec work properly:

    rlenc bw_bird.bmp bw_bird.rle
    rldec bw_bird.rle bw_bird_out.bmp
    diff bw_bird.bmp bw_bird_out.bmp

The diff program should state that the two files are identical.  (Actually,
if the files are the same then diff says nothing; it only says something if
the files are different.)

