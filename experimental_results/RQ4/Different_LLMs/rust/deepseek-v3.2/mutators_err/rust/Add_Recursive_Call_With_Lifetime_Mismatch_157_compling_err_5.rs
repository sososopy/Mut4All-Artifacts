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

pub struct Add_Recursive_Call_With_Lifetime_Mismatch_157;

impl Mutator for Add_Recursive_Call_With_Lifetime_Mismatch_157 {
    fn name(&self) -> &str {
        "Add_Recursive_Call_With_Lifetime_Mismatch_157"
    }
    fn mutate(&self, file: &mut syn::File) {
        let concrete_types = collect_concrete_types(file);
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                // Check if function returns impl Trait
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        // Check for explicit lifetime parameters
                        if item_fn.sig.generics.lifetimes().count() == 0 {
                            continue;
                        }
                        // Check that one of the lifetime bounds references a declared lifetime
                        let declared_lifetimes: HashSet<String> = item_fn.sig.generics.lifetimes()
                            .map(|lp| lp.lifetime.ident.to_string())
                            .collect();
                        let mut lifetime_bound_present = false;
                        for bound in &type_impl_trait.bounds {
                            if let TypeParamBound::Lifetime(lifetime) = bound {
                                if declared_lifetimes.contains(&lifetime.ident.to_string()) {
                                    lifetime_bound_present = true;
                                    break;
                                }
                            }
                        }
                        if !lifetime_bound_present {
                            continue;
                        }
                        // Check if function body already contains a recursive call to itself
                        let mut visitor = RecursiveCallVisitor {
                            fn_name: item_fn.sig.ident.clone(),
                            found: false,
                        };
                        visitor.visit_block(&item_fn.block);
                        if visitor.found {
                            continue;
                        }
                        let mut candidate_types = Vec::new();
                        for ty in &concrete_types {
                            // For simplicity, we assume any concrete type could implement the trait.
                            // In a real implementation, we would need trait resolution.
                            candidate_types.push(ty.clone());
                        }
                        if candidate_types.is_empty() {
                            continue;
                        }
                        // Choose a random concrete type
                        let mut rng = thread_rng();
                        let chosen_type = candidate_types.choose(&mut rng).unwrap();
                        // Generate a variable name
                        let var_name = Ident::new("_recursive_temp", Span::call_site());
                        // Create the let-binding statement
                        let call_stmt: Stmt = parse_quote! {
                            let #var_name: #chosen_type = #item_fn.sig.ident();
                        };
                        // Insert the let-binding at the beginning of the block
                        item_fn.block.stmts.insert(0, call_stmt);
                        // Replace the return expression with a default value of the chosen type
                        // We need to find the return expression in the block.
                        // For simplicity, we assume the last statement is the1 return.
                        // In a more robust implementation, we would analyze the block.
                        if let Some(last_stmt) = item_fn.block.stmts.last_mut() {
                            if let Stmt::Expr(expr, _) = last_stmt {
                                // Replace with a default value based on type
                                let default_expr = default_value_for_type(chosen_type);
                                *expr = default_expr;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning `impl Trait` with explicit lifetime parameters that are bound in the return type but not actually used. It introduces a recursive call to the same function, stores the result in a variable of a concrete type that implements the trait, and then returns a default value of that concrete type. This creates a mismatch between the lifetime bound on the opaque type and the actual returned type, combined with recursion, potentially triggering region inference issues or lifetime errors."
    }
}

struct RecursiveCallVisitor {
    fn_name: Ident,
    found: bool,
}

impl<'a> Visit<'a> for RecursiveCallVisitor {
    fn visit_expr_call(&mut self, i: &ExprCall) {
        if let Expr::Path(path) = &*i.func {
            if path.path.segments.last().unwrap().ident == self.fn_name {
                self.found = true;
            }
        }
        syn::visit::visit_expr_call(self, i);
    }
}

fn collect_concrete_types(file: &syn::File) -> Vec<syn::Type> {
    let mut types = Vec::new();
    for item in &file.items {
        if let Item::Struct(item_struct) = item {
            types.push(syn::Type::Path(syn::TypePath {
                qself: None,
                path: syn::Path::from(item_struct.ident.clone()),
            }));
        }
        // Also collect primitive types
        types.push(syn::Type::Path(syn::TypePath {
            qself: None,
            path: syn::Path::from(Ident::new("i32", Span::call_site())),
        }));
        types.push(syn::Type::Path(syn::TypePath {
            qself: None,
            path: syn::Path::from(Ident::new("bool", Span::call_site())),
        }));
        types.push(syn::Type::Path(syn::TypePath {
            qself: None,
            path: syn::Path::from(Ident::new("String", Span::call_site())),
        }));
    }
    types
}

fn default_value_for_type(ty: &syn::Type) -> syn::Expr {
    if let syn::Type::Path(type_path) = ty {
        if type_path.path.is_ident("i32") {
            parse_quote! { 0 }
        } else if type_path.path.is_ident("bool") {
            parse_quote! { false }
        } else if type_path.path.is_ident("String") {
            parse_quote! { String::new() }
        } else {
            // For other types, use a default unit expression
            parse_quote! { () }
        }
    } else {
        parse_quote! { () }
    }
}