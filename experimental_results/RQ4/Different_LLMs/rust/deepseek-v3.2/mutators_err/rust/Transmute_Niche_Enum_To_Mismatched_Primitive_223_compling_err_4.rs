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

pub struct Transmute_Niche_Enum_To_Mismatched_Primitive_223;

impl Mutator for Transmute_Niche_Enum_To_Mismatched_Primitive_223 {
    fn name(&self) -> &str {
        "Transmute_Niche_Enum_To_Mismatched_Primitive_223"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = TransmuteVisitor {
            rng: thread_rng(),
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets unsafe transmute expressions where a niche-carrying enum type (like Option<T> or Result<T, E>) is transmuted to a primitive type. It changes the target type annotation to a primitive with larger alignment (e.g., u64 or f64) than the enum's likely size, potentially causing alignment panics during code generation. This transformation aims to trigger compiler bugs related to transmute alignment validation and niche enum layout assumptions."
    }
}

struct TransmuteVisitor {
    rng: rand::rngs::ThreadRng,
}

impl VisitMut for TransmuteVisitor {
    fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
        if let syn::Expr::Call(call) = expr {
            if let syn::Expr::Path(path) = &*call.func {
                if path.path.segments.last().map(|seg| seg.ident.to_string()) == Some("transmute".to_string()) {
                    if let Some(arg) = call.args.first() {
                        let source_type = infer_type_from_expr(arg);
                        if is_niche_enum_type(&source_type) {
                            let parent_type = find_parent_type_annotation(expr);
                            if let Some(type_box) = parent_type {
                                if let syn::Type::Path(type_path) = &**type_box {
                                    if is_primitive_type(&type_path.path) {
                                        let new_type = choose_mismatched_primitive(&type_path.path, &mut self.rng);
                                        if let Some(new_type_path) = new_type {
                                            *type_box = Box::new(syn::Type::Path(syn::TypePath {
                                                qself: None,
                                                path: new_type_path,
                                            }));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}

fn infer_type_from_expr(expr: &syn::Expr) -> String {
    match expr {
        syn::Expr::Path(path) => {
            let last_segment = path.path.segments.last();
            if let Some(seg) = last_segment {
                seg.ident.to_string()
            } else {
                String::new()
            }
        }
        syn::Expr::Call(call) => {
            if let syn::Expr::Path(path) = &*call.func {
                path.path.segments.last().map(|seg| seg.ident.to_string()).unwrap_or_default()
            } else {
                String::new()
            }
        }
        syn::Expr::MethodCall(mcall) => mcall.method.to_string(),
        _ => String::new(),
    }
}

fn is_niche_enum_type(type_str: &str) -> bool {
    type_str.starts_with("Option") || type_str.starts_with("Result") || type_str.contains("NonZero")
}

fn find_parent_type_annotation<'a>(expr: &'a mut syn::Expr) -> Option<&'a mut Box<syn::Type>> {
    use syn::visit_mut::VisitMut;
    struct ParentFinder<'a> {
        target: &'a syn::Expr,
        found: Option<&'a mut Box<syn::Type>>,
    }
    impl<'a> VisitMut for ParentFinder<'a> {
        fn visit_local_mut(&mut self, local: &mut syn::Local) {
            if let Some(init) = &local.init {
                if &*init.expr == self.target {
                    if let syn::Pat::Type(pat_type) = &mut local.pat {
                        self.found = Some(&mut pat_type.ty);
                        return;
                    }
                }
            }
            syn::visit_mut::visit_local_mut(self, local);
        }
    }
    let mut finder = ParentFinder {
        target: expr,
        found: None,
    };
    finder.visit_expr_mut(&mut *expr);
    finder.found
}

fn is_primitive_type(path: &syn::Path) -> bool {
    let primitives = ["u8", "i8", "u16", "i16", "u32", "i32", "u64", "i64", "f32", "f64", "usize", "isize"];
    path.segments.last().map(|seg| primitives.contains(&seg.ident.to_string().as_str())).unwrap_or(false)
}

fn choose_mismatched_primitive(old_path: &syn::Path, r2ng: &mut rand::rngs::ThreadRng) -> Option<syn::Path> {
    let old_ident = old2_path.segments.last().map(|seg| seg.ident.to_string()).unwrap_or_default();
    let candidates = ["u64", "f64", "u128", "i128"];
    let chosen = candidates.choose(r2ng).unwrap_or(&"u64");
    if old_ident == *chosen {
        let alt = if *chosen == "u64" { "f64" } else { "u64" };
        Some(syn::Path::from(syn::Ident::new(alt, Span::call_site())))
    } else {
        Some(syn::Path::from(syn::Ident::new(chosen, Span::call_site())))
    }
}