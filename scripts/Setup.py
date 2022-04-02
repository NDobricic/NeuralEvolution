import os
import subprocess
import platform
import sys

from SetupPython import PythonConfiguration as PythonRequirements

# Make sure everything we need for the setup is installed
PythonRequirements.Validate()

from SetupPremake import PremakeConfiguration as PremakeRequirements
os.chdir('./../') # Change from devtools/scripts directory to root

premakeInstalled = PremakeRequirements.Validate()

print("\nUpdating submodules...")
subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

if (premakeInstalled):
    if platform.system() == "Windows":
        print("\nRunning premake...")
        if len(sys.argv) < 2 or sys.argv[1] == "vs2019":
            subprocess.call([os.path.abspath("./scripts/GenProjects-vs2019.bat"), "nopause"])
            print("\nSetup completed!")
        elif sys.argv[1] == "codeblocks":
            subprocess.call([os.path.abspath("./scripts/GenProjects-codeblocks.bat"), "nopause"])
            print("\nSetup completed!")
        else:
            print("\nInvalid argument {sys.argv[1]}.")
    else:
        print("\nPlatform not supported.")
else:
    print("Hazel requires Premake to generate project files.")

