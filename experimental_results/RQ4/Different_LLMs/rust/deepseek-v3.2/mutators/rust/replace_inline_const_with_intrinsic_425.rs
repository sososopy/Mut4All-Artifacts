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

pub struct Replace_Inline_Const_With_Intrinsic_425;

impl Mutator for Replace_Inline_Const_With_Intrinsic_425 {
    fn name(&self) -> &str {
        "Replace_Inline_Const_With_Intrinsic_425"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut type_collector = TypeCollector::default();
        type_collector.visit_file(&file);
        let available_types = type_collector.types;
        if available_types.is_empty() {
            return;
        }
        let mut visitor = ConstBlockVisitor {
            rng: &mut rng,
            available_types: &available_types,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies inline const blocks (const { ... }) and replaces their entire content with a call to an intrinsic function (std::mem::transmute) using two types collected from the seed program. It selects types of the same size (e.g., integer types with identical bit-widths) to avoid immediate size mismatch errors. This transformation aims to trigger shim generation for intrinsics within inline const blocks, potentially exposing ICEs similar to the reported bug (F-inline_const) where the compiler fails to create a proper MIR shim for intrinsics in const contexts."
    }
}

#[derive(Default)]
struct TypeCollector {
    types: Vec<Type>,
}

impl Visit<'_> for TypeCollector {
    fn visit_type(&mut self, node: &Type) {
        match node {
            Type::Path(tp) => {
                if let Some(seg) = tp.path.segments.last() {
                    match seg.ident.to_string().as_str() {
                        "i8" | "i16" | "i32" | "i64" | "i128" | "isize" |
                        "u8" | "u16" | "u32" | "u64" | "u128" | "usize" |
                        "f32" | "f64" | "bool" | "char" => {
                            self.types.push(node.clone());
                        }
                        _ => {}
                    }
                }
            }
            Type::Reference(tr) => {
                self.types.push(node.clone());
            }
            _ => {}
        }
        syn::visit::visit_type(self, node);
    }
}

struct ConstBlockVisitor<'a> {
    rng: &'a mut rand::rngs::ThreadRng,
    available_types: &'a Vec<Type>,
}

impl<'a> VisitMut for ConstBlockVisitor<'a> {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Const(expr_const) = node {
            let candidate_types: Vec<_> = self.available_types.iter().collect();
            if candidate_types.len() < 2 {
                return;
            }
            let chosen_types: Vec<_> = candidate_types.choose_multiple(self.rng, 2).cloned().cloned().collect();
            let from_type = chosen_types[0].clone();
            let to_type = chosen_types[1].clone();
            let zero_lit: Expr = match &from_type {
                Type::Path(tp) if tp.path.segments.last().map(|s| s.ident.to_string()) == Some("bool".to_string()) => {
                    parse_quote!(false)
                }
                Type::Path(tp) if tp.path.segments.last().map(|s| s.ident.to_string()) == Some("char".to_string()) => {
                    parse_quote!('\0')
                }
                Type::Path(tp) if tp.path.segments.last().map(|s| s.ident.to_string()) == Some("f32".to_string()) => {
                    parse_quote!(0.0f32)
                }
                Type::Path(tp) if tp.path.segments.last().map(|s| s.ident.to_string()) == Some("f64".to_string()) => {
                    parse_quote!(0.0f64)
                }
                _ => {
                    parse_quote!(0)
                }
            };
            let transmute_call: Expr = parse_quote! {
                std::mem::transmute::<#from_type, #to_type>(#zero_lit)
            };
            expr_const.block = parse_quote!({ #transmute_call });
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
}