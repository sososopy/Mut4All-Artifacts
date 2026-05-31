use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Substitute_Inline_Assembly_Macro_Argument_177;

impl Mutator for Substitute_Inline_Assembly_Macro_Argument_177 {
    fn name(&self) -> &str {
        "Substitute_Inline_Assembly_Macro_Argument_177"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                if mac.mac.path.is_ident("global_asm") {
                    let mut new_tokens = mac.mac.tokens.clone();
                    let mut replaced = false;
                    for token in new_tokens.iter_mut() {
                        if let TokenTree::Group(group) = token {
                            if let Some(Expr::Path(path)) = group.stream().into_iter().next() {
                                if path.path.is_ident("sym") {
                                    let new_expr = parse_quote!({
                                        || {};
                                        0
                                    });
                                    group.stream = proc_macro2::TokenStream::from(new_expr);
                                    replaced = true;
                                    break;
                                }
                            }
                        }
                    }
                    if replaced {
                        mac.mac.tokens = new_tokens;
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator substitutes the inline assembly macro argument with a new argument that includes a nested function or closure. This transformation increases the complexity of the code and tests the compiler's ability to handle such constructs, potentially exposing bugs related to inline assembly and nested functions or closures."
    }
}