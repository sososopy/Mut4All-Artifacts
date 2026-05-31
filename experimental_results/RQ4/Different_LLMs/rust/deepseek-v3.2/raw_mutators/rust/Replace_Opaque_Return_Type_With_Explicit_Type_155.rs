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

pub struct Replace_Opaque_Return_Type_With_Explicit_Type_155;

impl Mutator for Replace_Opaque_Return_Type_With_Explicit_Type_155 {
    fn name(&self) -> &str {
        "Replace_Opaque_Return_Type_With_Explicit_Type_155"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = OpaqueReturnTypeVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning opaque `impl Trait` types and replaces the opaque type in match expression patterns with a concrete type alias that is a supertype or subtype of the inferred concrete type. It searches for existing type aliases in scope and replaces the pattern's type path with a relevant alias, creating a potential type mismatch that may expose variance or subtyping bugs in pattern matching. The mutation only modifies patterns, not the matched expression."
    }
}

struct OpaqueReturnTypeVisitor {
    opaque_return_types: Vec<TypePath>,
    type_aliases: Vec<TypePath>,
}

impl OpaqueReturnTypeVisitor {
    fn new() -> Self {
        Self {
            opaque_return_types: Vec::new(),
            type_aliases: Vec::new(),
        }
    }
}

impl VisitMut for OpaqueReturnTypeVisitor {
    fn visit_file_mut(&mut self, file: &mut File) {
        self.collect_opaque_types_and_aliases(file);
        self.visit_file_mut(file);
    }

    fn visit_expr_match_mut(&mut self, expr_match: &mut ExprMatch) {
        if let Expr::Call(call) = &*expr_match.expr {
            if let Expr::Path(path_expr) = &*call.func {
                let func_path = &path_expr.path;
                if self.opaque_return_types.iter().any(|opaque_path| opaque_path == func_path) {
                    for arm in &mut expr_match.arms {
                        if let Pat::Path(pat_path) = &mut arm.pat {
                            let segments = &mut pat_path.path.segments;
                            if segments.len() > 0 {
                                let last_segment = segments.last_mut().unwrap();
                                if let PathArguments::None = last_segment.arguments {
                                    let ident = &last_segment.ident;
                                    for alias_path in &self.type_aliases {
                                        if alias_path.path.segments.len() > 0 {
                                            let alias_last_segment = alias_path.path.segments.last().unwrap();
                                            if alias_last_segment.ident == *ident {
                                                pat_path.path = alias_path.path.clone();
                                }
                            }
                        }
                    }
                }
            }
        }
        self.visit_expr_match_mut(expr_match);
    }
}

fn collect_opaque_types_and_aliases(&mut self, file: &mut File) {
    for item in &mut file.items {
        if let Item::Fn(item_fn) = item {
            if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                if let Type::ImplTrait(_) = &**return_type {
                    let func_name = &item_fn.sig.ident;
                    let func_path = TypePath {
                        qself: None,
                        path: SynPath::from(func_name.clone()),
                    };
                    self.opaque_return_types.push(func_path);
                }
            }
        }
        if let Item::Type(item_type) = item {
            let alias_name = &item_type.ident;
            let alias_path = TypePath {
                qself: None,
                path: SynPath::from(alias_name.clone()),
            };
            self.type_aliases.push(alias_path);
        }
    }
}