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

pub struct Replace_Const_Generic_Arg_With_Named_Const_412;

impl Mutator for Replace_Const_Generic_Arg_With_Named_Const_412 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_Arg_With_Named_Const_412"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_items = Vec::new();
        let mut replacements = Vec::new();
        let mut visitor = ConstGenericVisitor {
            const_items: &mut const_items,
            replacements: &mut replacements,
        };
        visitor.visit_file_mut(file);
        for (path, const_arg, new_const_name) in replacements {
            if let Some(segment) = path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(expr) = arg {
                            if let Expr::Lit(lit) = expr {
                                if let Lit::Int(int_lit) = &lit.lit {
                                    let value = int_lit.base10_parse::<usize>().unwrap();
                                    let const_name = Ident::new(&new_const_name, Span::call_site());
                                    *expr = parse_quote!(#const_name);
                                }
                            }
                        }
                    }
                }
            }
        }
        for const_item in const_items {
            file.items.insert(0, const_item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inherent associated types with const generic parameters. It replaces literal const arguments with named constants that evaluate to the same value, testing the compiler's const evaluation and type resolution paths. This transformation exposes potential bugs in how the compiler handles different representations of the same constant in nested generic contexts involving type aliases and inherent associated types."
    }
}

struct ConstGenericVisitor<'a> {
    const_items: &'a mut Vec<syn::Item>,
    replacements: &'a mut Vec<(syn::Path, syn::Expr, String)>,
}

impl<'a> VisitMut for ConstGenericVisitor<'a> {
    fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
        if let Expr::Call(call_expr) = expr {
            if let Expr::Path(path_expr) = &*call_expr.func {
                if let Some(segment) = path_expr.path.segments.last() {
                    if let PathArguments::AngleBracketed(args) = &segment.arguments {
                        for arg in &args.args {
                            if let GenericArgument::Const(const_expr) = arg {
                                if let Expr::Lit(lit) = const_expr {
                                    if let Lit::Int(int_lit) = &lit.lit {
                                        let value = int_lit.base10_parse::<usize>().unwrap();
                                        let new_const_name = format!("MUTATED_CONST_{}", value);
                                        let const_item: syn::Item = parse_quote! {
                                            const #new_const_name: usize = #value;
                                        };
                                        self.const_items.push(const_item);
                                        self.replacements.push((path_expr.path.clone(), const_expr.clone(), new_const_name));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}