use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemEnum, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Meta, MetaList, NestedMeta,
    Pat, PatType, Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier,
    Type, TypeImplTrait, TypeParamBound, TypePath, parse_quote, punctuated::Punctuated,
    spanned::Spanned, token, token::Comma, token::{Paren, Plus}, visit::Visit, visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Change_Repr_Packed_To_C_330;

impl Mutator for Change_Repr_Packed_To_C_330 {
    fn name(&self) -> &str {
        "Change_Repr_Packed_To_C_330"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(ItemEnum { attrs, .. }) = item {
                for attr in attrs.iter_mut() {
                    if attr.path().is_ident("repr") {
                        if let Ok(Meta::List(meta_list)) = attr.parse_meta() {
                            if meta_list.tokens.to_string().contains("packed") {
                                *attr = parse_quote! { #[repr(C)] };
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies enum definitions with the #[repr(packed)] attribute and replaces it with #[repr(C)]. This transformation aims to address potential alignment issues associated with packed representations by switching to a C representation, which maintains a defined layout while avoiding alignment problems."
    }
}