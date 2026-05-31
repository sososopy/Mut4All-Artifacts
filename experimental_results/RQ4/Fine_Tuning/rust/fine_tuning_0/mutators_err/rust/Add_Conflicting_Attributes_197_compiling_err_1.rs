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

pub struct Add_Conflicting_Attributes_197;

impl Mutator for Add_Conflicting_Attributes_197 {
    fn name(&self) -> &str {
        "Add_Conflicting_Attributes_197"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if !func.attrs.is_empty() {
                    // Add a conflicting attribute
                    func.attrs.push(parse_quote!(#[inline(always)]));
                    
                    // Ensure the function is called within the same module
                    let func_ident = &func.sig.ident;
                    let call_expr: Expr = parse_quote! {
                        #func_ident();
                    };
                    
                    // Add a call to the function in the first function found in the module
                    for item in &mut file.items {
                        if let syn::Item::Fn(other_func) = item {
                            if other_func.sig.ident != func.sig.ident {
                                other_func.block.stmts.push(Stmt::Expr(call_expr.clone()));
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions with existing attributes and adds a potentially conflicting attribute, #[inline(always)], to them. It ensures that the function is invoked within the same module to observe how the compiler handles the combination of attributes and whether it leads to unexpected behavior or compilation issues."
    }
}