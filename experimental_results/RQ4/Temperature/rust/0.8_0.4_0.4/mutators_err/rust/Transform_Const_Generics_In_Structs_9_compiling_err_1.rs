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

pub struct Transform_Const_Generics_In_Structs_9;

impl Mutator for Transform_Const_Generics_In_Structs_9 {
    fn name(&self) -> &str {
        "Transform_Const_Generics_In_Structs_9"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_const_generic = false;
                let mut const_generic_ident = None;

                // Check if the struct has const generics
                if let Some(generics) = &item_struct.generics.params.first() {
                    if let GenericParam::Const(const_param) = generics {
                        has_const_generic = true;
                        const_generic_ident = Some(const_param.ident.clone());
                    }
                }

                if has_const_generic {
                    // Remove const generic from the struct definition
                    item_struct.generics.params = item_struct
                        .generics
                        .params
                        .clone()
                        .into_iter()
                        .filter(|param| !matches!(param, GenericParam::Const(_)))
                        .collect();

                    // Replace occurrences of the const generic with a fixed value
                    if let Some(const_ident) = const_generic_ident {
                        for field in &mut item_struct.fields {
                            if let Type::Array(type_array) = &mut field.ty {
                                if let Expr::Path(expr_path) = &*type_array.len {
                                    if expr_path.path.is_ident(&const_ident) {
                                        type_array.len = Box::new(Expr::Lit(parse_quote!(1)));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}