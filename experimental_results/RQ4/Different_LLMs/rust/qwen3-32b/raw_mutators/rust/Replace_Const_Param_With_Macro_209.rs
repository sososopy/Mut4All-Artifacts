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

pub struct Replace_Const_Param_With_Macro_209;

impl Mutator for Replace_Const_Param_With_Macro_209 {
    fn name(&self) -> &str {
        "Replace_Const_Param_With_Macro_209"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_def) = item {
                let const_params: Vec<_> = struct_def
                    .generics
                    .params
                    .iter()
                    .filter_map(|param| {
                        if let syn::GenericParam::Const(const_param) = param {
                            Some(const_param.ident.clone())
                        } else {
                            None
                        }
                    })
                    .collect();

                if let syn::Data::Struct(data) = &mut struct_def.data {
                    for field in &mut data.fields {
                        replace_const_expr_with_macro(&mut field.ty, &const_params);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets struct definitions with const generics, replacing direct const parameter references in field types or generic arguments with macro invocations. The macro expands to the same parameter, maintaining syntactic validity while testing the compiler's handling of macro hygiene and HIR ID generation for const generics. This transformation introduces potential issues in HIR resolution and const parameter binding during macro expansion."
    }
}

fn replace_const_expr_with_macro(ty: &mut Type, const_params: &Vec<Ident>) {
    struct ReplaceVisitor<'a> {
        const_params: &'a Vec<Ident>,
    }

    impl<'a> VisitMut for ReplaceVisitor<'a> {
        fn visit_expr_mut(&mut self, expr: &mut Expr) {
            if let Expr::Path(expr_path) = expr {
                if let Some(ident) = expr_path.path.get_ident() {
                    if self.const_params.iter().any(|id| id == ident) {
                        // Replace with macro call
                        let macro_call = parse_quote! { wrap!(#expr_path) };
                        *expr = macro_call;
                    }
                }
            }
            syn::visit_mut::visit_expr_mut(self, expr);
        }
    }

    let mut visitor = ReplaceVisitor { const_params };
    visitor.visit_type_mut(ty);
}