# https://krakendev.io/blog/generating-warnings-in-xcode

TAGS="TODO:|FIXME:"
find "${SRCROOT}" \( -name "*.h" -or -name "*.c" -or -name "*.c++" -or -name "*.cpp" -or -name "*.hpp" \) -print0 | xargs -0 egrep --with-filename --line-number --only-matching "($TAGS).*\$" | perl -p -e "s/($TAGS)/ warning: \$1/"
