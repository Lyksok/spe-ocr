// void rotate_pixbuf(GdkPixbuf *src_pixbuf, GdkPixbuf *dst_pixbuf, double angle)
// {
//     printf("🔄 Rotating pixbuf by angle: %.2f\n", angle);
//     int src_width = gdk_pixbuf_get_width(src_pixbuf);
//     int src_height = gdk_pixbuf_get_height(src_pixbuf);
//     double angle_rad = angle * PI / 180.0;
//     double cos_angle = cos(angle_rad);
//     double sin_angle = sin(angle_rad);
//     int dst_width =
//         (int)(fabs(src_width * cos_angle) + fabs(src_height * sin_angle));
//     int dst_height =
//         (int)(fabs(src_width * sin_angle) + fabs(src_height * cos_angle));

//     int n_channels = gdk_pixbuf_get_n_channels(src_pixbuf);

//     printf("📏 Source Width: %d, Source Height: %d, Destination Width: %d, "
//            "Destination Height: %d, Channels: %d\n",
//            src_width, src_height, dst_width, dst_height, n_channels);

//     // Ensure dst_pixbuf has the correct dimensions
//     if (gdk_pixbuf_get_width(dst_pixbuf) != dst_width ||
//         gdk_pixbuf_get_height(dst_pixbuf) != dst_height)
//     {
//         // g_object_unref(dst_pixbuf);
//         dst_pixbuf =
//             gdk_pixbuf_new(GDK_COLORSPACE_RGB, gdk_pixbuf_get_has_alpha(src_pixbuf),
//                            8, dst_width, dst_height);
//     }

//     guchar *src_pixels = gdk_pixbuf_get_pixels(src_pixbuf);
//     guchar *dst_pixels = gdk_pixbuf_get_pixels(dst_pixbuf);
//     int src_rowstride = gdk_pixbuf_get_rowstride(src_pixbuf);
//     int dst_rowstride = gdk_pixbuf_get_rowstride(dst_pixbuf);

//     for (int y = 0; y < dst_height; y++)
//     {
//         for (int x = 0; x < dst_width; x++)
//         {
//             double src_x = (x - dst_width / 2) * cos_angle -
//                            (y - dst_height / 2) * sin_angle + src_width / 2;
//             double src_y = (x - dst_width / 2) * sin_angle +
//                            (y - dst_height / 2) * cos_angle + src_height / 2;

//             if (src_x >= 0 && src_x < src_width - 1 && src_y >= 0 &&
//                 src_y < src_height - 1)
//             {
//                 int x1 = (int)src_x;
//                 int y1 = (int)src_y;
//                 int x2 = x1 + 1;
//                 int y2 = y1 + 1;

//                 // Ensure x2 and y2 are within bounds
//                 if (x2 >= src_width)
//                     x2 = src_width - 1;
//                 if (y2 >= src_height)
//                     y2 = src_height - 1;

//                 double dx = src_x - x1;
//                 double dy = src_y - y1;

//                 for (int c = 0; c < n_channels; c++)
//                 {
//                     // Top-left pixel
//                     guchar *p1 = src_pixels + y1 * src_rowstride + x1 * n_channels + c;
//                     // Top-right pixel
//                     guchar *p2 = src_pixels + y1 * src_rowstride + x2 * n_channels + c;
//                     // Bottom-left pixel
//                     guchar *p3 = src_pixels + y2 * src_rowstride + x1 * n_channels + c;
//                     // Bottom-right pixel
//                     guchar *p4 = src_pixels + y2 * src_rowstride + x2 * n_channels + c;

//                     // Bilinear interpolation
//                     double value = (1 - dx) * (1 - dy) * (*p1) + dx * (1 - dy) * (*p2) +
//                                    (1 - dx) * dy * (*p3) + dx * dy * (*p4);

//                     dst_pixels[y * dst_rowstride + x * n_channels + c] = (guchar)value;
//                 }
//             }
//             else
//             {
//                 for (int c = 0; c < n_channels; c++)
//                 {
//                     dst_pixels[y * dst_rowstride + x * n_channels + c] =
//                         0; // Set to black or transparent
//                 }
//             }
//         }
//     }
// }