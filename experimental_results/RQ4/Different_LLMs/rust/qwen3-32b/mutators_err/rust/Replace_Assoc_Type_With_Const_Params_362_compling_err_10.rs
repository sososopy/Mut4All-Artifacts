use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Assoc_Type_With_Const_Params_362;

impl Mutator for Replace_Assoc_Type_With_Const_Params_362 {
    fn name(&self) -> &str {
        "Replace_Assoc_Type_With_Const_Params_362"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if impl_item.trait_.is_none() {
                    if impl_item.generics.params.is_empty() {
                        impl_item.generics.params.push(parse_quote!(const C: usize));
                    }
                    for impl_item in &mut impl_item.items {
                        if let syn::ImplItem::Type(type_alias) = impl_item {
                            if type_alias.generics.params.is_empty() {
                                type_alias.generics.params.push(parse_quote!(const I: usize));
                            }
                            if let Type::Path(type_path) = &type_alias.ty {
                                let struct_name = type_path.path.segments[0].ident.to_string();
                                let new_type = parse_quote! {
                                    #struct_name::<C, I>
                                };
                                type_alias.ty = new_type;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}