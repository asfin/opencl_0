#define pi 3.14159565

template <typename T>
T deg_to_rad(T angle) {
    return angle*pi/180;
}
template <typename T>
T rad_to_deg(T angle) {
    return 180*angle/pi;
}

class Object {
public:
    Object(float X, float Y, float Z) : X(X), Y(Y), Z(Z) {
    }
    float X, Y, Z;
    float distance_flat(Object b) {
        return sqrt((X - b.X)*(X - b.X) + (Y - b.Y)*(Y - b.Y));
    }
};

void raytrace(cv::Mat img) {
    int screen_x = img.rows;
    int screen_y = img.cols;
    int hx = screen_x/2;
    int hy = screen_y/2;

    float cam_dist = 200.0;
    float cam_fov_x_angle = 2*atan(0.5*screen_x/cam_dist);
    float cam_fov_y_angle = 2*atan(0.5*screen_y/cam_dist);
    cout << "fov x: " << rad_to_deg(cam_fov_x_angle) << " fov y: " << rad_to_deg(cam_fov_y_angle) << endl;
    float obj_r = 200.0;
    Object obj(600.0, 0.0, 0.0);
    Object cam(0.0, 0.0, 0.0);
    vector<float> cam_vec({pi/4, pi/4, 0.0});
    cout << "dist: " << obj.distance_flat(cam) << endl;

    int c, px, py;
    float ax, ay;
    float obj_dist = obj.distance_flat(cam);
    float obj_ax = cam_dist*obj.X;
    for (int x = 0; x < screen_x; x++) {
        auto p = img.ptr<uchar>(x);
        px = x-hx;
        ax = px/cam_dist;
        for (int y = 0; y < screen_y; y++) {
            py = y-hy;
            ay = py/cam_dist;
            c = 255;
            if (ay*obj_dist < obj.Y+obj_r && ay*obj_dist > obj.Y-obj_r) {
                if (ax*obj_dist < obj.Z+obj_r && ax*obj_dist > obj.Z-obj_r) {
                    // Got bounding box collision here
//                    float fc = 4*obj.Y*obj.Y - 2*obj.Y +4*obj_r-ax;
                    float fc = 4*obj.X*obj.X-4*(1+ay)*(obj.X*obj.X-obj_r);
                    float fc1, fc2;
                    if (fc >= 0) {
//                        fc1 = (-obj.Y + sqrt(obj_r) + sqrt(fc))/(2*obj.Y + sqrt(obj_r));
//                        fc2 = (-obj.Y + sqrt(obj_r) - sqrt(fc))/(2*obj.Y + sqrt(obj_r));
                        fc1 = (2*obj.X + sqrt(fc))/(2*ay + 2);
                        fc2 = (2*obj.X - sqrt(fc))/(2*ay + 2);
                        float _px, _py;
                        _px = fc2;
                        _py = _px*ay;
//                        cout << rad_to_deg(ay) << ' ' << _px << ' ' << _py << endl;
                        c += _py;
                    }

                    fc = 4*obj.X*obj.X-4*(1+ax)*(obj.X*obj.X-obj_r);
                    if (fc >= 0) {
//                        fc1 = (-obj.Y + sqrt(obj_r) + sqrt(fc))/(2*obj.Y + sqrt(obj_r));
//                        fc2 = (-obj.Y + sqrt(obj_r) - sqrt(fc))/(2*obj.Y + sqrt(obj_r));
                        fc1 = (2*obj.X + sqrt(fc))/(2*ax + 2);
                        fc2 = (2*obj.X - sqrt(fc))/(2*ax + 2);
                        float _px, _py;
                        _px = fc2;
                        _py = _px*ax;
                        cout << rad_to_deg(ay) << ' ' << _px << ' ' << _py << endl;
                        c += _py;
                    }
                }
            }
            if (c < 255) {
                p[y] = c/4;
            }

        }
    }
}