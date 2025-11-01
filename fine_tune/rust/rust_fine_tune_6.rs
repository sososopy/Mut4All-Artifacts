use crate::mutator::Mutator;
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
pub struct Add_Generic_Type_Bounds_6;
impl Mutator for Add_Generic_Type_Bounds_6 {
    fn name(&self) -> &str {
        "Add_Generic_Type_Bounds_6 "
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let generics = &mut func.sig.generics;
                for param in &mut generics.params {
                    if let syn::GenericParam::Type(type_param) = param {
                        type_param.bounds.push(parse_quote!(?Sized));
                        type_param.bounds.push(parse_quote!(Sized));
                        type_param.bounds.push(parse_quote!(Default));
                        type_param.bounds.push(parse_quote!(Clone));
                        type_param.bounds.push(parse_quote!(PartialEq));
                        type_param.bounds.push(parse_quote!(Ord));
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(item) = impl_item {
                        let generics = &mut item.sig.generics;
                        for param in &mut generics.params {
                            if let syn::GenericParam::Type(type_param) = param {
                                type_param.bounds.push(parse_quote!(?Sized));
                                type_param.bounds.push(parse_quote!(Sized));
                                type_param.bounds.push(parse_quote!(Default));
                                type_param.bounds.push(parse_quote!(Clone));
                                type_param.bounds.push(parse_quote!(PartialEq));
                                type_param.bounds.push(parse_quote!(Ord));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets all generic type parameters in free functions and impl methods. It adds multiple trait bounds to each type parameter, including logically conflicting constraints like both `?Sized` and `Sized`, along with common traits such as `Default`, `Clone`, `PartialEq`, and `Ord`. This transformation stresses the compiler's generic constraint resolution system by introducing contradictory and heavy trait bounds, making it effective for uncovering weaknesses in type inference, trait solver logic, and compile-time bound checking. "
    }
}
