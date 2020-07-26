#include "WorkLogger.h"

#include "../time/TimeKeeper.h"
#include "../ui/Interface.h"

WorkLogger *workLogger;

WorkLogger::WorkLogger(Board *board):
  _board(board)
{
  ; // empty
}

void WorkLogger::logWork(void)
{
  static const char fieldDelim[] = "\t";
  static const size_t fieldDelimLen = sizeof(fieldDelim);

  if (nullptr != timeKeeper) {
    if (nullptr != interface) {
      Project *project = interface->project();
      Activity *activity = interface->activity();
      if (nullptr != project && nullptr != activity) {
        if (nullptr != _board) {
          char *filepath = nullptr;
          if (_board->assertWorkLog(&filepath, timeKeeper->timeStamp())) {
            char *dateTime     = timeKeeper->currentTime(RFC3339);
            char *activityId   = activity->id();
            char *projectName  = project->name();
            char *activityDesc = activity->desc();

            if (nullptr != filepath    &&
                nullptr != dateTime    &&
                nullptr != activityId  &&
                nullptr != projectName &&
                nullptr != activityDesc) {
              size_t dateTimeLen     = strlen(dateTime);
              size_t activityIdLen   = strlen(activityId);
              size_t projectNameLen  = strlen(projectName);
              size_t activityDescLen = strlen(activityDesc);

              size_t entryLen =
                dateTimeLen     + fieldDelimLen +
                activityIdLen   + fieldDelimLen +
                projectNameLen  + fieldDelimLen +
                activityDescLen ;

              char *entry = (char *)calloc(entryLen+1, sizeof(*entry));
              snprintf(entry, entryLen+1, "%s%s%s%s%s%s%s",
                dateTime, fieldDelim,
                activityId, fieldDelim,
                projectName, fieldDelim,
                activityDesc
              );

              infof("log(%s): { 'dateTime': '%s', 'project': '%s', 'id': '%s', 'desc': '%s' }",
                filepath, dateTime, projectName, activityId, activityDesc
              );
              _board->appendWorkLogEntry(filepath, entry);

            }
            free(dateTime);

          } else {
            errf("%s", "failed to create log file");
          }

          // free the filepath string if it was allocated
          if (nullptr != filepath) {
            free(filepath);
          }

        } // _board
      } // project, activity
    } // interface
  } // timeKeeper
}

void WorkLogger::update(void)
{
  ; // empty
}

void WorkLogger::onMinuteChange(void)
{
  ; // empty
}

void WorkLogger::onSecondChange(void)
{
  ; // empty
}

void WorkLogger::onDayChange(void)
{
  ; // empty
}

void WorkLogger::onRegHoursWorked(void)
{
  ; // empty
}

void WorkLogger::onMaxHoursWorked(void)
{
  ; // empty
}

void WorkLogger::onWorkBlockChange(void)
{
  logWork();
}

void WorkLogger::onIsWorkingChange(void)
{
  if (nullptr != timeKeeper) {
    infof("is working: %s", timeKeeper->isWorking() ? "true" : "false");
  }
}
