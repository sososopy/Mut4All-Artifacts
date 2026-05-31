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

pub struct Transform_Static_To_Mutable_Static_60;

impl Mutator for Transform_Static_To_Mutable_Static_60 {
    fn name(&self) -> &str {
        "Transform_Static_To_Mutable_Static_60"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Static(item_static) = item {
                if item_static.mutability.is_none() {
                    item_static.mutability = Some(syn::StaticMutability::Mut(syn::token::Mut { span: Span::call_site() }));
                    
                    // Find usages of this static variable and wrap them in unsafe blocks
                    let var_ident = &item_static.ident;
                    let mut visitor = UnsafeWrapper {
                        var_ident: var_ident.clone(),
                    };
                    visitor.visit_file_mut(file);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms immutable static variables into mutable ones by changing `static` to `static mut`. It then wraps usages of these variables in unsafe blocks to ensure semantic correctness. This transformation tests the compiler's handling of mutable static variables and their interaction with unsafe code, potentially uncovering issues in constant evaluation and safety checks."
    }
}

struct UnsafeWrapper {
    var_ident: Ident,
}

impl VisitMut for UnsafeWrapper {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Path(ExprPath { path, .. }) = expr {
            if path.is_ident(&self.var_ident) {
                *expr = parse_quote! {
                    unsafe { #expr }
                };
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}