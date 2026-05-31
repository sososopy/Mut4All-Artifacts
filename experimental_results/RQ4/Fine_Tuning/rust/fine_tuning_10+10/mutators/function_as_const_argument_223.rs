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

pub struct Function_As_Const_Argument_223;

impl Mutator for Function_As_Const_Argument_223 {
    fn name(&self) -> &str {
        "Function_As_Const_Argument_223"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut function_name = None;
        let feature_flag = parse_quote! {
            #![feature(min_generic_const_args)]
        };

            file.attrs.insert(0, feature_flag);

        for item in &file.items {
            if let Item::Fn(ItemFn { sig, .. }) = item {
                function_name = Some(sig.ident.clone());
                break;
            }
        }

        if let Some(func_ident) = function_name {
            for item in &mut file.items {
                if let Item::Struct(ItemStruct { generics, .. }) = item {
                    if generics.where_clause.is_none() {
                        generics.where_clause = Some(syn::WhereClause {
                            where_token: Default::default(),
                            predicates: Punctuated::new(),
                        });
                    }

                    if let Some(where_clause) = &mut generics.where_clause {
                        let const_expr: Expr = parse_quote!(#func_ident());
                        let predicate: syn::WherePredicate = parse_quote! {
                            [(); #const_expr]:
                        };
                        where_clause.predicates.push(predicate);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets functions and const arguments in generic parameters. It identifies a function declaration and modifies a generic parameter constraint by introducing a const argument using the identified function. This aims to explore potential compiler bugs related to const expression evaluation involving function calls."
    }
}