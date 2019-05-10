#pragma once
#include <inkview.h>
#include <string>
#include <memory>

class BrokenSmartBitmap;

namespace pocketbook {
namespace utilities {

/**
 * @brief The SmartBitmap class is another wrapper over ibitmap
 * it is used for space economy and decreasing time for copieng ibitmap *
 *
 * There is some advices about using this class :
 *
 * 1) It is possible to load icons that are hosted on filesystem using SmartBitmap::SmartBitmap("beautiful.jpg"),
 *  but for better readability you should use SmartBitmap::LoadJPEG(...) and SmartBitmap::LoadTIFF(...) and others
 *
 * 2) It is possible to load icons that are hosted in themes using SmartBitmap::SmartBitmap("close_icon").
 *  Name of the icon should not contain extension to be treated as theme icon
 *  but for better readability you should use SmartBitmapLoader::FromTheme(...)
 *
 * 3) You do not need to make a cache for themes icons, there is already a cache for theme icons in SmartBitmap class
 *
 * 4) SmartBitmap FromScreen(int x, int y, int w, int h) should be used to make a copy from screen instead
 * from inkview function ibitmap * BitmapFromScreen(...)
 *
 * 5) You do not need to load broken_image from theme in case a normal image is not found in themes. broken_image
 * is already embedded to smarbitmap source. If image isn't exists in theme than broken image returned

 */



class SmartBitmap
{
public:
    /**
     * @brief SmartBitmap constructs empty SmartBitmap object
     */
    SmartBitmap();

    /**
     * @brief SmartBitmap ctor constructs white bitmap with given size
     * @param width_
     * @param height_
     */
    SmartBitmap(int width_, int height_);

    /**
     * @brief SmartBitmap loads image
     * @see SmartBitmap::Load(...)
     */
    SmartBitmap(const std::string & path, bool dither = false);
    
    /**
     * @brief SmartBitmap constructs a copy of
     * SmartBitmap @param other
     */
    SmartBitmap(const SmartBitmap & other);

    /**
     * @brief ~SmartBitmap() frees ibitmap if object
     * contains last reference on ibitmap
     */
    ~SmartBitmap();


    /**
     * @brief Bitmap has been copied
     * ibitmap * @param bmp
     */
    void SetBitmap(const ibitmap * bmp);

    /**
     * @brief Pass ownership

      */
    void SetBitmapWithOwnership(const ibitmap* bmp);

    /**
      @brief create SmartBitmap from ibitmap. Actual bitmap is copied
      */
    static SmartBitmap ToSmartBitmap(const ibitmap* bmp);

    /**
     * @brief Load is used to load image from the file system
     * image type is determined by file extension
     * @param path is fullpath to the image on the filesystem
     * if @param path does not contain extension path is considered as theme name
     * @param dither flag determine if the image is dithered
     * @see inkview.h: ibitmap *LoadPNG(const char *path, int dither);
     * @return true if successfully loaded
     */
    bool Load(const std::string & path, bool dither = false);
    
    /**
     * @brief width
     * @return width of the image, returns 0
     * if object is empty
     */
    int width() const;
    int height() const;

    int depth() const;
    
    /**
     * @brief Release loose reference to ibitmap
     * and deletes it if it is last reference
     */
    void Release();    
    
    /**
     * @brief SavePNG saves 
     * at @param path 
     * @return PB_OK if success
     */
    int SavePNG(const std::string & path) const;

    /**
     * @brief SaveBMP saves
     * at @param path
     * @return PB_OK if success
     */
    int SaveBMP(const std::string & path) const;
    
    /**
     * @brief operator = copies reference from
     * SmartBitmap @param other
     * @return reference to this
     */
    SmartBitmap & operator=(const SmartBitmap & other);

    /**
     * @brief GetBitmap 
     * @return inkview const pointer to bitmap
     */
    const ibitmap * GetBitmap() const;
    
    std::pair<int,int> GetSize() const;

    SmartBitmap get8bppBitmap() const;
    
    /**
     * @brief isEmpty
     * @return true if object shares a reference to ibitmap
     */
    bool isEmpty() const;
    operator bool () const { return !isEmpty(); }

    bool IsValid() const { return !isEmpty(); }

    bool LoadPNG(const std::string & path, bool dither = false);
    
    bool LoadJPEG(const std::string & path, int width=-1, int height=-1, int brightness=95, int contrast=105, bool proportional=1);
    
    bool LoadBMP(const std::string & path);

    bool LoadTIFF(const std::string & path, int width=-1, int height=-1, int br=95, int co=105, bool proportional=1);
    /**
     * @brief LoadFromTheme loads image from theme
     * @param name is a key in the theme
     * @return SmartBitmap object with a single reference
     */
    void LoadFromTheme(const std::string & name);
    void LoadFromTheme(const std::string & name, const pocketbook::utilities::SmartBitmap &other);
    void LoadFromTheme(const std::string & name, const std::string & def) { LoadFromTheme(name, SmartBitmap(def)); }

    /** @brief FromTheme is a synonym for LoadFromTheme
     */
    void FromTheme(const std::string & name) { LoadFromTheme(name); }
    void FromTheme(const std::string & name, const pocketbook::utilities::SmartBitmap &def) { LoadFromTheme(name, def); }
    void FromTheme(const std::string & name, const std::string & def) { LoadFromTheme(name, def); }

    void FromScreen(int x, int y, int w, int h);

    bool operator==(const SmartBitmap & other) const {
        return GetBitmap() == other.GetBitmap();
    }
    bool operator!=(const SmartBitmap & other) const {
        return !(*this == other);
    }
private:

    std::shared_ptr<const ibitmap> data_;
    
    static std::string getExt(const std::string & path);
    static SmartBitmap LoadFromThemeInternal(const std::string & name);

    void init(ibitmap * bmp);
    void init(const SmartBitmap & other);
    friend class ::BrokenSmartBitmap;
};

class SmartBitmapLoader {
public:
    static SmartBitmap FromThemeCached(const std::string& key) { return FromTheme(key); }
    static SmartBitmap FromTheme(const std::string& key) { return SmartBitmap(key); }
    static SmartBitmap LoadFromTheme(const std::string& key) { return FromTheme(key); }
    static SmartBitmap FromTheme(const std::string & name, const pocketbook::utilities::SmartBitmap &def) { SmartBitmap b; b.LoadFromTheme(name, def); return b;}
    static SmartBitmap LoadFromTheme(const std::string & name, const pocketbook::utilities::SmartBitmap &other) { return FromTheme(name, other); }
    static SmartBitmap FromTheme(const std::string & name, const std::string & def) { return LoadFromTheme(name, def); }
    static SmartBitmap LoadFromTheme(const std::string & name, const std::string & def) { return LoadFromTheme(name, SmartBitmap(def)); }
    static SmartBitmap FromScreen(int x, int y, int w, int h) { SmartBitmap b; b.FromScreen(x, y, w, h); return b; }
    static SmartBitmap LoadPNG(const std::string & path, bool dither = false) {SmartBitmap b; b.LoadPNG(path, dither); return b;}
    static SmartBitmap LoadJPEG(const std::string & path, int width=-1, int height=-1, int brightness=95, int contrast=105, bool proportional=1) {
        SmartBitmap b;
        b.LoadJPEG(path, width, height, brightness, contrast, proportional);
        return b;
    }
    static SmartBitmap LoadBMP(const std::string & path) {
        SmartBitmap b;
        b.LoadBMP(path);
        return b;
    }
    static SmartBitmap LoadTIFF(const std::string & path, int width=-1, int height=-1, int br=95, int co=105, bool proportional=1) {
        SmartBitmap b;
        b.LoadTIFF(path, width, height, br, co, proportional);
        return b;
    }
};

} // namespace utilities
} // namespace pocketbook
