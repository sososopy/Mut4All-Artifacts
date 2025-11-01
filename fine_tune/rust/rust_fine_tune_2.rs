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
pub struct Replace_Return_Type_2;
impl Mutator for Replace_Return_Type_2 {
    fn name(&self) -> &str {
        "Replace_Return_Type_2"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(i) = item {
                if i.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, ref mut ty) = i.sig.output {
                    *ty = Box::new(syn::Type::Infer(syn::TypeInfer {
                        underscore_token: Default::default(),
                    }));
                }
            }
            if let syn::Item::Impl(tag1) = item {
                for impl_item in &mut tag1.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if let syn::ReturnType::Type(_, ref mut ty) = method.sig.output {
                            *ty = Box::new(syn::Type::Infer(syn::TypeInfer {
                                underscore_token: Default::default(),
                            }));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets all non-main functions in the source file, including both free functions and methods within impl blocks. It replaces any explicit return type with the underscore `_` type, which requires the compiler to infer the return type. This transformation leverages Rust's type inference system to increase AST ambiguity and stress-test the compiler's ability to resolve types, particularly in more complex return scenarios involving traits, generics, and lifetimes."
    }
}
