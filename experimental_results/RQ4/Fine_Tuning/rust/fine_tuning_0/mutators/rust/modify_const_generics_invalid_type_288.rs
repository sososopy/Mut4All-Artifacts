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

pub struct Modify_Const_Generics_Invalid_Type_288;

impl Mutator for Modify_Const_Generics_Invalid_Type_288 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_Invalid_Type_288"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for param in &mut func.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Path(type_path) = &mut const_param.ty {
                            if type_path.path.segments.len() == 1 {
                                let segment = &mut type_path.path.segments[0];
                                if segment.ident == "i32" {
                                    segment.ident = Ident::new("String", Span::call_site());
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets function definitions using const generics and modifies the type of the const generic parameter to an invalid or incompatible type. By changing the type from a primitive like `i32` to a non-primitive like `String`, it aims to explore scenarios where the Rust compiler's type checking of const generics may lead to internal errors, especially when dealing with incomplete features like `adt_const_params`."
    }
}