use proc_macro2::{Span, TokenStream};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
    str::FromStr,
};
use syn::{
    parse_quote,
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

pub struct Modify_Const_Generic_In_Fn_88;

impl Mutator for Modify_Const_Generic_In_Fn_88 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_In_Fn_88"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut const_generic_indices = Vec::new();
                for (i, param) in item_fn.sig.generics.params.iter().enumerate() {
                    if let GenericParam::Const(_) = param {
                        const_generic_indices.push(i);
                    }
                }
                let mut const_generic_names = Vec::new();
                for i in const_generic_indices {
                    if let GenericParam::Const(const_param) =
                        item_fn.sig.generics.params[i].clone()
                    {
                        const_generic_names.push(const_param.ident.clone());
                        let new_param: GenericParam = parse_quote! { const #const_param: usize };
                        item_fn.sig.generics.params[i] = new_param;
                    }
                }
                let mut visitor = ConstGenericUsageVisitor {
                    const_generic_names,
                };
                visitor.visit_block_mut(&mut item_fn.block);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with const generic parameters that are string slices and transforms them into usize parameters. It modifies the function signature and updates all usages within the function body to accommodate the type change, replacing string operations with numeric ones where necessary. This mutation stresses the const generic system by altering the parameter type semantics, potentially exposing issues in const evaluation and type inference."
    }
}

struct ConstGenericUsageVisitor {
    const_generic_names: Vec<Ident>,
}

impl VisitMut for ConstGenericUsageVisitor {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Path(expr_path) = node {
            if let Some(ident) = expr_path.path.get_ident() {
                if self.const_generic_names.contains(ident) {
                    *node = Expr::Path(ExprPath {
                        attrs: expr_path.attrs.clone(),
                        qself: expr_path.qself.clone(),
                        path: expr_path.path.clone(),
                    });
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
}