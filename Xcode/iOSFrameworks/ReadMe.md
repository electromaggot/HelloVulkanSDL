Tiny project to build SDL as iOS Frameworks

Why?  Because SDL only builds static binaries, i.e. '.a' files, not
the more-convenient '.framework's for iOS.

Our other SDL-dependent repositories rely upon & include these frameworks.

Currently supports:  SDL  and  SDL_image

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Thus open:  SDL-iOS.xcodeproj  then  SDL_image-iOS.xcodeproj
   and do:  BuildFatFramework

which build:  SDL_iOS.framework ,  SDL_image_iOS.framework

(You can ignore the "iOSFrameworksBuild.sh" script in this directory,
 although it does serve some educational purpose if you get stuck or
 wish to extend what's done here.)

