###########################################################################################################
# This script will automatically be called by PlatformIO during the build process (as pre-action script)
# It is responsible for uploading the files to littleFS
#
# You do NOT need to run it manually
###########################################################################################################

Import("env")

def before_upload(source, target, env):
    env.Execute(f"pio run -e {env['PIOENV']} --target uploadfs")

env.AddPreAction("upload", before_upload)