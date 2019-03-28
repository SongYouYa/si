
TEMPLATE = subdirs
SUBDIRS += \
    AnalyzerAssistant \
    assistant \
    siutility \
    filesystemshell  \
    SafeImager \
    DeviceTask \
    sifile \
    TemplateFile \
    FileAnalyzer \
    report \
    CommonAnalyzer \
    sinetwork \
    WindowsAnalyzer \
    MacAnalyzer

AnalyzerAssistant.subdir = application/AnalyzerAssistant
assistant.subdir = common/assistant
filesystemshell.subdir = common/filesystemshell
siutility.subdir = common/siutility
DeviceTask.subdir =application/DeviceTask
SafeImager.subdir = ui/SafeImager
sifile.subdir = common/sifile
TemplateFile.subdir = common/TemplateFile
FileAnalyzer.subdir = common/FileAnalyzer
report.subdir = common/report
CommonAnalyzer.subdir = application/CommonAnalyzer
sinetwork.subdir = common/sinetwork
WindowsAnalyzer.subdir = application/WindowsAnalyzer
MacAnalyzer.subdir = application/MacAnalyzer

DeviceTask.depends = sifile
assistant.depends = DeviceTask AnalyzerAssistant
TemplateFile.depends = FileAnalyzer
report.depends = filesystemshell siutility

WindowsAnalyzer.depends = CommonAnalyzer
MacAnalyzer.depends = CommonAnalyzer
AnalyzerAssistant.depends += WindowsAnalyzer MacAnalyzer

SafeImager.depends = assistant TemplateFile sinetwork report
