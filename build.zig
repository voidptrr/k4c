const std = @import("std");

const c_flags = &.{
    "-std=c17",
    "-Wall",
    "-Wextra",
    "-Wpedantic",
};

fn addCommonSettings(b: *std.Build, module: *std.Build.Module) void {
    module.addIncludePath(b.path("include"));
    module.addIncludePath(b.path("src"));
}

fn pathLessThan(_: void, lhs: []const u8, rhs: []const u8) bool {
    return std.mem.lessThan(u8, lhs, rhs);
}

fn discoverCFiles(b: *std.Build, root_path: []const u8) []const []const u8 {
    const allocator = b.allocator;
    const io = b.graph.io;
    var root = std.Io.Dir.cwd().openDir(io, root_path, .{ .iterate = true }) catch |err| {
        std.debug.panic("unable to open {s}: {s}", .{ root_path, @errorName(err) });
    };
    defer root.close(io);

    var walker = root.walk(allocator) catch @panic("OOM");
    defer walker.deinit();

    var files = std.array_list.Managed([]const u8).init(allocator);
    while (walker.next(io) catch |err| {
        std.debug.panic("unable to walk {s}: {s}", .{ root_path, @errorName(err) });
    }) |entry| {
        if (entry.kind != .file or !std.mem.endsWith(u8, entry.path, ".c")) {
            continue;
        }

        const path = std.fs.path.join(allocator, &.{ root_path, entry.path }) catch @panic("OOM");
        files.append(path) catch @panic("OOM");
    }

    std.sort.heap([]const u8, files.items, {}, pathLessThan);
    return files.toOwnedSlice() catch @panic("OOM");
}

fn testNameFromPath(b: *std.Build, path: []const u8) []const u8 {
    const prefix = "tests/";
    const suffix = "_test.c";
    const relative = if (std.mem.startsWith(u8, path, prefix)) path[prefix.len..] else path;
    const without_suffix = if (std.mem.endsWith(u8, relative, suffix))
        relative[0 .. relative.len - suffix.len]
    else
        relative;
    const name = b.allocator.dupe(u8, without_suffix) catch @panic("OOM");
    std.mem.replaceScalar(u8, name, '/', '-');
    std.mem.replaceScalar(u8, name, '_', '-');
    return name;
}

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const sources = discoverCFiles(b, "src");
    const tests = discoverCFiles(b, "tests");

    const lib_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });
    addCommonSettings(b, lib_mod);
    lib_mod.addCSourceFiles(.{
        .files = sources,
        .flags = c_flags,
    });

    const lib = b.addLibrary(.{
        .name = "vstd",
        .root_module = lib_mod,
        .linkage = .static,
    });
    b.installArtifact(lib);
    b.getInstallStep().dependOn(&b.addInstallDirectory(.{
        .source_dir = b.path("include"),
        .install_dir = .header,
        .install_subdir = "",
    }).step);

    const test_step = b.step("test", "Build and run the C test suite");
    for (tests) |test_path| {
        const test_mod = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
        });
        addCommonSettings(b, test_mod);
        test_mod.addCSourceFile(.{
            .file = b.path(test_path),
            .flags = c_flags,
        });

        const exe = b.addExecutable(.{
            .name = testNameFromPath(b, test_path),
            .root_module = test_mod,
        });
        test_mod.linkLibrary(lib);

        const run = b.addRunArtifact(exe);
        test_step.dependOn(&run.step);
    }
}
