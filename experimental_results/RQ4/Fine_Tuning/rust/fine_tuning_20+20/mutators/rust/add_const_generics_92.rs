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

pub struct Add_Const_Generics_92;

impl Mutator for Add_Const_Generics_92 {
    fn name(&self) -> &str {
        "Add_Const_Generics_92"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.generics.params.len() > 0 {
                    let mut new_generics = item_struct.generics.clone();
                    new_generics.params.push(parse_quote!(const N1: usize));
                    new_generics.params.push(parse_quote!(const N2: usize));
                    item_struct.generics = new_generics;
                    item_struct.generics.where_clause = Some(parse_quote!(where [(); N1 + N2]: Sized));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets generic struct declarations that use const generics. It modifies the struct to introduce an additional const generic parameter and updates the where clause to use a combination of the original and new constants. This transformation stresses the const evaluation and generic resolution systems by altering the struct's signature and its constraints, potentially uncovering bugs in how the compiler handles complex const generic scenarios."
    }
}