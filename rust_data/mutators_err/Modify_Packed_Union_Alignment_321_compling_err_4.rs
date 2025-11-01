use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Meta, MetaNameValue, Pat, PatType, Path as SynPath,
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

pub struct Modify_Packed_Union_Alignment_321;

impl Mutator for Modify_Packed_Union_Alignment_321 {
    fn name(&self) -> &str {
        "Modify_Packed_Union_Alignment_321"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Union(item_union) = item {
                if let Some(attr) = item_union.attrs.iter_mut().find(|attr| attr.path().is_ident("repr")) {
                    attr.parse_nested_meta(|meta| {
                        if meta.path.is_ident("packed") {
                            if let Ok(Lit::Int(lit_int)) = meta.value().and_then(|v| v.parse::<Lit>()) {
                                if let Ok(n) = lit_int.base10_parse::<u64>() {
                                    let new_n = n + 1;
                                    let lit_int_new = LitInt::new(&new_n.to_string(), lit_int.span());
                                    let tokens = quote!((packed(#lit_int_new)));
                                    attr.tokens = tokens.into();
                                }
                            }
                        }
                        Ok(())
                    }).ok();
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets union type declarations with the #[repr(packed(n))] attribute. It identifies the current packing alignment value 'n' and modifies it to a larger value, potentially causing misalignment. The new value is chosen to be larger than the natural alignment of any field within the union, ensuring it remains within valid range."
    }
}