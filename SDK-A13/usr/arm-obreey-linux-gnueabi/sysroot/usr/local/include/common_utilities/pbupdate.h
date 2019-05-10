#pragma once
namespace pocketbook
{
namespace utilities
{
class PBUpdate
{
public:
    enum TUpdate {
        FullUpdate,
        SoftUpdate
    };
    static PBUpdate * Instance();
	bool IsImageOnScreenAnalyzer();
	TUpdate GetNextUpdateType();
	TUpdate DoNextUpdate(bool fullUpdateForce=false, bool updateHQ = false);
    void IncreaseUpdate();
    void ResetUpdateCount();
    void SetFullupdateAfterCount(int count);
    
    // Next time you call DoNextUpdate you will have FullUpdate
    void MakeNextUpdateFull();
    int GetFullupdateAfterCount() const;
private:
    PBUpdate();
    static PBUpdate * instance;
    int m_currentCount;
    int m_afterCount;
    int m_neverFull;
    int m_nextFull;
};
}
}
