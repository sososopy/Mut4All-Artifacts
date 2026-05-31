use proc_macro2::Span;
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, path::Path, process::Command};
use syn::{
    Expr, File, Ident, Item, ItemFn, Stmt, parse_quote,
    visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Add_Conflicting_Attributes_197;

impl Mutator for Add_Conflicting_Attributes_197 {
    fn name(&self) -> &str {
        "Add_Conflicting_Attributes_197"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut func_to_call = None;
        
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if !func.attrs.is_empty() {
                    // Add a conflicting attribute
                    func.attrs.push(parse_quote!(#[inline(always)]));
                    
                    // Store the function identifier to call later
                    func_to_call = Some(func.sig.ident.clone());
                }
            }
        }
        
        if let Some(func_ident) = func_to_call {
            let call_expr: Expr = parse_quote! {
                #func_ident();
            };
            
            for item in &mut file.items {
                if let syn::Item::Fn(other_func) = item {
                    if other_func.sig.ident != func_ident {
                        other_func.block.stmts.push(Stmt::Expr(call_expr.clone(), None));
                        break;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies functions with existing attributes and adds a potentially conflicting attribute, #[inline(always)], to them. It ensures that the function is invoked within the same module to observe how the compiler handles the combination of attributes and whether it leads to unexpected behavior or compilation issues."
    }
}