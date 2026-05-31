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

pub struct Nested_Type_Complexity_72;

impl Mutator for Nested_Type_Complexity_72 {
    fn name(&self) -> &str {
        "Nested_Type_Complexity_72"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let mut rng = thread_rng();
                let depth = rng.gen_range(3..6); // Randomly choose depth for nested types
                let mut current_type: Type = parse_quote!(i32);
                
                for _ in 0..depth {
                    current_type = parse_quote!(Vec<#current_type>);
                }
                
                for field in &mut item_struct.fields {
                    field.ty = current_type.clone();
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator increases the complexity of struct field types by nesting them within multiple layers of `Vec`. This transformation aims to stress the compiler's type checking and inference mechanisms, potentially exposing bugs related to deeply nested generic types, memory management issues, or ICEs during type resolution."
    }
}