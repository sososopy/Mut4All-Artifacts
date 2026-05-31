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

pub struct Recursive_Function_Return_Type_Mutation_127;

impl Mutator for Recursive_Function_Return_Type_Mutation_127 {
    fn name(&self) -> &str {
        "Recursive_Function_Return_Type_Mutation_127"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut recursive_functions = Vec::new();
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                let fn_name = &item_fn.sig.ident;
                let mut has_recursive_call = false;
                for stmt in &item_fn.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Call(ExprCall { func, .. }) = expr {
                            if let Expr::Path(ExprPath { path, .. }) = &**func {
                                if let Some(segment) = path.segments.last() {
                                    if segment.ident == *fn_name {
                                        has_recursive_call = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                if has_recursive_call {
                    if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                        if let Type::ImplTrait(_) = &**return_type {
                            recursive_functions.push(fn_name.clone());
                        }
                    }
                }
            }
        }
        if recursive_functions.is_empty() {
            return;
        }
        let mut rng = thread_rng();
        let target_fn_name = recursive_functions.choose(&mut rng).unwrap();
        let mut existing_traits = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                existing_traits.insert(item_trait.ident.to_string());
            }
        }
        let mut target_item_index = None;
        for (index, item) in file.items.iter().enumerate() {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == *target_fn_name {
                    target_item_index = Some(index);
                    break;
                }
            }
        }
        if let Some(index) = target_item_index {
            let mut target_item = file.items.remove(index);
            if let Item::Fn(item_fn) = &mut target_item {
                let mut concrete_type = None;
                let mut concrete_trait = None;
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let trait_name = trait_bound.path.segments.last().unwrap().ident.to_string();
                                if existing_traits.contains(&trait_name) {
                                    concrete_trait = Some(trait_name);
                                    break;
                                }
                            }
                        }
                    }
                }
                if concrete_trait.is_none() {
                    for item in &file.items {
                        if let Item::Struct(item_struct) = item {
                            concrete_type = Some(item_struct.ident.to_string());
                            break;
                        }
                    }
                }
                if concrete_trait.is_none() && concrete_type.is_none() {
                    concrete_type = Some("u32".to_string());
                }
                if let Some(trait_name) = concrete_trait {
                    item_fn.sig.output = ReturnType::Type(
                        Default::default(),
                        Box::new(Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(Ident::new(&trait_name, Span::call_site())),
                        })),
                    );
                } else if let Some(type_name) = concrete_type {
                    let new_trait_name = format!("NewTrait{}", rng.r#gen::<u32>());
                    file.items.insert(
                        index,
                        Item::Trait(parse_quote! {
                            trait #new_trait_name {}
                        }),
                    );
                    file.items.insert(
                        index + 1,
                        Item::Impl(parse_quote! {
                            impl #new_trait_name for #type_name {}
                        }),
                    );
                    item_fn.sig.output = ReturnType::Type(
                        Default::default(),
                        Box::new(Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(Ident::new(&new_trait_name, Span::call_site())),
                        })),
                    );
                }
            }
            file.items.insert(index, target_item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with recursive calls that return `impl Trait`. It replaces the return type with a concrete trait or type already present in the code, or introduces a new trait implemented by a suitable type. This transformation forces the compiler to resolve concrete return types, potentially exposing bugs in type resolution and trait implementation handling."
    }
}