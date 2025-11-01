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

pub struct Modify_Const_Generic_Parameters_186;

impl Mutator for Modify_Const_Generic_Parameters_186 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Parameters_186"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { generics, .. }) = item {
                for param in &mut generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(TypePath { path, .. }) = &*const_param.ty {
                            if !matches!(path.segments.last().unwrap().ident.to_string().as_str(), "usize" | "bool" | "char") {
                                const_param.ty = Box::new(parse_quote!(usize));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions with const generic parameters that are not of type integers, bool, or char. It modifies these const generic parameters to use a supported type, such as usize. This change ensures compliance with Rust's const generic constraints and tests the robustness of the compiler's const evaluation path."
    }
}