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

pub struct Modify_Closure_Argument_Type_Mismatch_41;

impl Mutator for Modify_Closure_Argument_Type_Mismatch_41 {
    fn name(&self) -> &str {
        "Modify_Closure_Argument_Type_Mismatch_41"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut target_closure = None;
                for (i, input) in item_fn.sig.inputs.iter().enumerate() {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::TraitObject(type_trait_object) = &*pat_type.ty {
                            let mut has_fn_mut = false;
                            for bound in &type_trait_object.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound
                                        .path
                                        .segments
                                        .last()
                                        .map_or(false, |seg| seg.ident == "FnMut")
                                    {
                                        has_fn_mut = true;
                                        break;
                                    }
                                }
                            }
                            if has_fn_mut {
                                target_closure = Some(i);
                                break;
                            }
                        }
                    }
                }
                if let Some(target_closure) = target_closure {
                    let mut closure_index = 0;
                    let mut found = false;
                    for stmt in &mut item_fn.block.stmts {
                        if let Stmt::Expr(Expr::Call(expr_call), _) = stmt {
                            if let Expr::Path(expr_path) = &*expr_call.func {
                                if expr_path.path.is_ident(&item_fn.sig.ident) {
                                    if closure_index == target_closure {
                                        if let Some(arg) = expr_call.args.first_mut() {
                                            if let Expr::Closure(expr_closure) = arg {
                                                let mut new_stmts = expr_closure
                                                    .body
                                                    .clone()
                                                    .into_iter()
                                                    .collect::<Vec<_>>();
                                                new_stmts.push(Stmt::Expr(
                                                    Expr::Verbatim(quote! { "error" }),
                                                    None,
                                                ));
                                                expr_closure.body = new_stmts.into_iter().collect();
                                                found = true;
                                                break;
                                            }
                                        }
                                    }
                                    closure_index += 1;
                                }
                            }
                        }
                    }
                    if found {
                        let mut new_stmts = item_fn
                            .block
                            .stmts
                            .clone()
                            .into_iter()
                            .collect::<Vec<_>>();
                        new_stmts.push(Stmt::Expr(
                            Expr::Verbatim(quote! { "error" }),
                            None,
                        ));
                        item_fn.block = new_stmts.into_iter().collect();
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "Identify a function that accepts a `FnMut` trait object as a parameter. Within the function, locate the index of this parameter. Then, in the main function, find the corresponding closure argument passed to this parameter and modify its body to include a type-mismatched expression, such as a string literal, to induce a type error."
    }
}