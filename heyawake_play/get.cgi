#!/bin/bash
echo "Content-type: text/html"
echo ""
echo "<res>"
cat ./gamedata/$QUERY_STRING
echo "</res>"