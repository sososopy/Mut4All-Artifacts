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

pub struct Swap_Async_Trait_Recursion_Order_305;

impl Mutator for Swap_Async_Trait_Recursion_Order_305 {
    fn name(&self) -> &str {
        "Swap_Async_Trait_Recursion_Order_305"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut visitor = TraitRecursionVisitor {
            rng: &mut rng,
            mutated: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets indirect recursion through async trait method calls in trait implementations with associated types. It identifies patterns where TraitA defines an async method returning Self::Assoc where Assoc: TraitB, and TraitB is implemented for types implementing TraitA, with TraitB's async method calling TraitA's method on self and then TraitB's method on the result. The operator mutates by either swapping the order of the two awaited method calls or duplicating one of the calls to create a longer chain, altering the recursion pattern while preserving syntactic validity."
    }
}

struct TraitRecursionVisitor<'a, R: Rng> {
    rng: &'a mut R,
    mutated: bool,
}

impl<'a, R: Rng> VisitMut for TraitRecursionVisitor<'a, R> {
    fn visit_item_impl_mut(&mut self, i: &mut syn::ItemImpl) {
        if self.mutated {
            return;
        }

        let trait_ = match &i.trait_ {
            Some((_, path, _)) => path,
            None => return,
        };

        let trait_name = trait_.segments.last().map(|seg| &seg.ident).unwrap();

        for impl_item in &mut i.items {
            if let syn::ImplItem::Fn(method) = impl_item {
                if method.sig.asyncness.is_none() {
                    continue;
                }

                let mut stmt_visitor = MethodCallVisitor {
                    trait_name: trait_name.clone(),
                    rng: self.rng,
                    found_pattern: None,
                };
                stmt_visitor.visit_block_mut(&mut method.block);

                if let Some((first_call, second_call)) = stmt_visitor.found_pattern {
                    if self.rng.gen_bool(0.5) {
                        *first_call = *second_call.clone();
                        *second_call = *first_call.clone();
                        self.mutated = true;
                    } else {
                        let duplicate = *second_call.clone();
                        let new_call: syn::Expr = parse_quote! {
                            #duplicate.await
                        };
                        let new_stmt = Stmt::Expr(new_call, None);
                        method.block.stmts.push(new_stmt);
                        self.mutated = true;
                    }
                }
            }
        }

        syn::visit_mut::visit_item_impl_mut(self, i);
    }
}

struct MethodCallVisitor<'a, R: Rng> {
    trait_name: Ident,
    rng: &'a mut R,
    found_pattern: Option<(Box<syn::Expr>, Box<syn::Expr>)>,
}

impl<'a, R: Rng> VisitMut for MethodCallVisitor<'a, R> {
    fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
        if self.found_pattern.is_some() {
            return;
        }

        if let syn::Expr::Await(await_expr) = expr {
            if let syn::Expr::MethodCall(method_call) = &*await_expr.base {
                let method_name = &method_call.method;
                let receiver = &method_call.receiver;

                if let syn::Expr::Await(inner_await) = &**receiver {
                    if let syn::Expr::MethodCall(inner_method_call) = &*inner_await.base {
                        let inner_method_name = &inner_method_call.method;
                        
                        if method_name != inner_method_name {
                            self.found_pattern = Some((
                                Box::new(syn::Expr::MethodCall(inner_method_call.clone())),
                                Box::new(syn::Expr::MethodCall(method_call.clone())),
                            ));
                        }
                    }
                }
            }
        }

        syn::visit_mut::visit_expr_mut(self, expr);
    }
}