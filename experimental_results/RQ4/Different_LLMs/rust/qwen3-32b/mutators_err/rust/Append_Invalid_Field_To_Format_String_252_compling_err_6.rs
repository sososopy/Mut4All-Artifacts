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

pub struct Append_Invalid_Field_To_Format_String_252;

impl Mutator for Append_Invalid_Field_To_Format_String_252 {
    fn name(&self) -> &str {
        "Append_Invalid_Field_To_Format_String_252"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            match item {
                Item::Fn(item_fn) => {
                    for attr in &mut item_fn.attrs {
                        if let Ok(meta) = syn::parse2(attr.tokens().clone()) {
                            if let syn::Meta::NameValue(nv) = meta {
                                if let Some(ident) = nv.path.get_ident() {
                                    if ident == "error" {
                                        if let Expr::Lit(expr_lit) = nv.value {
                                            if let Lit::Str(lit_str) = &expr_lit.lit {
                                                let original = lit_str.value();
                                                let re = Regex::new(r"\{(\d+)\}").unwrap();
                                                if let Some(_) = re.find(&original) {
                                                    let new_str = re.replace(&original, r"{\$1.invalid}").to_string();
                                                    let new_lit = syn::LitStr::new(&new_str, lit_str.span());
                                                    expr_lit.lit = syn::Lit::Str(new_lit);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                },
                _ => {}
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator appends an invalid field access to a valid format string in derive macro attributes. It targets format strings like {0} and transforms them into {0.invalid}, creating invalid field references. This tests macro expansion paths that process format strings with invalid field accesses, potentially exposing bugs in format string parsing and error reporting."
    }
}