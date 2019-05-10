#pragma once
#include <inkview.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <utility>
#include <common_utilities/cpputils.h>
#include "flags.h"

namespace pocketbook
{
namespace keyboard
{
class Keyboard;
class KeyboardListener
{
    PB_NON_COPYABLE(KeyboardListener)
public:
    KeyboardListener(){}
    virtual ~KeyboardListener();
    /**
     * @brief hasClosed is called back when keyboard has closed by itself or by using Keyboard::Close
     * @param withCancel true if user canels text input
     * !!IMPORTANT: hasClosed may not be called back if inkview ::CloseKeyboard was used directly
     */
    virtual void hasClosed(bool withCancel) = 0;
    virtual void hasOpened() = 0;
    virtual void textEntered(const std::string & text) = 0;
private:
    Keyboard * m_parent;
    friend class Keyboard;
};
enum FLAGS : int {
    NORMAL = KBD_NORMAL,
    ENTEXT = KBD_ENTEXT,
    PHONE = KBD_PHONE,
    NUMERIC = KBD_NUMERIC,
    IPADDR = KBD_IPADDR,
    FILENAME = KBD_FILENAME,
    URL = KBD_URL,
    DATE = KBD_DATE,
    TIME = KBD_TIME,
    DATETIME = KBD_DATETIME,
    HEX = KBD_HEX,
    HOURS = KBD_HOURS,
    UPPER = KBD_UPPER,
    LOWER = KBD_LOWER,
    FIRSTUPPER = KBD_FIRSTUPPER,
    PASSWORD = KBD_PASSWORD,
    NOSELECT = KBD_NOSELECT,
    SCREENTOP = KBD_SCREENTOP,
    NOHEADER = KBD_NOHEADER,
    NOEVTSHOW = KBD_NOEVTSHOW,
    SENDKEYBOARDSTATEEVENTS = KBD_SENDKEYBOARDSTATEEVENTS,
    NOTOUCH = KBD_NOTOUCH,
    PASSEVENTS = KBD_PASSEVENTS,
    NOUPDATE_AFTER_CLOSE = KBD_NOUPDATE_AFTER_CLOSE,
    EN_DEFAULT = KBD_EN_DEFAULT,
    NOUPDATE_AFTER_OPEN = KBD_NOUPDATE_AFTER_OPEN,
    NO_SELFCLOSE_ON_OK = KBD_NO_SELFCLOSE_ON_OK,
    /**
     * Enter is not a bitmap but a text
     */
    CUSTOM_ENTER_KEY = KBD_CUSTOM_ENTER_KEY,
    /**
     * "Enter" key is inverted
     */
    MARKED_ENTER_KEY = KBD_MARKED_ENTER_KEY,
};
PB_DECLARE_FLAGS(FlagsMask, FLAGS)

class Keyboard
{
    PB_NON_COPYABLE(Keyboard)
public:
    Keyboard();
    virtual ~Keyboard();

    /**
     * @brief AddListener adds a listener
     * @param handler gets virtual functions called when virtual functions are called
     * @return id wich may be used with RemoveListener
     */
    unsigned int AddListener(KeyboardListener * handler, bool detachWhenClosed = true);

    /**
     * @brief AddListener does logicaly the same as AddListener(KeyboardListener * handler, bool detachWhenClosed = true)
     * but with lambda
     */
    unsigned int AddListener(std::function<void (bool canceledInput, const std::string&)> textEntered, bool detachWhenClosed = true);
    void RemoveListener(KeyboardListener * handler);

    /**
     * @brief RemoveListener
     * @param id
     */
    void RemoveListener(unsigned int id);
    void SetTitle(const std::string& title);
    void SetFlags(Flags<FLAGS> flags);
    void AddFlag(FLAGS flag) { m_flags |= flag; }
    Flags<FLAGS> GetFlags() const;
    void SetEnterText(const std::string& text);

    bool IsNoKeyboardBuffer() const;
    void SetNoKeyboardBuffer(bool set = true);


    void Open();
    /**
     * @brief Open
     * @param handler after closing keyboard is detached
     */
    void Open(KeyboardListener* handler);
    /**
     * @brief Open
     * @param textEntered is detached after closing keyboard
     */
    void Open(std::function<void (bool canceledInput, const std::string&)> textEntered);

    std::string OpenSynchro(bool * canceled);


    void Close();
    bool IsOpened() const;
    int GetHeight() const;


    protected:
    void OnClosed(bool withCancel);
    void OnOpened();
    void OnTextEntered(const std::string& text);

private:
    unsigned int AddListenerPrivate(KeyboardListener * handler, bool detachWhenClosed, bool isLambda);
    struct ListenerInfo{
        bool detachWhenClosed;
        bool isLambdaProxy;
        int id;
    };
    static void keyboardHandler(char *text);
    FlagsMask m_flags;
    std::string m_title;
    std::string m_enterText;
    int m_height;
    std::map<KeyboardListener *, ListenerInfo> m_keyboardListeners;
    char buffer[2048];
    bool m_noKeyboardBuffer;
    static Keyboard * keyboardInvoker;
    static int counter;
};

} //namespace keyboard
} //namespace pocketbook


