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

pub struct Modify_Function_Return_Type_With_Generics_213;

impl Mutator for Modify_Function_Return_Type_With_Generics_213 {
    fn name(&self) -> &str {
        "Modify_Function_Return_Type_With_Generics_213"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let syn::ReturnType::Type(_, ref mut ty) = method.sig.output {
                            if let syn::Type::Path(type_path) = &**ty {
                                if type_path
                                    .path
                                    .segments
                                    .last()
                                    .map(|seg| seg.ident == "Iter" || seg.ident == "IterMut")
                                    .unwrap_or(false)
                                {
                                    *ty = Box::new(syn::Type::Path(syn::TypePath {
                                        qself: None,
                                        path: syn::Path {
                                            leading_colon: type_path.path.leading_colon,
                                            segments: type_path
                                                .path
                                                .segments
                                                .iter()
                                                .map(|seg| {
                                                    if seg.ident == "Iter"
                                                        || seg.ident == "IterMut"
                                                    {
                                                        syn::PathSegment {
                                                            ident: seg.ident.clone(),
                                                            arguments:
                                                                syn::PathArguments::None,
                                                        }
                                                    } else {
                                                        seg.clone()
                                                    }
                                                })
                                                .collect(),
                                        },
                                    }));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions within impl blocks that return iterator types with a specified `Item` type. It modifies the return type by removing the generic `Item` type parameter from `std::slice::Iter` or `std::slice::IterMut`. This transformation stresses the compiler's handling of associated types and generics in iterator contexts, potentially exposing weaknesses in type inference and metadata resolution."
    }
}