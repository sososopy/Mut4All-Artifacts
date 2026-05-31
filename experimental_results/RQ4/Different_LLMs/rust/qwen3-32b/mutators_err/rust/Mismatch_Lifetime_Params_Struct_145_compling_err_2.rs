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

pub struct Mismatch_Lifetime_Params_Struct_145;

impl Mutator for Mismatch_Lifetime_Params_Struct_145 {
    fn name(&self) -> &str {
        "Mismatch_Lifetime_Params_Struct_145"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                // Check if the struct has any lifetime parameters
                let has_lifetimes = item_struct.generics.params.iter().any(|param| matches!(param, GenericParam::Lifetime(_)));
                if has_lifetimes {
                    if let syn::Data::Struct(data_struct) = &mut item_struct.data {
                        let fields = match &mut data_struct.fields {
                            syn::Fields::Named(named) => &mut named.named,
                            syn::Fields::Unnamed(unnamed) => &mut unnamed.unnamed,
                            syn::Fields::Unit => continue,
                        };
                        for field in fields {
                            if let Type::Reference(type_ref) = &mut field.ty {
                                // Replace the lifetime with an undeclared one
                                let new_lt = Lifetime::new("'b", Span::call_site());
                                type_ref.lifetime = Some(new_lt);
                                break; // Only modify the first matching field
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a mismatch between declared and used lifetime parameters in struct definitions. It modifies a struct with declared lifetimes by replacing a reference field's lifetime with an undeclared one (e.g., 'b instead of 'a). This creates an undeclared lifetime reference, forcing the compiler to process invalid variance constraints and potentially triggering ICE during type analysis."
    }
}