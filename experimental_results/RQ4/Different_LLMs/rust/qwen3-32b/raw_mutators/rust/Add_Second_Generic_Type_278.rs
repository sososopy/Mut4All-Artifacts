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

pub struct Add_Second_Generic_Type_278;

impl Mutator for Add_Second_Generic_Type_278 {
    fn name(&self) -> &str {
        "Add_Second_Generic_Type_278"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let generics = &mut func.sig.generics;
                if generics.params.len() == 0 {
                    continue;
                }

                // Find the first type parameter
                let first_type = generics.params.iter().find(|param| {
                    if let GenericParam::Type(_) = param {
                        true
                    } else {
                        false
                    }
                });

                if let Some(first_type) = first_type {
                    // Clone the first type parameter's bounds and create a new one with name U
                    let first_type_param = if let GenericParam::Type(ty) = first_type {
                        ty
                    } else {
                        continue;
                    };

                    let new_type_name = Ident::new("U", Span::call_site());
                    let mut new_bounds = first_type_param.bounds.clone();
                    new_bounds.push(parse_quote!(PartialEq<T>));

                    let new_type = GenericParam::Type(syn::TypeParam {
                        ident: new_type_name.clone(),
                        colon_token: Some(token::Colon::default()),
                        bounds: new_bounds,
                        eq_token: None,
                        default: None,
                        attrs: vec![],
                    });

                    // Add the new type parameter to the generics
                    generics.params.push(new_type);

                    // Add where clause if needed
                    let mut has_where_clause = false;
                    if let Some(where_clause) = &mut generics.where_clause {
                        where_clause.predicates.push(parse_quote!(U: PartialEq<T>));
                        has_where_clause = true;
                    }

                    if !has_where_clause {
                        let mut new_where = parse_quote!(where U: PartialEq<T>);
                        generics.where_clause = Some(new_where);
                    }

                    // Insert comparison statements into the function body
                    let new_statements = parse_quote! {
                        let a: T = unimplemented!();
                        let b: U = unimplemented!();
                        a == b;
                    };

                    let mut block = func.block.clone();
                    block.stmts.splice(0..0, new_statements.stmts);
                    func.block = block;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a second generic type parameter with the same constraints as an existing parameter and uses both in a type comparison context. This forces the compiler to track type parameters with potentially identical constraints but distinct names, exposing bugs in index/name resolution during type comparison operations."
    }
}