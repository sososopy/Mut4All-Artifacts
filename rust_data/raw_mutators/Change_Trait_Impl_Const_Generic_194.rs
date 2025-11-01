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

pub struct Change_Trait_Impl_Const_Generic_194;

impl Mutator for Change_Trait_Impl_Const_Generic_194 {
    fn name(&self) -> &str {
        "Change_Trait_Impl_Const_Generic_194"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    if let Some(last_segment) = path.segments.last() {
                        if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                            for arg in &args.args {
                                if let GenericArgument::Const(expr) = arg {
                                    if let Expr::Path(ExprPath { path, .. }) = expr {
                                        if let Some(ident) = path.get_ident() {
                                            if ident == "usize" || ident == "bool" || ident == "char" {
                                                let new_type: Type = parse_quote! { f64 };
                                                let new_generic_param: GenericParam = parse_quote! { const #ident: #new_type };
                                                item_impl.generics.params = Punctuated::new();
                                                item_impl.generics.params.push(new_generic_param);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations involving const generics. It identifies const generic parameters of integer, bool, or char types and changes them to a floating-point type, such as f64. This introduces a type error, as floating-point types are not valid for const generics, potentially revealing issues in the compiler's handling of const generics."
    }
}