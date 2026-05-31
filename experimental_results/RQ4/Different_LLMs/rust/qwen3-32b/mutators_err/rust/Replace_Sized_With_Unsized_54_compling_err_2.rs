use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Replace_Sized_With_Unsized_54;

impl Mutator for Replace_Sized_With_Unsized_54 {
    fn name(&self) -> &str {
        "Replace_Sized_With_Unsized_54"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(s) = item {
                for field in &mut s.fields {
                    field.ty = parse_quote! { dyn Fn() };
                }
            } else if let Item::Const(c) = item {
                c.ty = parse_quote! { dyn Fn() };
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces struct field types and const declarations with an unsized trait object type (dyn Fn()). This forces the compiler to handle unsized types in contexts requiring sizedness, such as struct layout computation or const-eval, potentially triggering layout assertion failures or ICEs during type checking and evaluation."
    }
}