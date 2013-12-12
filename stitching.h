
namespace cv {
  class Mat;
};

class Stitching{
 public:
  Stitching ();
  virtual ~Stitching() {}

  const char* GetOpenCVVersion();

 private:
};
