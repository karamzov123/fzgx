#!/usr/bin/env python3
"""Entry point: `uv run tools/fzgx.py <command>` (see tools/fzgx/cli.py)."""

import sys

from fzgx.cli import main

if __name__ == "__main__":
    sys.exit(main())
