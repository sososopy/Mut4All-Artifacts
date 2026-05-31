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

pub struct Modify_Lifetime_With_Static_396;

impl Mutator for Modify_Lifetime_With_Static_396 {
    fn name(&self) -> &str {
        "Modify_Lifetime_With_Static_396"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(type_ref) = &mut *pat_type.ty {
                            type_ref.lifetime = Some(Lifetime::new("'static", Span::call_site()));
                        }
                    }
                }
            }
            if let syn::Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Reference(type_ref) = &mut field.ty {
                        type_ref.lifetime = Some(Lifetime::new("'static", Span::call_site()));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies the lifetimes of references in function parameters and struct fields to 'static. This transformation enforces the longest possible lifetime, potentially leading to conflicts with existing lifetime constraints and uncovering bugs in lifetime resolution or borrow checking."
    }
}