"%JAVA_HOME%"\bin\javac.exe -d classes src\*.java
cd classes
"%JAVA_HOME%"\bin\jar cfe ../bin/ConfigRulesTool.jar ConfigRulesTool *.class