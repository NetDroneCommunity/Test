#!/bin/bash
#
# NetDrone Engine
# Copyright © 2022 Origin Studio Inc.
#

# export MSBuildSDKsPath="/opt/dotnet_core/sdk/$(dotnet --version)/Sdks"

sh ./build/dotnet/make-src-clean.sh

dotnet build -c Debug ./consolechat-dotnet.sln
dotnet build -c Release ./consolechat-dotnet.sln
