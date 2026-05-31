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

pub struct Replace_Async_Trait_Method_With_Non_Async_303;

impl Mutator for Replace_Async_Trait_Method_With_Non_Async_303 {
    fn name(&self) -> &str {
        "Replace_Async_Trait_Method_With_Non_Async_303"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut async_trait_methods = Vec::new();
        let mut await_sites = Vec::new();
        
        struct AsyncTraitMethodFinder {
            methods: Vec<(Ident, Type)>,
        }
        impl Visit<'_> for AsyncTraitMethodFinder {
            fn visit_item_trait(&mut self, i: &syn::ItemTrait) {
                for item in &i.items {
                    if let syn::TraitItem::Fn(method) = item {
                        if method.sig.asyncness.is_some() {
                            let return_type = match &method.sig.output {
                                ReturnType::Default => parse_quote!(()),
                                ReturnType::Type(_, ty) => *ty.clone(),
                            };
                            self.methods.push((method.sig.ident.clone(), return_type));
                        }
                    }
                }
                syn::visit::visit_item_trait(self, i);
            }
        }
        
        struct AwaitSiteFinder<'a> {
            method_names: &'a Vec<(Ident, Type)>,
            sites: Vec<(syn::Expr, Ident)>,
        }
        impl<'a> Visit<'_> for AwaitSiteFinder<'a> {
            fn visit_expr_await(&mut self, i: &syn::ExprAwait) {
                if let syn::Expr::MethodCall(method_call) = &*i.base {
                    for (ident, _) in self.method_names {
                        if method_call.method == *ident {
                            self.sites.push((*i.base.clone(), ident.clone()));
                            break;
                        }
                    }
                }
                syn::visit::visit_expr_await(self, i);
            }
        }
        
        let mut finder = AsyncTraitMethodFinder { methods: Vec::new() };
        finder.visit_file(file);
        async_trait_methods = finder.methods;
        
        let mut await_finder = AwaitSiteFinder {
            method_names: &async_trait_methods,
            sites: Vec::new(),
        };
        await_finder.visit_file(file);
        await_sites = await_finder.sites;
        
        for item in &mut file.items {
            if let syn::Item::Trait(trait_def) = item {
                for trait_item in &mut trait_def.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        if method.sig.asyncness.is_some() {
                            method.sig.asyncness = None;
                            
                            let return_type = match &method.sig.output {
                                ReturnType::Default => parse_quote!(()),
                                ReturnType::Type(_, ty) => *ty.clone(),
                            };
                            
                            let future_return: Type = parse_quote! {
                                impl std::future::Future<Output = #return_type>
                            };
                            method.sig.output = ReturnType::Type(Default::default(), Box::new(future_return));
                            
                            if let Some(block) = &mut method.default {
                                let ready_expr: Expr = parse_quote! {
                                    std::future::ready(())
                                };
                                let mut new_stmts = Vec::new();
                                new_stmts.push(Stmt::Expr(ready_expr, None));
                                block.stmts = new_stmts;
                            }
                        }
                    }
                }
            }
        }
        
        struct AwaitRemover {
            sites: Vec<(syn::Expr, Ident)>,
        }
        impl VisitMut for AwaitRemover {
            fn visit_expr_await_mut(&mut self, i: &mut syn::ExprAwait) {
                if let syn::Expr::MethodCall(method_call) = &*i.base {
                    for (expr, ident) in &self.sites {
                        if let syn::Expr::MethodCall(mc) = expr {
                            if mc.method == *ident && method_call.method == *ident {
                                *i = syn::ExprAwait {
                                    attrs: i.attrs.clone(),
                                    base: Box::new(expr.clone()),
                                    dot_token: i.dot_token,
                                    await_token: i.await_token,
                                };
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_await_mut(self, i);
            }
        }
        
        let mut remover = AwaitRemover { sites: await_sites };
        remover.visit_file_mut(file);
        
        for item in &mut file.items {
            if let syn::Item::Impl(impl_def) = item {
                for impl_item in &mut impl_def.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.asyncness.is_some() {
                            for (ident, return_type) in &async_trait_methods {
                                if method.sig.ident == *ident {
                                    method.sig.asyncness = None;
                                    
                                    let future_return: Type = parse_quote! {
                                        impl std::future::Future<Output = #return_type>
                                    };
                                    method.sig.output = ReturnType::Type(Default::default(), Box::new(future_return));
                                    
                                    let ready_expr: Expr = parse_quote! {
                                        std::future::ready(())
                                    };
                                    let mut new_stmts = Vec::new();
                                    new_stmts.push(Stmt::Expr(ready_expr, None));
                                    method.block.stmts = new_stmts;
                                    
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async trait methods and their call sites. It transforms async trait methods into synchronous methods that return impl Future, removing the async keyword and adjusting return types. At call sites, it removes .await suffixes. This transformation tests the compiler's handling of async/await desugaring, trait method compatibility, and future type inference, potentially revealing bugs in async trait implementation and await expression resolution."
    }
}