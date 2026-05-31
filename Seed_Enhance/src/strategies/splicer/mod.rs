use core::str;
use ouroboros::self_referencing;
use std::{
    collections::HashMap, error::Error, ffi::OsStr, fs::read, path::PathBuf, sync::LazyLock,
};
use tree_sitter::{Language, Tree};
use tree_splicer::{
    node_types::NodeTypes,
    splice::{Config, Splicer},
};
use walkdir::WalkDir;

use crate::{conf::Args, fuzz::fuzzbase::code_mask_feature, info, util::glob_next};

use crate::fuzz::{
    feature_list::{FORBID_FEATURE, INCOMPLETE_FEATURE},
    fuzzbase::Fuzzer,
};

static NODE_TYPES: LazyLock<NodeTypes> =
    LazyLock::new(|| NodeTypes::new(tree_sitter_rust::NODE_TYPES).unwrap());
static LANGUAGE: LazyLock<Language> = LazyLock::new(tree_sitter_rust::language);

static EXCEPTIONS: &[&str] = &[
    // runtime
    "ui/closures/issue-72408-nested-closures-exponential.rs",
    "ui/issues/issue-74564-if-expr-stack-overflow.rs",
    "library/stdarch/crates/core_arch/src/mod.rs", //10+ mins
    // memory
    "ui/issues/issue-50811.rs",
    "ui/issues/issue-29466.rs",
    "src/tools/miri/tests/run-pass/float.rs",
    "ui/numbers-arithmetic/saturating-float-casts-wasm.rs",
    "ui/numbers-arithmetic/saturating-float-casts-impl.rs",
    "ui/numbers-arithmetic/saturating-float-casts.rs",
    "ui/wrapping-int-combinations.rs",
    // glacier/memory/time:
    "fixed/23600.rs",
    "23600.rs",
    "fixed/71699.rs",
    "71699.rs",
    // runtime
    "library/stdarch/crates/core_arch/src/x86/avx512bw.rs",
    "library/stdarch/crates/core_arch/src/x86/mod.rs",
    // 3.5 hours when reporting errors :(
    "library/stdarch/crates/core_arch/src/lib.rs",
    // memory 2.0
    "run-make-fulldeps/issue-47551/eh_frame-terminator.rs",
    // infinite recursion in rustdoc, can take tens of minutes in ci
    "ui/recursion/issue-38591-non-regular-dropck-recursion.rs",
    "ui/dropck/dropck_no_diverge_on_nonregular_2.rs",
    "ui/dropck/dropck_no_diverge_on_nonregular_1.rs",
    // 900 mb output, can take 5-10 minutes
    "run-make-fulldeps/issue-47551/eh_frame-terminator.rs",
    // very slow
    "library/stdarch/crates/core_arch/src/x86/mod.rs",
    "library/core/src/lib.rs",
    "library/stdarch/crates/core_arch/src/mod.rs",
    "compiler/rustc_middle/src/lib.rs",
    "library/stdarch/crates/core_arch/src/x86/avx512f.rs",
    "ui/structs-enums/struct-rec/issue-84611.rs",
    "ui/structs-enums/struct-rec/issue-74224.rs",
    "ui/dropck/dropck_no_diverge_on_nonregular_3.rs",
    "library/portable-simd/crates/core_simd/src/lib.rs", // 12+ minutes
    "ui-fulldeps/myriad-closures.rs",
    "src/tools/miri/tests/pass/float.rs",
    "library/stdarch/crates/core_arch/src/arm_shared/neon/generated.rs",
    "library/stdarch/crates/core_arch/src/aarch64/mod.rs",
    "library/stdarch/crates/core_arch/src/aarch64/neon/generated.rs",
    "library/stdarch/crates/core_arch/src/aarch64/neon/mod.rs",
    "src/tools/cargo/tests/testsuite/main.rs",
    "src/tools/clippy/clippy_lints/src/lib.rs",
    "library/stdarch/crates/stdarch-gen/src/main.rs",
    "src/tools/rust-analyzer/crates/proc-macro-srv/src/abis/abi_1_58/proc_macro/mod.rs",
    "src/tools/rust-analyzer/crates/proc-macro-srv/src/abis/abi_1_63/proc_macro/mod.rs",
    "ui/issues/issue-22638.rs",
    "ui/issues/issue-72933-match-stack-overflow.rs",
    "ui/recursion/issue-86784.rs",
    "ui/associated-types/issue-67684.rs",
];

fn parse(s: &str) -> Result<Tree, Box<dyn Error>> {
    let mut parser = tree_sitter::Parser::new();
    parser.set_language(*LANGUAGE)?;
    let res = parser.parse(s, None).ok_or("tree-sitter parse failed!")?;
    Ok(res)
}

type TreeHolder = HashMap<String, (Vec<u8>, Tree)>;
fn parse_dir(p: &Vec<PathBuf>) -> Result<TreeHolder, Box<dyn Error>> {
    let mut res = HashMap::new();
    for d in p {
        info!("Loading dir {}", d.as_os_str().to_str().unwrap_or_default());
        'continue_here: for f in WalkDir::new(d) {
            let f = f?;
            if f.file_type().is_dir() {
                continue;
            }
            if f.path().extension() != Some(OsStr::new("rs")) {
                continue;
            }
            if EXCEPTIONS.iter().any(|s| f.path().ends_with(s)) {
                continue;
            }
            let path = f.path();
            let s = read(path)?;
            let c = str::from_utf8(&s)?;

            for f in FORBID_FEATURE {
                if c.contains(f) {
                    continue 'continue_here;
                }
            }

            let tree = parse(c)?;
            let path = path
                .as_os_str()
                .to_str()
                .ok_or("Path not stringable")?
                .to_string();
            res.insert(path, (s, tree));
        }
    }
    Ok(res)
}

fn do_splicer(trees: &TreeHolder, seed: Option<u64>) -> Result<Splicer, Box<dyn Error>> {
    let seed = match seed {
        Some(s) => s,
        None => glob_next(),
    };
    let config = Config {
        chaos: 5,
        deletions: 5,
        language: *LANGUAGE,
        inter_splices: 16,
        max_size: 1024 * 1024, // 1M
        node_types: NODE_TYPES.clone(),
        reparse: 2,
        seed,
    };
    let splicer = Splicer::new(config, trees).ok_or("Init splicer failed, no files.")?;
    Ok(splicer)
}

#[self_referencing]
struct SplicerFuzzerHolder {
    trees: TreeHolder,
    #[borrows(trees)]
    #[not_covariant]
    splicer: Splicer<'this>,
}
impl Clone for SplicerFuzzerHolder {
    fn clone(&self) -> Self {
        let trees = self.borrow_trees().clone();
        let res = SplicerFuzzerHolderBuilder {
            trees,
            splicer_builder: |t| do_splicer(t, Some(glob_next())).unwrap(),
        }
        .build();
        res
    }
}

#[derive(Clone)]
pub struct SplicerFuzzer {
    inner: SplicerFuzzerHolder,
}
impl Fuzzer for SplicerFuzzer {
    #[allow(clippy::new_ret_no_self)]
    fn new(args: &Args) -> Result<Box<dyn Fuzzer>, Box<dyn Error>> {
        let dir = &args.input;
        let trees = parse_dir(dir)?;
        let res = SplicerFuzzerHolderBuilder {
            trees,
            splicer_builder: |t| do_splicer(t, Some(glob_next())).unwrap(),
        }
        .build();
        let res = Self { inner: res };
        Ok(Box::new(res))
    }
    fn generate(&mut self) -> Result<Vec<u8>, Box<dyn Error>> {
        'continue_here: for _ in 0..20 {
            let code = self
                .inner
                .with_mut(|field| field.splicer.next())
                .ok_or("Ref err?")?;
            let c = str::from_utf8(&code)?;

            for f in FORBID_FEATURE {
                if c.contains(f) {
                    continue 'continue_here;
                }
            }
            for f in INCOMPLETE_FEATURE {
                if c.contains(f) {
                    continue 'continue_here;
                }
            }

            let res = code_mask_feature(&code)?;
            return Ok(res);
        }
        let def_res = b"fn main(){}";
        Ok(def_res.to_vec())
    }
}
