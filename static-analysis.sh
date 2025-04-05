#!/usr/bin/env bash

./build-example.sh
CodeChecker version
CodeChecker analyze build/compile_commands.json --ignore skip.list --enable sensitive --output build/reports
CodeChecker parse build/reports/ --export html --output build/reports_html
