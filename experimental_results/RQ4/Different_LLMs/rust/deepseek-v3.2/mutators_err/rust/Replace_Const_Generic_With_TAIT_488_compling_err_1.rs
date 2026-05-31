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

pub struct Replace_Const_Generic_With_TAIT_488;

impl Mutator for Replace_Const_Generic_With_TAIT_488 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_TAIT_488"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut tait_aliases = Vec::new();
        for item in &file.items {
            if let Item::Type(item_type) = item {
                if let Type::ImplTrait(_) = &item_type.ty {
                    tait_aliases.push(item_type.ident.clone());
                }
            }
        }
        if tait_aliases.is_empty() {
            return;
        }
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if !item_fn.sig.asyncness.is_some() {
                    continue;
                }
                for param in &mut item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &const_param.ty {
                            let selected_alias = tait_aliases.choose(&mut rng).unwrap();
                            let new_type: Type = parse_quote!(crate::#selected_alias);
                            const_param.ty = new_type;
                        }
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(impl_fn) = impl_item {
                        if !impl_fn.sig.asyncness.is_some() {
                            continue;
                        }
                        for param in &mut impl_fn.sig.generics.params {
                            if let GenericParam::Const(const_param) = param {
                                if let Type::Path(type_path) = &const_param.ty {
                                    let selected_alias = tait_aliases.choose(&mut rng).unwrap();
                                    let new_type: Type = parse_quote!(crate::#selected_alias);
                                    const_param.ty = new_type;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions with const generic parameters. It scans the module for type alias impl trait (TAIT) definitions and replaces concrete const generic parameter types with references to existing TAITs (e.g., crate::Alias). This transformation tests the compiler's ability to handle const generics with opaque types in async contexts, potentially triggering internal compiler errors related to type resolution, const evaluation, and async function code generation."
    }
}