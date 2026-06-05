
struct Trans_NS___cxx11_basic_string {
  int *c_str();
};
int snprintf(...);
enum LogSeverity {};
enum LogId {};
template <typename F>
void SplitByLogdChunks(LogId, LogSeverity, char *, char *, int, char *msg, F) {
  long max_size;
  Trans_NS___cxx11_basic_string file_header;
  char logd_chunk[max_size];
  auto write_to_logd_chunk = [&](char *, int) {
    *file_header.c_str() = snprintf(sizeof(logd_chunk));
  };
  char newline;
  write_to_logd_chunk(msg, newline);
}
void LogdLogChunk();
LogId LogdLogger_id;
LogSeverity LogdLogger_severity;
char LogdLogger_tag, LogdLogger_file, LogdLogger_message;
int LogdLogger_line;
void LogdLogger() {
  SplitByLogdChunks(LogdLogger_id, LogdLogger_severity, &LogdLogger_tag,
                    &LogdLogger_file, LogdLogger_line, &LogdLogger_message,
                    LogdLogChunk);
}
