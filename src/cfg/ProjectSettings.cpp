#include "ProjectSettings.h"

Activity::Activity(void):
  _id(nullptr),
  _desc(nullptr)
{
  ; // empty
}

Activity::Activity(const char *id, const char *desc):
  Activity()
{
  if (nullptr != id) {
    size_t len = strlen(id);
    if (len > 0) {
      _id = (char *)calloc(len+1, sizeof(*id));
      _id = (char *)strncpy(_id, id, len);
    }
  }
  if (nullptr != desc) {
    size_t len = strlen(desc);
    if (len > 0) {
      _desc = (char *)calloc(len+1, sizeof(*desc));
      _desc = (char *)strncpy(_desc, desc, len);
    }
  }
}

char *Activity::id(void) { return _id; }
char *Activity::desc(void) { return _desc; }

char *Activity::idDesc(void) // be sure to free() after use!
{
  static const char sep[] = ": ";

  size_t length  = 0;
  size_t copied  = 0;

  size_t idLen   = 0;
  size_t descLen = 0;
  size_t sepLen  = sizeof(sep);

  bool   incId   = false;
  bool   incDesc = false;

  // initial pass to determine buffer length
  char *idStr = id();
  if (nullptr != idStr) {
    idLen = strlen(idStr);
    if (idLen > 0) {
      length += idLen;
      incId = true;
    }
  }
  char *descStr = desc();
  if (nullptr != descStr) {
    descLen = strlen(descStr);
    if (descLen > 0) {
      length += descLen;
      incDesc = true;
    }
  }
  if (incId && incDesc) {
    length += sepLen;
  }

  // allocate buffer. caller is responsible for free()ing after use!
  char *idDesc = (char *)calloc(length+1, sizeof(char));

  // copy ID/description to the buffer
  if (incId) {
    if (copied + idLen <= length) {
      idDesc = strncat(idDesc, idStr, idLen);
      copied += idLen;
    }
    if (incDesc) {
      if (copied + sepLen <= length) {
        idDesc = strncat(idDesc, sep, sepLen);
        copied += sepLen;
      }
    }
  }
  if (incDesc) {
    if (copied + descLen <= length) {
      idDesc = strncat(idDesc, descStr, descLen);
      copied += descLen;
    }
  }
  return idDesc;
}


// -----------------------------------------------------------------------------


Project::Project(void):
  _name(nullptr),
  _acts(new LinkedList<Activity *>())
{
  ; // empty
}

Project::Project(const char *name):
  Project()
{
  if (nullptr != name) {
    size_t len = strlen(name);
    if (len > 0) {
      _name = (char *)calloc(len + 1, sizeof(*name));
      _name = (char *)strncpy(_name, name, len);
    }
  }
}

char *Project::name(void) { return _name; }

size_t Project::activityCount(void)
  { return nullptr != _acts ? _acts->size() : 0; }

void Project::add(Activity *act)
  { if (nullptr != _acts) { _acts->add(act); } }

Activity *Project::activity(int n)
  { return n < activityCount() ? _acts->get(n) : nullptr; }

char *Project::joinActivities(const char *sep) // be sure to free() after use!
{
  size_t count     = activityCount();
  size_t sepLength = nullptr != sep ? strlen(sep) : 0;
  size_t length    = count > 0 ? (count - 1) * sepLength : 0;
  size_t copied    = 0;

  // first pass to determine buffer length
  for (int i = 0; i < count; ++i) {
    Activity *a  = activity(i);       if (nullptr == a)      { continue; }
    char *idDesc = a->idDesc();       if (nullptr == idDesc) { continue; }
    length += strlen(idDesc);
    free(idDesc);
  }

  // allocate buffer. caller is responsible for free()ing!
  char *activities = (char *)calloc(length+1, sizeof(char));

  // second pass to build the string
  for (int i = 0; i < count; ++i) {
    Activity *a  = activity(i);       if (nullptr == a)      { continue; }
    char *idDesc = a->idDesc();       if (nullptr == idDesc) { continue; }
    // prepend the separator if this isn't the first element
    if (sepLength > 0 && i > 0) {
      if (copied + sepLength <= length) {
        activities = strncat(activities, sep, sepLength);
        copied += sepLength;
      }
    }
    // copy the activity ID/description to the result
    size_t idDescLength = strlen(idDesc);
    if (copied + idDescLength <= length) {
      activities = strncat(activities, idDesc, idDescLength);
      copied += idDescLength;
    }
    free(idDesc);
  }

  return activities;
}

// -----------------------------------------------------------------------------


ProjectSettings::ProjectSettings(void):
  _proj(new LinkedList<Project *>())
{
  ; // empty
}

bool ProjectSettings::loadStream(ReadBufferingStream &stream)
{
  DynamicJsonDocument doc(48*1024);
  DeserializationError error = deserializeJson(doc, stream);
  if (error) {
    errf("JSON parse error: %s", error.c_str());
    return false;
  }
  for (JsonVariant projJson : doc["projects"].as<JsonArray>()) {
    const char *name = projJson["name"];
    Project *p = new Project(name);
    for (JsonVariant actJson : projJson["activities"].as<JsonArray>()) {
      const char *id   = actJson["id"];
      const char *desc = actJson["description"];
      p->add(new Activity(id, desc));
    }
    _proj->add(p);
  }
  return true;
}

size_t ProjectSettings::projectCount(void)
  { return nullptr != _proj ? _proj->size() : 0; }

Project *ProjectSettings::project(int n)
  { return n < projectCount() ? _proj->get(n) : nullptr; }

char *ProjectSettings::joinProjects(const char *sep) // be sure to free() after use!
{
  size_t count     = projectCount();
  size_t sepLength = nullptr != sep ? strlen(sep) : 0;
  size_t length    = count > 0 ? (count - 1) * sepLength : 0;
  size_t copied    = 0;

  // first pass to determine buffer length
  for (int i = 0; i < count; ++i) {
    Project *p = project(i);     if (nullptr == p)    { continue; }
    char *name = p->name();      if (nullptr == name) { continue; }
    length += strlen(name);
  }

  // allocate buffer. caller is responsible for free()ing!
  char *projects = (char *)calloc(length+1, sizeof(char));

  // second pass to build the string
  for (int i = 0; i < count; ++i) {
    Project *p = project(i);     if (nullptr == p)    { continue; }
    char *name = p->name();      if (nullptr == name) { continue; }
    // prepend the separator if this isn't the first element
    if (sepLength > 0 && i > 0) {
      if (copied + sepLength <= length) {
        projects = strncat(projects, sep, sepLength);
        copied += sepLength;
      }
    }
    // copy the project name to the result
    size_t nameLength = strlen(name);
    if (copied + nameLength <= length) {
      projects = strncat(projects, name, nameLength);
      copied += nameLength;
    }
  }

  return projects;
}

char *ProjectSettings::joinActivities(int projectIndex, const char *sep)
{
  Project *p = project(projectIndex);
  return nullptr != p ? p->joinActivities(sep) : nullptr;
}
