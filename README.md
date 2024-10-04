# Source map compiler tools
## Goals
- Bring tools up to the same feature set as Slammin' Source Map Tools ([Knockout thread](https://knockout.chat/thread/992/1), [Original Facepunch thread](http://web.archive.org/web/20190611221800/https://forum.facepunch.com/dev/bvenk/Slammin-Source-map-tools/))
- Add more features ?!!?!?
- Support for SP/MP/TF2 on single code base

## TODO
- Test (lol)
- Check the SSE/SSE2 versions of math functions aren't causing any bugs
- Attribute every feature and change where possible

## Global changes
- C++20
- Use a modified VPC to remove registry key requirement
- Reorganised VVIS and VRAD .vpc files

## VBSP
### New/Community features & fixes
- [Parallax corrected cubemap support](https://developer.valvesoftware.com/wiki/Parallax_Corrected_Cubemaps), specially [Mapbase's implementation](https://github.com/mapbase-source/source-sdk-2013/)
- Materialsystem is loaded when using the `VPROJECT` environment variable to determine the currently active game

### Replicated Slammin' features
- [x] Enabled SSE2
- [ ] Properly support power of 4 displacements _??? I've removed the warning, but no idea what was actually changed_
- [x] Added missing content types to csg.cpp
- [x] `-maxluxelscale`, lets you limit the maximum luxel scale in your map
- [x] Detail brushes now preserve their smoothing group
- [x] Stripped out .map displacement parser, you can't export .map files with dispinfo, this has been obsolete for over 15 years
- [ ] New `%compile` command for VMTs, `%compileNoShadows`, makes a material not recieve shadows, only direct light, this is used on water usually
- [ ] Any prop with `$staticprop` can now be a static prop (IE physics props)
- [x] Increased detail sprites limit
- [x] Custom lightmappedgeneric shaders are supported now (such as SDK_Lightmappedgeneric)
- [x] Implemented support for `func_detail_blocker`
- [ ] Improved brush precision
- [x] Leaktest is now enabled by default, command is now `-noleaktest`
- [x] `-nodefaultcubemap`, disables automatic skybox cubemap generation (necessary for 2013 SP)
- [x] `-blocksize`, lets you customise the size of visleaf splitting "blocks", default is every 1024 units ~~Removed~~ Readded
- [ ] ~~`-visgranularity`, lets you automatically place hint brushes in your map, syntax is `-visgranularity X Y Z`. X, Y and Z can be any integer value, for example, -visgranularity 0 0 512 would create a VIS split every 512 units vertically. (CSGO)~~ Removed
- [x] Subsitution of propper_models for prop_statics after Propper has run.

## Propper
- Merged with VBSP code but is its own product
- Probably works

## VVIS
### New/Community features & fixes
- 64-bit support! 3 different versions compiled with different instruction sets: SSE2, AVX and AVX2, which are automatically selected based on your CPU's capabilities. This has not been extensively tested but this should almost always give reasonably faster compile times

### Replicated Slammin' features
- [x] Enabled SSE2

## VRAD
### New/Community features & fixes
- [Removed debug counter that cripples lighting compile times](https://github.com/ValveSoftware/source-sdk-2013/pull/436)
- [Unlimited texlight definitions](https://github.com/Petercov/Source-PlusPlus/commit/d70d401d0006dec4c9600c0de7ec2216a205b1e0)
- [`-AllowSkyboxSample`](https://github.com/Petercov/Source-PlusPlus/commit/0354b917cf8548edf928cb7f92a41eace376eb39) which mulitiplies your chosen ambient light color by the color of the 2D skybox texture
- `"compresslightmaps"` keyvalue for `prop_static`. Not particularly recommended at lower lightmap resolutions as it's extremely crusty. An option for highly specific situations or of last resort
- Worldlight's made accessible as gamelumps. Useful for if you're using bsp compression and want [Dynamic RTT shadows](https://developer.valvesoftware.com/wiki/Dynamic_RTT_shadow_angles_in_Source_2007) to work _TODO include modified worldlight.cpp_
- Fixed texture shadows failing with multiple `$cdmaterials` defines _TODO verify this still works_
- 64 bit support! This has resulted in a performance gain so small it might as well be within margin of error. If you're using absurdly high lightmap resolutions you might want this
- Reduced lighting samples for props when not compiling with `-final`. Reduces compile times if you have a lot of props with lightmaps

### Replicated Slammin' features
- [x] Enabled SSE2
- [x] `-ambientocclusion`, `-aosamples` and `-aoscale`, adds ambient occlusion to your map and how many samples you want ([From CS:GO via G-String](https://github.com/Biohazard90/g-string_2013/commit/d3b4d1402693d75fe0fcdc67ae49bcfaa22a895e))
- [x] `-softencosine`, softens coloured lighting so it blends more accurately (See above)
- [ ] `-coring`, scale the light threshold before a luxel is completely unlit _this doesn't actually seem to do anything in slammin and is just a long since functioning debug argument_
- [ ] `-dispchop`, adjust the amount of patches generated for displacement faces
- [x] `-ambient`, when supplied with three RGB values, will apply a constant light value to every luxel in your map
- [x] `-reflectivityscale`, multiplier for the reflectivity of textures in your map
- [ ] Change `-final` to do extra things
- [ ] Improved displacement lightmap quality
- [x] `-nostaticproplighting`, `-nostaticproppolys` and `-notextureshadows`
- [ ] `-extrapasses`, lets you scale how many extra passes you want your map to go through, default is 4, differences above this value are extremely minimal _so I might not implement it_
- [ ] Any prop with `$staticprop` can now be a lit static prop (IE physics props)
- [ ] Single-line comments can now be placed in .rad files.
- [ ] Corrected issues related to luxel placement on displacement surfaces, luxels will now be correctly placed, resulting in higher quality lightmaps (with no extra cost) _This is probably what causes bizzare displacement lighting issues_
- [x] Enabled supersampling capabilities for displacement surfaces, further increases lightmap quality, you may experience a very slightly longer compile time
- [x] Defaults to `.dx90` VTX files when loading models now
- [ ] `-worldtextureshadows`, allows world polys to cast texture shadows
- [ ] `-translucentshadows` to enable the effect on $translucent surfaces.
- [ ] Transparent textures loaded by vrad are now bilinear filtered, for more accurate shadows
- [ ] `light_directional` support
- [ ] Improved description of options when showing help.
- [ ] `-skyboxrecurse`, 3D skybox recursion is disabled by default now
- [x] HDR is enabled by default

## Additional credits
- [alliedmodders/hl2sdk](https://github.com/alliedmodders/hl2sdk/tree/tf2) - code changes and tier0 and vstdlib libs for 64-bit support
- [craftablescience/sourcepp](https://github.com/craftablescience/sourcepp) - vtfpp for loading VTF's under 64-bit