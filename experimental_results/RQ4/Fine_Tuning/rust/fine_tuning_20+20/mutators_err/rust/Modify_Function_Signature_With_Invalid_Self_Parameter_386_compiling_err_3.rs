use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
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

pub struct Modify_Function_Signature_With_Invalid_Self_Parameter_386;

impl Mutator for Modify_Function_Signature_With_Invalid_Self_Parameter_386 {
    fn name(&self) -> &str {
        "Modify_Function_Signature_With_Invalid_Self_Parameter_386"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_impl = false;
                for attr in &item_fn.attrs {
                    if attr.path().is_ident("cfg") {
                        if attr.parse_args_with(|input: ParseStream| {
                            input.parse::<Meta>().map(|_| ())
                        }).is_ok() {
                            if attr
                                .tokens
                                .to_string()
                                .contains("target_family = \"wasm\"")
                            {
                                has_impl = true;
                                break;
                            }
                        }
                    }
                }
                if !has_impl {
                    item_fn.sig.inputs.insert(0, parse_quote!(&self));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets standalone functions by injecting an invalid `&self` parameter into their signatures. It avoids modifying functions within `impl` blocks or traits by checking for `wasm` target-specific attributes, which are used here as a heuristic to identify associated functions. This transformation is designed to provoke semantic errors related to improper use of `self` in non-associated contexts, thereby testing the compiler's robustness in handling function signature constraints."
    }
}