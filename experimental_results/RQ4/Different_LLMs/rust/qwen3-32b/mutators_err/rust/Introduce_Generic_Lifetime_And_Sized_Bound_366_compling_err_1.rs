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

pub struct Introduce_Generic_Lifetime_And_Sized_Bound_366;

impl Mutator for Introduce_Generic_Lifetime_And_Sized_Bound_366 {
    fn name(&self) -> &str {
        "Introduce_Generic_Lifetime_And_Sized_Bound_366"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut modified_const_fns = HashSet::new();

        // First pass: add lifetime and where clause to const fns returning usize
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    if let ReturnType::Type(_, ty) = &mut func.sig.output {
                        if let Type::Path(type_path) = &**ty {
                            if type_path.path.is_ident("usize") {
                                // Add lifetime parameter
                                let lifetime = parse_quote!('a);
                                func.sig.generics.params.push(GenericParam::Lifetime(lifetime));
                                // Add where clause
                                let where_clause = func.sig.generics.where_clause.get_or_insert_with(|| {
                                    WhereClause {
                                        where_token: token::Where::default(),
                                        predicates: Punctuated::new(),
                                    }
                                });
                                where_clause.predicates.push(parse_quote! { &'a (): Sized });
                                modified_const_fns.insert(func.sig.ident.to_string());
                            }
                        }
                    }
                }
            }
        }

        // Second pass: find functions with array size expressions calling modified const fns
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some((_, ty)) = &mut local.ty {
                            if let Type::Array(array_type) = &mut **ty {
                                if let Expr::Call(call_expr) = &mut array_type.len {
                                    if let ExprPath(path_expr) = &call_expr.func {
                                        let func_name = path_expr.path.segments.first().unwrap().ident.to_string();
                                        if modified_const_fns.contains(&func_name) {
                                            // Add lifetime parameter to function's generics
                                            let lifetime = parse_quote!('a);
                                            func.sig.generics.params.push(GenericParam::Lifetime(lifetime));
                                            // Modify the call to include the lifetime
                                            let ident = path_expr.path.segments.first().unwrap().ident.clone();
                                            let new_path = parse_quote!(#ident::<'a>);
                                            call_expr.func = Box::new(Expr::Path(ExprPath {
                                                attrs: Vec::new(),
                                                qself: None,
                                                path: new_path,
                                            }));
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
        ""
    }
}