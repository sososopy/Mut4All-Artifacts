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

pub struct Modify_Closure_Return_Type_216;

impl Mutator for Modify_Closure_Return_Type_216 {
    fn name(&self) -> &str {
        "Modify_Closure_Return_Type_216"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut closure_rewriter = ClosureRewriter::default();
                closure_rewriter.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets closures within functions, specifically those returning primitive types like `u8`. It changes the return type to another primitive type that requires conversion, such as `i16`, and adjusts the return expression accordingly. This transformation stresses the compiler's type inference and conversion mechanisms, potentially revealing bugs in closure handling and MIR optimizations."
    }
}

#[derive(Default)]
struct ClosureRewriter {
    target_closure: Option<ExprClosure>,
}

impl VisitMut for ClosureRewriter {
    fn visit_expr_closure_mut(&mut self, node: &mut ExprClosure) {
        if self.target_closure.is_none() {
            if let Some(return_type) = infer_closure_return_type(node) {
                if is_primitive_type(&return_type) {
                    self.target_closure = Some(node.clone());
                }
            }
        }
        syn::visit_mut::visit_expr_closure_mut(self, node);
    }
}

fn infer_closure_return_type(closure: &ExprClosure) -> Option<Type> {
    let block = ExprBlock {
        attrs: Vec::new(),
        label: None,
        block: Block {
            brace_token: token::Brace::default(),
            stmts: vec![Stmt::Expr(Expr::Closure(closure.clone()), None)],
        },
    };
    let fn_item = ItemFn {
        attrs: Vec::new(),
        vis: Visibility::Inherited,
        sig: Signature {
            constness: None,
            asyncness: None,
            unsafety: None,
            abi: None,
            fn_token: token::Fn::default(),
            ident: Ident::new("__closure_infer", Span::call_site()),
            generics: Generics::default(),
            paren_token: token::Paren::default(),
            inputs: Punctuated::new(),
            variadic: None,
            output: ReturnType::Default,
        },
        block: Box::new(block.block),
    };
    let tokens = quote!(#fn_item);
    let parsed_file = syn::parse2::<File>(tokens).ok()?;
    let inferred_type = parsed_file
        .items
        .iter()
        .filter_map(|item| {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, ty) = &item_fn.sig.output {
                    Some((**ty).clone())
                } else {
                    None
                }
            } else {
                None
            }
        })
        .next();
    inferred_type
}

fn is_primitive_type(ty: &Type) -> bool {
    if let Type::Path(type_path) = ty {
        if type_path.qself.is_none() {
            if let Some(ident) = type_path.path.get_ident() {
                return matches!(
                    ident.to_string().as_str(),
                    "bool" | "char"
                        | "u8"
                        | "u16"
                        | "u32"
                        | "u64"
                        | "u128"
                        | "usize"
                        | "i8"
                        | "i16"
                        | "i32"
                        | "i64"
                        | "i128"
                        | "isize"
                        | "f32"
                        | "f64"
                );
            }
        }
    }
    false
}

impl ClosureRewriter {
    fn rewrite_closure(&self, closure: &mut ExprClosure) {
        if let Some(target_closure) = &self.target_closure {
            if closure == target_closure {
                if let Some(return_type) = infer_closure_return_type(closure) {
                    if let Some(new_type) = suggest_alternative_primitive_type(&return_type) {
                        let new_expr = rewrite_closure_return_expr(&closure.body, &new_type);
                        closure.body = Box::new(new_expr);
                    }
                }
            }
        }
    }
}

fn suggest_alternative_primitive_type(ty: &Type) -> Option<Type> {
    if let Type::Path(type_path) = ty {
        if type_path.qself.is_none() {
            if let Some(ident) = type_path.path.get_ident() {
                let new_ident = match ident.to_string().as_str() {
                    "u8" => "i16",
                    "u16" => "i32",
                    "u32" => "i64",
                    "u64" => "i128",
                    "u128" => "usize",
                    "usize" => "u8",
                    "i8" => "u16",
                    "i16" => "i32",
                    "i32" => "i64",
                    "i64" => "i128",
                    "i128" => "isize",
                    "isize" => "i8",
                    "f32" => "f64",
                    "f64" => "f32",
                    "bool" => "u8",
                    "char" => "u32",
                    _ => return None,
                };
                return Some(Type::Path(TypePath {
                    qself: None,
                    path: SynPath::from(Ident::new(new_ident, ident.span())),
                }));
            }
        }
    }
    None
}

fn rewrite_closure_return_expr(expr: &Expr, new_type: &Type) -> Expr {
    let as_expr = Expr::Cast(ExprCast {
        attrs: Vec::new(),
        expr: Box::new(expr.clone()),
        as_token: token::As::default(),
        ty: Box::new(new_type.clone()),
    });
    as_expr
}

impl VisitMut for ClosureRewriter {
    fn visit_expr_closure_mut(&mut self, node: &mut ExprClosure) {
        self.rewrite_closure(node);
        syn::visit_mut::visit_expr_closure_mut(self, node);
    }
}