#include "WorkLogger.h"

#include "../time/TimeKeeper.h"
#include "../ui/Interface.h"

#define FIELD_DELIM "\t"

WorkLogger *workLogger;

WorkLogger::WorkLogger(Board *board):
  _board(board)
{
  ; // empty
}

void WorkLogger::logWork(void)
{
  static const char fieldDelim[] = FIELD_DELIM;
  static const size_t fieldDelimLen = sizeof(fieldDelim);

  if (nullptr != timeKeeper && nullptr != interface && nullptr != _board) {

    Project *project = interface->project();
    Activity *activity = interface->activity();
    if (nullptr != project && nullptr != activity) {

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
          size_t workBlockLen    = WorkLogger::digitCount(WORK_BLOCK_MINUTES);
          size_t activityIdLen   = strlen(activityId);
          size_t projectNameLen  = strlen(projectName);
          size_t activityDescLen = strlen(activityDesc);

          size_t entryLen =
            dateTimeLen     + fieldDelimLen +
            workBlockLen    + fieldDelimLen +
            activityIdLen   + fieldDelimLen +
            projectNameLen  + fieldDelimLen +
            activityDescLen ;

          char *entry = (char *)calloc(entryLen+1, sizeof(*entry));
          snprintf(entry, entryLen+1, "%s%s%d%s%s%s%s%s%s",
            dateTime,           fieldDelim,
            WORK_BLOCK_MINUTES, fieldDelim,
            activityId,         fieldDelim,
            projectName,        fieldDelim,
            activityDesc
          );
          // infof("log(%s): { 'dateTime': '%s', 'minutes': '%s', 'project': '%s', 'id': '%s', 'desc': '%s' }",
          //   filepath, dateTime, WORK_BLOCK_MINUTES, projectName, activityId, activityDesc
          // );
          if (!_board->appendWorkLogEntry(filepath, entry)) {
            errf("%s", "failed to write log entry");
          }
        }
        free(dateTime);

      } else {
        errf("%s", "failed to create log file");
      }

      // free the filepath string if it was allocated
      if (nullptr != filepath) {
        free(filepath);
      }

    } // project, activity
  } // timeKeeper, interface, _board
}

uint32_t WorkLogger::parseMinutes(char *line)
{
  char *token = strtok(line, FIELD_DELIM); // first token is timestamp
  if (nullptr != token) {
    token = strtok(0, FIELD_DELIM); // second token is minutes
    if (nullptr != token) {
      return (uint32_t)strtoul(token, 0, 10);
    }
  }

  return 0;
}

bool WorkLogger::totalMinutesWorked(uint32_t *minutes)
{
  bool success = false;

  if (nullptr != timeKeeper && nullptr != _board) {

    char *filepath = nullptr;
    success =
      _board->assertWorkLog(&filepath, timeKeeper->timeStamp()) &&
      _board->parseWorkLogMinutes(minutes, filepath);

    // free the filepath string if it was allocated
    if (nullptr != filepath) {
      free(filepath);
    }
  }

  return success;
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
  if (nullptr != timeKeeper) {
    if (timeKeeper->isWorking()) {
      logWork();
    }
  }
}

void WorkLogger::onIsWorkingChange(void)
{
  if (nullptr != timeKeeper) {
    infof("is working: %s", timeKeeper->isWorking() ? "true" : "false");
  }
}
