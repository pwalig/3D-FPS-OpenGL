# usage:
# mesh name serves as a type
# object name serves as a script name
# custom properties serve as arguments
# all objects rotations must be set to XZY Euler
# collections hidden from render will not be exported

import bpy
import os
script_dir = os.path.dirname(__file__) #<-- absolute dir the script is in

for collection in bpy.data.collections:
    if collection.hide_render:
        continue
    scene_file = open(os.path.join(script_dir, "../" + collection.name + ".json"), "w")
    scene_file.write("{\n  \"scripts\" : [\n")
    last_obj = collection.all_objects[-1]
    for obj in collection.all_objects:
        scene_file.write("    {\n")
        scene_file.write("      \"type\" : \"" + obj.data.name + "\",\n")
        scene_file.write("      \"name\" : \"" + obj.name + "\",\n")
        scene_file.write("      \"args\" : {\n")
        scene_file.write("        \"position\" : {\n")
        scene_file.write("          \"x\" : " + str(-obj.location.x) + ",\n")
        scene_file.write("          \"y\" : " + str(obj.location.z) + ",\n")
        scene_file.write("          \"z\" : " + str(obj.location.y) + "\n")
        scene_file.write("        },\n")
        scene_file.write("        \"rotation\" : {\n")
        scene_file.write("          \"x\" : " + str(-obj.rotation_euler.x) + ",\n")
        scene_file.write("          \"y\" : " + str(obj.rotation_euler.z) + ",\n")
        scene_file.write("          \"z\" : " + str(obj.rotation_euler.y) + "\n")
        scene_file.write("        },\n")
        scene_file.write("        \"size\" : {\n")
        scene_file.write("          \"x\" : " + str(obj.scale.x) + ",\n")
        scene_file.write("          \"y\" : " + str(obj.scale.z) + ",\n")
        scene_file.write("          \"z\" : " + str(obj.scale.y) + "\n")
        scene_file.write("        }")
        for K in obj.keys():
                if K not in ['_RNA_UI', 'cycles']:
                    scene_file.write(",\n        \""+K+"\" : " + str(obj[K]))
        scene_file.write("\n      }\n")
        if obj == last_obj:
            scene_file.write("    }\n")
        else:
            scene_file.write("    },\n")
    scene_file.write("  ]\n}")