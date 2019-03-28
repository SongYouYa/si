
TEMPLATE = subdirs

SUBDIRS += \
    test \
    sinetwork \
    sifile

sifile.subdir = common/sifile
sinetwork.subdir = common/sinetwork
test.subdir = common/test

test.depends = sinetwork sifile
