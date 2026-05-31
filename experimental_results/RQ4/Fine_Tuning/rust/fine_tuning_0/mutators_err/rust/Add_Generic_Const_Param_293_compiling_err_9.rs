use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    Expr, File, FnArg, Ident, Item, ItemFn, ReturnType, Stmt, Type, parse_quote,
    punctuated::Punctuated, spanned::Spanned, visit_mut::VisitMut,
};

use crate::mutator::Mutator;

pub struct Add_Generic_Const_Param_293;

impl Mutator for Add_Generic_Const_Param_293 {
    fn name(&self) -> &str {
        "Add_Generic_Const_Param_293"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let ReturnType::Type(_, _) = &func.sig.output {
                    let generics = &mut func.sig.generics;
                    generics.params.push(parse_quote!(const N: usize));
                    
                    let new_block: syn::Block = parse_quote! {{
                        N + { #func.block }
                    }};
                    
                    func.block = Box::new(new_block);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a generic constant parameter `N` of type `usize` to non-main functions with a simple return type. It modifies the function body to incorporate this parameter, increasing the complexity and testing the compiler's handling of const generics and type checking."
    }
}