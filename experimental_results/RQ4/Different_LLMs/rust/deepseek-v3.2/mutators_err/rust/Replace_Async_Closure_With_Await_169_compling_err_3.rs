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

pub struct Replace_Async_Closure_With_Await_169;

impl Mutator for Replace_Async_Closure_With_Await_169 {
    fn name(&self) -> &str {
        "Replace_Async_Closure_With_Await_169"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut async_fn_names = HashSet::new();
        let mut dummy_async_fn_added = false;
        let mut visitor = AsyncFnCollector {
            names: &mut async_fn_names,
        };
        visitor.visit_file(&file);
        if async_fn_names.is_empty() {
            let dummy_fn = parse_quote! {
                async fn dummy_async() {}
            };
            file.items.push(Item::Fn(dummy_fn));
            async_fn_names.insert(Ident::new("dummy_async", Span::call_site()));
            dummy_async_fn_added = true;
        }
        let mut mutator = ClosureAwaitMutator {
            async_fn_names: &async_fn_names,
        };
        mutator.visit_file_mut(file);
        if dummy_async_fn_added {
            let dummy_fn = parse_quote! {
                async fn dummy_async() {}
            };
            file.items.push(Item::Fn(dummy_fn));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets closures within method chains that call async functions without .await. It modifies such closures to include .await on the async call and ensures the entire method chain ends with .await. If the seed lacks async functions, a dummy async function is introduced. This transformation stresses the compiler's async/await handling, closure type inference, and method chain resolution, potentially causing type mismatches or lifetime issues."
    }
}

struct AsyncFnCollector<'a> {
    names: &'a mut HashSet<Ident>,
}

impl<'a> Visit<'a> for AsyncFnCollector<'a> {
    fn visit_item_fn(&mut self, i: &'a ItemFn) {
        if i.sig.asyncness.is_some() {
            self.names.insert(i.sig.ident.clone());
        }
        syn::visit::visit_item_fn(self, i);
    }
}

struct ClosureAwaitMutator<'a> {
    async_fn_names: &'a HashSet<Ident>,
}

impl<'a> VisitMut for ClosureAwaitMutator<'a> {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::MethodCall(method_call) = expr {
            let mut chain_modified = false;
            let mut chain_expr = &mut *method_call.receiver;
            while let Expr::MethodCall(inner_method) = chain_expr {
                chain_expr = &mut *inner_method.receiver;
            }
            if let Expr::Closure(closure) = chain_expr {
                if let Expr::Block(block) = &*closure.body {
                    if block.asyncness.is_some() {
                        if let Some(stmt) = block.stmts.first() {
                            if let Stmt::Expr(expr_stmt, _) = stmt {
                                if let Expr::Call(call) = expr_stmt {
                                    if let Expr::Path(path) = &*call.func {
                                        if let Some(segment) = path.path.segments.last() {
                                            if self.async_fn_names.contains(&segment.ident) {
                                                let new_call = parse_quote! {
                                                    #call.await
                                                };
                                                closure.body = Box::new(Expr::Block(syn::ExprBlock {
                                                    attrs: vec![],
                                                    label: None,
                                                    block: syn::Block {
                                                        brace_token: syn::token::Brace::default(),
                                                        stmts: vec![Stmt::Expr(new_call, None)],
                                                    },
                                                }));
                                                chain_modified = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if chain_modified {
                let mut last_expr = expr;
                while let Expr::MethodCall(inner) = last_expr {
                    if inner.receiver.as_ref() == chain_expr {
                        break;
                    }
                    last_expr = &mut *inner.receiver;
                }
                if let Expr::MethodCall(last_method) = last_expr {
                    if last_method.receiver.as_ref() == chain_expr {
                        let new_method = parse_quote! {
                            #last_method.await
                        };
                        *expr = new_method;
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}