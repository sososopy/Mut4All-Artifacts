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

pub struct Introduce_Closure_With_Lifetime_In_Const_Array_194;

impl Mutator for Introduce_Closure_With_Lifetime_In_Const_Array_194 {
    fn name(&self) -> &str {
        "Introduce_Closure_With_Lifetime_In_Const_Array_194"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ConstExprVisitor<'a> {
            mutator: &'a Introduce_Closure_With_Lifetime_In_Const_Array_194,
        }
        impl<'a> VisitMut for ConstExprVisitor<'a> {
            fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
                if let syn::Expr::Const(expr_const) = expr {
                    let new_expr = self.mutator.transform_const_expr(&expr_const.expr);
                    *expr = new_expr;
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
            fn visit_type_mut(&mut self, ty: &mut syn::Type) {
                if let syn::Type::Array(type_array) = ty {
                    if let syn::Expr::Const(expr_const) = &type_array.len {
                        let new_expr = self.mutator.transform_const_expr(&expr_const.expr);
                        type_array.len = new_expr;
                    }
                }
                syn::visit_mut::visit_type_mut(self, ty);
            }
            fn visit_generic_argument_mut(&mut self, arg: &mut syn::GenericArgument) {
                if let syn::GenericArgument::Const(expr_const) = arg {
                    let new_expr = self.mutator.transform_const_expr(&expr_const);
                    *arg = syn::GenericArgument::Const(new_expr);
                }
                syn::visit_mut::visit_generic_argument_mut(self, arg);
            }
            fn visit_path_segment_mut(&mut self, segment: &mut syn::PathSegment) {
                if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        self.visit_generic_argument_mut(arg);
                    }
                }
                syn::visit_mut::visit_path_segment_mut(self, segment);
            }
            fn visit_type_path_mut(&mut self, type_path: &mut syn::TypePath) {
                for segment in &mut type_path.path.segments {
                    self.visit_path_segment_mut(segment);
                }
                syn::visit_mut::visit_type_path_mut(self, type_path);
            }
            fn visit_item_mut(&mut self, item: &mut syn::Item) {
                if let syn::Item::Struct(item_struct) = item {
                    if let syn::Fields::Named(fields_named) = &mut item_struct.fields {
                        for field in &mut fields_named.named {
                            self.visit_type_mut(&mut field.ty);
                        }
                    }
                }
                syn::visit_mut::visit_item_mut(self, item);
            }
        }
        let mut visitor = ConstExprVisitor {
            mutator: self,
        };
        syn::visit_mut::visit_file_mut(&mut visitor, file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const expressions in type positions, such as array sizes, generic const arguments, and type annotations. It replaces each eligible const expression with a block containing a closure with an explicit lifetime parameter referencing an undefined variable, followed by the original const expression. This introduces a fresh lifetime within a const context, potentially triggering compiler bugs related to lifetime lowering and def-id assignment in const generics and array size expressions."
    }
}
impl Introduce_Closure_With_Lifetime_In_Const_Array_194 {
    fn transform_const_expr(&self, expr: &syn::Expr) -> syn::Expr {
        let closure_param = syn::Pat::Type(syn::PatType {
            attrs: Vec::new(),
            pat: Box::new(syn::Pat::Wild(syn::PatWild {
                attrs: Vec::new(),
                underscore_token: token::Underscore::default(),
            })),
            colon_token: token::Colon::default(),
            ty: Box::new(syn::Type::Reference(syn::TypeReference {
                and_token: token::And::default(),
                lifetime: Some(syn::Lifetime::new("'a", Span::call_site())),
                mutability: Some(token::Mut::default()),
                elem: Box::new(syn::Type::Tuple(syn::TypeTuple {
                    paren_token: token::Paren::default(),
                    elems: Punctuated::new(),
                })),
            })),
        });
        let closure_body = syn::Expr::Call(syn::ExprCall {
            attrs: Vec::new(),
            func: Box::new(syn::Expr::Path(syn::ExprPath {
                attrs: Vec::new(),
                qself: None,
                path: syn::Path::from(syn::Ident::new("undefined_var", Span::call_site())),
            })),
            paren_token: token::Paren::default(),
            args: Punctuated::new(),
        });
        let closure = syn::Expr::Closure(syn::ExprClosure {
            attrs: Vec::new(),
            lifetimes: None,
            constness: None,
            asyncness: None,
            movability: None,
            capture: None,
            or1_token: token::Or::default(),
            inputs: Punctuated::from_iter(vec![closure_param]),
            or2_token: token::Or::default(),
            output: syn::ReturnType::Default,
            body: Box::new(closure_body),
        });
        let block = syn::Expr::Block(syn::ExprBlock {
            attrs: Vec::new(),
            label: None,
            block: syn::Block {
                brace_token: token::Brace::default(),
                stmts: vec![
                    syn::Stmt::Expr(closure, None),
                    syn::Stmt::Expr(expr.clone(), None),
                ],
            },
        });
        block
    }
}