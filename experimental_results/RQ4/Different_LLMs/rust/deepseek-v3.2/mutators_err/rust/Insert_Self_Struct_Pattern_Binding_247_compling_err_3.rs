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

pub struct Insert_Self_Struct_Pattern_Binding_247;

impl Mutator for Insert_Self_Struct_Pattern_Binding_247 {
    fn name(&self) -> &str {
        "Insert_Self_Struct_Pattern_Binding_247"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = SelfPatternVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets let statements inside impl blocks that destructure a value of the implemented type using a struct pattern. It replaces an existing field binding identifier with the keyword Self, or inserts Self as a new binding if the pattern uses shorthand field init syntax. This transformation tests the compiler's handling of Self as a pattern binding identifier, which is a reserved keyword typically used in type contexts. By introducing Self as a binding, we aim to provoke parsing ambiguities, name resolution conflicts, or type inference errors, especially around the interaction between pattern bindings and the Self type alias within impl blocks."
    }
}

struct SelfPatternVisitor;

impl VisitMut for SelfPatternVisitor {
    fn visit_item_impl_mut(&mut self, node: &mut syn::ItemImpl) {
        let impl_ty = match &*node.self_ty {
            syn::Type::Path(type_path) => type_path.path.get_ident().cloned(),
            _ => None,
        };
        if let Some(struct_name) = impl_ty {
            for item in &mut node.items {
                if let syn::ImplItem::Fn(method) = item {
                    self.visit_block_mut(&mut method.block);
                    let mut visitor = SelfPatternVisitor;
                    visitor.visit_block_mut(&mut method.block);
                }
            }
        }
        syn::visit_mut::visit_item_impl_mut(self, node);
    }
    fn visit_block_mut(&mut self, node: &mut syn::Block) {
        for stmt in &mut node.stmts {
            if let syn::Stmt::Local(local) = stmt {
                if let Some((pat, expr)) = extract_struct_pattern(&mut local.pat, &local.init) {
                    mutate_pattern(pat, expr);
                }
            }
        }
        syn::visit_mut::visit_block_mut(self, node);
    }
}

fn extract_struct_pattern<'a>(pat: &'a mut syn::Pat, init: &'a Option<syn::Expr>) -> Option<(&'a mut syn::PatStruct, &'a syn::Expr)> {
    if let syn::Pat::Struct(pat_struct) = pat {
        if pat_struct.path.is_ident("Self") {
            if let Some(init_expr) = init {
                return Some((pat_struct, init_expr));
            }
        }
    }
    None
}

fn mutate_pattern(pat_struct: &mut syn::PatStruct, _expr: &syn::Expr) {
    let mut rng = thread_rng();
    for field in &mut pat_struct.fields {
        match &mut field.member {
            syn::Member::Named(ident) => {
                if rng.gen_bool(0.5) {
                    if let syn::Pat::Ident(pat_ident) = &mut *field.pat {
                        let has_ref = matches!(pat_ident.by_ref, Some(_));
                        let mutability = pat_ident.mutability.clone();
                        let attrs = std::mem::take(&mut pat_ident.attrs);
                        let new_ident = syn::Ident::new("Self", pat_ident.ident.span());
                        let new_pat = if has_ref {
                            parse_quote! { ref #mutability #new_ident }
                        } else {
                            parse_2quote! { #mutability #new_ident }
                        };
                        field.pat = Box::new(new_pat);
                        if !attrs.is_empty() {
                            if let syn::Pat::Ident(new_pat_ident) = &mut *field.pat {
                                new_pat_ident.attrs = attrs;
                            }
                        }
                    } else if let syn::Pat::Type(pat_type) = &mut *field.pat {
                        if let syn::Pat::Ident(pat_ident) = &mut *pat_type.pat {
                            let new_ident = syn::Ident::new("Self", pat_ident.ident.span());
                            pat_ident.ident = new_ident;
                        }
                    }
                }
            }
            syn::Member::Unnamed(_) => {}
        }
    }
    if !pat_struct.fields.is_empty() && rng.gen_bool(0.3) {
        let last_field = pat_struct.fields.last_mut().unwrap();
        if let syn::Pat::Ident(pat_ident) = &*last_field.pat {
            if pat_ident.ident != "Self" {
                let new_ident = syn::Ident::new("Self", pat_ident.ident.span());
                let mut new_pat = pat_ident.clone();
                new_pat.ident = new_ident;
                last_field.pat = Box::new(syn::Pat::Ident(new_pat));
            }
        }
    }
}