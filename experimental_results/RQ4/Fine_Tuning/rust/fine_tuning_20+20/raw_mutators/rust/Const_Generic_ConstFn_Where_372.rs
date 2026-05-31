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

pub struct Const_Generic_ConstFn_Where_372;

impl Mutator for Const_Generic_ConstFn_Where_372 {
    fn name(&self) -> &str {
        "Const_Generic_ConstFn_Where_372"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(where_clause) = &mut item_fn.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            for bound in &predicate_type.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let Some(PathSegment {
                                        ident,
                                        arguments: PathArguments::AngleBracketed(args),
                                    }) = trait_bound.path.segments.last()
                                    {
                                        if ident == "for" {
                                            for arg in &args.args {
                                                if let GenericArgument::Const(expr) = arg {
                                                    if let Expr::Block(expr_block) = expr {
                                                        if let Some((_, expr)) =
                                                            expr_block.block.stmts.last()
                                                        {
                                                            if let Stmt::Expr(Expr::Call(
                                                                expr_call,
                                                            )) = expr
                                                            {
                                                                if let Expr::Path(expr_path) =
                                                                    &*expr_call.func
                                                                {
                                                                    if let Some(PathSegment {
                                                                        ident,
                                                                        ..
                                                                    }) = expr_path
                                                                        .path
                                                                        .segments
                                                                        .last()
                                                                    {
                                                                        let const_fn_name =
                                                                            ident.to_string();
                                                                        let mut const_fn_visitor =
                                                                            ConstFnVisitor {
                                                                                const_fn_name,
                                                                                found_const_fn:
                                                                                    None,
                                                                            };
                                                                        const_fn_visitor.visit_file_mut(file);
                                                                        if let Some(
                                                                            found_const_fn,
                                                                        ) = const_fn_visitor
                                                                            .found_const_fn
                                                                        {
                                                                            let mut const_fn_mutator = ConstFnMutator {
                                                                                const_fn_name,
                                                                            };
                                                                            const_fn_mutator.visit_item_const_mut(found_const_fn);
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
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with a `where` clause that uses a generic constant expression, specifically looking for const functions within such expressions. It modifies the body of the const function to return a different constant value type, such as changing from `usize` to `bool`, and adjusts the surrounding context accordingly. This transformation challenges the compiler's handling of const evaluation and type inference within generic constraints."
    }
}

struct ConstFnVisitor {
    const_fn_name: String,
    found_const_fn: Option<*mut ItemConst>,
}

impl VisitMut for ConstFnVisitor {
    fn visit_item_const_mut(&mut self, i: &mut ItemConst) {
        if i.ident == self.const_fn_name {
            self.found_const_fn = Some(i as *mut ItemConst);
        }
    }
}

struct ConstFnMutator {
    const_fn_name: String,
}

impl VisitMut for ConstFnMutator {
    fn visit_item_const_mut(&mut self, i: &mut ItemConst) {
        if i.ident == self.const_fn_name {
            i.expr = Box::new(Expr::Lit(ExprLit {
                attrs: Vec::new(),
                lit: Lit::Bool(LitBool {
                    value: true,
                    span: Span::call_site(),
                }),
            }));
        }
    }
}