use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemMacro, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Insert_Invalid_Attr_On_MacroStmt_231;

impl Mutator for Insert_Invalid_Attr_On_MacroStmt_231 {
    fn name(&self) -> &str {
        "Insert_Invalid_Attr_On_MacroStmt_231"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(macro_item) = item {
                let macro_str = macro_item.tokens.to_string();
                if macro_str.contains("$s:stmt") {
                    let re = Regex::new(r"(=>\s*\{[^}]*)\$(s)\b").unwrap();
                    let modified_str = re.replace_all(&macro_str, "$1#[test] $${$2}").to_string();
                    if let Ok(tokens) = syn::parse_str(&modified_str) {
                        macro_item.tokens = tokens;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts an invalid attribute (e.g., #[test]) on a macro-generated statement that is expected to be a tail expression. This forces the parser to misinterpret the annotated statement as a non-tail expression, exposing compiler handling of attribute placement during macro expansion. The transformation targets macros that accept $stmt:stmt patterns, modifying their expansion to apply mismatched attributes."
    }
}