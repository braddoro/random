#!/bin/bash
find . | xargs grep '$1' -sli
