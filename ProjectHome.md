The ofxEngine project is an addon for the openFrameworks creative C++ coding library, and is intended to turn oF into an engine for the development of video games. The engine will be segmented into three levels:

  1. A set of abstract base classes to provide interoperability and cofunctionality between different systems within the engine;
  1. A set of managerial and workhorse classes to serve in the operation of classes derived from Level-1 classes, and to provide an interface for controlled access to additional functionalities of oF and its other addons;
  1. A set of extensibility classes, including a specialized XML-based "Mod" system and extensible parser, to combine to allow different levels of customization both within and without C/C++ knowledge.

This project is dependant upon the openFrameworks C++ library (prerelease v0.06), available at http://www.openframeworks.cc/, as well as several of its addons.