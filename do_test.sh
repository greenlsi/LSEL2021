#/bin/sh
# Absolute path to this script
SCRIPT=$(readlink -f "$0")
# Absolute path this script is in
SCRIPTPATH=$(dirname "$SCRIPT")

for d in $SCRIPTPATH/*/ ; do
  if test -d $d/test ; then
    cd $d
    ceedling test:all
  fi
done

