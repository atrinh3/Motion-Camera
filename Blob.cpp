class Blob
{
    public:
    int distanceThreshold, grayscaleThreshold, minx, miny, maxx, maxy;
    BlobFinder(int px, int py)
    {
        minx = px;
        miny = py;
        maxx = px;
        maxy = py;
    }
    
    void addPoint(x, y)
    {
        minx = min(minx, x);
        miny = min(miny, y);
        maxx = max(maxx, x);
        maxy = max(maxy, y);
    }
    
    void getArea()
    {
        return (maxx - minx) * (maxy - miny);
    }
    
    boolean isNear(float px, float py, float dist)
    {
        float cx = (maxx + minx) / 2.f;
        float cy = (maxy + miny) / 2.f;
        d = getDistance(px, cx, py, cy);
        return (d*d < dist * dist)
    }
    
    float getDistance(float x1, float x2, float y1, float y2)
    {
        w = abs(x2 - x1);
        h = abs(y1 - y2);
        return sqrt(w * w + h * h);
    }
    
    void printSpecs()
    {
        int width = maxx - minx;
        int height = maxy - miny;
        float cx = (maxx + minx) / 2.f;
        float cy = (maxy + miny) / 2.f;
        printf("w:     " + width + "\n");
        printf("h:     " + height + "\n");
        printf("(x,y): (" + cx + "," + cy + ")\n");
    }
}
