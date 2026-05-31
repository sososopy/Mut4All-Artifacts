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

pub struct Modify_Match_Arms_Mutate_Variable_Call_Method_281;

impl Mutator for Modify_Match_Arms_Mutate_Variable_Call_Method_281 {
    fn name(&self) -> &str {
        "Modify_Match_Arms_Mutate_Variable_Call_Method_281"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for stmt in &mut func.block.stmts {
                    if let Stmt::Expr(expr, _) = stmt {
                        if let Expr::Match(match_expr) = &mut *expr {
                            if let Expr::Path(path_expr) = &mut *match_expr.expr {
                                let var_name = path_expr.path.segments[0].ident.clone();
                                let var_span = path_expr.path.segments[0].ident.span();
                                let mut made_mut = false;
                                for input in &mut func.sig.inputs {
                                    if let FnArg::Typed(pat_type) = input {
                                        if let Pat::Ident(ident_pat) = pat_type.pat.as_mut() {
                                            if ident_pat.ident == var_name {
                                                if ident_pat.mutability.is_none() {
                                                    ident_pat.mutability = Some(syn::token::Mut {
                                                        span: Span::call_site(),
                                                    });
                                                }
                                                made_mut = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if !made_mut {
                                    for s in &mut func.block.stmts {
                                        if let Stmt::Local(local) = s {
                                            if let Pat::Ident(ident_pat) = Box::new(local.pat).as_mut() {
                                                if ident_pat.ident == var_name {
                                                    if ident_pat.mutability.is_none() {
                                                        ident_pat.mutability = Some(syn::token::Mut {
                                                            span: Span::call_site(),
                                                        });
                                                    }
                                                    made_mut = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                for arm in &mut match_expr.arms {
                                    if let Pat::TupleStruct(tuple_struct) = Box::new(arm.pat).as_mut() {
                                        let first_segment = &tuple_struct.path.segments[0];
                                        if first_segment.ident == "None" {
                                            let assign_expr = parse_quote! {
                                                #var_name = Some(0)
                                            };
                                            arm.body = Box::new(assign_expr);
                                        } else if first_segment.ident == "Some" {
                                            if let Pat::Tuple(tuple_pat) = Box::new(tuple_struct.elems[0]).as_mut() {
                                                if tuple_pat.elems.len() == 1 {
                                                    if let Pat::Ident(inner_ident) = Box::new(tuple_pat.elems[0]).as_mut() {
                                                        let method_call_expr = parse_quote! {
                                                            #inner_ident.method_call()
                                                        };
                                                        arm.body = Box::new(method_call_expr);
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
        "The mutation operator modifies match expressions on Option<T> variables within function bodies. It ensures the variable is mutable, then rewrites the None arm to assign the variable to Some(...) and the Some arm to invoke a method on the bound inner value. This creates conflicting expectations between the arms, potentially exposing compiler logic flaws during type-checking and unwrapping operations."
    }
}