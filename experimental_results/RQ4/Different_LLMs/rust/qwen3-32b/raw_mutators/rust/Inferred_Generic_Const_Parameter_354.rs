use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprMethodCall, ExprPath, ExprStruct, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeArray, TypeImplTrait,
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

pub struct Inferred_Generic_Const_Parameter_354;

impl Mutator for Inferred_Generic_Const_Parameter_354 {
    fn name(&self) -> &str {
        "Inferred_Generic_Const_Parameter_354"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let func_type_params: HashSet<Ident> = func.sig.generics.params.iter()
                    .filter_map(|param| match param {
                        GenericParam::Type(type_param) => Some(type_param.ident.clone()),
                        _ => None,
                    })
                    .collect();
                
                let mut has_required_where_clause = false;
                if let Some(where_clause) = &func.sig.where_clause {
                    for predicate in &where_clause.predicates {
                        if let WherePredicate::Type(type_predicate) = predicate {
                            if let Type::Array(array_type) = &**type_predicate.bounded_ty {
                                if let Type::Path(len_path) = &*array_type.len {
                                    if len_path.path.segments.len() == 2 {
                                        let first_segment = &len_path.path.segments[0];
                                        let second_segment = &len_path.path.segments[1];
                                        if func_type_params.contains(&first_segment.ident) && second_segment.ident == "CONST" {
                                            has_required_where_clause = true;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                
                if has_required_where_clause {
                    for stmt in &mut func.block.stmts {
                        if let Stmt::Expr(expr) = stmt {
                            if let Expr::Struct(expr_struct) = &mut **expr {
                                if let PathArguments::AngleBracketed(args) = &mut expr_struct.path.args {
                                    let mut all_args_are_func_type_params = true;
                                    for arg in &args.args {
                                        if let GenericArgument::Type(ty) = arg {
                                            if let Type::Path(type_path) = &**ty {
                                                if type_path.path.segments.len() != 1 {
                                                    all_args_are_func_type_params = false;
                                                    break;
                                                }
                                                let segment = &type_path.path.segments[0];
                                                if !func_type_params.contains(&segment.ident) {
                                                    all_args_are_func_type_params = false;
                                                    break;
                                                }
                                            } else {
                                                all_args_are_func_type_params = false;
                                                break;
                                            }
                                        } else {
                                            all_args_are_func_type_params = false;
                                            break;
                                        }
                                    }
                                    if all_args_are_func_type_params {
                                        let mut new_path = expr_struct.path.clone();
                                        if let Some(last_segment) = new_path.segments.last_mut() {
                                            last_segment.ident = Ident::new("new", last_segment.ident.span());
                                            last_segment.arguments = PathArguments::None;
                                        }
                                        let new_call = parse_quote! {
                                            #new_path.new()
                                        };
                                        *expr = Box::new(Expr::MethodCall(new_call));
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