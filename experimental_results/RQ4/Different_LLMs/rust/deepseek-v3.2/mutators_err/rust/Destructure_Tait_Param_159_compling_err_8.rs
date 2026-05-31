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

pub struct Destructure_Tait_Param_159;

impl Mutator for Destructure_Tait_Param_159 {
    fn name(&self) -> &str {
        "Destructure_Tait_Param_159"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut tait_definitions = Vec::new();
        let mut struct_definitions = Vec::new();
        
        for item in &file.items {
            if let Item::Type(item_type) = item {
                if let Type::ImplTrait(_) = &*item_type.ty {
                    tait_definitions.push((item_type.ident.clone(), item_type.ty.clone()));
                }
            }
            if let Item::Struct(item_struct) = item {
                struct_definitions.push((item_struct.ident.clone(), item_struct.clone()));
            }
        }
        
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for input in &item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            let param_type_name = type_path.path.segments.last().map(|seg| &seg.ident);
                            for (tait_name, tait_type) in &tait_definitions {
                                if param_type_name == Some(&tait_name) {
                                    if let Type::ImplTrait(type_impl_trait) = &**tait_type {
                                        for (struct_name, struct_def) in &struct_definitions {
                                            if struct_def.fields.iter().any(|field| {
                                                if let syn::Field { ty, .. } = field {
                                                    if let Type::Path(path) = &*ty {
                                                        path.path.segments.last().map(|seg| seg.ident.to_string()) == Some("String".to_string())
                                                    } else {
                                                        false
                                                    }
                                                } else {
                                                    false
                                                }
                                            }) {
                                                let pattern_fields = struct_def.fields.iter().map(|field| {
                                                    let field_name = field.ident.as_ref().unwrap();
                                                    field_name.clone()
                                                }).collect::<Punctuated<Ident, Comma>>();
                                                
                                                let destructure_stmt: Stmt = parse_quote! {
                                                    let #struct_name { #pattern_fields } = #pat_type.pat;
                                                };
                                                
                                                item_fn.block.stmts.insert(0, destructure_stmt);
                                            }
                                        }
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
        "The mutation operator identifies functions with parameters of TAIT (type alias impl trait) types that resolve to structs with nontrivial drop glue (e.g., containing String fields). It then inserts a destructuring pattern match at the beginning of the function body, binding the struct's fields. This transformation tests the compiler's ability to handle pattern matching on opaque types, drop order analysis, and type resolution for TAITs in destructuring contexts."
    }
}