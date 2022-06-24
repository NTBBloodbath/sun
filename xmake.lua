-- project
set_project("sunc")

-- version
set_version("0.1.0a", { build = "%Y-%m-%d_%H:%M:%S" })

-- set xmake min version
set_xmakever("2.6.0")

-- '-Wall,-Wextra' equivalent
set_warnings("allextra")

-- set C++ standards
set_languages("cxx17")

-- build modes
add_rules("mode.debug", "mode.release", "mode.releasedbg", "mode.valgrind")
if is_mode("release") then
  -- '-O2' equivalent
  set_optimize("faster")
  -- strip unneeded debugging symbols
  set_strip("all")
  -- hide symbols
  set_symbols("hidden")
end

-- set default build mode to debugging
-- NOTE: we will probably want to switch it to releasedbg later
set_defaultmode("debug")

-- third-party dependencies
for _, dep in ipairs(os.dirs("$(projectdir)/third-party"), path.basename) do
  add_includedirs(dep)
end
add_requires("catch2", { optional = true })

-- enable version locks, similar to cargo.lock
set_policy("package.requires_lock", true)

-- headers directories
add_includedirs("$(projectdir)/src")

-- default target
target("sun")
  set_kind("binary")
  set_default(true)

  -- source files
  add_files("$(projectdir)/src/*.cpp")

  -- dependencies (managed by xmake)
  add_packages("catch2")

-- format source code
target("fmt")
  set_default(false)

  on_run(function(_)
    cprint("${bright green}[ 56%%]:${clear} Formatting Sun source code ...")
    local inc_files = os.files("$(projectdir)/src/*.hpp")
    local src_files = os.files("$(projectdir)/src/*.cpp")
    table.join2(src_files, inc_files)
    for _, filepath in ipairs(src_files) do
      os.run("clang-format -style=file --verbose --sort-includes -i " .. filepath)
    end
    cprint("${bright green}[100%%]:${clear} Done")
  end)
  on_uninstall(function(_)
    return
  end)

-- generate documentation
target("docs")
  set_default(false)

  on_run(function(_)
    cprint("${bright green}[ 56%%]:${clear} Generating documentation ...")
    os.run("doxygen Doxyfile")
    cprint("${bright green}[100%%]:${clear} Done")
  end)
  on_uninstall(function(_)
    return
  end)
