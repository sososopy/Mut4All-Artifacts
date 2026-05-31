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

pub struct Modify_Const_Generic_Expr_In_Struct_408;

impl Mutator for Modify_Const_Generic_Expr_In_Struct_408 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Expr_In_Struct_408"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_idents = HashSet::new();
        for item in &file.items {
            if let Item::Const(item_const) = item {
                const_idents.insert(item_const.ident.to_string());
            }
        }
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut modified = false;
                let mut expr_str = String::new();
                let generics = item_struct.generics.const_params();
                for param in generics {
                    if param.ty == syn::parse_quote!(bool) {
                        if let Some(default) = &param.default {
                            expr_str = default.to_token_stream().to_string();
                            if expr_str.contains("&&") || expr_str.contains("||") {
                                continue;
                            }
                            let mut expr = syn::parse2::<Expr>(default.to_token_stream())
                                .unwrap();
                            let mut visitor = ConstExprVisitor {
                                const_idents: &const_idents,
                                found_ident: false,
                            };
                            visitor.visit_expr(&expr);
                            if visitor.found_ident {
                                expr_str = format!("{} && {}", expr_str, expr_str);
                                modified = true;
                            }
                        }
                    }
                }
                if modified {
                    let struct_ident = &item_struct.ident;
                    let struct_ident_str = struct_ident.to_string();
                    let struct_pat = Regex::new(&format!(r"\b{}\b", struct_ident_str)).unwrap();
                    for item in &new_items {
                        if let Item::Type(item_type) = item {
                            let mut new_item = item.clone();
                            let mut new_item_str = new_item.to_token_stream().to_string();
                            if struct_pat.is_match(&new_item_str) {
                                new_item_str = struct_pat
                                    .replace_all(&new_item_str, |_: &regex::Captures| {
                                        format!("{}<{{ {} }}>", struct_ident_str, expr_str)
                                    })
                                    .to_string();
                                new_item = syn::parse_str(&new_item_str).unwrap();
                            }
                            new_items.push(new_item);
                        } else {
                            new_items.push(item.clone());
                        }
                    }
                } else {
                    new_items.push(Item::Struct(item_struct.clone()));
                }
            } else {
                new_items.push(item.clone());
            }
        }
        *file = syn::File {
            shebang: file.shebang.clone(),
            attrs: file.attrs.clone(),
            items: new_items,
        };
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with const generic parameters of type bool, specifically those with a default expression. It modifies the default expression by duplicating it with a logical AND, effectively transforming a simple boolean expression into a more complex one. This change is propagated to all type aliases referencing the struct, ensuring the mutation is consistently applied across the codebase. The goal is to test the compiler's handling of complex const expressions and their impact on type resolution and code generation."
    }
}

struct ConstExprVisitor<'a> {
    const_idents: &'a HashSet<String>,
    found_ident: bool,
}

impl<'a> Visit<'a> for ConstExprVisitor<'a> {
    fn visit_expr_path(&mut self, i: &'a ExprPath) {
        if let Some(ident) = i.path.get_ident() {
            if self.const_idents.contains(&ident.to_string()) {
                self.found_ident = true;
            }
        }
        syn::visit::visit_expr_path(self, i);
    }
}