#ifndef JOB_STATUS_H
#define JOB_STATUS_H

#include <string>

namespace pocketbook {
namespace utilities {

class JobStatus
{
public:

    enum JobType {
        JT_UNKNOWN,
        JT_DOWNLOAD,
        JT_INSTALL
    };

    JobStatus();
    void resetJobStatus();

    int getOverallProgress();

    JobType getCurrentJobType();
    void setCurrentJobType(JobType job_type);

    std::string getCurrentJobMessage();
    void setCurrentJobMessage(std::string current_job_message);

    int getCurrentJobIndex();
    void setCurrentJobIndex(int current_job_index);

    int getJobsCount();
    void setJobsCount(int jobs_count);

    int getCurrentJobProgress();
    void setCurrentJobProgress(int current_job_progress);

    int getLastError();
    void setLastError(int last_error);

    int readFromStream(std::istream& stream);
    int writeToStream(std::ostream& stream);

    int readFromFile(std::string filepath);
    int writeToFile(std::string filepath);


private:
    JobType current_job_type_ = JobType::JT_UNKNOWN;
    std::string current_job_message_;
    int current_job_index_ = 0;
    int jobs_count_ = 0;
    int current_job_progress_ = 0;
    int last_error_ = 0;
};

}
}

#endif // JOB_STATUS_H
