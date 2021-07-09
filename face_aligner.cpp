// include

//------------------------------------------------------------------------
// Find Face Chips
//  args: : vector of boxes (bounds bear face & coords of face features)
//  		image of bear
//			transform_features: return features used
//			face_features: return transformed features
//  return: vector of facechips.
//  actions:write out
//------------------------------------------------------------------------
std::vector<matrix<rgb_pixel> > find_chips(
    std::vector<image_dataset_metadata::box> &boxes,
    matrix<rgb_pixel> &img,
    const float padding,
    std::vector<float> eyes_loc,
    std::string &transform_features,                     // features used in transformation
    std::vector<dlib::vector<double, 2> > &face_features // for all features
)
{
    std::vector<matrix<rgb_pixel> > faces;
    point part[6];
    // display face lear, rear, nose on uniform grey canvas
    std::vector<image_window::overlay_circle> chip_circles;
    const rgb_pixel color_r(255, 0, 0);
    const rgb_pixel color_g(0, 255, 0);
    const rgb_pixel color_b(0, 0, 255);
    float nose_x = .50, nose_y = .70;
    float leye_x = eyes_loc[0], leye_y = eyes_loc[1];
    float reye_x = eyes_loc[2], reye_y = eyes_loc[3];
    std::string bearID;
    //---------------
    for (auto &&b : boxes) // for each face
    {
        part[0] = b.parts["head_top"];
        part[1] = b.parts["lear"];
        part[2] = b.parts["leye"];
        part[3] = b.parts["nose"];
        part[4] = b.parts["rear"];
        part[5] = b.parts["reye"];

        // chip_details based on get_face_chip_details
        const double mean_face_shape_x[] = {0, 0, leye_x, nose_x, 0, reye_x};
        const double mean_face_shape_y[] = {0, 0, leye_y, nose_y, 0, reye_y};
        //const double mean_face_shape_x[] = { 0, 0, 0.62, 0.54, 0, 0.38 }; // derrived from HOG image
        //const double mean_face_shape_y[] = { 0, 0, 0.45, 0.62, 0, 0.45 }; // derrived from HOG image
        // double padding = -0.12; // using negative padding so we don't have to adjust mean face shape
        chip_details face_chip_details;

        std::vector<dlib::vector<double, 2> > from_points, to_points;
        //for (unsigned long i : {3, 5, 2})  // follow the order from face pose (nose, reye, leye)
        // --------------------!!!!!!!
        transform_features = "reye leye";
        // --------------------!!!!!!!
        // MAKE SURE transform_features REFLECTS FEATURES USED BELOW
        for (unsigned long i : {5, 2}) // follow order from face pose (reye, leye) EYES_ONLY
        {
            // Ignore top and ears
            if ((i == 0) || (i == 1) || (i == 4))
                continue;

            dlib::vector<double, 2> p;
            p.x() = (padding + mean_face_shape_x[i]) / (2 * padding + 1);
            p.y() = (padding + mean_face_shape_y[i]) / (2 * padding + 1);
            from_points.push_back(p * g_chip_size);
            to_points.push_back(part[i]);
            //cout << "from:" << p*g_chip_size << endl;
            //cout << "to:" << shape.part(i) << endl;
        }

        face_chip_details = chip_details(from_points, to_points, chip_dims(g_chip_size, g_chip_size));

        // ---------------------------------------
        // get mapping for display
        point_transform_affine pta = get_mapping_to_chip(face_chip_details);
        auto leye = pta(part[2]);
        auto nose = pta(part[3]);
        auto reye = pta(part[5]);
        clean_xy(leye);
        clean_xy(reye);
        clean_xy(nose);
        auto leye_new = leye * chip_x;
        auto nose_new = nose * chip_x;
        nose_new.x() = std::min(nose_new.x(), (double)(g_chip_size * chip_x - g_feature_radius - 1));
        nose_new.y() = std::min(nose_new.y(), (double)(g_chip_size * chip_x - g_feature_radius - 1));
        if (nose_new.y() < 0)
            nose_new.y() = g_feature_radius;
        if (nose_new.x() < 0)
            nose_new.x() = g_feature_radius;
        auto reye_new = reye * chip_x;
        face_features.push_back(leye);
        face_features.push_back(nose);
        face_features.push_back(reye);

        chip_circles.push_back(image_window::overlay_circle(reye_new, g_feature_radius, color_r));
        add_overlay_circle(reye_new, g_feature_radius, color_r);
        chip_circles.push_back(image_window::overlay_circle(leye_new, g_feature_radius, color_b));
        add_overlay_circle(leye_new, g_feature_radius, color_b);
        chip_circles.push_back(image_window::overlay_circle(nose_new, g_feature_radius, color_g));
        add_overlay_circle(nose_new, g_feature_radius, color_g);
        // WIN: g_win_chip.add_overlay (chip_circles);

        // extract the face chip
        matrix<rgb_pixel> face_chip;
        extract_image_chip(img, face_chip_details, face_chip);

        // save the face chip
        faces.push_back(face_chip);
    }
    return (faces);
}

void align(std::string face_file)
{   
    // load the data
    dlib::image_dataset_metadata::dataset data;
    load_image_dataset_metadata(data, face_file);

    // for each image
    for (int i = 0; i < metadata.images.size(); i++)
    {
        // load the image;
        string curImageFile = metadata.images[i].filename;
        matrix<rgb_pixel> img;
        try {
            load_image(img, curImageFile);
        } catch (image_load_error& e) {
            cout << "Error loading image " << curImageFile << endl; 
        }

        for 
}
