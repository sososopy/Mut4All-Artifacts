use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemConst, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Const_With_Zero_Sized_Closure_75;

impl Mutator for Replace_Const_With_Zero_Sized_Closure_75 {
    fn name(&self) -> &str {
        "Replace_Const_With_Zero_Sized_Closure_75"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut constants_to_mutate = Vec::new();
        let mut function_refs = std::collections::HashMap::new();
        let mut new_functions_to_add = Vec::new();
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                if let Type::Path(type_path) = &*item_const.ty {
                    let last_segment = type_path.path.segments.last();
                    if let Some(segment) = last_segment {
                        if segment.ident == "fn" || matches!(segment.arguments, PathArguments::AngleBracketed(_)) {
                            constants_to_mutate.push((item_const.clone(), item as *mut _ as usize));
                        }
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = ConstantVisitor {
                    constant_name: None,
                    found_refs: Vec::new(),
                };
                visitor.visit_item_fn(item_fn);
                for (const_name, ref_expr) in visitor.found_refs {
                    function_refs.entry(const_name).or_insert(Vec::new()).push(item_fn.sig.ident.clone());
                }
            }
        }
        for (item_const, item_ptr) in constants_to_mutate {
            let const_name = &item_const.ident;
            let new_closure: Expr = parse_quote! { || {} };
            unsafe {
                let item = &mut *(item_ptr as *mut Item);
                if let Item::Const(const_item) = item {
                    const_item.expr = Box::new(new_closure);
                }
            }
            let ref_count = function_refs.get(&const_name).map(|v| v.len()).unwrap_or(0);
            if ref_count < 2 {
                let new_fn_name = Ident::new(&format!("use_{}_extra", const_name), Span::call_site());
                let visibility = &item_const.vis;
                let new_fn: ItemFn = parse_quote! {
                    #visibility fn #new_fn_name() {
                        let _ = &#const_name;
                    }
                };
                new_functions_to_add.push(new_fn);
            }
        }
        for new_fn in new_functions_to_add {
            file.items.push(Item::Fn(new_fn));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant items with function pointer types. It replaces the constant's initializer with a zero-sized closure (|| {}). Then, it ensures the constant is referenced via a shared reference from at least two distinct functions. If only one function references it, a new public function is added that also references the constant via &CONST_NAME. This transformation aims to create scenarios where the compiler's incremental compilation and optimization passes may incorrectly handle the constant's allocation identity across different compilation sessions, particularly for zero-sized closures referenced from multiple functions."
    }
}
struct ConstantVisitor {
    constant_name: Option<Ident>,
    found_refs: Vec<(Ident, Expr)>,
}
impl<'ast> Visit<'ast> for ConstantVisitor {
    fn visit_expr(&mut self, node: &'ast Expr) {
        if let Expr::Path(expr_path) = node {
            if expr_path.path.segments.len() == 1 {
                let ident = &expr_path.path.segments[0].ident;
                self.constant_name = Some(ident.clone());
                self.found_refs.push((ident.clone(), node.clone()));
            }
        }
        syn::visit::visit_expr(self, node);
    }
}