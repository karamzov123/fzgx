"""Materialize a private input forest without following directory-link cycles."""
import os
from pathlib import Path
import shutil


def copy_private_inputs(inputs):
    """Copy (source, destination, omit_disc_images) roots.

    Internal aliases are remapped into the private forest. External files and
    directories are materialized, and backlinks target private copies, never the
    original input. No directory symlink is recursively followed by copytree.
    """
    roots = [(Path(src).resolve(strict=True), Path(dst), omit) for src, dst, omit in inputs]
    mapped = {src: dst for src, dst, _ in roots}

    def private_target(target):
        for src in sorted(mapped, key=lambda p: len(p.parts), reverse=True):
            if target.is_relative_to(src):
                return mapped[src] / target.relative_to(src)
        return None

    def clone(src, dst, omit_discs):
        mapped[src] = dst
        def ignore(_directory, names):
            return [name for name in names if Path(name).suffix.lower() in {'.iso', '.gcm'}] if omit_discs else []
        shutil.copytree(src, dst, symlinks=True, ignore=ignore)
        def walk_error(error):
            raise error
        for directory, directories, files in os.walk(src, followlinks=False, onerror=walk_error):
            for name in directories + files:
                link = Path(directory) / name
                local = dst / link.relative_to(src)
                if not link.is_symlink() or not local.is_symlink():
                    continue
                target = link.resolve(strict=True)
                private = private_target(target)
                local.unlink()
                if private is not None:
                    local.symlink_to(os.path.relpath(private, local.parent), target_is_directory=target.is_dir())
                elif target.is_dir():
                    clone(target, local, omit_discs)
                elif target.is_file():
                    shutil.copy2(target, local)
                    mapped[target] = local
                else:
                    raise RuntimeError(f'Unsupported input target: {link}')

    for src, dst, omit in roots:
        clone(src, dst, omit)
