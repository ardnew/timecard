#ifndef __PROJECT_SETTINGS_H__
#define __PROJECT_SETTINGS_H__

#include "../util/LinkedList.h"
#include "../../timecard.h"

class Activity
{
protected:
  char *_id;
  char *_desc;
public:
  Activity(void);
  Activity(const char *id, const char *desc);
  char *id(void);
  char *desc(void);
  char *idDesc(void);
};

class Project
{
protected:
  char *_name;
  LinkedList<Activity *> *_acts;
public:
  Project(void);
  Project(const char *name);
  char *name(void);
  size_t activityCount(void);
  void add(Activity *act);
  Activity *activity(int n);
  char *joinActivities(const char *sep);
};

class ProjectSettings
{
protected:
  LinkedList<Project *> *_proj;
public:
  ProjectSettings(void);
  bool loadStream(ReadBufferingStream &stream);
  size_t projectCount(void);
  Project *project(int n);
  char *joinProjects(const char *sep);
  char *joinActivities(int projectIndex, const char *sep);
};

#endif // __PROJECT_SETTINGS_H__
