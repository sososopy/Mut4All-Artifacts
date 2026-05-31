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

pub struct Add_Recursive_Type_Alias_411;

impl Mutator for Add_Recursive_Type_Alias_411 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_Alias_411"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let recursive_type_name = Ident::new("RecursiveType", Span::call_site());
        let recursive_type_alias: Item = parse_quote! {
            type #recursive_type_name = Option<Box<#recursive_type_name>>;
        };
        if rng.gen_bool(0.5) {
            file.items.push(recursive_type_alias);
        }
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident != "main" {
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if rng.gen_bool(0.5) {
                                pat_type.ty = Box::new(Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(recursive_type_name.clone()),
                                }));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive type alias `type RecursiveType = Option<Box<RecursiveType>>;` and randomly replaces function parameter types with this alias. This transformation creates deeply nested types that can challenge the compiler's type resolution and recursion handling mechanisms, potentially leading to ICEs or stack overflows during compilation."
    }
}