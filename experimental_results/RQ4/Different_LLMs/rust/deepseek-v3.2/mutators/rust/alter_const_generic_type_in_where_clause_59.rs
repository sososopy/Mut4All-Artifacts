use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemImpl, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Alter_Const_Generic_Type_In_Where_Clause_59;

impl Mutator for Alter_Const_Generic_Type_In_Where_Clause_59 {
    fn name(&self) -> &str {
        "Alter_Const_Generic_Type_In_Where_Clause_59"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut structs_with_const_generics = Vec::new();
        let mut impls_for_structs = Vec::new();

        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut const_params = Vec::new();
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_params.push((const_param.ident.clone(), const_param.ty.clone()));
                    }
                }
                if !const_params.is_empty() {
                    structs_with_const_generics.push((item_struct.clone(), const_params));
                }
            }
            if let Item::Impl(item_impl) = item {
                if let Type::Path(type_path) = &*item_impl.self_ty {
                    let struct_name = &type_path.path.segments.last().unwrap().ident;
                    let mut const_params = Vec::new();
                    for param in &item_impl.generics.params {
                        if let GenericParam::Const(const_param) = param {
                            const_params.push((const_param.ident.clone(), const_param.ty.clone()));
                        }
                    }
                    impls_for_structs.push((item_impl.clone(), struct_name.clone(), const_params));
                }
            }
        }

        for (struct_item, struct_const_params) in structs_with_const_generics {
            for (impl_item, struct_name, impl_const_params) in &mut impls_for_structs {
                if struct_item.ident == *struct_name {
                    let mut rng = thread_rng();
                    let integer_types = ["i8", "i16", "i32", "i64", "i128", "u8", "u16", "u32", "u64", "u128", "usize", "isize"];
                    
                    for (struct_param_name, struct_param_type) in &struct_const_params {
                        for (impl_param_name, impl_param_type) in &mut *impl_const_params {
                            if struct_param_name == impl_param_name {
                                if let Type::Path(type_path) = &struct_param_type {
                                    if let Some(segment) = type_path.path.segments.last() {
                                        let current_type = segment.ident.to_string();
                                        if integer_types.contains(&current_type.as_str()) {
                                            let impl_param_type_str = quote::quote!(#impl_param_type).to_string();
                                            let mut available_types = integer_types.iter().filter(|&t| t != &current_type && t != &impl_param_type_str).collect::<Vec<_>>();
                                            if !available_types.is_empty() {
                                                let new_type = available_types.choose(&mut rng).unwrap();
                                                let new_type_ident = Ident::new(new_type, Span::call_site());
                                                
                                                for item in &mut file.items {
                                                    if let Item::Struct(item_struct) = item {
                                                        if item_struct.ident == struct_item.ident {
                                                            for param in &mut item_struct.generics.params {
                                                                if let GenericParam::Const(const_param) = param {
                                                                    if const_param.ident == *struct_param_name {
                                                                        const_param.ty = parse_quote!(#new_type_ident);
                                    }
                                }
                            }
                        }
                    }
                }

                                                for item in &mut file.items {
                                                    if let Item::Impl(item_impl) = item {
                                                        if let Type::Path(type_path) = &*item_impl.self_ty {
                                                            if type_path.path.segments.last().unwrap().ident == struct_item.ident {
                                                                let where_clause = &mut item_impl.generics.where_clause;
                                                                if where_clause.is_none() {
                                                                item_impl.generics.where_clause = Some(WhereClause {
                                                                where_token: Default::default(),
                                                                predicates: Punctuated::new(),
                                                                });
                                                                }
                                                                if let Some(where_clause) = &mut item_impl.generics.where_clause {
                                                                let expr: Expr = parse_quote! { #impl_param_name as usize + 1 };
                                                                let predicate: WherePredicate = parse_quote! { [(); #expr]: };
                                                                where_clause.predicates.push(predicate);
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
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs with const generic parameters and their corresponding impl blocks. It changes the type of a const generic parameter in the struct definition to a different built-in integer type while keeping the impl parameter type unchanged. It also ensures the impl has a where-clause containing an arithmetic expression using the const parameter, potentially with mismatched types. This creates type conflicts between struct and impl const generics and introduces complex const evaluation scenarios in where-clauses, aiming to trigger compiler ICEs related to const generic type mismatches and arithmetic expression evaluation."
    }
}