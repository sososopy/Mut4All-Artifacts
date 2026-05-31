use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, str::FromStr, time::Duration};
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

pub struct Indirect_ImplTrait_Recursive_Function_Call_154;

impl Mutator for Indirect_ImplTrait_Recursive_Function_Call_154 {
    fn name(&self) -> &str {
        "Indirect_ImplTrait_Recursive_Function_Call_154"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut recursive_impl_trait_fns = vec![];
        let mut trait_names = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                trait_names.insert(item_trait.ident.to_string());
            }
        }
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut has_trait_bound = false;
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Trait(trait_bound) = bound {
                                let trait_name = trait_bound.path.segments.last().unwrap().ident.to_string();
                                if trait_names.contains(&trait_name) {
                                    has_trait_bound = true;
                                    break;
                                }
                            }
                        }
                        if has_trait_bound {
                            let mut visitor = RecursiveCallVisitor {
                                fn_name: item_fn.sig.ident.to_string(),
                                has_recursive_call: false,
                            };
                            visitor.visit_block(&item_fn.block);
                            if visitor.has_recursive_call {
                                recursive_impl_trait_fns.push(item_fn.sig.ident.clone());
                            }
                        }
                    }
                }
            }
        }
        for fn_name in &recursive_impl_trait_fns {
            let indirect_fn_name = Ident::new(&format!("indirect_{}", fn_name), Span::call_site());
            let mut new_item_fn = None;
            for item in &file.items {
                if let Item::Fn(item_fn) = item {
                    if &item_fn.sig.ident == fn_name {
                        let mut new_fn = item_fn.clone();
                        new_fn.sig.ident = indirect_fn_name.clone();
                        let mut visitor = RecursiveCallRewriter {
                            fn_name: fn_name.clone(),
                            indirect_fn_name: indirect_fn_name.clone(),
                        };
                        visitor.visit_block_mut(&mut new_fn.block);
                        new_item_fn = Some(new_fn);
                        break;
                    }
                }
            }
            if let Some(new_fn) = new_item_fn {
                file.items.push(Item::Fn(new_fn));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with `impl Trait` return types that are recursive and the trait is defined in the source code. It creates an additional function that calls the original recursively, then rewrites all recursive calls in the original function to go through this new indirection. This transformation tests the compiler's handling of recursive `impl Trait` returns and function call indirection, potentially exposing weaknesses in type inference and trait resolution."
    }
}

struct RecursiveCallVisitor {
    fn_name: String,
    has_recursive_call: bool,
}

impl<'ast> Visit<'ast> for RecursiveCallVisitor {
    fn visit_expr_call(&mut self, node: &'ast ExprCall) {
        if let Expr::Path(expr_path) = &*node.func {
            if expr_path.path.segments.len() == 1 && expr_path.path.segments[0].ident == self.fn_name {
                self.has_recursive_call = true;
            }
        }
        syn::visit::visit_expr_call(self, node);
    }
}

struct RecursiveCallRewriter {
    fn_name: Ident,
    indirect_fn_name: Ident,
}

impl VisitMut for RecursiveCallRewriter {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Call(expr_call) = node {
            if let Expr::Path(expr_path) = &*expr_call.func {
                if expr_path.path.segments.len() == 1 && expr_path.path.segments[0].ident == self.fn_name {
                    let new_expr: Expr = parse_quote! { #indirect_fn_name(#(#expr_call.args),*) };
                    *node = new_expr;
                    return;
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
}