#!/bin/bash

checker=ceedling

echo ""
echo " ========================================================= "
echo "     Running Unit Tests                                    "
echo "     using $checker ...                                    "
echo " ========================================================= "
echo ""

$checker version
$checker clobber
$checker test:all
