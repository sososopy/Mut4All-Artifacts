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

pub struct Modify_Const_Generic_Type_177;

impl Mutator for Modify_Const_Generic_Type_177 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Type_177"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for param in &mut func.sig.generics.params {
                    if let syn::GenericParam::Const(const_param) = param {
                        if const_param.ty == parse_quote!(usize) {
                            const_param.ty = parse_quote!(u32);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies functions with const generics of type `usize` and changes them to `u32`. This transformation tests the compiler's ability to handle changes in const generic parameter types and ensures type compatibility is maintained throughout the function's usage."
    }
}