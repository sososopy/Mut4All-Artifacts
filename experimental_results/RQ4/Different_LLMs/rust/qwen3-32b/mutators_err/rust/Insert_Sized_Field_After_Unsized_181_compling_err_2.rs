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

pub struct Insert_Sized_Field_After_Unsized_181;

impl Mutator for Insert_Sized_Field_After_Unsized_181 {
    fn name(&self) -> &str {
        "Insert_Sized_Field_After_Unsized_181"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                let mut found_unsized = false;
                if let syn::Fields::Named(ref mut named) = s.fields {
                    for (i, field) in named.named.iter_mut().enumerate() {
                        if !found_unsized && self.is_unsized_type(&field.ty) {
                            found_unsized = true;
                            let new_field = parse_quote! { field: () };
                            named.named.insert(i + 1, new_field);
                            break;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a sized field (unit type `()`) after the first unsized field in a struct definition. This exploits the compiler's struct layout rules, which require all fields after an unsized field to also be unsized. The transformation forces an invalid layout computation, likely exposing bugs in the layout calculation logic."
    }
}

impl Insert_Sized_Field_After_Unsized_181 {
    fn is_unsized_type(&self, ty: &Type) -> bool {
        match ty {
            Type::Slice(_) => true,
            Type::TraitObject(_) => true,
            Type::Str => true,
            _ => false,
        }
    }
}