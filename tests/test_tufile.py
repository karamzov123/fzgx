from fzgx import tufile


SAMPLE = '''#include "types.h"
#include "rel/main_rel/camera.h"

/* fzgx:begin fn_a */
extern u32 helper(void);

u32 fn_a(void) {
    return helper();
}
/* fzgx:end fn_a */

/* fzgx:begin fn_b noprologue */
#include "types.h"
u32 fn_b(void) { return 1; }
/* fzgx:end fn_b */
'''


def test_parse_render_round_trip():
    tf = tufile.parse(SAMPLE)
    assert tf.prologue == '#include "types.h"\n#include "rel/main_rel/camera.h"\n'
    assert [b.name for b in tf.blocks] == ["fn_a", "fn_b"]
    assert tf.blocks[1].flags == ["noprologue"]
    assert tf.render() == SAMPLE


def test_gen_text_uses_prologue_unless_flagged():
    tf = tufile.parse(SAMPLE)
    assert tufile.gen_text(tf, "fn_a").startswith('#include "types.h"\n#include "rel/main_rel/camera.h"\n\nextern u32 helper')
    assert tufile.gen_text(tf, "fn_b") == '#include "types.h"\nu32 fn_b(void) { return 1; }\n'
    assert "not matched yet" in tufile.gen_text(tf, "fn_missing")


def test_split_includes_and_merge_prologue():
    inc, body = tufile.split_includes('#include "rel/main_rel/globals.h"\n\nvoid f(void) {}\n#include "x.h"\n')
    assert inc == ['#include "rel/main_rel/globals.h"', '#include "x.h"']
    assert body == "void f(void) {}\n"
    pro = tufile.merge_prologue('#include "rel/main_rel/camera.h"\n', ['#include "types.h"', '#include "rel/main_rel/camera.h"'])
    assert pro == '#include "types.h"\n#include "rel/main_rel/camera.h"\n'
    assert tufile.merge_prologue(pro, ['#include "types.h"']) == pro


def test_unterminated_block_is_an_error():
    try:
        tufile.parse("/* fzgx:begin x */\nint x;\n")
    except ValueError as e:
        assert "unterminated" in str(e)
    else:
        raise AssertionError("expected ValueError")
