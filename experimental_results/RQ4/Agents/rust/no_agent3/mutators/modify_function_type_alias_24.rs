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

pub struct Modify_Function_Type_Alias_24;

impl Mutator for Modify_Function_Type_Alias_24 {
    fn name(&self) -> &str {
        "Modify_Function_Type_Alias_24"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Type(type_alias) = item {
                if let Type::Path(TypePath { path, .. }) = &*type_alias.ty {
                    if let Some(last_segment) = path.segments.last() {
                        if let PathArguments::AngleBracketed(args) = &last_segment.arguments {
                            if last_segment.ident == "FnOnce" {
                                let mut new_args = args.clone();
                                new_args.args.push(parse_quote!(u32));
                                let new_return_type: Type = parse_quote!(u16);
                                let new_type: Type = parse_quote!(FnOnce(u32) -> u16);
                                type_alias.ty = Box::new(new_type);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets type aliases defining function types. It identifies type aliases that represent function types and modifies the function signature by adding a parameter or changing the return type. This mutation aims to explore edge cases in the const evaluation of function types, particularly in complex structures, to stress the compiler's type inference mechanisms."
    }
}