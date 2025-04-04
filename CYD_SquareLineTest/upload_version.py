Import("env")

def after_build(source, target, env):
    print("Running uploadfs after build...")
    env.Execute("pio run --target uploadfs")

env.AddPostAction("buildprog", after_build)