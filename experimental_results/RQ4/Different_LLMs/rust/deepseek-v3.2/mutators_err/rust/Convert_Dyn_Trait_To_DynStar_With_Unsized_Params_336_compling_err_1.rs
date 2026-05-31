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

pub struct Convert_Dyn_Trait_To_DynStar_With_Unsized_Params_336;

impl Mutator for Convert_Dyn_Trait_To_DynStar_With_Unsized_Params_336 {
    fn name(&self) -> &str {
        "Convert_Dyn_Trait_To_DynStar_With_Unsized_Params_336"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_dyn_star_feature = false;
        let mut has_unsized_fn_params_feature = false;
        for attr in &file.attrs {
            if let Ok(meta) = attr.parse_meta() {
                if let syn::Meta::List(list) = meta {
                    if list.path.is_ident("feature") {
                        for nested in list.nested {
                            if let syn::NestedMeta::Meta(syn::Meta::Path(path)) = nested {
                                if path.is_ident("dyn_star") {
                                    has_dyn_star_feature = true;
                                }
                                if path.is_ident("unsized_fn_params") {
                                    has_unsized_fn_params_feature = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        if !has_dyn_star_feature || !has_unsized_fn_params_feature {
            let mut new_attrs = Vec::new();
            if !has_dyn_star_feature && !has_unsized_fn_params_feature {
                new_attrs.push(parse_quote!(#![feature(dyn_star, unsized_fn_params)]));
            } else if !has_dyn_star_feature {
                new_attrs.push(parse_quote!(#![feature(dyn_star)]));
            } else if !has_unsized_fn_params_feature {
                new_attrs.push(parse_quote!(#![feature(unsized_fn_params)]));
            }
            new_attrs.extend(file.attrs.iter().cloned());
            file.attrs = new_attrs;
        }
        let mut trait_paths = Vec::new();
        let mut visitor = TraitCollector {
            traits: &mut trait_paths,
        };
        visitor.visit_file(file);
        if trait_paths.is_empty() {
            trait_paths.push(syn::Path::from(Ident::new("Debug", Span::call_site())));
        }
        let mut rng = thread_rng();
        let selected_trait = trait_paths.choose(&mut rng).cloned().unwrap();
        let mut found_generic_fn = false;
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_trait_bound = false;
                let mut has_unsized_bound = false;
                let mut param_name = None;
                for param in &item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = param {
                        if let Pat::Ident(pat_ident) = &*pat_type.pat {
                            param_name = Some(pat_ident.ident.clone());
                        }
                    }
                }
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Type(type_param) = param {
                        for bound in &type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path == selected_trait {
                                    has_trait_bound = true;
                                }
                            }
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if let Some(lifetimes) = &trait_bound.lifetimes {
                                    if lifetimes.lifetime_token.is_some() {
                                    }
                                }
                            }
                        }
                        for bound in &type_param.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                if trait_bound.path.segments.last().map(|s| s.ident.to_string()) == Some("Sized".to_string()) {
                                    has_unsized_bound = true;
                                }
                            }
                        }
                    }
                }
                if has_trait_bound && !has_unsized_bound {
                    if let Some(param_ident) = param_name {
                        let conversion_stmt: Stmt = parse_quote! {
                            let _: dyn* #selected_trait = #param_ident;
                        };
                        item_fn.block.stmts.insert(0, conversion_stmt);
                        found_generic_fn = true;
                    }
                }
            }
        }
        if !found_generic_fn {
            let new_fn_name = Ident::new("mutate_dyn_conversion", Span::call_site());
            let mut new_fn: ItemFn = parse_quote! {
                fn #new_fn_name<T: #selected_trait + ?Sized>(x: T) {
                    let _: dyn* #selected_trait = x;
                }
            };
            for item in &mut file.items {
                if let Item::Fn(item_fn) = item {
                    if item_fn.sig.ident == "main" {
                        let mut found_expr = None;
                        let mut visitor = ExprCollector {
                            exprs: &mut found_expr,
                            trait_path: &selected_trait,
                        };
                        visitor.visit_block(&item_fn.block);
                        if let Some(expr) = found_expr {
                            let call_stmt: Stmt = parse_quote! {
                                #new_fn_name(&#expr as &dyn #selected_trait);
                            };
                            item_fn.block.stmts.insert(0, call_stmt);
                        } else {
                            let dummy_expr: Expr = parse_quote!(42);
                            let call_stmt: Stmt = parse_quote! {
                                #new_fn_name(&#dummy_expr as &dyn #selected_trait);
                            };
                            item_fn.block.stmts.insert(0, call_stmt);
                        }
                        break;
                    }
                }
            }
            file.items.insert(0, Item::Fn(new_fn));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets dyn* Trait conversion from dyn Trait with unsized function parameters. It ensures the crate enables dyn_star and unsized_fn_params features. It identifies traits used in the seed program, preferring common ones like Debug. If a generic function with T: Trait + ?Sized exists, it inserts a conversion let _: dyn* Trait = x;. Otherwise, it creates a new generic function with that conversion and adds a call in main with a dyn Trait argument cast from an existing expression. This transformation aims to trigger ICEs related to dyn* conversion and unsized parameter handling."
    }
}

struct TraitCollector<'a> {
    traits: &'a mut Vec<syn::Path>,
}

impl<'a> Visit<'a> for TraitCollector<'a> {
    fn visit_trait_bound(&mut self, node: &'a TraitBound) {
        if !self.traits.contains(&node.path) {
            self.traits.push(node.path.clone());
        }
        syn::visit::visit_trait_bound(self, node);
    }
    fn visit_path(&mut self, node: &'a syn::Path) {
        if node.segments.len() == 1 {
            let ident = node.segments.last().unwrap().ident.to_string();
            if ident == "Debug" || ident == "Display" || ident == "Clone" {
                if !self.traits.contains(node) {
                    self.traits.push(node.clone());
                }
            }
        }
        syn::visit::visit_path(self, node);
    }
}

struct ExprCollector<'a> {
    exprs: &'a mut Option<syn::Expr>,
    trait_path: &'a syn::Path,
}

impl<'a> Visit<'a> for ExprCollector<'a> {
    fn visit_expr(&mut self, node: &'a syn::Expr) {
        if self.exprs.is_none() {
            if let syn::Expr::Path(expr_path) = node {
                if expr_path.path.segments.len() == 1 {
                    *self.exprs = Some(node.clone());
                }
            } else if let syn::Expr::Lit(_) = node {
                *self.exprs = Some(node.clone());
            }
        }
        syn::visit::visit_expr(self, node);
    }
}