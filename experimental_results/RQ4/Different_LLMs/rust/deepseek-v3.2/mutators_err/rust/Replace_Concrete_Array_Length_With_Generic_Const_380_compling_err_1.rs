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

pub struct Replace_Concrete_Array_Length_With_Generic_Const_380;

impl Mutator for Replace_Concrete_Array_Length_With_Generic_Const_380 {
    fn name(&self) -> &str {
        "Replace_Concrete_Array_Length_With_Generic_Const_380"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                self.mutate_fn(item_fn);
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        self.mutate_fn(func);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets match arms with slice patterns containing concrete lengths. It replaces those concrete lengths with generic constant expressions referencing existing const generic parameters. If no const generic parameter exists, it adds one. This transformation introduces unevaluated constants in patterns, potentially triggering internal compiler errors during pattern compilation when the compiler expects concrete integers but encounters generic constants."
    }
}

impl Replace_Concrete_Array_Length_With_Generic_Const_380 {
    fn mutate_fn(&self, func: &mut ItemFn) {
        if func.sig.ident == "main" {
            return;
        }

        let mut has_const_generic = false;
        let mut const_param_name = None;
        for param in &func.sig.generics.params {
            if let GenericParam::Const(const_param) = param {
                has_const_generic = true;
                const_param_name = Some(const_param.ident.clone());
                break;
            }
        }

        if !has_const_generic {
            func.sig.generics.params.push(parse_quote!(const N: usize));
            const_param_name = Some(Ident::new("N", Span::call_site()));
        }

        let const_name = const_param_name.unwrap();
        let mut visitor = PatternVisitor {
            const_name: const_name.clone(),
            changed: false,
        };
        visitor.visit_item_fn_mut(func);

        if visitor.changed {
            for arg in &mut func.sig.inputs {
                if let FnArg::Typed(PatType { ty, .. }) = arg {
                    self.adjust_type_for_const_generic(ty, &const_name);
                }
            }
        }
    }

    fn adjust_type_for_const_generic(&self, ty: &mut Box<Type>, const_name: &Ident) {
        match &mut **ty {
            Type::Array(arr) => {
                if let Expr::Lit(expr_lit) = &*arr.len {
                    if let Lit::Int(lit_int) = &expr_lit.lit {
                        let len_value = lit_int.base10_parse::<usize>().unwrap_or(0);
                        if len_value > 0 {
                            let new_expr: Expr = parse_quote!(#const_name);
                            arr.len = Box::new(new_expr);
                        }
                    }
                }
            }
            Type::Slice(slice) => {
                let new_type: Type = parse_quote!([#slice.elem; #const_name]);
                *ty = Box::new(new_type);
            }
            _ => {}
        }
    }
}

struct PatternVisitor {
    const_name: Ident,
    changed: bool,
}

impl VisitMut for PatternVisitor {
    fn visit_expr_match_mut(&mut self, expr: &mut ExprMatch) {
        for arm in &mut expr.arms {
            self.visit_pat_mut(&mut arm.pat);
        }
        syn::visit_mut::visit_expr_match_mut(self, expr);
    }

    fn visit_pat_mut(&mut self, pat: &mut Pat) {
        match pat {
            Pat::Slice(slice_pat) => {
                if let Some((_, len)) = &slice_pat.len {
                    if let Expr::Lit(expr_lit) = &**len {
                        if let Lit::Int(_) = &expr_lit.lit {
                            let new_expr: Expr = parse_quote!(#self.const_name);
                            slice_pat.len = Some((token::Semi::default(), Box::new(new_expr)));
                            self.changed = true;
                        }
                    }
                }
            }
            Pat::TupleStruct(tuple_struct) => {
                if tuple_struct.path.segments.last().map(|s| s.ident.to_string()) == Some("[]".to_string()) {
                    let new_pat: Pat = parse_quote!([..; #self.const_name]);
                    *pat = new_pat;
                    self.changed = true;
                }
            }
            _ => {}
        }
        syn::visit_mut::visit_pat_mut(self, pat);
    }
}