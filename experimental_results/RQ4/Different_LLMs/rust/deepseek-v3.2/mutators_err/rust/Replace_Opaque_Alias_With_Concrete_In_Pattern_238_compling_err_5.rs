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

pub struct Replace_Opaque_Alias_With_Concrete_In_Pattern_238;

impl Mutator for Replace_Opaque_Alias_With_Concrete_In_Pattern_238 {
    fn name(&self) -> &str {
        "Replace_Opaque_Alias_With_Concrete_In_Pattern_238"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut opaque_aliases = Vec::new();
        for item in &file.items {
            if let Item::Type(item_type) = item {
                if let Type::ImplTrait(_) = *item_type.ty {
                    opaque_aliases.push(item_type.ident.clone());
                }
            }
        }
        let mut visitor = OpaqueAliasVisitor {
            opaque_aliases: &opaque_aliases,
            replacements: Vec::new(),
        };
        visitor.visit_file(file);
        for (local, concrete_type) in visitor.replacements {
            if let Some(stmt) = find_local_stmt(file, &local) {
                if let Stmt::Local(local_stmt) = stmt {
                    if let Some(init) = &local_stmt.init {
                        if let Some(inferred_type) = infer_concrete_type(&init.expr) {
                            local_stmt.pat = Pat::Type(PatType {
                                attrs: Vec::new(),
                                pat: Box::new(local_stmt.pat.clone()),
                                colon_token: token::Colon::default(),
                                ty: Box::new(inferred_type),
                            });
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies local variable declarations using opaque type aliases (defined via `type Alias = impl Trait;`) and replaces the opaque alias annotation with the concrete type inferred from the initializer expression. This transformation specifically targets pattern matching contexts where opaque types are matched or destructured, testing the compiler's consistency in handling pattern exhaustiveness and constructor categorization between opaque and concrete types. It aims to expose internal compiler errors related to type representation mismatches in closure upvar contexts and other pattern matching scenarios."
    }
}

struct OpaqueAliasVisitor<'a> {
    opaque_aliases: &'a Vec<Ident>,
    replacements: Vec<(Ident, Type)>,
}

impl<'a> Visit<'a> for OpaqueAliasVisitor<'a> {
    fn visit_local(&mut self, local: &'a Local) {
        if let Some(init) = &local.init {
            if let Some(inferred_type) = infer_concrete_type(&init.expr) {
                if let Pat::Ident(pat_ident) = &local.pat {
                    self.replacements.push((pat_ident.ident.clone(), inferred_type));
                }
            }
        }
        syn::visit::visit_local(self, local);
    }
}

fn find_local_stmt<'a>(file: &'a mut File, ident: &Ident) -> Option<&'a mut Stmt> {
    let mut result = None;
    for item in &mut file.items {
        if let Item::Fn(item_fn) = item {
            for stmt in &mut item_fn.block.stmts {
                if let Stmt::Local(local) = stmt {
                    if let Pat::Ident(pat_ident) = &local.pat {
                        if pat_ident.ident == *ident {
                            result = Some(stmt);
                            break;
                        }
                    }
                }
            }
        }
    }
    result
}

fn infer_concrete_type(expr: &Expr) -> Option<Type> {
    match expr {
        Expr::Call(ExprCall { func, args, .. }) => {
            if let Expr::Path(ExprPath { path, .. }) = &**func {
                if let Some(last_seg) = path.segments.last() {
                    match last_seg.ident.to_string().as_str() {
                        "Some" => {
                            if args.len() == 1 {
                                if let Some(tuple_type) = infer_tuple_type(&args[0]) {
                                    return Some(parse_quote!(Option<#tuple_type>));
                                }
                            }
                        }
                        "Ok" => {
                            if args.len() == 1 {
                                if let Some(inner_type) = infer_concrete1(&args[0]) {
                                    return Some(parse_quote!(Result<#inner_type, _>));
                                }
                            }
                        }
                        "Err" => {
                            if args.len() == 1 {
                                if let Some(inner_type) = infer_concrete1(&args[0]) {
                                    return Some(parse_quote!(Result<_, #inner_type>));
                                }
                            }
                        }
                        _ => {}
                    }
                }
            }
        }
        Expr::Tuple(expr_tuple) => {
            if !expr_tuple.elems.is_empty() {
                let mut elem_types = Vec::new();
                for elem in &expr_tuple.elems {
                    if let Some(elem_type) = infer_concrete1(elem) {
                        elem_types.push(elem_type);
                    } else {
                        return None;
                    }
                }
                let tuple_type = if elem_types.len() == 1 {
                    parse_quote!((#(elem_types[0])))
                } else {
                    let elem_types_iter = elem_types.iter();
                    parse_quote!((#(#elem_types_iter),*))
                };
                return Some(tuple_type);
            }
        }
        Expr::Lit(expr_lit) => {
            match &expr_lit.lit {
                Lit::Int(lit_int) => {
                    let suffix = lit_int.suffix();
                    if suffix.is_empty() {
                        return Some(parse_quote!(i32));
                    } else {
                        return Some(parse_quote!(#suffix));
                    }
                }
                Lit::Float(lit_float) => {
                    let suffix = lit_float.suffix();
                    if suffix.is_empty() {
                        return Some(parse_quote!(f64));
                    } else {
                        return Some(parse_quote!(#suffix));
                    }
                }
                Lit::Str(_) => return Some(parse_quote!(&str)),
                Lit::ByteStr(_) => return Some(parse_quote!(&[u8])),
                Lit::Byte(_) => return Some(parse_quote!(u8)),
                Lit::Char(_) => return Some(parse_quote!(char)),
                Lit::Bool(_) => return Some(parse_quote!(bool)),
                _ => {}
            }
        }
        Expr::Array(expr_array) => {
            if !expr_array.elems.is_empty() {
                if let Some(first_type) = infer_concrete1(&expr_array.elems[0]) {
                    return Some(parse_quote!([#first_type; #expr_array.elems.len()]));
                }
            }
        }
        Expr::Path(ExprPath { path, .. }) => {
            if let Some(last_seg) = path.segments.last() {
                match last_seg.ident.to_string().as_str() {
                    "None" => return Some(parse_quote!(Option<_>)),
                    "true" | "false" => return Some(parse_quote!(bool)),
                    _ => {}
                }
            }
        }
        _ => {}
    }
    None
}

fn infer_concrete1(expr: &Expr) -> Option<Type> {
    match expr {
        Expr::Call(ExprCall { func, args, .. }) => {
            if let Expr::Path(ExprPath { path, .. }) = &**func {
                if let Some(last_seg) = path.segments.last() {
                    match last_seg.ident.to_string().as_str() {
                        "Some" => {
                            if args.len() == 1 {
                                if let Some(tuple_type) = infer_tuple_type(&args[0]) {
                                    return Some(parse_quote!(Option<#tuple_type>));
                                }
                            }
                        }
                        "Ok" => {
                            if args.len() == 1 {
                                if let Some(inner_type) = infer_concrete1(&args[0]) {
                                    return Some(parse_quote!(Result<#inner_type, _>));
                                }
                            }
                        }
                        "Err" => {
                            if args.len() == 1 {
                                if let Some(inner_type) = infer_concrete1(&args[0]) {
                                    return Some(parse_quote!(Result<_, #inner_type>));
                                }
                            }
                        }
                        _ => {}
                    }
                }
            }
        }
        Expr::Tuple(expr_tuple) => {
            if !expr_tuple.elems.is_empty() {
                let mut elem_types = Vec::new();
                for elem in &expr_tuple.elems {
                    if let Some(elem_type) = infer_concrete1(elem) {
                        elem_types.push(elem_type);
                    } else {
                        return None;
                    }
                }
                let tuple_type = if elem_types.len() == 1 {
                    parse_quote!((#(elem_types[0])))
                } else {
                    let elem_types_iter = elem_types.iter();
                    parse_quote!((#(#elem_types_iter),*))
                };
                return Some(tuple_type);
            }
        }
        Expr::Lit(expr_lit) => {
            match &expr_lit.lit {
                Lit::Int(lit_int) => {
                    let suffix = lit_int.suffix();
                    if suffix.is_empty() {
                        return Some(parse_quote!(i32));
                    } else {
                        return Some(parse_quote!(#suffix));
                    }
                }
                Lit::Float(lit_float) => {
                    let suffix = lit_float.suffix();
                    if suffix.is_empty() {
                        return Some(parse_quote!(f64));
                    } else {
                        return Some(parse_quote!(#suffix));
                    }
                }
                Lit::Str(_) => return Some(parse_quote!(&str)),
                Lit::ByteStr(_) => return Some(parse_quote!(&[u8])),
                Lit::Byte(_) => return Some(parse_quote!(u8)),
                Lit::Char(_) => return Some(parse_quote!(char)),
                Lit::Bool(_) => return Some(parse_quote!(bool)),
                _ => {}
            }
        }
        Expr::Array(expr_array) => {
            if !expr_array.elems.is_empty() {
                if let Some(first_type) = infer_concrete1(&expr_array.elems[0]) {
                    return Some(parse_quote!([#first_type; #expr_array.elems.len()]));
                }
            }
        }
        Expr::Path(ExprPath { path, .. }) => {
            if let Some(last_seg) = path.segments.last() {
                match last_seg.ident.to_string().as_str() {
                    "None" => return Some(parse_quote!(Option<_>)),
                    "true" | "false" => return Some(parse_quote!(bool)),
                    _ => {}
                }
            }
        }
        _ => {}
    }
    None
}

fn infer_tuple_type(expr: &Expr) -> Option<Type> {
    if let Expr::Tuple(expr_tuple) = expr {
        if !expr_tuple.elems.is_empty() {
            let mut elem_types = Vec::new();
            for elem in &expr_tuple.elems {
                if let Some(elem_type) = infer_concrete1(elem) {
                    elem_types.push(elem_type);
                } else {
                    return None;
                }
            }
            let tuple_type = if elem_types.len() == 1 {
                parse_quote!((#(elem_types[0])))
            } else {
                let elem_types_iter = elem_types.iter();
                parse_quote!((#(#elem_types_iter),*))
            };
            return Some(tuple_type);
        }
    }
    None
}