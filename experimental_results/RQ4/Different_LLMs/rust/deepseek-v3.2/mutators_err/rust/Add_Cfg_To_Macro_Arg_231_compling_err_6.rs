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

pub struct Add_Cfg_To_Macro_Arg_231;

impl Mutator for Add_Cfg_To_Macro_Arg_231 {
    fn name(&self) -> &str {
        "Add_Cfg_To_Macro_Arg_231"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = MacroCfgVisitor::default();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro invocations within function bodies. It identifies macro calls where at least one argument is a simple expression statement (e.g., `();`) and prefixes that argument with an empty `#[cfg()]` attribute. This transformation mimics a bug scenario where cfg attributes are placed on non-tail expressions after macro expansion, potentially causing compiler confusion about conditional compilation contexts within macro-expanded code. The mutation leverages existing macro definitions without introducing new declarations, focusing on the interaction between macro expansion and attribute processing."
    }
}

#[derive(Default)]
struct MacroCfgVisitor {
    in_function: bool,
}

impl VisitMut for MacroCfgVisitor {
    fn visit_item_fn_mut(&mut self, i: &mut ItemFn) {
        let old_in_function = self.in_function;
        self.in_function = true;
        syn::visit_mut::visit_item_fn_mut(self, i);
        self.in_function = old_in_function;
    }

    fn visit_expr_macro_mut(&mut self, expr: &mut ExprMacro) {
        if self.in_function {
            if let Some(args) = extract_macro_args(&expr.mac) {
                let mut new_args = Vec::new();
                let mut rng = thread_rng();
                let mut candidate_indices = Vec::new();
                
                for (i, arg) in args.iter().enumerate() {
                    if is_simple_expr_stmt(arg) {
                        candidate_indices.push(i);
                    }
                }
                
                if let Some(&idx) = candidate_indices.choose(&mut rng) {
                    for (i, arg) in args.iter().enumerate() {
                        if i == idx {
                            let mut new_tokens = proc_macro2::TokenStream::new();
                            new_tokens.extend(quote! { #[cfg()] });
                            new_tokens.extend(arg.clone());
                            new_args.push(new_tokens);
                        } else {
                            new_args.push(arg.clone());
                        }
                    }
                    
                    if let Ok(new_mac) = syn::parse2::<Macro>(quote! {
                        #expr.mac.path ! ( #(#new_args),* )
                    }) {
                        expr.mac = new_mac;
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_macro_mut(self, expr);
    }
}

fn extract_macro_args(mac: &Macro) -> Option<Vec<proc_macro2::TokenStream>> {
    let tokens = mac.tokens.clone();
    let args: syn::Result<Punctuated<Expr, Comma>> = syn::parse2(quote! { #tokens });
    
    match args {
        Ok(exprs) => {
            let mut result = Vec::new();
            for expr in exprs.iter() {
                result.push(quote! { #expr });
            }
            Some(result)
        }
        Err(_) => None,
    }
}

fn is_simple_expr_stmt(tokens: &proc_macro2::TokenStream) -> bool {
    if let Ok(expr) = syn::parse2::<Expr>(tokens.clone()) {
        if let Expr::Tuple(tuple) = expr {
            if tuple.elems.is_empty() {
                return true;
            }
        }
    }
    
    let token_string = tokens.to_string();
    token_string.trim() == "();" || token_string.trim() == "()"
}